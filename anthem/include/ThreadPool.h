//
// Created by kirito on 18-11-28.
//

#ifndef ANTHEMS_THREADPOOL_H
#define ANTHEMS_THREADPOOL_H

#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <future>
class ThreadPool{
    std::queue<std::function<void(void)>>m_tasks;
    std::vector<std::thread>m_threads;
    std::mutex m_lock;
    std::condition_variable m_notify;
    bool m_stop = false;
    using ulock=std::unique_lock<std::mutex>;
public:
    explicit  ThreadPool(size_t cap)noexcept{
        m_threads.emplace_back([&](){
            while (true){
                ulock l(m_lock);
                m_notify.wait(l,[this](){
                    return m_stop||!m_tasks.empty();
                });
                if(m_stop&&m_tasks.empty()){
                    break;
                }
                auto task=m_tasks.front();
                m_tasks.pop();
                l.unlock();
                task();
            }
        });
    }
    template <typename F,typename... Args>
    auto add(F&&f,Args&&...args)noexcept{
        using r=std::invoke_result_t<F,Args...>;
        auto tpp=std::make_shared<std::packaged_task<r(void)>>(
                [&]()->r{
                    return std::invoke(std::forward<F>(f),std::forward<Args>(args)...);
                });

        auto res=tpp->get_future();
        ulock l(m_lock);
        if(m_stop){
            return std::async(std::forward<F>(f),std::forward<Args>(args)...);
        }
        m_tasks.emplace([f=std::move(tpp)](){
            (*f)();
        });
        l.unlock();
        m_notify.notify_one();
        return res;
    }
    ~ThreadPool(){
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
#endif //ANTHEMS_THREADPOOL_H
