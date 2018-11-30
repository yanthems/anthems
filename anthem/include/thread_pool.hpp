#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <functional>
#include <future>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>

class thread_pool {
public:
    explicit thread_pool(size_t capacity) noexcept {
        for (size_t i = 0; i < capacity; i++) {
            m_threads.emplace_back([&]() -> void {
                for (;;) {
                    std::function<void(void)>task;
                    {
                        std::unique_lock<std::mutex> ulock(m_lock);
                        m_notify.wait(ulock, [this]() {
                            return m_stop || !m_tasks.empty();
                        });
                        if (m_stop && m_tasks.empty())
                            return;
                        task = std::move(m_tasks.front());
                        m_tasks.pop();
                    }
                    task();
                }
            });
        }
    }
    template<typename F, typename... Args>
    auto add(F &&f, Args &&... args) noexcept {
        using r=std::invoke_result_t<F, Args...>;
        auto tmp=std::make_tuple(std::forward<Args>(args)...);
        auto fff=[&,t=std::move(tmp)]()->r{
            return std::apply(std::forward<F>(f),t);
        };
        auto task_pack_ptr = std::make_shared<std::packaged_task<r(void)>>(fff);
        auto result = task_pack_ptr->get_future();
        {
            std::unique_lock<std::mutex> ulock(m_lock);
            if (m_stop) {
                return std::async(std::forward<F>(f), std::forward<Args>(args)...);
            }
            m_tasks.emplace([=]() { (*task_pack_ptr)(); });
        }
        m_notify.notify_one();
        return result;
    }
    ~thread_pool() {
        {
            std::unique_lock<std::mutex> ulock(m_lock);
            m_stop = true;
        }
        m_notify.notify_all();
        for (auto &t : m_threads)
            t.join();
    }

private:
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_lock;
    std::condition_variable m_notify;
    bool m_stop = false;
public:
    thread_pool(const thread_pool&)= delete;
    thread_pool(thread_pool&&)= default;
    thread_pool&operator=(const thread_pool&)= delete;
    thread_pool&operator=(thread_pool&&)= default;
};

#endif //THREAD_POOL_H