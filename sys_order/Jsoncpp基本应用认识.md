Jsoncpp基本应用认识

​	Json::Value类：是json与外界进行数据中转的一个对象类，重载了大量的操作符：’=’、’[]’、’+’等

​	Json::Reader类：实现反序列化，将json格式字符串转换为多个数据对象，存储在Json::Value

​	Json::Writer类（基类）：实现序列化，将Json::Value对象中的数据序列化为json格式的字符串

​		子类：Json::FastWriter / Json::StyledWrite

​		接口： `std::string write(Json::Value& val)`实现序列化

`

```c++
#include <iostream>
#include <jsoncpp/json/json.h>
#include <string>

//序列化
void seria()
{
	const char* name = "WhiteShirtI";
	int id = 10001;
	const char* sex = "man";
	int score[3] = {99, 89, 98};
    Json::Value val;
    val["姓名"] = name;
    val["学号"] = id;
    val["性别"] = sex;
    for (int i = 0; i < 3; ++i)
    {
        val["成绩"].append(score[i]);
    }
    Json::StyledWriter writer;
    //Json::FastWriter writer;//输出结果全在一行中
    std::cout << writer.write(val) << std::endl;
}

int main()
{
	seria();

	return 0;

}
```
编译链接：`g++ -std=c++11 json.cpp -o json_test -ljsoncpp`

运行结果：

![image-20210610153246978](C:\Users\45431\AppData\Roaming\Typora\typora-user-images\image-20210610153246978.png)