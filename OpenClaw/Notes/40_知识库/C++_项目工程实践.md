# C++ 项目工程实践

**来源**: 笔录 2026-02-27/28《工程-C++学习》

#C++ #工程实践 #源码阅读

---

## 核心知识点

### 项目结构扫描方法

面对一个陌生 C++ 代码库，按以下顺序阅读：

```
1. README / BUILD.md          # 项目说明，构建方式
2. CMakeLists.txt / Makefile  # 构建系统入口
3. 头文件 (.h / .hpp)         # 数据结构定义
4. main.cpp / main.cc         # 入口点
5. 核心模块 (.cpp)            # 业务逻辑
```

**快速了解架构的命令**：
```bash
# 目录结构
tree -L 2 src/

# 头文件依赖
find . -name "*.h" | head -20

# 统计代码行数
find . -name "*.cpp" -o -name "*.cc" | xargs wc -l
```

### 编译与构建

**常见构建系统**：
- **CMake**: 最通用，跨平台（Windows/Linux/macOS）
- **Make**: 直接，适合小型项目
- **Ninja**: 速度快，配合 CMake 使用

**典型 CMakeLists.txt 结构**：
```cmake
cmake_minimum_required(VERSION 3.16)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
add_executable(myapp main.cpp src/module.cpp)
target_include_directories(myapp PRIVATE ${CMAKE_SOURCE_DIR}/include)
```

### C++ 异常处理

**基本语法**：
```cpp
try {
    throw std::runtime_error("error message");
} catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
}
```

**关键规则**：
- 异常沿调用栈向上传播，直到被 `catch` 捕获
- `catch(...)` 捕获所有异常
- 构造函数的异常：在异常抛出前，对象的成员已构造完成，基类构造同理

### 单元测试框架

**Google Test（gtest）示例**：
```cpp
#include <gtest/gtest.h>

TEST(MySuite, TestCase) {
    EXPECT_EQ(add(1, 2), 3);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

---

## 来源笔录

- 2026-02-27/28《工程-C++学习》系列
