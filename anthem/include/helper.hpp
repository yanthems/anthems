
#ifndef ANTHEMS_HELPER_HPP
#define ANTHEMS_HELPER_HPP

#include "logger.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <sstream>

namespace anthems{

#define LINE2STR(x) #x
#define LINE2STRHELP(x) LINE2STR(x)
#define POS (__FILE__ ":[line " LINE2STRHELP(__LINE__) "]")
#define TIME (anthems::time::get_time_str())

class time{
public:
    static std::string get_now_str();
    static std::string get_date_str();
    static std::string get_time_str();
    static std::time_t get_time_t();
};
class time_init{
private:
    time_init();
    static inline time_init* ptr=nullptr;
public:
    static time_t get_now_t();
    static time_init* get();
    std::string get_now_format(const std::string&format);
};

}

#endif //ANTHEMS_HELPER_HPP
