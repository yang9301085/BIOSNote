### `find_first_not_of` & `find_last_not_of`
函数:从左往右找第一个不在集合里的字符,默认从开头开始
`find_last_not_of` 同理
```C++
std::size_t std::string::find_first_not_of(char __c, std::size_t __pos = 0U) const noexcept
```
 **+4 overloads**
 ```C++
 // 1) 传一个 std::string（字符集合）
size_type find_first_not_of(const std::string& str,
                            size_type pos = 0) const;

// 2) 传一个 C 字符串 + 起始位置 + 字符数（字符集合来自 s[0..count-1]）
size_type find_first_not_of(const char* s,
                            size_type pos,
                            size_type count) const;

// 3) 传一个 C 字符串 + 起始位置（字符集合来自以 '\0' 结尾的 s）
size_type find_first_not_of(const char* s,
                            size_type pos = 0) const;

// 4) 传一个字符（只跳过这个字符）
size_type find_first_not_of(char ch,
                            size_type pos = 0) const;

 ```
**Parameters:**  
- `__c` – Character to avoid.  
- `__pos` – Index of character to search from (default 0).
**Returns:**  
- Index of first occurrence.
- Starting from __pos, searches forward for a character other than __c within this string. If found, returns the index where it was found. If not found, returns npos.
eg:
1. 接受char类型参数，单个字符
```C++
std::string CommandParser::trimCopy(const std::string& inputLine){
    std::string::size_type left=0;
    std::string::size_type right=0;
    left=inputLine.find_first_not_of(' ');
    right=inputLine.find_last_not_of(' ');
    return inputLine;
}
```
2. 字符集合
```C++
std::string CommandParser::trimCopy(const std::string& inputLine){
    std::string::size_type left=0;
    std::string::size_type right=0;
    const char* whitespaceSet=" \t\r\n";
    left=inputLine.find_first_not_of(whitespaceSet);
    right=inputLine.find_last_not_of(whitespaceSet);
    return inputLine;
}
```

---

### `substr`

`inline std::string std::string::substr(std::size_t __pos, std::size_t __n) const`

Get a substring.

**Parameters:**  
- `__pos` – Index of first character (default 0).  
- `__n` – Number of characters in substring (default remainder).

**Returns:**  
The new string.

**Exceptions:**  
- std::out_of_range If __pos > size().
- Construct and return a new string using the __n characters starting at __pos. If the string is too short, use the remainder of the characters. If __pos is beyond the end of the string, out_of_range is thrown.

eg:
```C++
std::string CommandParser::trimCopy(const std::string& inputLine){

    std::string::size_type left=0;
    std::string::size_type right=0;
    std::string::size_type len=0;
    const char* whitespaceSet=" \t\r\n";
    left=inputLine.find_first_not_of(whitespaceSet);
    if(left==std::string::npos){
        return "";
    }
    right=inputLine.find_last_not_of(whitespaceSet);
    len=right-left+1;
    return inputLine.substr(left,len);
}
```

---
`std::string::size_type` 是 `std::string`内部定义的一个**类型别名**

用途：专门用来表示 `std::string` 的长度、下标、`find` 返回值这种“位置”

关键点：

- **无符号整数**（unsigned）
    
- 位宽和平台有关（32 位通常 32bit，64 位通常 64bit）
    
- 你不用关心它具体是 `unsigned int` 还是 `unsigned long long`，反正它就是“字符串用的下标类型”。
---

`size_t`是 C/C++ 里表示“**对象大小（bytes）**”的标准类型

- 定义在 `<cstddef>`（C 里是 `<stddef.h>`）。
    
- 用途：`sizeof` 的结果就是 `size_t`，以及很多容器的 `size()` 返回的也是 `size_t` 或兼容类型。

关键点：

- **无符号整数**
    
- 平台相关：32 位上通常 32bit，64 位上通常 64bit
    
- 用来表达“大小”和“计数”，非常常见。

很多实现里：`std::string::size_type` **通常就是** `size_t`。  
但标准层面只保证：

- `std::string::size_type` 能表达字符串的大小和下标
    
- `size_t` 能表达内存大小

**`size_t` 是“大小计数通用型”，`std::string::size_type` 是“string 专用下标型”**

