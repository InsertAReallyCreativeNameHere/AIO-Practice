#pragma once

#include <chrono>
#include <coroutine>
#include <moodycamel/concurrentqueue.h>
#include <skarupke/function.h>
#include <stop_token>
#include <thread>
#include <type_traits>
#include <vector>

#define return_async return { }

namespace Marble
{
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
            T returnValue;

            inline Task get_return_object() { return Task(std::coroutine_handle<Promise>::from_promise(*this)); }

            inline std::suspend_always initial_suspend() { return { }; }
            inline std::suspend_always final_suspend() noexcept { return { }; }
            inline std::suspend_always return_value(T t)
            {
                this->returnValue = t;
                return { };
            }

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
            this->coro.resume();
        }
        Task(Task&& other) : coro(std::coroutine_handle<Promise>::from_address(other.coro.address()))
        {
            other.coro = nullptr;
        }
        ~Task()
        {
            if (this->coro)
                this->coro.destroy();
        }

        inline Task& operator=(Task&& other)
        {
            this->coro = other.coro;
            other.coro = nullptr;

            return *this;
        }

        inline T returnValue()
        {
            return this->coro.promise().returnValue;
        }
    };
    template <>
    class Task<void>
    {
        struct Promise final
        {
            inline Task get_return_object() { return Task(std::coroutine_handle<Promise>::from_promise(*this)); }

            inline std::suspend_always initial_suspend() { return { }; }
            inline std::suspend_always final_suspend() noexcept { return { }; }

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
            this->coro.resume();
        }
        Task(Task&& other) : coro(std::coroutine_handle<Promise>::from_address(other.coro.address()))
        {
            other.coro = nullptr;
        }
        ~Task()
        {
            if (this->coro)
                this->coro.destroy();
        }

        inline Task& operator=(Task&& other)
        {
            this->coro = other.coro;
            other.coro = nullptr;

            return *this;
        }
    };
    struct WaitUntilSecondsPassed
    {
        WaitUntilSecondsPassed(float seconds) : seconds(seconds), since(std::chrono::high_resolution_clock::now())
        {
        }

        inline bool await_ready() const noexcept { return false; }
        inline void await_suspend(std::coroutine_handle<> handle)
        {
            while (std::chrono::high_resolution_clock::now() < this->since + std::chrono::microseconds((uint64_t)(this->seconds * 1000000 + 0.5f)))
                std::this_thread::yield();
            handle.resume();
        }
        inline void await_resume() { }
    private:
        float seconds;
        std::chrono::high_resolution_clock::time_point since;
    };

    class Parallel final
    {
        inline static moodycamel::ConcurrentQueue<skarupke::function<void()>> threadJobs;
        inline static size_t threadCount;
        inline static std::thread* threads;
        inline static std::atomic<bool>* threadsShouldExit;
        inline static std::atomic<bool>* threadsExited;
    public:
        Parallel() = delete;

        inline static void init(size_t threadCount = std::thread::hardware_concurrency())
        {
            Parallel::threadCount = threadCount;
            Parallel::threads = new std::thread[threadCount];
            Parallel::threadsShouldExit = new std::atomic<bool>[threadCount];
            Parallel::threadsExited = new std::atomic<bool>[threadCount];

            for (size_t i = 0; i < Parallel::threadCount; i++)
            {
                Parallel::threadsShouldExit[i] = false;
                Parallel::threadsExited[i] = false;
                Parallel::threads[i] = std::thread
                (
                    [](size_t index)
                    {
                        skarupke::function<void()> func;
                        while (Parallel::threadsShouldExit[index].load(std::memory_order_relaxed) == false)
                        {
                            while (Parallel::threadJobs.try_dequeue(func))
                                func();
                        }
                        Parallel::threadsExited[index].store(true, std::memory_order_relaxed);
                    },
                    i
                );
                Parallel::threads[i].detach();
            }
        }
        inline static void deinit()
        {
            for (size_t i = 0; i < Parallel::threadCount; i++)
                Parallel::threadsShouldExit[i].store(true, std::memory_order_relaxed);
            for (size_t i = 0; i < Parallel::threadCount; i++)
                while (Parallel::threadsExited[i].load(std::memory_order_relaxed) == false);

            skarupke::function<void()> func;
            while (Parallel::threadJobs.try_dequeue(func));

            delete[] Parallel::threads;
            delete[] Parallel::threadsShouldExit;
            delete[] Parallel::threadsExited;
        }

        template <typename... Args>
        inline static void invoke(const std::vector<skarupke::function<void(Args...)>> functions, Args... args)
        {
            skarupke::function<void()>* jobs = new skarupke::function<void()>[functions.size()];
            std::atomic<bool>* jobsCompleted = new std::atomic<bool>[functions.size()] { false };

            for (size_t i = 0; i < functions.size(); i++)
            {
                jobs[i] = [&, i]()
                {
                    functions[i](args...);
                    jobsCompleted[i].store(true, std::memory_order_relaxed);
                };
            }

            threadJobs.enqueue_bulk(jobs, functions.size());

            for (size_t i = 0; i < functions.size(); i++)
                while (jobsCompleted[i].load(std::memory_order_relaxed) == false)
                    std::this_thread::yield();

            delete[] jobs;
            delete[] jobsCompleted;
        }
    };
}