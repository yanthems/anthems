#ifndef TASK_POOL_H
#define TASK_POOL_H
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
struct task{
    virtual void run()=0;
};

class task_pool{
    std::queue<std::shared_ptr<task>>m_tasks;
    std::vector<std::thread>m_threads;
    std::mutex m_lock;
    std::condition_variable m_notify;
    bool m_stop = false;
    using ulock=std::unique_lock<std::mutex>;
public:
    explicit task_pool(size_t cap)noexcept{
        m_threads.emplace_back([&](){
            while (true){
                std::shared_ptr<task>tmp;
                {
                    ulock l(m_lock);
                    m_notify.wait(l, [this]{
                        return m_stop || !m_tasks.empty();
                    });
                    if (m_stop && m_tasks.empty()) {
                        break;
                    }
                    tmp = m_tasks.front();
                    m_tasks.pop();
                }
                tmp->run();
            }
        });
    }
    void add(std::shared_ptr<task>ptr) noexcept{
        {
            ulock l(m_lock);
            if (m_stop) {
                std::async([=]() { ptr->run(); });
            }
            m_tasks.emplace(ptr);
        }
        m_notify.notify_one();
    }
    ~task_pool()noexcept{
        {
            ulock l(m_lock);
            m_stop=true;
        }
        m_notify.notify_one();
        for(auto&t:m_threads){
            t.join();
        }
    }
};

#endif //TASK_POOL_H
