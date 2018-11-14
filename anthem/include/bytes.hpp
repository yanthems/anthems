#ifndef ANTHEMS_BYTES_HPP
#define ANTHEMS_BYTES_HPP

#include <string>
#include <utility>
#include <vector>
#include <ostream>

namespace anthems {

using byte = unsigned char;
const constexpr std::size_t ToEnd = std::size_t(-1);

class bytes : public std::vector<byte> {
    using super = std::vector<byte>;
public:
    static const constexpr std::size_t Middle_Block = 1024;
    static const constexpr std::size_t Small_Block = 512;
    static const constexpr std::size_t Large_Block = 2048;
    static const constexpr std::size_t Block_Size = Middle_Block;

    bytes(const std::string &str);

    bytes(const char *str);

    bytes(std::size_t t);

    bytes();

    bytes& cover(const bytes&data,std::size_t begin=0);

    bytes &operator+=(const bytes &data);

    bytes &operator+=(const char *str);

    bytes &operator+=(const std::string &str);

    friend bytes operator+(const bytes &src, const bytes &data);

    friend bytes operator+(const bytes &src, const char *str);

    friend bytes operator+(const bytes &src, const std::string &str);

    bytes split(std::size_t begin, std::size_t end = ToEnd);

    friend std::ostream &operator<<(std::ostream &out, const bytes &data);
};

}
#endif // !ANTHEMS_BYTES_HPP
