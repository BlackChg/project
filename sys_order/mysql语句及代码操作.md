登录：`mysql -uroot -ppasswd`

注意事项：

​	1、sql语句都要以;结尾

​	2、sql语句不区分大小写

​	3、sql语句中，数据库名称、表名称、字段名称不能世界使用sql关键字，如果使用则加上两个单引号



# sql语句操作

**数据库操作**

​	创建数据库：`create database if not exists dbname;`

​	查看所有数据库：`show databases;`

​	选择要操作的数据库`use dbname;`

​	查看当前所操作的数据库：`select database();`

​	删除一个数据库`drop database dbname;`

**表操作**

​	常用数据类型：`int`整形、`decimal(数字长度, 小数位数)`浮点型、`varchar(字符长度)`字符串型、`datetime`日期、`bit`值为1或0

​	创建表：`create table if not exists tbname(id int, name varchar(20), score decimal(3,1), birth datetime);`

​	查看所有表：`show tables;`

​	查看指定表的结构：`desc tbname;`

​	删除指定表：`drop table tbname;`

​	表中字段的约束：

​		`primary key`主键约束 约束的指定字段的值非空且唯一

​		`unique key`唯一约束 约束指定字段必须唯一(使用不需要加key)

​		`not null`非空约束 约束指定字段的值不能为空

​		`auto_increment`自增属性（只能用于整形的主键字段）

​		如：`create table if not exists tbname(id int primary key auto_increment, name varchar(20) not null unique, sex bit, score decimal(3,1), birth datetime);`

**表数据的操作**

​	增：

​		添加一个数据的部分字段：`insert tbname(id, name, sex) values(null, "whiteshirti", 0);`

​		添加一个数据的所有字段：`insert tbname values(null, "aGuang", 0, 92.5, "1999-9-23 6:00:00");`

​	删：

​		删除数据：`delete from tbname;`删除所有数据

​		删除指定数据：`delete from tbname where id = 2;`删除id为2的数据

​	改：

​		修改所有数据：`update tbname set name="yaNan", birth="1999-12-20 18:00:00";`将所有数据的name字段改为yaNan，所									有birth字段都改为"1999-12-20 18:00:00"

​		修改指定数据：`update tbname set name="yaNan", birth="1999-12-20 18:00:00" where id = 2;`

​	查：

​		查询表中所有数据的所有字段：`select * from tbname;`

​		指定列查询：`select id, name, score from tbname;`

​		条件查找：`select * from tbname where id=4;`

​		按序查找：`select id, name score from tbname order by id desc;`默认为升序，加上`desc`则为降序

​		分页查找：`select * from tbname limit 3 offset 2;`limit 3表示只显示3行数据，offset表示先偏移2个数据在查找

**数据库语句导入操作**

​	`mysql -uroot -ppasswd < db.sql`将db.sq文件l里的语句都会执行一遍，便于管理



# 使用代码连接和操作数据库

## 初始化有关接口

**初始化mysql句柄**

​	`MYSQL *mysql_init(MYSQL *mysql)`

​	参数：mysql为要操作数据库的操作句柄，这里通常置位NULL

​	返回值：成功返回初始化好了的数据库操作句柄，失败返回NULL

**通过操作句柄连接mysql服务器**

​	`	MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user, const char *passwd,`

`  const char *dbname, unsigned int port, const char *unix_socket, unsigned long client_flag)`
	参数：mysql为操作数据库的操作句柄；host为连接的mysql服务器的地址；user为数据库用户名；passwd为数据库的用户密码；dbname为默认选择的数据库名称；port为mysql服务端的端口，默认为0，表示3306端口；unix_socket为通信管道文件或者socket文件，通常置NULL；client_flag为客户端标志位，通常置0

​	返回值：成功返回数据库句柄，失败返回NULL

**设置当前客户端的字符集**

​	`int mysql_set_character_set(MYSQL *mysql, const char *csname)`

​	参数：mysql为数据库的操作句柄；csname为字符集的名称，通常置为“utf8”

​	返回值：成功返回0，失败返回非0

**选择操作的数据库**

​	`int mysql_select_db(MYSQL *mysql, const char *db)`

​	参数：mysql为数据库的操作句柄；db为要切换选择的数据库名称

​	返回值：成功返回0，失败返回非0

## 操作数据有关接口

注：1,2要加锁保护，其操作并非原子操作，存在线程安全

**执行sql语句**

​	`int mysql_query(MYSQL *mysql, const char *sql)`

​	参数：mysql为数据库的操作句柄；sql为要执行的sql语句

​	返回值：成功返回0，失败返回非0

**保存查询结果到本地**

​	`MYSQL_RES *mysql_store_result(MYSQL *mysql)`

​	参数：mysql为数据库的操作句柄；

​	返回值：成功返回结果集的指针，失败返回NULL

**获取结果集中的行数与列数**

​	`uint64_t mysql_num_rows(MYSQL_RES *result)`

​	参数：result为结果集指针

​	返回值：返回结果集中的数据条数

​	`unsigned int mysql_num_fields(MYSQL_RES *result)`

​	参数：result为结果集指针

​	返回值：返回结果集中每条数据的列数

**遍历结果集**

​	`MYSQL_ROW mysql_fetch_row(MYSQL_RES *result)`

​	参数：result为结果集指针

​	返回值：返回值为字符数组，数组的每个元素都是一个字符串，通过row[0]获得某条数据第0列的数据

​	注：这个接口会保存当前读取结果位置，每次获取的都是下一条数据

**释放结果集**

​	`void mysql_free_result(MYSQL_RES *result) `

​	参数：result为结果集指针

​	返回值：void

**关闭数据库客户端连接，销毁句柄**

​	 `void mysql_close(MYSQL *mysql)`

​	参数：mysql为数据库的操作句柄

​	返回值：void

**获取mysql接口执行错误原因**

 	`const char *mysql_error(MYSQL *mysql)`

​	参数：mysql为数据库的操作句柄

​	返回值：执行错误的原因