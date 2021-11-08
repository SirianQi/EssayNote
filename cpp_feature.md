[toc]



## 内联函数



## lambda表达式

### 1、 lambda解决的问题

lambda表达式解决的是需要获取局部变量，完成一个短小功能，且不希望使用全局函数的问题

### 2、lambda用法

#### 简介

`[capture list](params list) -> return type{function body}`

`[capture list]`捕获到的局部变量参数列表

`(params list)`函数参数列表，如果没有入参，可连括号一起省略

`-> return type`返回类型，可以通过auto推导，可省略

`{function body}`函数体，实现lambda具体功能

##### e.g. 1直接调用

```c++
// 输出局部变量的值
void function(stirng& sz){
  string sz_param = "capture param";
  auto lambda_func = [sz](string s){
    cout << sz << s;
  }
  
  lambda_func(sz);
  return;
}
```

##### e.g.2给到函数调用

```c++
// 查找__first到__last范围内第一个符合_pred要求到元素并返回
bool find_if(_InputIerator __first, _InputIterator __last, _Pedicate __pred){
  for(;__first != __last; ++__first){
    if(_pred(*__first)){
      break;
    }
  }
  return __first;
}
// 查找words中第一个长度大于length的元素并输出
void function(vector<string> &words, max length){
  // 查找
  auto word = find_if(words.begin(), words.end(), [length](const string& word){
    return word.size() > legnth;
  });
  // 输出
  cout << *word;
  return;
}
```

#### 捕获变量

我们平时定义函数参数列表会有这么几种方式：

`int var` 值传递

`int& var`引用传递

 `const int var`常量值传递

`const int& var`常量引用传递

lambda表达式在捕获临时变量时，也会有多种方式。

##### 常量值捕获（显示捕获）

 ```  c++
 string sz = "capture param"; 
 auto lambda_func = [sz](){
   // sz = "alter param"; // 普通场景下为常量值捕获 修改会导致编译失败
   cout << sz;
 }
 lambda_func();
 ```

##### 值捕获（显示捕获）

````c++
string sz = "capture param"; 
auto lambda_func = [sz]()mutable{	// 使用mutable关键字时()不可省略
  sz = "alter param";
  cout << sz;
}
lambda_func();
````

##### 引用捕获（显示捕获）

```c++
string sz = "capture param"; 
auto lambda_func = [&sz](){
  cout << sz << s;
}
lambda_func(); // 引用捕获的lambda在调用时必须保证局部变量没有被销毁，否则就会造成悬垂指针
```

##### 隐式捕获

```` c++
ostream& os = cout;
string sz = "capture param"; 
auto lambda_func1 = [&, sz](){	// os隐式捕获，sz显示捕获
  os << sz;
}
auto lambda_func2 = [=, &os](){	// os显示捕获，sz隐式捕获
  os << sz;
}
lambda_func1();
lambda_func2();
````

**混合使用的情况下，捕获列表的第一个元素必须是 & 或 = ，代表默认的捕获方式。剩下其他的元素就必须与第一个元素不同的捕获方式。以下是详细列表说明：**

| 捕获形式          | 说明                                                         |
| ----------------- | ------------------------------------------------------------ |
| []                | 无需捕获变量                                                 |
| [capture list]    | 显示捕获变量列表                                             |
| [&]               | 隐式捕获变量列表，所有变量采用引用捕获                       |
| [=]               | 隐式捕获变量列表，所有变量采用值捕获                         |
| [&, capture list] | 混合捕获变量列表，所有隐式捕获变量都为引用捕获，所有写显示捕获变量都为值捕获 |
| [=, capture list] | 混合捕获变量列表，所有隐式捕获变量都为值捕获，所有隐式捕获变量都为引用捕获 |

**实践中尽量避免使用隐式捕获，以防由于疏忽造成悬垂指针**

### 3、lambda本质（待补充2021.10.31

lambda 表达式在编译时被翻译成一个未命名的未命名对象，产生的类中含有一个重载的函数调用运算符`operator()`，即仿函数。



## bind方法

## 仿函数

