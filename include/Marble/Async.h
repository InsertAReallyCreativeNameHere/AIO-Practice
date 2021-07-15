#pragma once

#include <chrono>
#include <condition_variable>
#include <coroutine>
#include <map>
#include <moodycamel/concurrentqueue.h>
#include <skarupke/function.h>
#include <stop_token>
#include <thread>
#include <type_traits>
#include <vector>

namespace Marble
{
    class Parallel;
    template <typename T> class Task;
    class ThreadPool final
    {
        struct Thread
        {
            std::thread _thread;
            std::atomic_flag shouldExit = ATOMIC_FLAG_INIT;
            std::atomic_flag exited = ATOMIC_FLAG_INIT;

            Thread() :
            _thread
            (
                [this]()
                {
                    skarupke::function<void()> func;
                    while (!this->shouldExit.test(std::memory_order_relaxed))
                    {
                        std::this_thread::sleep_for(std::chrono::microseconds(10));
                        while (ThreadPool::threadJobs.try_dequeue(func))
                            func();
                    }
                    this->exited.test_and_set(std::memory_order_relaxed);
                }
            )
            {
                this->_thread.detach();
            }
        };

        inline static moodycamel::ConcurrentQueue<Thread*> threads;
        inline static moodycamel::ConcurrentQueue<skarupke::function<void()>> threadJobs;

        inline static struct Initializer final {
            Initializer()
            {
                size_t tc(std::thread::hardware_concurrency());
                --tc;

                if (tc != 0)
                {
                    for (size_t i = 0; i < tc; i++)
                        ThreadPool::threads.enqueue(new Thread);
                }
                else ThreadPool::threads.enqueue(new Thread);
            }
            ~Initializer()
            {
                skarupke::function<void()> func;
                Thread* t;
                while (ThreadPool::threads.try_dequeue(t))
                {
                    t->shouldExit.test_and_set(std::memory_order_relaxed);
                    while (!t->exited.test(std::memory_order_relaxed));
                }
                while (ThreadPool::threadJobs.try_dequeue(func));
            }
        } init;
    public:
        ThreadPool() = delete;

        template <typename> friend class Marble::Task;
        friend class Marble::Parallel;
    };

    struct ParallelLoopState final
    {
        ParallelLoopState(std::atomic<bool>& _break) : _break(_break)
        {
        }

        inline void breakAll() const noexcept
        {
            this->_break.store(true, std::memory_order_relaxed);
        }
        inline bool breakRequested()
        {
            return this->_break.load(std::memory_order_relaxed);
        }
    private:
        std::atomic<bool>& _break;
    };
    class Parallel final
    {
    public:
        Parallel() = delete;

        template <typename... Args>
        inline static void invoke(const std::vector<skarupke::function<void(Args...)>> functions, Args... args)
        {
            skarupke::function<void(ThreadPool::Thread*)>* jobs = new skarupke::function<void(ThreadPool::Thread*)>[functions.size()];
            std::atomic<bool>* jobsCompleted = new std::atomic<bool>[functions.size()] { false };

            for (size_t i = 0; i < functions.size(); i++)
            {
                jobs[i] = [&, i](ThreadPool::Thread*)
                {
                    functions[i](args...);
                    jobsCompleted[i].store(true, std::memory_order_relaxed);
                };
            }

            ThreadPool::threadJobs.enqueue_bulk(jobs, functions.size());

            for (size_t i = 0; i < functions.size(); i++)
                while (jobsCompleted[i].load(std::memory_order_relaxed) == false);

            delete[] jobs;
            delete[] jobsCompleted;
        }
        inline static void forRange(size_t fromInclusive, size_t toExclusive, const skarupke::function<void(size_t)>& predicate)
        {
            skarupke::function<void()>* jobs = new skarupke::function<void()>[toExclusive - fromInclusive];
            std::atomic<bool>* jobsCompleted = new std::atomic<bool>[toExclusive - fromInclusive] { false };

            for (size_t i = 0; i < toExclusive - fromInclusive; i++)
            {
                jobs[i] = [&, i]()
                {
                    predicate(i + fromInclusive);
                    jobsCompleted[i].store(true, std::memory_order_relaxed);
                };
            }

            ThreadPool::threadJobs.enqueue_bulk(jobs, toExclusive - fromInclusive);

            for (size_t i = 0; i < toExclusive - fromInclusive; i++)
                while (jobsCompleted[i].load(std::memory_order_relaxed) == false);

            delete[] jobs;
            delete[] jobsCompleted;
        }
        inline static void forRangeBreakable(size_t fromInclusive, size_t toExclusive, const skarupke::function<void(size_t, const ParallelLoopState&)>& predicate)
        {
            skarupke::function<void()>* jobs = new skarupke::function<void()>[toExclusive - fromInclusive];
            std::atomic<bool>* jobsCompleted = new std::atomic<bool>[toExclusive - fromInclusive] { false };
            std::atomic<bool> _break;
            _break.store(false, std::memory_order_relaxed);
            ParallelLoopState state(_break);

            for (size_t i = 0; i < toExclusive - fromInclusive; i++)
            {
                jobs[i] = [&, i]()
                {
                    if (!_break.load(std::memory_order_relaxed))
                        predicate(i + fromInclusive, state);
                    jobsCompleted[i].store(true, std::memory_order_relaxed);
                };
            }

            ThreadPool::threadJobs.enqueue_bulk(jobs, toExclusive - fromInclusive);

            for (size_t i = 0; i < toExclusive - fromInclusive; i++)
                while (jobsCompleted[i].load(std::memory_order_relaxed) == false);

            delete[] jobs;
            delete[] jobsCompleted;
        }
        template <typename Container, typename Func>
        inline static void forEach(Container& container, const Func&& predicate)
        {
            skarupke::function<void()>* jobs = new skarupke::function<void()>[container.size()];
            std::atomic<bool>* jobsCompleted = new std::atomic<bool>[container.size()] { false };

            size_t i = 0;
            for (auto it = container.begin(); it != container.end(); ++it, i++)
            {
                jobs[i] = [&, it, i]()
                {
                    predicate(*it);
                    jobsCompleted[i].store(true, std::memory_order_relaxed);
                };
            }

            ThreadPool::threadJobs.enqueue_bulk(jobs, container.size());

            for (size_t i = 0; i < container.size(); i++)
                while (jobsCompleted[i].load(std::memory_order_relaxed) == false);

            delete[] jobs;
            delete[] jobsCompleted;
        }
    };

    template <typename T>
    class Generator
    {
        struct Promise final
        {
            T value;

            inline Generator get_return_object() { return Generator(std::coroutine_handle<Promise>::from_promise(*this)); }

            inline std::suspend_always initial_suspend() { return { }; }
            inline std::suspend_always final_suspend() noexcept { return { }; }
            inline std::suspend_always yield_value(T t)
            {
                value = std::move(t);
                return { };
            }

            inline std::suspend_always unhandled_exception() { return { }; }
            inline void return_void() { }
        };
        std::coroutine_handle<Promise> coro;
    public:
        using promise_type = Promise;

        Generator() : coro(nullptr)
        {
        }
        explicit Generator(std::coroutine_handle<Promise> handle) : coro(handle)
        {
        }
        Generator(Generator&& other) : coro(std::coroutine_handle<Promise>::from_address(other.coro.address()))
        {
            other.coro = nullptr;
        }
        ~Generator()
        {
            if (this->coro)
                this->coro.destroy();
        }

        Generator& operator=(Generator&& other)
        {
            this->coro = other.coro;
            other.coro = nullptr;

            return *this;
        }

        inline T value() const
        {
            return this->coro.promise().value;
        }
        inline bool moveNext() const
        {
            this->coro.resume();
            return this->coro.done();
        }
    };

    template <typename T = void>
    class Task
    {
        struct Promise final
        {
            inline Task get_return_object() { return Task(std::coroutine_handle<Promise>::from_promise(*this)); }

            inline std::suspend_never initial_suspend() { return { }; }
            inline std::suspend_never final_suspend() noexcept { return { }; }

            inline std::suspend_always unhandled_exception() { return { }; }
            inline void return_void() { }
        };
        std::coroutine_handle<Promise> coro;
    public:
        using promise_type = Promise;

        Task() : coro(nullptr)
        {
        }
        explicit Task(std::coroutine_handle<Promise> handle) : coro(handle)
        {
        }
        Task(Task&& other) noexcept : coro(std::coroutine_handle<Promise>::from_address(other.coro.address()))
        {
            other.coro = nullptr;
        }
        ~Task()
        {
        }

        bool await_ready()
        {
            return false;
        }
        void await_suspend()
        {
            ThreadPool::threadJobs.enqueue([coro = this->coro](ThreadPool::Thread*) { coro.resume(); if (coro.done()) coro.destroy(); });
        }
        void await_resume()
        {

        }

        Task& operator=(Task&& other) noexcept
        {
            this->coro = other.coro;
            other.coro = nullptr;

            return *this;
        }
    };
    template <>
    class Task<void>
    {
        struct Promise final
        {
            std::atomic_flag hasCompleted = ATOMIC_FLAG_INIT;
            std::atomic_flag stopReq = ATOMIC_FLAG_INIT;

            inline Task get_return_object() { return Task(std::coroutine_handle<Promise>::from_promise(*this)); }

            inline std::suspend_always initial_suspend() { return { }; }
            inline std::suspend_never final_suspend() noexcept { this->hasCompleted.test_and_set(std::memory_order_relaxed); return { }; }

            inline void unhandled_exception() { }
            inline void return_void() { }
        };
        std::coroutine_handle<Promise> coro;
    public:
        using promise_type = Promise;

        Task() : coro(nullptr)
        {
        }
        explicit Task(std::coroutine_handle<Promise> handle) : coro(handle)
        {
        }
        Task(Task&& other) noexcept : coro(std::coroutine_handle<Promise>::from_address(other.coro.address()))
        {
            other.coro = nullptr;
        }
        ~Task()
        {
        }

        Task& operator=(Task&& other) noexcept
        {
            this->coro = other.coro;
            other.coro = nullptr;

            return *this;
        }

        bool await_ready() { return false; }
        void await_suspend(std::coroutine_handle<Promise> handle)
        {
            auto f = [=, this](auto&& f) -> void
            {
                if (this->coro.promise().hasCompleted.test())
                    handle.resume();
                else ThreadPool::threadJobs.enqueue([_f = std::move(f)]() { _f(_f); });
            };
            ThreadPool::threadJobs.enqueue([_f = std::move(f)]() { _f(_f); });
        }
        void await_resume() { }

        void wait()
        {
            while (!this->coro.promise().hasCompleted.test())
                std::this_thread::yield();
        }
        bool wait(std::chrono::milliseconds timeout)
        {
            auto begin = std::chrono::high_resolution_clock::now();
            while (!this->coro.promise().hasCompleted.test())
            {
                if (std::chrono::high_resolution_clock::now() > begin + timeout)
                    return false;
                std::this_thread::yield();
            }
            return true;
        }

        void runAsynchronously()
        {
            this->coro.resume();
        }
        void runSynchronously()
        {
            this->coro.resume();
        }

        void cancel()
        {
            this->coro.promise().stopReq.test_and_set(std::memory_order_relaxed);
        }

        struct Yield
        {
            bool await_ready() { return false; }
            template <typename T>
            void await_suspend(std::coroutine_handle<T> handle)
            {
                ThreadPool::threadJobs.enqueue([handle]()
                {
                    handle.resume();
                });
            }
            void await_resume() {   }
        };
        struct Delay
        {
            Delay(size_t milli) : milli(milli), from(std::chrono::high_resolution_clock::now())
            {
            }

            bool await_ready() { return false; }
            void await_suspend(std::coroutine_handle<> handle)
            {
                auto f = [=, this](auto&& f) -> void
                {
                    if (std::chrono::high_resolution_clock::now() > this->from + std::chrono::milliseconds(milli))
                        handle.resume();
                    else ThreadPool::threadJobs.enqueue([=]() { f(f); });
                };
                ThreadPool::threadJobs.enqueue([=]() { f(f); });
            }
            void await_resume() { }
        private:
            size_t milli;
            std::chrono::high_resolution_clock::time_point from;
        };
    };
}