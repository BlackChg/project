从github上获得httplib库

`git clone https://github.com/yhirose/cpp-httplib.git`

功能：主要是搭建http服务器或客户端，实现了http协议网络通信，使用库的目的就是把中心当到了业务处理上而不是服务器搭建上

测试：

	#include "httplib.h"
	
	void hello(const httplib::Request& req, httplib::Response& rsp)
	{
		std::cout << req.method << std::endl;	
		std::cout << req.path << std::endl;	
		std::cout << req.body << std::endl;	
		auto it = req.headers.begin();
		for (; it != req.headers.end(); ++it)
		{
			std::cout << it->first << " = " << it->second << std::endl;
		}
		it = req.params.begin();
		for (; it != req.params.end(); ++it)
		{
			std::cout << it->first << " = " << it->second << std::endl;
		}
	    rsp.status = 200;
	    rsp.body = "<html><body><h1>Hello WhiteShirtI</h1></body></heml>";
	    rsp.set_header("Content-Type", "text/html");
	
	    return ;
	}
	
	int main()
	{
		httplib::Server srv;
		//get请求方法
		srv.Get("/hello", hello);
		srv.listen("0.0.0.0", 8080);
		return 0;
	}
运行结果：

​	![image-20210614221644442](C:\Users\45431\AppData\Roaming\Typora\typora-user-images\image-20210614221644442.png)

![image-20210614221702783](C:\Users\45431\AppData\Roaming\Typora\typora-user-images\image-20210614221702783.png)

## httplib工作流程

httplib中主要有三个类

​	httplib::Server

​	httplib::Request

​	httplib::Response

Server类中主要有的成员是一个map类型的请求与处理的路由表`map<pair<string,string>, function> route`

![image-20210616153615251](C:\Users\45431\AppData\Roaming\Typora\typora-user-images\image-20210616153615251.png)

该表的主要功能就是完成请求方式资源路径和对应的处理函数的映射关系，其中key是由请求方式和资源路径组成的，而val就是对应的指向处理函数的指针

接口listen("0.0.0.0", 8080)主要是搭建了一个能够监听当前主机多有IP，主机号为8080的TCP服务器，



当有服务端收到一个客户端的连接时，会将新建立的连接抛入到线程池中，而线程池中的线程负责与指定的客户端进行通信

![image-20210616154237664](C:\Users\45431\AppData\Roaming\Typora\typora-user-images\image-20210616154237664.png)

**线程工作流程：**

 1. 接收请求数据，按照http请求协议格式进行解析（请求方法、资源路径、查询字符串、头部字段、正文等等...）

 2. 实例化httplib::Request对象，然后将解析得到的信息填入到该对象中

    ![image-20210616155854645](C:\Users\45431\AppData\Roaming\Typora\typora-user-images\image-20210616155854645.png)

    

3. 根据请求信息，在route路由表中查找针对这个请求有没有对应的处理函数，如果没有则直接返回404，表示资源不存在，如果有这个路径并且有对应的处理函数，则执行对应的处理函数，传入求情信息，并实例化一个空的httplib::Response对象，将其传入到处理函数中，然后针对用户的请求完成对应的业务处理后，将对应的响应结果信息填充到Response对象中

   class Response
   {
       int status; //响应状态码
       string body; //响应正文
       map<string,string> headers;//头部信息
   }

   ![image-20210616161133623](C:\Users\45431\AppData\Roaming\Typora\typora-user-images\image-20210616161133623.png)

4. 线程执行完处理函数之后，就得到了一个填充完毕的Response对象，然后根据该对象的填充后的属性组织http响应协议格式的数据，然后回复给客户端

   ![image-20210616161745047](C:\Users\45431\AppData\Roaming\Typora\typora-user-images\image-20210616161745047.png)

5. 最后一步是等待下一个http请求，如果有则处理，没有就关闭连接

**主要接口介绍**

​	注册路由表成员函数：Get()、Post()、Put()、Delete()

​	设置静态资源默认路径：set_base_dir(char* path);设置这个路径之后，当前端在请求静态资源的时候，就会自动先到该path路径下查找有没有对应请求的静态资源文件，如果有则自动读取文件数据并进行回复

演示`set_base_dir`接口，我们先将百度首页的html代码拷贝到我们指定的目录下

![image-20210616164052230](C:\Users\45431\AppData\Roaming\Typora\typora-user-images\image-20210616164052230.png)

然后在服务器代码中使用该接口

srv.set_base_dir("./wwwroot");

此时我们访问时，服务器会自动到该wwwroot目录下查找资源，当我们请求的资源路径后面不写时，其默认访问的就是指定目录下的index.html静态资源文件

![image-20210616164252442](C:\Users\45431\AppData\Roaming\Typora\typora-user-images\image-20210616164252442.png)

在注册路由表中，假设我们要删除一条数据时要就要在请求路径加上指定删除的数据的id，我们可以借助正则表达式来解决这个问题，使用/d+，功能就是匹配一个数字字符或多个数字字符，再利用C++11中R"()"将请求路径包含其中，表示括号中的字符串去除了特殊字符的特殊含义，再将正则表达式中的/d+用括号括起来，表示捕捉匹配到的字符串，其中捕捉的字符串会存在特定的数组中

如：`srv.Delete(R"(/dish/(\d+))", DishDelete);` 表示当发送dish/id时表示该请求的处理函数为DishDelete，让后通过httplib库封装了C++11中获取捕捉字符串的数组，matches[]来获取，其第0个元素是表示前面的路径，第一个元素就是我们捕捉到的字符数字，我们再可以通过stoi来将其转换为数字来操作

