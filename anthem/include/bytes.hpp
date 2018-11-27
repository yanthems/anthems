#ifndef ANTHEMS_BYTES_HPP
#define ANTHEMS_BYTES_HPP

#include <string>
#include <utility>
#include <vector>
#include <ostream>
#include <array>
namespace anthems {

using byte = unsigned char;
const constexpr std::size_t ToEnd = std::size_t(-1);

class bytes : public std::vector<byte> {
    using super = std::vector<byte>;
public:
//    static const constexpr std::size_t Small_Block = 2048;
//    static const constexpr std::size_t Middle_Block = 4196;
//    static const constexpr std::size_t Large_Block = 8192;

    //2 + 10 + 4096
    static const constexpr std::size_t Block_Size = 4108;

    bytes(const std::string &str);

    bytes(const char *str);

    bytes(std::size_t t);

    bytes()= default;

    bytes& cover(const bytes&data,std::size_t begin=0);

    bytes &operator+=(const bytes &data);

    bytes &operator+=(const char *str);

    bytes &operator+=(const std::string &str);

    friend bytes operator+(const bytes &src, const bytes &data);

    friend bytes operator+(const bytes &src, const char *str);

    friend bytes operator+(const bytes &src, const std::string &str);

    bytes split(std::size_t begin, std::size_t end = ToEnd);

    std::string to_string();

    template <int n>
    std::array<byte,n> to_array(){
        std::array<byte,n>res;
        std::size_t count=0;
        for(auto &i:*this){
            res[count++]=i;
        }
        return res;
    };

    friend std::ostream &operator<<(std::ostream &out,const bytes &data);
};

}
#endif // !ANTHEMS_BYTES_HPP
