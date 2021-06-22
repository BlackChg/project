# 项目介绍
## 项目名称
点餐系统
## 项目简介
用户可以通过浏览器与后台服务器进行交互，实现用户查看菜品信息以及下单功能，以及管理员对菜品和订单的管理功能
## 开发环境
Linux
## 项目开发语言及工具
C++、Makefile、vim、g++、Json、mysql、http、线程、html

## 框架
相类似的MVC框架：其中业务模块是管理数据的，如菜品信息和菜品订单，外界想要访问数据必须通过这个模块来完成。用户界面模块也就是浏览器前端界面，用户或者管理员的操作都是通过这个界面来完成的。业务控制模块主要是针对页面请求完成相对应的业务处理
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210610130923709.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ0NDQzOTg2,size_16,color_FFFFFF,t_70)
## 项目实现概要
**数据管理模块**：基于mysql数据库实现数据存储管理，并且封装数据库访问类向外提供与业务分离的数据信息

**业务处理模块**：基于http协议使用httplib库搭建http服务器，与前端进行通信，并且采用RESTful风格的网络接口设计来实现与前端动态数据的交互，如：菜品信息、菜品数据、订单数据等进行交互

**前端界面模块**：基于简单的html与css和vue.js以及ajax实现前端界面的静态展示以及动态数据获取渲染功能
## 代码实现概要
代码：[点餐系统]()
**一 ) 业务数据管理模块**
1、数据存储工具：mysql

2、数据库的设计：两张表，其中一张是菜品信息表，该表字段有菜品id、菜品名称、菜品单价、添加时间。另一张是订单表，该表的字段有订单id、订单菜品、订单状态、修改时间

3、数据管理代码设计：主要有两个类，一个是菜品数据类、一个是订单数据类。在这两个类中都提供对应的增删改查的接口，其中菜品数据类的查操作又分为两种，一种是获取单个菜品，一种是获取所有菜品。而订单数据类中的改操作也分为两种，一种是修改订单中的菜品，一种是修改当前订单的状态。查操作一种也是获取所有订单，另一种是获取单个订单

**二) 业务控制模块**
该模块主要是从网页获取客户端的请求，对该请求处理后反馈给用户

1、搭建服务器：HTTP-TCP服务器（采用httplib库搭建）

2、CS通信接口设计：指定的请求对应的是指定的业务处理和响应。例如获取所有订单`GET / orders HTTP/1.1`
其中客户端的请求分为静态页面请求和动态数据请求
2.1、静态页面请求主要是通过html页面来实现（以及依赖的css/js文件）
菜品信息主页面----index.html。对应的客户端所发送的请求：`GET /index.html HTTP/1.1`。而服务端对该请求的响应：首行`HTTP/1.1 200 OK`、响应头部`Content-Type:text/html`、正文`/index.html`文加数据
2.2、动态数据请求主要有菜品数据和订单数据，其通信接口设计采用RESTful的设计风格：这里采用Json格式定义正文序列化方式
定义的操作类型：添加-POST、删除-DELETE、修改-PUT、获取-GET
例如：添加菜品----客户端请求格式`POST /dish HTTP/1.1`正文：{"name": "鱼香肉丝", "price": 15}；对应的服务端响应格式`HTTP/1.1 200 OK`或者添加失败时`HTTP1.1 500` 正文：{"result":"false", "reason":",mysql error"}。后面的操作以及订单数据操作都类似

**三) 前端界面模块**
主要是对html的代码编写及渲染
1、html代码实现：html+css+js，html主要完成页面框架、css主要是对页面框架进行渲染、js主要完成页面的动态渲染

# 运行结果
**index.html页面**
主页面
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210621221619239.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ0NDQzOTg2,size_16,color_FFFFFF,t_70)
选择需要点的菜品
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210621221724816.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ0NDQzOTg2,size_16,color_FFFFFF,t_70)
点击下单
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210621221807275.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ0NDQzOTg2,size_16,color_FFFFFF,t_70)
后台数据库得到更新

**admin.html页面**
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021062122200338.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ0NDQzOTg2,size_16,color_FFFFFF,t_70)
添加菜品
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210621222047174.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ0NDQzOTg2,size_16,color_FFFFFF,t_70)
添加成功
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210621222107890.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ0NDQzOTg2,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210621222126482.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ0NDQzOTg2,size_16,color_FFFFFF,t_70)
