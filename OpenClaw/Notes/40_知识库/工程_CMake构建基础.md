# 工程_CMake构建基础

**来源**: 笔录 2026-02-04/2026-02-05《C++ 项目学习》

#C++ #CMake #Build #工程

---

# C++ 项目构建基础（CMake）

## 核心知识点

### CMake 基本命令
```bash
cmake -S . -B build     # 配置项目（生成 Makefile/Ninja 文件）
cmake --build build     # 编译
cmake --install build   # 安装
```

### CMakeLists.txt 核心语法
```cmake
cmake_minimum_required(VERSION 3.20)
project(MyProject LANGUAGES CXX)

add_executable(myapp main.cpp utils.cpp)
target_include_directories(myapp PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(myapp PRIVATE ${CMAKE_DL_LIBS})
```

### 常见问题
- `namespace` 爆红：检查 CMake 是否正确包含所有源文件
- 找不到头文件：确认 `include_directories` 或 `target_include_directories`
- `error: 'xxx' was not declared`：头文件 inclusion 顺序问题

### 编译错误处理
- `error C2065: undeclared identifier`：检查变量/函数声明
- `error LNK2001: unresolved external symbol`：检查链接库是否正确添加
- `warning: implicit int to float conversion`：数值类型转换问题

