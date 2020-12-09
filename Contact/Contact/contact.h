#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<string.h>

//定义操作选项
typedef enum
{
	QUIT,
	ADD,
	DEL,
	FIND,
	MODIFY,
	SHOW,
	CLEAR,
	SORT
}OPER_ENUM;


#define DEFAULT_CONTACT_SIZE 2
#define MAX_NAME_SIZE 20
#define MAX_ADDR_SIZE 256

//定义人员信息结构
typedef struct PersonInfo
{
	char name[MAX_NAME_SIZE];
	char sex[3];
	char tel[12];
	char addr[MAX_ADDR_SIZE];
	int age;
}PersonInfo;

//定义通讯录
typedef struct Contact
{
	PersonInfo* data;//存的数据
	size_t size; //当前数据量
	size_t capacity; //总容量
}Contact;

//初始化通讯录
void InitContact(Contact* pct);

//增加数据
void AddContact(Contact* pct);

//显示通讯录
void ShowContact(Contact* pct);

//查找通讯录
void FindContact(Contact* pct);

//删除数据
void DelContact(Contact* pct);

//修改数据
void ModifyContact(Contact* pct);

//清除数据
void ClearContact(Contact* pct);

//排序数据
void SortContact(Contact* pct);

//摧毁通讯录
void DestroyContact(Contact* pct);

//加载通讯录
void LoadContact(Contact* pct);

//保存通讯录
void SaveContact(Contact* pct);

//检查容量
void CheckCapacity(Contact* pct);

//保存通讯录
void SaveContact(Contact* pct);

//加载通讯录
void LoadContact(Contact* pct);