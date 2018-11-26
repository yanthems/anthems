

#include "helper.hpp"

#include <cstdio>

namespace anthems {
class time_init{
private:
    time_init();

public:
    static time_t get_now_t();
    static time_init* get();
    std::string get_now_format(const std::string&format){
        auto t=get_now_t();
        // ref: https://zh.cppreference.com/w/cpp/io/manip/put_time
        auto res = std::put_time(std::localtime(&t), format.c_str());
        std::stringstream ss;
        ss<<res;
        return ss.str();
    }
};
static anthems::time_init *ptr = nullptr;
time_init::time_init() {
#ifdef __linux
    //linux设置时区 环境变量
        auto tz = getenv("TZ");
//        printf("TZ : %s\n", tz);
        //tzset();
        if (tz == NULL) {
            putenv(const_cast<char*>("TZ=Asia/Shanghai"));
//            printf("TZ : %s\n", getenv("TZ"));
        }
#endif
}
time_init* time_init::get() {
    if (ptr== nullptr){
        ptr=new time_init;
    }
    return ptr;
}
time_t time_init::get_now_t() {
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

std::time_t time::get_time_t() {
    return time_init::get()->get_now_t();
}
std::string time::get_now_str() {
    return time_init::get()->get_now_format("%Y-%m-%d %H:%M:%S");
}
std::string time::get_date_str() {
    return time_init::get()->get_now_format("%Y-%m-%d");
}
std::string time::get_time_str() {
    return time_init::get()->get_now_format("%H:%M:%S");
}

}
