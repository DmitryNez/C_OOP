#pragma once
#include <tuple>
template <typename...Args>
class Iterator {
private:
    std::tuple<Args...>* ptr;
public:
    Iterator(std::tuple<Args...>* first) :ptr(first) {

    }
    std::tuple<Args...>& operator++() {
        return *(++ptr);
    }

    bool operator !=(const Iterator& other) {
        return this->ptr != other.ptr;
    }

    std::tuple<Args...>& operator*() {
        return *(ptr);
    }

};