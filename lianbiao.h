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
//ʵ�����ƣ�ͨѶ¼����

typedef struct _contactslist//������ϵ�˽ṹ���ں��������绰����
{
	string name;//������string����
	string tel;
}CL;
typedef struct _CLNode {//����ڵ㣬�����������ָ����
	CL contact;
	struct _CLNode* next;
	struct _CLNode* prev; // ���ָ��ǰһ���ڵ��ָ��
}Node;//����ڵ���Node

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
const int numThreads2 = 15;//ָ�������Ķ���д�ߵ��߳�

void add(List& mylist);
void search(List& mylist, int Position);
void searchone(List& mylist, string name);
void update(List& mylist, int Position, int threadId);
void updateone(List& mylist, int Position);
void delone(List& mylist, int fixedPosition);
void delone2(List& mylist, int fixedPosition);
void display(List& mylist);//������ʾ����ͨѶ¼��������������
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
unsigned int __stdcall WriterThreadUpd(void* p);// д���޸��߳�
unsigned int __stdcall WriterThreadDel(void* p);// д��ɾ���߳�
unsigned int __stdcall ReaderThread(void* p);// �����̣߳���������

#endif
