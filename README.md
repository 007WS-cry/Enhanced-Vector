# Enhanced Vector

## 简介

Enhanced Vector 是一个扩展和修改了C++标准库中`std::vector`功能的容器类，提供了增强的向量操作和数值计算功能。此类库专为学习和实验目的设计，提供了标准`vector`的基本功能，同时添加了一些额外特性。

## 功能特点

### 基本容器操作

- **构造与析构**
    - 默认构造函数：`enhanced_vector(const int &size = 0, const Type &value = Type())`
    - 迭代器区间构造：`enhanced_vector(iterator first, iterator last)`
    - 拷贝构造函数：`enhanced_vector(const enhanced_vector &other)`
    - 移动构造函数：`enhanced_vector(enhanced_vector &&other)`
    - 赋值运算符：`operator=`（拷贝和移动版本）
    - 析构函数：`~enhanced_vector()`

- **元素访问**
    - 下标操作：`operator[]`
    - 首元素访问：`front()`
    - 尾元素访问：`back()`

- **迭代器**
    - 正向迭代器：`begin()`, `end()`
    - 常量迭代器：`cbegin()`, `cend()`
    - 反向迭代器：`rbegin()`, `rend()`
    - 常量反向迭代器：`crbegin()`, `crend()`

- **容量操作**
    - 大小查询：`size()`
    - 容量查询：`capacity()`
    - 判断是否为空：`empty()`
    - 调整大小：`resize(const int &new_size)`, `resize(const int &new_size, const Type &value)`
    - 预留空间：`reserve(const int &new_capacity)`
    - 收缩到适合大小：`shrink_to_fit()`

- **修改操作**
    - 尾部添加元素：`push_back(const Type &value)`
    - 尾部移除元素：`pop_back()`
    - 头部添加元素：`push_front(const Type &value)`
    - 头部移除元素：`pop_front()`
    - 插入元素：`insert(iterator pos, const Type &value)`
    - 删除元素：`erase(iterator pos)`, `erase(iterator first, iterator last)`
    - 清空容器：`clear()`
    - 赋值：`assign(iterator first, iterator last)`, `assign(const int &size, const Type &value)`

### 增强功能

- **数值操作**（仅适用于数值类型）
    - 求和：`sum()`
    - 求平均值：`mean()`

- **向量运算**
    - 向量加法：`operator+`（数值类型执行对应位置元素相加，非数值类型执行拼接）
    - 向量减法：`operator-`（仅适用于数值类型）
    - 向量内积：`operator*`（仅适用于数值类型）
    - 向量拼接：`operator+=`

- **比较操作**
    - 相等比较：`operator==`, `operator!=`
    - 大小比较：`operator<`, `operator<=`, `operator>`, `operator>=`

- **输入输出**
    - 输出流：`operator<<`
    - 输入流：`operator>>`
    - 打印功能：`print(iterator first, iterator last)`

## 与std::vector对比

### 相同点
- 提供了动态数组的基本功能
- 支持随机访问、迭代器操作
- 提供了大部分标准容器所需的成员函数

### 增强功能
1. **双端操作**：除了`push_back`和`pop_back`外，还提供了`push_front`和`pop_front`操作
2. **数值统计**：对数值类型提供了求和和求平均值功能
3. **向量运算**：支持向量加法、减法和内积运算
4. **容量控制**：使用2的幂次方扩容策略
5. **输出格式化**：提供了格式化输出功能

## 优势

1. **功能整合**：结合了`vector`和部分`deque`的功能
2. **数学运算**：内置数值计算功能，适合数据分析场景
3. **数据感知**：自动识别数值类型并提供相应操作
4. **友好接口**：提供了直观的打印和流操作接口

## 不足

1. **性能开销**：
    - `push_front`和`pop_front`操作需要移动所有元素，时间复杂度为O(n)
    - 数值统计功能需要额外存储和计算开销
    - 某些操作可能导致不必要的内存重分配

2. **内存管理**：
    - 使用固定的幂次方扩容策略，可能造成内存浪费
    - `addCapacity`方法中存在内存泄漏问题（临时数组`_temp_data`未正确初始化）

3. **设计限制**：
    - 向量运算仅支持数值类型
    - 缺少一些现代C++容器的高级功能（如`emplace`系列操作）
    - 未实现移动语义优化的插入操作

4. **安全性**：
    - 部分操作缺少边界检查
    - 重载运算符时缺少类型安全检查

## 使用示例

### 方式一：直接包含头文件（仅头文件库）

```cpp
#include "enhanced_vector.hpp"
#include <iostream>

int main() {
    // 创建整数向量
    enhanced::enhanced_vector<int> vec1(5, 10);  // 创建包含5个值为10的元素的向量
    
    // 添加元素
    vec1.push_back(20);
    vec1.push_front(5);
    
    // 打印向量
    std::cout << "vec1: " << vec1 << std::endl;
    
    // 数值计算
    std::cout << "Sum: " << vec1.sum() << std::endl;
    std::cout << "Mean: " << vec1.mean() << std::endl;
    
    // 创建另一个向量
    enhanced::enhanced_vector<int> vec2(vec1.size(), 1);
    
    // 向量运算
    auto vec3 = vec1 + vec2;
    std::cout << "vec1 + vec2: " << vec3 << std::endl;
    
    int dot_product = vec1 * vec2;
    std::cout << "vec1 * vec2 (dot product): " << dot_product << std::endl;
    
    return 0;
}
```

### 方式二：链接静态库

文件中包含`libenhanced.a`，以下是链接和使用该库的示例：

#### 项目结构
```
project/
├── include/
│   └── enhanced_vector.hpp
├── lib/
│   └── libenhanced.a
└── src/
    └── main.cpp
```

#### 编译命令
```bash
# 在项目根目录下执行以下命令编译程序
g++ -o myprogram src/main.cpp -I./include -L./lib -lenhanced -std=c++17
```

#### main.cpp 示例
```cpp
#include <enhanced_vector.hpp>
#include <iostream>
#include <vector>

// 示例：使用enhanced_vector进行数据分析
void analyze_data(const std::vector<double>& raw_data) {
    // 将std::vector数据转换到enhanced_vector
    enhanced::enhanced_vector<double> enhanced_data;
    for (const auto& value : raw_data) {
        enhanced_data.push_back(value);
    }
    
    // 使用enhanced_vector的增强功能
    std::cout << "数据集: " << enhanced_data << std::endl;
    std::cout << "数据量: " << enhanced_data.size() << std::endl;
    std::cout << "数据总和: " << enhanced_data.sum() << std::endl;
    std::cout << "数据平均值: " << enhanced_data.mean() << std::endl;
    
    // 创建另一个数据集进行对比
    enhanced::enhanced_vector<double> baseline(enhanced_data.size(), 1.0);
    
    // 计算差异
    auto difference = enhanced_data - baseline;
    std::cout << "与基准的差异: " << difference << std::endl;
    
    // 计算内积（可用于相似度计算）
    double dot = enhanced_data * baseline;
    std::cout << "与基准的内积: " << dot << std::endl;
}

int main() {
    // 示例数据
    std::vector<double> measurements = {1.2, 3.4, 2.5, 4.6, 3.7, 5.8};
    
    // 分析数据
    analyze_data(measurements);
    
    return 0;
}
```

#### CMakeLists.txt 示例
如果使用CMake构建系统，可以使用以下CMakeLists.txt：

```cmake
cmake_minimum_required(VERSION 3.10)
project(EnhancedVectorDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 指定库文件目录
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 添加可执行文件
add_executable(demo src/main.cpp)

# 链接静态库
target_link_libraries(demo enhanced)
```

## 未来改进方向

1. 修复内存管理问题，特别是`addCapacity`方法中的内存泄漏
2. 优化`push_front`和`pop_front`操作，考虑使用双向缓冲区或环形缓冲区实现
3. 添加更多现代C++容器特性，如`emplace`系列操作
4. 提高类型安全性和边界检查
5. 考虑使用更灵活的内存扩容策略
6. 添加更多数值计算功能（如标准差、中位数等）
7. 实现更完善的异常处理机制