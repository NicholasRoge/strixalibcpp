#ifndef Strixa_Util_TaskableThread_H
#define Strixa_Util_TaskableThread_H

#include <functional>
#include <queue>
#include <thread>

namespace Strixa
{
    namespace Util
    {
        class TaskableThread
        {
            private:
                struct ThreadData
                {
                    bool                              joining;
                    std::queue<std::function<void()>> tasks_async;
                    std::queue<std::function<void()>> tasks_sync;
                    std::thread                       thread;

                    ThreadData();
                    ThreadData(const ThreadData& copy) = delete;
                };
            
            /* Class Methods */
            private:
                static void loop(ThreadData* data);

            /* Instance Properties */
            private:                
                ThreadData *data;

            /* Instance Methods */
            public:
                TaskableThread();

                TaskableThread(const TaskableThread& thread) = delete;

                TaskableThread(TaskableThread&& thread);

                ~TaskableThread();

                std::thread::id get_id() const;

                void join();

                bool joinable() const;

                std::thread::native_handle_type native_handle() const;

                template <class TaskFunction,class... TaskFunctionArguments>
                void now(TaskFunction&& task,TaskFunctionArguments&&... arguments);

                template <class TaskFunction,class... TaskFunctionArguments>
                void schedule(TaskFunction&& task,TaskFunctionArguments&&... arguments);

                TaskableThread& operator=(TaskableThread&& thread);
        };

        template <class TaskFunction,class... TaskFunctionArguments>
        void TaskableThread::now(TaskFunction&& task,TaskFunctionArguments&&... arguments)
        {
            auto callable = std::bind(task,arguments...);
            bool called = false;


            this->data->tasks_sync.push([&]{
                callable();

                called = true;
            });

            while (!called) {
                std::this_thread::yield();
            }
        }

        template <class TaskFunction,class... TaskFunctionArguments>
        void TaskableThread::schedule(TaskFunction&& task,TaskFunctionArguments&&... arguments)
        {
            auto callable = std::bind(task,arguments...);
            auto test = callable;


            this->data->tasks_async.push([&,callable]() mutable {
                callable();
            });
        }
    }
}

#endif