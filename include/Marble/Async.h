#pragma once

#include <chrono>
#include <coroutine>
#include <map>
#include <moodycamel/concurrentqueue.h>
#include <skarupke/function.h>
#include <thread>
#include <type_traits>
#include <vector>

#define return_async return { }

namespace Marble
{
    class Parallel;
    template <typename T> class Task;
    class ThreadPool final
    {
        struct Thread
        {
            std::thread _thread;
            std::atomic<bool> shouldExit;
            std::atomic<bool> exited;

            Thread() : shouldExit(false), exited(false),
            _thread
            (
                [this]()
                {
                    skarupke::function<void(Thread*)> func;
                    while (this->shouldExit.load(std::memory_order_relaxed) == false)
                    {
                        while (ThreadPool::threadJobs.try_dequeue(func))
                            func(this);
                    }
                    this->exited.store(true, std::memory_order_relaxed);
                }
            )
            {
                this->_thread.detach();
            }
        };

        inline static moodycamel::ConcurrentQueue<Thread*> threads;
        inline static std::atomic<size_t> nthreads = 0;
        inline static moodycamel::ConcurrentQueue<skarupke::function<void(Thread*)>> threadJobs;

        inline static std::atomic<size_t> processConsumedThreads = 1;
    public:

        ThreadPool() = delete;

        inline static void pushThreads(size_t threadCount)
        {
            for (size_t i = 0; i < threadCount; i++)
                ThreadPool::threads.enqueue(new Thread);
            ThreadPool::nthreads += threadCount;
        }
        inline static void killThreads(size_t threadCount)
        {
            skarupke::function<void(Thread*)> func;
            Thread* t;
            for (size_t i = 0; i < threadCount; i++)
            {
                if (ThreadPool::threads.try_dequeue(t))
                {
                    t->shouldExit.store(true, std::memory_order_relaxed);
                    while (!t->exited.load(std::memory_order_relaxed));
                    delete t;
                }
                else
                {
                    ThreadPool::nthreads -= i - 1;
                    return;
                }
            }
            ThreadPool::nthreads -= threadCount;
        }
        inline static void killAllThreads()
        {
            skarupke::function<void(Thread*)> func;
            Thread* t;
            while (ThreadPool::threads.try_dequeue(t))
            {
                t->shouldExit.store(true, std::memory_order_relaxed);
                while (!t->exited.load(std::memory_order_relaxed));
                delete t;
            }
            while (ThreadPool::threadJobs.try_dequeue(func));
        }
        inline static size_t threadCount()
        {
            return ThreadPool::nthreads.load();
        }

        inline static void setProcessAdditionalConsumedHardwareThreads(size_t threads)
        {
            ThreadPool::processConsumedThreads = threads + 1;
        }
        inline static size_t availableHardwareThreads()
        {
            return std::thread::hardware_concurrency() - ThreadPool::processConsumedThreads.load();
        }

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
            skarupke::function<void(ThreadPool::Thread*)>* jobs = new skarupke::function<void(ThreadPool::Thread*)>[toExclusive - fromInclusive];
            std::atomic<bool>* jobsCompleted = new std::atomic<bool>[toExclusive - fromInclusive] { false };

            for (size_t i = 0; i < toExclusive - fromInclusive; i++)
            {
                jobs[i] = [&, i](ThreadPool::Thread*)
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
            skarupke::function<void(ThreadPool::Thread*)>* jobs = new skarupke::function<void(ThreadPool::Thread*)>[toExclusive - fromInclusive];
            std::atomic<bool>* jobsCompleted = new std::atomic<bool>[toExclusive - fromInclusive] { false };
            std::atomic<bool> _break;
            _break.store(false, std::memory_order_relaxed);
            ParallelLoopState state(_break);

            for (size_t i = 0; i < toExclusive - fromInclusive; i++)
            {
                jobs[i] = [&, i](ThreadPool::Thread*)
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
        template <typename T, typename Container>
        inline static void forEach(Container& container, const skarupke::function<void(T&)>& predicate)
        {
            skarupke::function<void(ThreadPool::Thread*)>* jobs = new skarupke::function<void(ThreadPool::Thread*)>[container.size()];
            std::atomic<bool>* jobsCompleted = new std::atomic<bool>[container.size()] { false };

            size_t i = 0;
            for (auto it = container.begin(); it != container.end(); ++it, i++)
            {
                jobs[i] = [&, it, i](ThreadPool::Thread*)
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
}