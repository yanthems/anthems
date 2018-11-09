#include "bytes.hpp"

#include <cstring>
namespace anthems {

bytes::bytes(const std::string&str)
    :super(str.length()) {
    std::size_t index = 0;
    for (const auto&i : str) {
        this->at(index++) = i;
        //(*this)[index++] = i;
    }
}

bytes::bytes(const char*str)
    :super(std::strlen(str)) {
    for (unsigned int index = 0; *str != '\0'; str++) {
        this->at(index++) = *str;
    }
}
bytes::bytes(std::size_t t)
    : super(t) {
}
bytes::bytes() {
}
bytes& bytes::operator+=(const bytes&data) {
    auto start = this->size();
    this->resize(start + data.size());
    for (auto i = 0; i < data.size(); i++) {
        this->at(start + i) = data[i];
    }
    return *this;
}
bytes& bytes::operator+=(const char*str) {
    auto start = this->size();
    int len = std::strlen(str);
    this->resize(start + len);
    for (auto i = 0; i < len; i++) {
        this->at(start + i) = *(str + i);
    }
    return *this;
}
bytes& bytes::operator+=(const std::string&str) {
    auto start = this->size();
    this->resize(start + str.size());
    for (auto i = 0; i < str.size(); i++) {
        this->at(start + i) = str[i];
    }
    return *this;
}
bytes operator+(const bytes&src, const bytes&data) {
    auto res = src;
    return res += data;
}
bytes operator+(bytes&&src, bytes&&data) {
    src += data;
    return src;
}
bytes operator+(const bytes&src, const char*str) {
    auto res = src;
    return res += str;
}
bytes operator+(const bytes&src, const std::string&str) {
    auto res = src;
    return res += str;
}
bytes bytes::split(std::size_t begin, std::size_t end) {
    if (end == ToEnd) {
        end = this->size();
    }
    auto res = bytes(end - begin);
    std::size_t index = 0;
    for (auto i = begin; i < end; i++) {
        res[index++] = this->at(i);
    }
    return res;
}

}