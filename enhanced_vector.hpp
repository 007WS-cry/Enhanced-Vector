/**
* 文件名: enhanced_vector.hpp
 * 项目名称: enhanced_vector
 * 文件创建日期: 2025-02-25
 * 作者: 007WS
 * 联系方式: a2901805528@163.com
 *
 * 最后编辑日期: 2025-03-24
 * 最后编辑者: 007WS
 * 最后编辑者联系方式: a2901805528@163.com
 *
 * 文件描述:
 * C++ STL vector 扩展版本（大学生练手项目）
 *
 * 许可证:
 * 无
 */

#ifndef ENHANCED_VECTOR_HPP
#define ENHANCED_VECTOR_HPP

#include <cstddef>
#include <iterator>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace enhanced {

template <typename Type>
class Iterator {
public:
    using value_type = Type;
    using reference = Type &;
    using pointer = Type *;
    using difference_type = std::ptrdiff_t;

    explicit Iterator(pointer ptr) : _ptr(ptr) {};

    reference operator*() const {
        return *_ptr;
    }
    pointer operator->() const {
        return _ptr;
    }

    Iterator &operator++() {
        ++_ptr;
        return *this;
    }
    Iterator operator++(int) {
        auto it = *this;
        ++_ptr;
        return it;
    }

    bool operator!=(const Iterator &other) const {
        return _ptr != other._ptr;
    }
    bool operator==(const Iterator &other) const {
        return _ptr == other._ptr;
    }

private:
    pointer _ptr;
};

template <typename Type>
class enhanced_vector {
public:
    typedef Iterator<Type> iterator;
    typedef const Iterator<Type> const_iterator;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    explicit enhanced_vector(iterator first, iterator last) {
        _size = std::distance(first, last);
        int tmp = 5;
        while ((1 << tmp) <= _size) {
            tmp++;
        }
        _capacity = (1 << tmp);
        _data = new Type[_capacity];
        std::copy(first, last, _data);
        if (std::is_integral_v<Type> || std::is_floating_point_v<Type>) {
            _check = true;
            _sum = new Type;
            _mean = new Type;
            for (auto &x: _data) {
                *_sum += x;
            }
            *_mean = *_sum / _size;
        }
        else {
            _check = false;
            _sum = nullptr;
            _mean = nullptr;
        }
    }

    explicit enhanced_vector(const int &size = 0, const Type &value = Type()) {
        _size = size;
        int tmp = 5;
        while ((1 << tmp) <= _size) {
            tmp++;
        }
        _capacity = (1 << tmp);
        _data = new Type[_capacity];
        std::fill(_data, _data + _size, value);
        if (std::is_integral_v<Type> || std::is_floating_point_v<Type>) {
            _check = true;
            _sum = new Type;
            _mean = new Type;
        }
        else {
            _check = false;
            _sum = nullptr;
            _mean = nullptr;
        }
    }

    enhanced_vector(const enhanced_vector &other) {
        _size = other._size;
        _capacity = other._capacity;
        _check = other._check;
        _data = new Type[_capacity];
        std::copy(other._data, other._data + _size, _data);
        if (_check) {
            _sum = new Type(*other._sum);
            _mean = new Type(*other._mean);
        } else {
            _sum = nullptr;
            _mean = nullptr;
        }
    }

    enhanced_vector(enhanced_vector &&other) noexcept {
        _size = other._size;
        _capacity = other._capacity;
        _check = other._check;
        _data = other._data;
        _sum = other._sum;
        _mean = other._mean;

        other._size = 0;
        other._capacity = 0;
        other._data = nullptr;
        other._sum = nullptr;
        other._mean = nullptr;
    }

    enhanced_vector& operator=(const enhanced_vector &other) {
        if (this != &other) {
            delete[] _data;
            if (_check) {
                delete _sum;
                delete _mean;
            }

            _size = other._size;
            _capacity = other._capacity;
            _check = other._check;

            _data = new Type[_capacity];
            std::copy(other._data, other._data + _size, _data);

            if (_check) {
                _sum = new Type(*other._sum);
                _mean = new Type(*other._mean);
            } else {
                _sum = nullptr;
                _mean = nullptr;
            }
        }
        return *this;
    }

    enhanced_vector& operator=(enhanced_vector &&other) noexcept {
        if (this != &other) {  // 避免自我赋值
            // 释放当前资源
            delete[] _data;
            if (_check) {
                delete _sum;
                delete _mean;
            }
            _size = other._size;
            _capacity = other._capacity;
            _check = other._check;
            _data = other._data;
            _sum = other._sum;
            _mean = other._mean;
            other._size = 0;
            other._capacity = 0;
            other._data = nullptr;
            other._sum = nullptr;
            other._mean = nullptr;
        }
        return *this;
    }

    ~enhanced_vector() {
        delete[] _data;
        if (_check) {
            delete _sum;
            delete _mean;
        }
    }

    iterator begin() const {
        return iterator(_data);
    }
    iterator end() const {
        return iterator(_data + _size);
    }
    const_iterator cbegin() const {
        return const_iterator(_data);
    }
    const_iterator cend() const {
        return const_iterator(_data + _size);
    }
    reverse_iterator rbegin() const {
        return reverse_iterator(end());
    }
    reverse_iterator rend() const {
        return reverse_iterator(begin());
    }
    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }
    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }

    void push_back(const Type &value) {
        if (_size == _capacity) {
            addCapacity(_size + 1);
        }
        _data[_size++] = value;
        if (_check) {
            *_sum += value;
            *_mean = *_sum / _size;
        }
    }

    void pop_back() {
        if (_size == 0) {
            throw std::runtime_error("Error: vector is empty.");
        }
        if (_check) {
            *_sum -= _data[_size - 1];
            *_mean = *_sum / _size;
        }
        _size--;
    }

    void push_front(const Type &value) {
        if (_size == _capacity) {
            addCapacity(_size + 1);
        }
        std::copy(_data, _data + _size, _data + 1);
        _data[0] = value;
        _size++;
        if (_check) {
            *_sum += value;
            *_mean = *_sum / _size;
        }
    }

    void pop_front() {
        if (_size == 0) {
            throw std::runtime_error("Error: vector is empty.");
        }
        _size--;
        if (_check) {
            *sum -= _data[0];
            *_mean = *_sum / _size;
        }
        std::copy(_data + 1, _data + _size, _data);
    }

    void insert(iterator pos, const Type &value) {
        if (std::max(pos, _size + 1) >= _capacity) {
            addCapacity(std::max(pos, _size + 1));
        }
        std::copy(pos, _data + _size, pos + 1);
        _data[pos] = value;
        _size++;
        if (_check) {
            *_sum += value;
            *_mean = *_sum / _size;
        }
    }

    void erase(iterator pos) {
        if (pos >= _size) {
            throw std::runtime_error("Error: index out of range.");
        }
        if (_check) {
            *_sum -= _data[pos];
            *_mean = *_sum / (_size - 1);
        }
        std::copy(pos + 1, _data + _size, pos);
        _size--;
    }

    void erase(iterator first, iterator last) {
        if (_size <= std::distance(first, last)) {
            throw std::runtime_error("Error: index out of range.");
        }
        if (_check) {
            for (auto &x: std::vector<Type>(first, last)) {
                *_sum -= x;
                *_mean = *_sum / (_size - std::distance(first, last));
            }
        }
        std::copy(last, _data + _size, first);
        _size -= std::distance(first, last);
    }

    void clear() {
        _size = 0;
        if (_check) {
            *_sum = 0;
            *_mean = 0;
        }
    }

    Type &operator[](const int &index) {
        return _data[index];
    }
    const Type &operator[](const int &index) const {
        return _data[index];
    }

    enhanced_vector operator+(const enhanced_vector &other) const {
        if (!_check) {
            enhanced_vector res_vector(_size + other._size);
            std::copy(_data, _data + _size, res_vector._data);
            std::copy(other._data, other._data + other._size, res_vector._data + _size);
            return res_vector;
        }
        if (other._size != _size) {
            throw std::runtime_error("Error: vectors are not the same size.");
        }
        enhanced_vector res_vector;
        res_vector._size = _size;
        res_vector._capacity = _capacity;
        res_vector._check = _check;
        res_vector._data = new Type[_capacity];
        res_vector._sum = new Type(*this->_sum + *other._sum);
        for (int i = 0; i < _size; i++) {
            res_vector._data[i] = _data[i] + other._data[i];
        }
        res_vector._mean = new Type(*res_vector._sum / res_vector._size);
        return res_vector;
    }

    enhanced_vector operator-(const enhanced_vector &other) const {
        if (other._size != _size) {
            throw std::runtime_error("Error: vectors are not the same size.");
        }
        if (!_check) {
            throw std::runtime_error("Error: invalid operation.");
        }
        enhanced_vector res_vector;
        res_vector._size = _size;
        res_vector._capacity = _capacity;
        res_vector._check = _check;
        res_vector._data = new Type[_capacity];
        res_vector._sum = new Type(*this->_sum - *other._sum);
        for (int i = 0; i < _size; i++) {
            res_vector._data[i] = _data[i] - other._data[i];
        }
        res_vector._mean = new Type(*res_vector._sum / res_vector._size);
        return res_vector;
    }

    Type operator*(const enhanced_vector &other) const {
        if (other._size != _size) {
            throw std::runtime_error("Error: vectors are not the same size.");
        }
        if (!_check) {
            throw std::runtime_error("Error: invalid operation.");
        }
        Type res{};  // 正确初始化
        for (int i = 0; i < _size; i++) {
            res += _data[i] * other._data[i];
        }
        return res;
    }

    bool operator!=(const enhanced_vector &other) const {
        if (_size != other._size) {
            return true;
        }
        for (int i = 0; i < _size; i++) {
            if (_data[i] != other._data[i]) {
                return true;
            }
        }
        return false;
    }

    bool operator==(const enhanced_vector &other) const {
        if (_size != other._size) {
            return false;
        }
        for (int i = 0; i < _size; i++) {
            if (_data[i] != other._data[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator<(const enhanced_vector &other) const {
        for (int i = 0; i < std::min(_size, other._size); i++) {
            if (_data[i] < other._data[i]) {
                return true;
            }
            else if (_data[i] > other._data[i]) {
                return false;
            }
        }
        return _size < other._size;
    }

    bool operator<=(const enhanced_vector &other) const {
        return (*this < other) || (*this == other);
    }

    bool operator>(const enhanced_vector &other) const {
        for (int i = 0; i < std::min(_size, other._size); i++) {
            if (_data[i] > other._data[i]) {
                return true;
            }
            else if (_data[i] < other._data[i]) {
                return false;
            }
        }
        return _size > other._size;
    }

    bool operator>=(const enhanced_vector &other) const {
        return (*this > other) || (*this == other);
    }

    enhanced_vector &operator+=(const enhanced_vector &other) {
        if (_size + other._size >= _capacity) {
            addCapacity(_size + other._size + 1);
        }
        std::copy(other._data, other._data + other._size, _data + _size);
        _size += other._size;
        return *this;
    }

    enhanced_vector &operator>=(const enhanced_vector &other) {
        return (*this > other) || (*this == other);
    }

    [[nodiscard]] int size() const {
        return _size;
    }
    [[nodiscard]] int capacity() const {
        return _capacity;
    }
    [[nodiscard]] bool empty() const {
        return _size == 0;
    }

    void resize(const int &new_size) {
        delete[] _data;
        int tmp = 5;
        while ((1 << tmp) <= new_size) {
            tmp++;
        }
        _capacity = 1 << tmp;
        _data = new Type[_capacity];
        _size = new_size;
    }

    void resize(const int &new_size, const Type &value) {
        delete[] _data;
        int tmp = 5;
        while ((1 << tmp) <= new_size) {
            tmp++;
        }
        _capacity = 1 << tmp;
        _data = new Type[_capacity];
        _size = new_size;
        std::fill(_data, _data + _size, value);
    }

    void reserve(const int &new_capacity) {
        delete[] _data;
        _capacity = new_capacity;
        _data = new Type[_capacity];
        _size = 0;
    }

    void shrink_to_fit() {
        enhanced_vector<Type> tmp_vec;
        tmp_vec._data = _data;
        tmp_vec._size = _size;
        tmp_vec._capacity = _size;
        *this = tmp_vec;
    }

    Type front() const {
        if (_size == 0) {
            throw std::runtime_error("Error: vector is empty.");
        }
        return _data[0];
    }

    Type back() const {
        if (_size == 0) {
            throw std::runtime_error("Error: vector is empty.");
        }
        return _data[_size - 1];
    }

    void assign(iterator first, iterator last) {
        delete[] _data;
        int tmp = 5;
        while ((1 << tmp) <= std::distance(first, last)) {
            tmp++;
        }
        _capacity = 1 << tmp;
        _data = new Type[_capacity];
        _size = std::distance(first, last);
        std::copy(first, last, _data);
    }

    void assign(const int &size, const Type &value) {
        delete[] _data;
        int tmp = 5;
        while ((1 << tmp) <= size) {
            tmp++;
        }
        _capacity = 1 << tmp;
        _data = new Type[_capacity];
        _size = size;
        std::fill(_data, _data + _size, value);
    }

    void print(iterator first, iterator last) const {
        std::cout << "{";
        for (auto it = first; it != last; ++it) {
            std::cout << *it << (it + 1 != last ? ", " : "");
        }
        std::cout << "}" << std::endl;
    }

    Type sum() {
        if (!_check) {
            throw std::runtime_error("Error: invalid type.");
        }
        return *_sum;
    }

    Type mean() {
        if (!_check) {
            throw std::runtime_error("Error: invalid type.");
        }
        return *_mean;
    }

    friend std::istream &operator>>(std::istream &in, enhanced_vector<Type> &vector) {
        for (int i = 0; i < vector._size; i++) {
            in >> vector._data[i];
        }
        return in;
    }

    friend std::ostream &operator<<(std::ostream &out, const enhanced_vector<Type> &vector) {
        out << "{";
        for (int i = 0; i < vector._size; i++) {
            out << vector._data[i] << (i + 1 != vector._size ? ", " : "");
        }
        out << "}";
        return out;
    }

private:
    int _size;
    int _capacity;
    bool _check;
    Type *_data;
    Type *_sum;
    Type *_mean;
    void addCapacity(const int &new_size) {
        Type *_temp_data;
        std::copy(_data, _data + _size, _temp_data);
        int tmp = 5;
        while ((1 << tmp) <= new_size) {
            tmp++;
        }
        delete[] _data;
        _capacity = (1 << tmp);
        try {
            _data = new Type[_capacity];
        }
        catch (std::bad_alloc &e) {
            std::cerr << e.what() << std::endl;
            exit(1);
        }
        std::copy(_temp_data, _temp_data + _size, _data);
        delete[] _temp_data;
    }
};

} // enhanced

#endif //ENHANCED_VECTOR_HPP
