Jsoncpp基本应用认识

​	Json::Value类：是json与外界进行数据中转的一个对象类，重载了大量的操作符：’=’、’[]’、’+’等

​	Json::Reader类：实现反序列化，将json格式字符串转换为多个数据对象，存储在Json::Value

​		接口：`int parse(std::string str, Json::Value& val);`

​        要将获得的val数据对象，要通过接口将其转换为指定的类型

​	Json::Writer类（基类）：实现序列化，将Json::Value对象中的数据序列化为json格式的字符串

​		子类：Json::FastWriter / Json::StyledWrite

​		接口： `std::string write(Json::Value& val)`实现序列化

`

```c++
#include <iostream>
#include <jsoncpp/json/json.h>
#include <string>

//序列化
std::string seria()
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
	std::string str = writer.write(val);
	std::cout << writer.write(val) << std::endl;
	return str;
}

void unseria(std::string& str)
{
	Json::Value val;
	Json::Reader reader;
	bool ret = reader.parse(str, val);
	if (ret == false)
	{
		std::cout<< "parse json failed \n";
		return ;
	}
	std::cout << val["姓名"].asString() << std::endl;
	std::cout << val["性别"].asString() << std::endl;
	std::cout << val["学号"].asInt() << std::endl;
	
	//获取数组的长度
	int num = val["成绩"].size();
	for (int i = 0; i < num; ++i)
	{
		std::cout << val["成绩"][i].asInt() << std::endl;
	}
}

int main()
{
	std::string str = seria();
	unseria(str);

	return 0;
}
```
编译链接：`g++ -std=c++11 json.cpp -o json_test -ljsoncpp`

运行结果：

![image-20210614140706293](C:\Users\45431\AppData\Roaming\Typora\typora-user-images\image-20210614140706293.png)