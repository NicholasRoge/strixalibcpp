#include <Strixa/Util/TaskableThread.h>

using Strixa::Util::TaskableThread;


TaskableThread::ThreadData::ThreadData()
{
}


TaskableThread::TaskableThread()
{
    this->data = new ThreadData();
    this->data->joining = false;
    this->data->thread = std::thread(&TaskableThread::loop,this->data);
}

TaskableThread::TaskableThread(TaskableThread&& taskable_thread)
{
    this->data = taskable_thread.data;
    taskable_thread.data = nullptr;
}

TaskableThread::~TaskableThread()
{
    if (this->data != nullptr) {
        while (this->data->joining);

        delete this->data;
    }
}

std::thread::id TaskableThread::get_id() const
{
    return this->data->thread.get_id();
}

void TaskableThread::join()
{
    this->data->joining = true;
    if (std::this_thread::get_id() != this->get_id()) {
        this->data->thread.join();
    }
}

bool TaskableThread::joinable() const
{
    return this->data->thread.joinable();
}

void TaskableThread::loop(ThreadData* data)
{
    while (!data->joining) {
        if (!data->tasks_sync.empty()) {
            data->tasks_sync.front()();
            data->tasks_sync.pop();
        } else if (!data->tasks_async.empty()) {
            data->tasks_async.front()();
            data->tasks_async.pop();
        } else {
            std::this_thread::yield();
        }
    }
    data->joining = false;
}

std::thread::native_handle_type TaskableThread::native_handle() const
{
    return this->data->thread.native_handle();
}

TaskableThread& TaskableThread::operator =(TaskableThread&& taskable_thread)
{
    this->data = taskable_thread.data;
    taskable_thread.data = nullptr;

    return *this;
}