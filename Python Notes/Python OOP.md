# 类 Class：
- 自定义的数据类型
- 定义对象的属性和方法
- 对象是类的实例
```python
# 定义一个类 
class Dog: 
	def __init__(self, name, age): 
		self.name = name 
		self.age = age 
	def bark(self): 
	print(f"{self.name} 正在汪汪叫！")
 
# 创建类的对象 
dog1 = Dog("小白", 3) 
dog1.bark()

```
![[chrome://new-tab-page/static/image/OnboardingAvatar.d955c4df.webp]]

