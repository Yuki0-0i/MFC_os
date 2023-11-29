#ifndef LIAN_BIAO_INCLUDE
#define LIAN_BIAO_INCLUDE

#include<stdio.h>
#include<string>
#include<iostream>
#include<cstdlib>
#include <mutex>
#include <vector> 
#include <windows.h>
#include <random>
#include <thread>
using namespace std;
//实验名称：通讯录管理

typedef struct _contactslist//定义联系人结构，内含姓名，电话号码
{
	string name;//都用了string类型
	string tel;
}CL;
typedef struct _CLNode {//定义节点，内设数据域和指针域
	CL contact;
	struct _CLNode* next;
	struct _CLNode* prev; // 添加指向前一个节点的指针
}Node;//定义节点名Node

struct List {
	Node* head;
	Node* tail;
	int length;
};

extern List mylist;
extern string name;
extern int updPosition;
extern int delPosition;
extern int insertCount1;
extern int delRemain;
extern HANDLE hwriteblock;
extern HANDLE hCoutMutex;
extern vector<string> message;
const int numThreads2 = 15;//指定数量的读者写者的线程

void add(List& mylist);
void search(List& mylist, int Position);
void searchone(List& mylist, string name);
void update(List& mylist, int Position, int threadId);
void updateone(List& mylist, int Position);
void delone(List& mylist, int fixedPosition);
void delone2(List& mylist, int fixedPosition);
void display(List& mylist);//遍历显示整个通讯录及总人数（表长）
void calculate(List& mylist);
void InitializeList(List& mylist);
void menu();
void delall(List& mylist);
void ExecuteThreads(List& mylist, unsigned int(__stdcall* threadFunction)(void*), const char* operationName);
void DeleteInitialization(List& mylist);
void RWInitialization(List& mylist);

unsigned int __stdcall InsertThread(void* arg);
unsigned int __stdcall InsertThread_Safe(void* arg);
unsigned int __stdcall DelThread(void* arg);
unsigned int __stdcall DelThread_Safe(void* arg);
unsigned int __stdcall WriterThreadUpd(void* p);// 写者修改线程
unsigned int __stdcall WriterThreadDel(void* p);// 写者删除线程
unsigned int __stdcall ReaderThread(void* p);// 读者线程，读者优先

#endif
