#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<string.h>

//�������ѡ��
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

//������Ա��Ϣ�ṹ
typedef struct PersonInfo
{
	char name[MAX_NAME_SIZE];
	char sex[3];
	char tel[12];
	char addr[MAX_ADDR_SIZE];
	int age;
}PersonInfo;

//����ͨѶ¼
typedef struct Contact
{
	PersonInfo* data;//�������
	size_t size; //��ǰ������
	size_t capacity; //������
}Contact;

//��ʼ��ͨѶ¼
void InitContact(Contact* pct);

//��������
void AddContact(Contact* pct);

//��ʾͨѶ¼
void ShowContact(Contact* pct);

//����ͨѶ¼
void FindContact(Contact* pct);

//ɾ������
void DelContact(Contact* pct);

//�޸�����
void ModifyContact(Contact* pct);

//�������
void ClearContact(Contact* pct);

//��������
void SortContact(Contact* pct);

//�ݻ�ͨѶ¼
void DestroyContact(Contact* pct);

//����ͨѶ¼
void LoadContact(Contact* pct);

//����ͨѶ¼
void SaveContact(Contact* pct);

//�������
void CheckCapacity(Contact* pct);

//����ͨѶ¼
void SaveContact(Contact* pct);

//����ͨѶ¼
void LoadContact(Contact* pct);