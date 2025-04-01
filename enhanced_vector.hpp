/**
* 文件名: enhanced_vector.hpp
 * 项目名称: enhanced_vector
 * 文件创建日期: 2025-02-25
 * 作者: 007WS
 * 联系方式: a2901805528@163.com
 *
 * 最后编辑日期: 2025-04-01
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
#include <memory>
#include <algorithm>

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
        _data = std::make_unique<Type[]>(_capacity);
        std::copy(first, last, _data.get());
        if (std::is_integral_v<Type> || std::is_floating_point_v<Type>) {
            _check = true;
            _sum = std::make_unique<Type>(0);
            _mean = std::make_unique<Type>(0);
            for (size_t i = 0; i < _size; ++i) {
                *_sum += _data[i];
            }
            *_mean = *_sum / _size;
        }
        else {
            _check = false;
        }
    }

    explicit enhanced_vector(const int &size = 0, const Type &value = Type()) {
        _size = size;
        int tmp = 5;
        while ((1 << tmp) <= _size) {
            tmp++;
        }
        _capacity = (1 << tmp);
        _data = std::make_unique<Type[]>(_capacity);
        std::fill(_data.get(), _data.get() + _size, value);
        if (std::is_integral_v<Type> || std::is_floating_point_v<Type>) {
            _check = true;
            _sum = std::make_unique<Type>(0);
            _mean = std::make_unique<Type>(0);
            for (size_t i = 0; i < _size; ++i) {
                *_sum += _data[i];
            }
            if (_size == 0) {
                *_mean = 0;
            }
            else {
                *_mean = *_sum / _size;
            }
        }
        else {
            _check = false;
        }
    }

    enhanced_vector(const enhanced_vector &other) {
        _size = other._size;
        _capacity = other._capacity;
        _check = other._check;
        _data = std::make_unique<Type[]>(_capacity);
        std::copy(other._data.get(), other._data.get() + _size, _data.get());
        if (_check) {
            _sum = std::make_unique<Type>(*other._sum);
            _mean = std::make_unique<Type>(*other._mean);
        }
    }

    enhanced_vector(enhanced_vector &&other) noexcept = default;

    enhanced_vector& operator=(const enhanced_vector &other) {
        if (this != &other) {
            _size = other._size;
            _capacity = other._capacity;
            _check = other._check;
            _data = std::make_unique<Type[]>(_capacity);
            std::copy(other._data.get(), other._data.get() + _size, _data.get());
            if (_check) {
                _sum = std::make_unique<Type>(*other._sum);
                _mean = std::make_unique<Type>(*other._mean);
            }
        }
        return *this;
    }

    enhanced_vector& operator=(enhanced_vector &&other) noexcept = default;

    ~enhanced_vector() = default;

    iterator begin() const {
        return iterator(_data.get());
    }
    iterator end() const {
        return iterator(_data.get() + _size);
    }
    const_iterator cbegin() const {
        return const_iterator(_data.get());
    }
    const_iterator cend() const {
        return const_iterator(_data.get() + _size);
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
            *_mean = *_sum / (_size - 1);
        }
        _size--;
    }

    void push_front(const Type &value) {
        if (_size == _capacity) {
            addCapacity(_size + 1);
        }
        std::copy_backward(_data.get(), _data.get() + _size, _data.get() + _size + 1);
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
        if (_check) {
            *_sum -= _data[0];
            *_mean = *_sum / (_size - 1);
        }
        std::copy(_data.get() + 1, _data.get() + _size, _data.get());
        _size--;
    }

    void insert(iterator pos, const Type &value) {
        size_t index = pos - begin();
        if (_size == _capacity) {
            addCapacity(_size + 1);
        }
        std::copy_backward(_data.get() + index, _data.get() + _size, _data.get() + _size + 1);
        _data[index] = value;
        _size++;
        if (_check) {
            *_sum += value;
            *_mean = *_sum / _size;
        }
    }

    void erase(iterator pos) {
        size_t index = pos - begin();
        if (index >= _size) {
            throw std::runtime_error("Error: index out of range.");
        }
        if (_check) {
            *_sum -= _data[index];
            *_mean = *_sum / (_size - 1);
        }
        std::copy(_data.get() + index + 1, _data.get() + _size, _data.get() + index);
        _size--;
    }

    void erase(iterator first, iterator last) {
        size_t first_idx = first - begin();
        size_t last_idx = last - begin();
        if (last_idx > _size) {
            throw std::runtime_error("Error: index out of range.");
        }
        size_t count = last_idx - first_idx;
        if (_check) {
            for (size_t i = first_idx; i < last_idx; ++i) {
                *_sum -= _data[i];
            }
            *_mean = *_sum / (_size - count);
        }
        std::copy(_data.get() + last_idx, _data.get() + _size, _data.get() + first_idx);
        _size -= count;
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
            std::copy(_data.get(), _data.get() + _size, res_vector._data.get());
            std::copy(other._data.get(), other._data.get() + other._size,
                     res_vector._data.get() + _size);
            return res_vector;
        }
        if (other._size != _size) {
            throw std::runtime_error("Error: vectors are not the same size.");
        }
        enhanced_vector res_vector;
        res_vector._size = _size;
        res_vector._capacity = _capacity;
        res_vector._check = _check;
        res_vector._data = std::make_unique<Type[]>(_capacity);
        res_vector._sum = std::make_unique<Type>(*_sum + *other._sum);
        for (int i = 0; i < _size; i++) {
            res_vector._data[i] = _data[i] + other._data[i];
        }
        res_vector._mean = std::make_unique<Type>(*res_vector._sum / res_vector._size);
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
        res_vector._data = std::make_unique<Type[]>(_capacity);
        res_vector._sum = std::make_unique<Type>(*_sum - *other._sum);
        for (int i = 0; i < _size; i++) {
            res_vector._data[i] = _data[i] - other._data[i];
        }
        res_vector._mean = std::make_unique<Type>(*res_vector._sum / res_vector._size);
        return res_vector;
    }

    Type operator*(const enhanced_vector &other) const {
        if (other._size != _size) {
            throw std::runtime_error("Error: vectors are not the same size.");
        }
        if (!_check) {
            throw std::runtime_error("Error: invalid operation.");
        }
        Type res{};
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
        std::copy(other._data.get(), other._data.get() + other._size,
                 _data.get() + _size);
        _size += other._size;
        return *this;
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
        auto new_data = std::make_unique<Type[]>(new_size);
        std::copy(_data.get(), _data.get() + std::min(_size, new_size), new_data.get());
        _data = std::move(new_data);
        _size = new_size;
        _capacity = new_size;
    }

    void resize(const int &new_size, const Type &value) {
        auto new_data = std::make_unique<Type[]>(new_size);
        std::copy(_data.get(), _data.get() + std::min(_size, new_size), new_data.get());
        if (new_size > _size) {
            std::fill(new_data.get() + _size, new_data.get() + new_size, value);
        }
        _data = std::move(new_data);
        _size = new_size;
        _capacity = new_size;
    }

    void reserve(const int &new_capacity) {
        if (new_capacity <= _capacity) return;

        auto new_data = std::make_unique<Type[]>(new_capacity);
        std::copy(_data.get(), _data.get() + _size, new_data.get());
        _data = std::move(new_data);
        _capacity = new_capacity;
    }

    void shrink_to_fit() {
        if (_size == _capacity) return;

        auto new_data = std::make_unique<Type[]>(_size);
        std::copy(_data.get(), _data.get() + _size, new_data.get());
        _data = std::move(new_data);
        _capacity = _size;
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
        size_t new_size = std::distance(first, last);
        int tmp = 5;
        while ((1 << tmp) <= new_size) {
            tmp++;
        }
        _capacity = 1 << tmp;
        _data = std::make_unique<Type[]>(_capacity);
        _size = new_size;
        std::copy(first, last, _data.get());
    }

    void assign(const int &size, const Type &value) {
        int tmp = 5;
        while ((1 << tmp) <= size) {
            tmp++;
        }
        _capacity = 1 << tmp;
        _data = std::make_unique<Type[]>(_capacity);
        _size = size;
        std::fill(_data.get(), _data.get() + _size, value);
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
    std::unique_ptr<Type[]> _data;
    std::unique_ptr<Type> _sum;
    std::unique_ptr<Type> _mean;

    void addCapacity(const int &new_size) {
        auto temp_data = std::make_unique<Type[]>(_size);
        std::copy(_data.get(), _data.get() + _size, temp_data.get());

        int tmp = 5;
        while ((1 << tmp) <= new_size) {
            tmp++;
        }
        _capacity = (1 << tmp);

        _data = std::make_unique<Type[]>(_capacity);
        std::copy(temp_data.get(), temp_data.get() + _size, _data.get());
    }
};

} // enhanced

#endif //ENHANCED_VECTOR_HPP