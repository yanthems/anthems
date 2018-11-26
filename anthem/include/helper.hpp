
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


}

#endif //ANTHEMS_HELPER_HPP
