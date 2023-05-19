### 一、当你拥有`new`出来的成员变量时，一定要自己定义拷贝构造函数和赋值运算符重载函数

`vector.push_back(Json())`会调用到拷贝构造函数

`map[key]=Json("hello world")`会先调用默认构造函数，然后再调用赋值运算符重载函数

如果你没有定义的话，编译器会自动为你生成，但它们都是浅拷贝的，这会导致`double delete`

### 二、需要将字符串字面量指针传入函数时，一定要定义一个形参为`const char*`的版本

当`Json(const bool value)`和`Json(const string& value)`同时存在时，调用`Json("Hello world!")`会调用到`Json(const bool value)`。

如果你调用的是`Json("Hello world!"s)`的话，也是可以优先调用到`Json(const string& value)`的辣，但这样就不太安全。

所以我们应该定义多一个`Json(const char* value)`，使得字符串字面量指针不需要经过转换就能调用到我们指定的对应构造函数。

### 三、抛出异常时，要连同`__FILE__`和`__LINE__`一起抛出，这才能有效定位错误

### 四、释放空间后，将指针置为`nullptr`。释放空间前，先判断指针是否为`nullptr`。这样可以有效防止`double delete`

### 五、设计完头文件后，就可以编写单元测试代码了。每通过一个单元测试后，实现下一单元测试所需方法前，要做好文件备份

