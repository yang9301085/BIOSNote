- 异常处理会把控制流沿着调用栈向上转移
- 要让一个异常被捕获（caught），抛出异常的 `throw` 表达式必须位于某个 `try` 块之内，并且该 `try` 块必须包含一个能够匹配该异常对象类型的处理器（`catch` 子句）。
- **抛出（throw）**：当代码执行到某处发现无法正常继续时，可以抛出一个异常（本质上是“被抛出的对象/值”）。抛出会把控制流转移到能够处理它的地方
- **捕获（catch）**：运行时会沿调用栈向上寻找第一个能匹配该异常类型的 `catch` 处理器，然后把控制权交给它
- **栈展开（stack unwinding）**：从 `throw` 到匹配的 `catch` 之间，所有已构造完成的 **自动对象（栈对象)** 会按作用域逆序析构。这就是 RAII 能在异常情况下自动释放资源的根
- 最小语法：
```cpp
try {
    // 可能失败的代码
    throw std::runtime_error("boom");
} catch (const std::exception& e) {
    // 兜底处理
}
```

**异常处理会把控制流沿着调用栈向上转移**

比如：
```cpp
main()
  -> A()
     -> B()
        -> C()
```
当程序跑到 `C()` 的时候，栈里大概是：
- 栈顶：`C()` 的现场（局部变量、返回地址）
- 下面：`B()` 的现场
- 再下面：`A()` 的现场
- 底部：`main()` 的现场

正常 return 是“按层返回”：
- `C()` return 回 `B()`
- `B()` return 回 `A()`
- `A()` return 回 `main()`
    
每一层都可以在 return 后继续执行下一行代码

有异常时“沿调用栈向上转移”
当 `C()` 里执行 `throw`：
- 程序**不会**继续执行 `C()` 里 `throw` 后面的代码
- 也**不会**回到 `B()` 里 `C()` 调用点的下一行继续执行
- 它会开始“往上找”：  
    **从当前函数开始，沿着调用栈一层层返回，寻找能匹配的 `catch`**

异常往上抛的时候，被“跳过”的那些函数栈帧会被销毁，里面的局部对象会自动析构
也就是所谓 **栈展开（stack unwinding)**

举例：
```cpp
void C() {
    File f("a.txt");     // 假设 File 析构会 close()
    throw MyException{};
    // 这行永远执行不到
}

void B() {
    std::vector<int> v(100);
    C();
    std::cout << "after C\n"; // 这行也执行不到
}

```
当 `C()` throw：
- `C()` 的局部对象 `f` 会析构（自动 close）
- 然后 `C()` 这一层被弹出
- 接着回到 `B()`，但不是继续执行，而是继续往上找 catch
- `B()` 的局部对象 `v` 也会析构
- 直到找到某层 catch 才停止展开

一个完整例子:
```cpp
#include <iostream>
#include <stdexcept>

void C() {
    std::cout << "C: before throw\n";
    throw std::runtime_error("boom");
    std::cout << "C: after throw\n"; // 永远不会执行
}

void B() {
    std::cout << "B: before C\n";
    C();
    std::cout << "B: after C\n"; // 永远不会执行
}

void A() {
    try {
        std::cout << "A: in try\n";
        B();
        std::cout << "A: after B\n"; // 永远不会执行
    } catch (const std::exception& e) {
        std::cout << "A: caught: " << e.what() << "\n";
    }
    std::cout << "A: after catch\n";
}

int main() {
    A();
}
```
输出大概是：
- `A: in try`
- `B: before C`
- `C: before throw`
- `A: caught: boom`
- `A: after catch`

`throw` 直接把控制流从 `C()` 拉到 `A()` 的 `catch`，中间 `B()` 的“after C”完全被跳过

**异常一抛出，下面那些“本来会执行的代码”都不会执行了**

---
C++ 提供了一系列标准的异常，定义在 \<exception\> 中，我们可以在程序中使用这些标准的异常。它们是以父子类层次结构组织起来的

![[Pasted image 20260204175339.png]]

下表是对上面层次结构中出现的每个异常的说明:

| 异常                        | 描述                                                        |
| ------------------------- | --------------------------------------------------------- |
| **std::exception**        | 该异常是所有标准 C++ 异常的父类。                                       |
| **std::bad_alloc**        | 该异常可以通过 **new** 抛出。                                       |
| **std::bad_cast**         | 该异常可以通过 **dynamic_cast** 抛出。                              |
| **std::bad_typeid**       | 该异常可以通过 **typeid** 抛出。                                    |
| **std::bad_exception**    | 这在处理 C++ 程序中无法预期的异常时非常有用。                                 |
| **std::logic_error**      | 理论上可以通过读取代码来检测到的异常。                                       |
| **std::domain_error**     | 当使用了一个无效的数学域时，会抛出该异常。                                     |
| **std::invalid_argument** | 当使用了无效的参数时，会抛出该异常。                                        |
| **std::length_error**     | 当创建了太长的 std::string 时，会抛出该异常。                             |
| **std::out_of_range**     | 该异常可以通过方法抛出，例如 std::vector 和 std::bitset<>::operator[]()。 |
| **std::runtime_error**    | 理论上不可以通过读取代码来检测到的异常。                                      |
| **std::overflow_error**   | 当发生数学上溢时，会抛出该异常。                                          |
| **std::range_error**      | 当尝试存储超出范围的值时，会抛出该异常。                                      |
| **std::underflow_error**  | 当发生数学下溢时，会抛出该异常。                                          |
