
#ifndef ANTHEMS_SAFE_QUEUE_HPP
#define ANTHEMS_SAFE_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
template <typename T>
class safe_queue final{
private:
    using ulock=std::unique_lock<std::mutex>;
    std::queue<T>q;
    std::mutex ml;
    std::condition_variable cv;
public:
    explicit safe_queue()= default;
    safe_queue(const safe_queue&)= delete;
    safe_queue(safe_queue&&)= default;
    safe_queue&operator=(const safe_queue&)= delete;
    safe_queue&operator=(safe_queue&&)= default;
    T front(){
        ulock lock(ml);
        cv.wait(lock,[this](){ return !q.empty();});
        return q.front();
    }
    T pop_front(){
        ulock lock(ml);
        cv.wait(lock,[this](){ return !q.empty();});
        auto res=std::move(q.front());
        q.pop();
        return res;
    }
    void pop(){
        ulock lock(ml);
        cv.wait(lock);
        q.pop();
    }
    void push(T&&v){
        {
            ulock lock(ml);
            q.push(std::forward<T>(v));
        }
        cv.notify_one();
    }
    void push(const T&v){
        {
            ulock lock(ml);
            q.push(v);
        }
        cv.notify_one();
    }
    bool empty(){
        ulock lock(ml);
        return q.empty();
    }
    size_t size(){
        ulock lock(ml);
        return q.size();
    }
};

#endif //ANTHEMS_SAFE_QUEUE_HPP
