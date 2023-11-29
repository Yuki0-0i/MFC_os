#include<stdio.h>
#include<string>
#include<iostream>
#include<cstdlib>
#include <mutex>
#include <vector> 
#include <windows.h>
#include <random>
#include <thread>
#include "safeVector.h"
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
Node* head = NULL;//初始化首节点为空
Node* tail = NULL;

List mylist;
HANDLE hmutex = CreateMutex(NULL, FALSE, NULL);
int updPosition = 3;
string name = "QT";
const int numThreads = 10;//指定数量的线程10
const int insertPerThread = 10000;//指定数量的插入量
int insertCount1 = numThreads * insertPerThread;//指定的正确插入量

int delPosition = 3;
const int insertCount2 = 100040; //指定数量的删除初始值
const int delCount = 10000;//指定数量的每次删除量
int delRemain = insertCount2 - numThreads * delCount;//删除后的正确剩余量

// 读者写者模型的一些全局变量和信号量
HANDLE hwriteblock; HANDLE hCoutMutex;
//const int numThreads2 = 15;//指定数量的读者写者的线程
volatile int readcount = 0;
int Position = 3;
const int insertCount3 = 400;
const int MAX_POSITIONS = 10; // 最大位置数
int modifiedPositions[MAX_POSITIONS] = { 0 };
int modifiedPositionsCount = 0;


HANDLE hthread[numThreads];
int threadcount = 0;
std::mutex listMutex;
std::mutex coutMutex;

safeVector message;

void InitializeList(List& mylist) {
    while (mylist.head != nullptr) {
        Node* temp = mylist.head;
        mylist.head = mylist.head->next;
        delete temp;
    }
    mylist.head = NULL;//初始化首节点为空
    mylist.tail = NULL;
    mylist.length = 0;
    //cout << "初始化通讯录链表成功！" << endl;
    //cout << "|" << "初始时总人数：" << mylist.length << endl;
}

//声明程序中要用到的函数
void add(List& mylist);
void search(List& mylist, int Position);
void searchone(List& mylist, string name);
void update(List& mylist, int Position, int threadId);
void updateone(List& mylist, int Position);
void delone(List& mylist, int fixedPosition);
void delone2(List& mylist, int fixedPosition);
void display(List& mylist);//遍历显示整个通讯录及总人数（表长）
void calculate(List& mylist);
void menu();
void delall(List& mylist);
void ExecuteThreads(List& mylist, unsigned int(__stdcall* threadFunction)(void*), const char* operationName);
void DeleteInitialization(List& mylist);
void RWInitialization(List& mylist);

void menu()//主菜单函数，显示选择功能界面
{
    cout << "|---------------MENU-------------------|\n";
    cout << " ★*★*★*★ 111.  销毁并退出通讯录 ★*★*★*★\n";
    cout << " ★*★*★*★ 1.  增加随机联系人 ★*★*★*★\n";
    cout << " ★*★*★*★ 2.  查找固定姓名QT联系人 ★*★*★*★\n";
    cout << " ★*★*★*★ 3.  修改联系人姓名为固定值 ★*★*★*★\n";
    cout << " ★*★*★*★ 4.  删除固定位置联系人 ★*★*★*★\n";
    cout << " ★*★*★*★ 5.  显示所有联系人 ★*★*★*★\n";
    cout << " ★*★*★*★ 6.  清空通讯录内容并初始化 ★*★*★*★\n";
    cout << "|---------------TEST-------------------|\n";
    cout << " ★*★*★*★ 7.  普通多线程插入测试ok ★*★*★*★\n";
    cout << " ★*★*★*★ 8.  普通多线程删除测试 ★*★*★*★\n";
    cout << " ★*★*★*★ 9.  安全多线程插入测试ok ★*★*★*★\n";
    cout << " ★*★*★*★ 10. 安全多线程删除测试 ★*★*★*★\n";
    cout << " ★*★*★*★ 11. 安全多线程读者写者 ★*★*★*★\n";
    cout << "键入(1-11或111):";
}

// Insert 函数
unsigned int __stdcall InsertThread(void* arg) {
    List& mylist = *reinterpret_cast<List*>(arg);
    for (int j = 0; j < insertPerThread; ++j) {
        add(mylist);
    }
    return 0;
}
// InsertThread_Safe 函数
unsigned int __stdcall InsertThread_Safe(void* arg) {
    List& mylist = *reinterpret_cast<List*>(arg);
    for (int j = 0; j < insertPerThread; ++j) {
        // 使用互斥量保护插入操作
        WaitForSingleObject(hmutex, INFINITE);
        add(mylist);
        ReleaseMutex(hmutex);
    }
    return 0;
}

void add(List& mylist)
{
    std::random_device rd;  // 真随机数生成器
    Node* p = new Node();//增加一个新的结点，在内存中申请一个结点所需空间，并用指针p指向这个结点
    // 生成随机的联系人名字（两个大写字母）
    char randomChar1 = 'A' + rd() % 26;
    char randomChar2 = 'A' + rd() % 26;
    string randomName = { randomChar1, randomChar2 };
    string randomTel;
    for (int i = 0; i < 8; ++i) {
        // 生成一个0到9之间的随机数
        int digit = rd() % 10;
        // 将随机数转换为字符并添加到结果中
        randomTel += std::to_string(digit);
    }
    p->contact.name = randomName;
    p->contact.tel = randomTel;

    if (mylist.head == NULL) {
        mylist.head = p;
        mylist.tail = p;
    }
    else {
        mylist.tail->next = p;
        p->prev = mylist.tail;
        mylist.tail = p;
    }
    p->next = NULL;
    mylist.length++;
}
void RWInitialization(List& mylist) {
    // 删除之前的链表内容
    while (mylist.head != nullptr) {
        Node* temp = mylist.head;
        mylist.head = mylist.head->next;
        delete temp;
    }
    cout << "初始化通讯录链表成功！" << endl;
    mylist.head = nullptr;
    mylist.tail = nullptr;
    mylist.length = 0;
    // 向链表安全地插入  个元素
    for (int i = 0; i < insertCount3; ++i) {
        // 使用互斥量保护插入操作
        WaitForSingleObject(hmutex, INFINITE);
        add(mylist);
        ReleaseMutex(hmutex);
    }
    cout << "|" << "初始时总人数：" << mylist.length << endl;
}
// 读者线程，读者优先
unsigned int __stdcall ReaderThread(void* p)
{
    char buffer[256] = {0};
    int threadId = *((int*)p);
    WaitForSingleObject(hmutex, INFINITE);
    readcount++;
    if (readcount == 1) {
        WaitForSingleObject(hwriteblock, INFINITE);
    }
    ReleaseMutex(hmutex);
    for (int i = 0; i < modifiedPositionsCount; ++i) {
        WaitForSingleObject(hCoutMutex, INFINITE);//输出不混乱
        int modifiedPosition = modifiedPositions[i];
        sprintf(buffer,"读者线程 %d 正在读取第 %d 的联系人信息:", threadId, modifiedPosition);
        message.push_back(buffer);
        search(mylist, modifiedPosition);
        ReleaseMutex(hCoutMutex);
        WaitForSingleObject(hCoutMutex, INFINITE);
        message.push_back("读取成功");
        ReleaseMutex(hCoutMutex);
    }
    WaitForSingleObject(hmutex, INFINITE);
    readcount--;
    if (readcount == 0) {
        ReleaseMutex(hwriteblock);
    }
    ReleaseMutex(hmutex);
    return 0;
}
// 写者修改线程
unsigned int __stdcall WriterThreadUpd(void* p)
{
    char buffer[256] = { 0 };
    int threadId = *((int*)p);
    int position = threadId * 10 / 2 + 1; // 计算对应的位置
    WaitForSingleObject(hwriteblock, INFINITE);
    sprintf(buffer, "写者线程 %d 正在修改第 %d 的联系人信息:", threadId, position);
    message.push_back(buffer);
    message.push_back("修改成功");
    update(mylist, position, threadId);
    // 记录修改的位置
    modifiedPositions[modifiedPositionsCount++] = position;
    ReleaseMutex(hwriteblock);
    return 0;
}
// 写者删除线程
unsigned int __stdcall WriterThreadDel(void* p)
{
    char buffer[256] = { 0 };
    int threadId = *((int*)p);
    WaitForSingleObject(hwriteblock, INFINITE);
    for (int i = 0; i < modifiedPositionsCount; ++i) {
        int modifiedPosition = modifiedPositions[i];
        sprintf(buffer, "写者线程 %d 正在删除第 %d 的联系人信息:", threadId, modifiedPosition);
        message.push_back(buffer);
        delone2(mylist, modifiedPosition);
        message.push_back("删除成功");
    }
    ReleaseMutex(hwriteblock);
    return 0;
}
void search(List& mylist, int position)
{
    if (position <= 0 || position > mylist.length) {
        cout << "对不起，通讯录中没有指定位置的联系人." << endl;
        return;
    }
    Node* p = mylist.head; int index = 1;
    while (p != NULL && index < position)
    {
        p = p->next;
        index++;
    }
    if (p != NULL) {// 输出指定位置的联系人信息
        cout << "|序号\t|★*\t姓名\t*★|★*\t电话\t\t*★|" << endl;
        cout << "|" << position << "\t|★ *\t" << p->contact.name << "\t*★|★*\t" << p->contact.tel << "\t*★|" << endl;
        cout << "----------------------------------------------------" << endl;
    }
    else {
        cout << "对不起，通讯录中没有指定位置的联系人." << endl;
    }
}

void update(List& mylist, int position, int threadId)
{
    int index = 1;
    Node* p = mylist.head;
    // 寻找指定位置的节点
    while (p != NULL && index < position) {
        p = p->next;
        index++;
    }
    if (p == NULL || index != position) {
        cout << "对不起，通讯录中没有指定位置的联系人." << endl;
        return;
    }
    // 修改联系人信息为指定的值，根据线程编号改不同的值
    switch (threadId % 5) {
    case 0:
        p->contact.name = "AA";
        p->contact.tel = "88888888";
        break;
    case 1:
        p->contact.name = "BB";
        p->contact.tel = "77777777";
        break;
    case 2:
        p->contact.name = "CC";
        p->contact.tel = "66666666";
    case 3:
        p->contact.name = "DD";
        p->contact.tel = "55555555";
    case 4:
        p->contact.name = "EE";
        p->contact.tel = "44444444";
        break;
    default:
        break;
    }
    // 输出修改后的结果
    cout << "---------------------CONTACTS-----------------------" << endl;
    cout << "|序号\t|★*\t姓名\t*★|★*\t电话\t\t*★|" << endl;
    cout << "|" << position << "\t|★ *\t" << p->contact.name << "\t*★|★*\t" << p->contact.tel << "\t*★|" << endl;
    cout << "----------------------------------------------------" << endl;
}

void searchone(List& mylist, string name) {
    Node* p = mylist.head;
    int count = 0; int position = 0;  // 记录链表中的序号
    cout << "|序号\t|★*\t姓名\t*★|★*\t电话\t\t*★|" << endl;
    cout << "----------------------------------------------------" << endl;
    while (p != NULL) {
        position++;
        if (p->contact.name == name) {
            count++;
            cout << "|" << position << "\t|★ *\t" << p->contact.name << "\t*★|★*\t" << p->contact.tel << "\t*★|" << endl;
        }
        p = p->next;
    }
    cout << "----------------------------------------------------" << endl;
    if (count == 0) {
        cout << "对不起，通讯录中没有找到姓名为 " << name << " 的联系人." << endl;
    }
    else {
        cout << "共找到 " << count << " 个姓名为 " << name << " 的联系人." << endl;
    }
}
void updateone(List& mylist, int position) {
    int index = 1;
    Node* p = mylist.head;
    // 寻找指定位置的节点
    while (p != NULL && index < position) {
        p = p->next;
        index++;
    }
    if (p == NULL || index != position) {
        cout << "对不起，通讯录中没有指定位置的联系人." << endl;
        return;
    }
    p->contact.name = "AA";
    p->contact.tel = "88888888";
    cout << "---------------------CONTACTS-----------------------" << endl;
    cout << "|序号\t|★*\t姓名\t*★|★*\t电话\t\t*★|" << endl;
    cout << "|" << position << "\t|★ *\t" << p->contact.name << "\t*★|★*\t" << p->contact.tel << "\t*★|" << endl;
    cout << "----------------------------------------------------" << endl;
}

void display(List& mylist) //遍历显示整个通讯录及总人数
{
    cout << "---------------------CONTACTS-----------------------" << endl;
    cout << "|序号\t|★*\t姓名\t*★|★*\t电话\t\t*★|" << endl;
    cout << "----------------------------------------------------" << endl;
    Node* p = mylist.head; int i = 1;//让p从第一个节点开始
    while (p != NULL)
    {
        cout << "|" << i << "\t|★ *\t" << p->contact.name << "\t*★|★*\t" << p->contact.tel << "\t*★|" << endl;
        cout << "----------------------------------------------------" << endl;//逐项输出
        p = p->next; i++;
    }
    //cout << "|"<< "总人数：" << mylist.length << endl;
    cout << "----------------------------------------------------" << endl;
    cout << "|" << "总人数：" << i - 1 << endl;
    system("pause"); system("cls");
}
//因为多线程并发，需要纠正length的数据
void calculate(List& mylist) {
    int count = 0;
    Node* current = mylist.head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    mylist.length = count;
}

void DeleteInitialization(List& mylist) {
    // 删除之前的链表内容
    while (mylist.head != nullptr) {
        Node* temp = mylist.head;
        mylist.head = mylist.head->next;
        delete temp;
    }
    cout << "初始化通讯录链表成功！" << endl;
    mylist.head = nullptr;
    mylist.tail = nullptr;
    mylist.length = 0;
    // 向链表安全地插入  个元素
    for (int i = 0; i < insertCount2; ++i) {
        // 使用互斥量保护插入操作
        WaitForSingleObject(hmutex, INFINITE);
        add(mylist);
        ReleaseMutex(hmutex);
    }
    cout << "|" << "初始时总人数：" << mylist.length << endl;
}
// Del 函数
unsigned int __stdcall DelThread(void* arg) {
    List& mylist = *reinterpret_cast<List*>(arg);
    for (int i = 0; i < delCount; ++i) {
        delone2(mylist, delPosition);
        //delone(mylist);
    }
    return 0;
}
// DelThread_Safe 函数
unsigned int __stdcall DelThread_Safe(void* arg) {
    List& mylist = *reinterpret_cast<List*>(arg);
    for (int i = 0; i < delCount; ++i) {
        WaitForSingleObject(hmutex, INFINITE);
        delone2(mylist, delPosition);
        ReleaseMutex(hmutex);
    }
    return 0;
}
void delone(List& mylist, int fixedPosition) {
    if (fixedPosition <= 0) {
        cout << "无效的位置。" << endl;
        return;
    }
    if (mylist.head == nullptr) {
        cout << "通讯录为空，无法删除。" << endl;
        return;
    }
    Node* p4, * p5;
    if (fixedPosition == 1) { // 删除首结点
        p4 = mylist.head;
        mylist.head = mylist.head->next;
        if (mylist.head != nullptr) {
            mylist.head->prev = nullptr; // 更新新的头结点的prev指针
        }
        cout << "通讯录已删除第一个联系人：" << p4->contact.name << "，电话：" << p4->contact.tel << endl;
        delete p4;
        mylist.length--;
        return;
    }
    Node* p = mylist.head;
    for (int i = 1; i < fixedPosition - 1 && p->next != nullptr; ++i) {
        p = p->next;
    }
    if (p->next == nullptr) {
        cout << "指定位置超出通讯录长度。" << endl;
        cout << "当前总人数：" << mylist.length << endl;
        return;
    }
    p5 = p->next;
    p->next = p->next->next;
    if (p->next != nullptr) {
        p->next->prev = p; // 更新下一个节点的prev指针
    }
    else {
        mylist.tail = p;
    }
    //cout << "通讯录已删除第" << fixedPosition << "个联系人：" << p5->contact.name << "，电话：" << p5->contact.tel << endl;
    delete p5;
    mylist.length--;
}
void delone2(List& mylist, int fixedPosition) {
    if (fixedPosition <= 0) {
        cout << "无效的位置。" << endl;
        return;
    }
    if (mylist.head == nullptr) {
        cout << "通讯录为空，无法删除。" << endl;
        return;
    }
    Node* p4, * p5;
    if (fixedPosition == 1) { // 删除首结点
        p4 = mylist.head;
        mylist.head = mylist.head->next;
        if (mylist.head != nullptr) {
            mylist.head->prev = nullptr; // 更新新的头结点的prev指针
        }
        cout << "通讯录已删除第一个联系人：" << p4->contact.name << "，电话：" << p4->contact.tel << endl;
        delete p4;
        mylist.length--;
        return;
    }
    Node* p = mylist.head;
    for (int i = 1; i < fixedPosition - 1 && p->next != nullptr; ++i) {
        p = p->next;
    }
    if (p->next == nullptr) {
        cout << "指定位置超出通讯录长度。" << endl;
        cout << "当前总人数：" << mylist.length << endl;
        return;
    }
    p5 = p->next;
    p->next = p->next->next;
    if (p->next != nullptr) {
        p->next->prev = p; // 更新下一个节点的prev指针
    }
    else {
        mylist.tail = p;
    }
    cout << "通讯录已删除第" << fixedPosition << "个联系人：" << p5->contact.name << "，电话：" << p5->contact.tel << endl;
    delete p5;
    mylist.length--;
}

void delall(List& mylist) {
    Node* current = mylist.head;
    Node* nextNode;
    while (current != NULL) {
        nextNode = current->next;
        delete current;  // 使用 free 释放当前节点的内存
        current = nextNode;
    }
    mylist.head = NULL;  // 将头指针设为NULL
    mylist.tail = NULL;  // 将尾指针设为NULL
    mylist.length = 0;
    cin.ignore();  // 忽略回车键的输入
    cin.get();
}

// 定义一个函数，用于创建指定数量的线程，并等待它们完成
void ExecuteThreads(List& mylist, unsigned int(__stdcall* threadFunction)(void*), const char* operationName) {
    HANDLE threads[numThreads] = { NULL };

    for (int i = 0; i < numThreads; ++i) {
        threads[i] = (HANDLE)_beginthreadex(NULL, 0, threadFunction, &mylist, 0, NULL);
        //cout << "线程" << i + 1 << "执行完毕！" << "（" << operationName << "操作）" << endl;

        if (threads[i] == NULL) {
            //cerr << "Error " << i + 1 << endl;
            return;
        }
    }
    // 等待所有线程完成
    WaitForMultipleObjects(numThreads, threads, TRUE, INFINITE);
    // 关闭线程句柄
    for (int i = 0; i < numThreads; ++i) {
        CloseHandle(threads[i]);
    }
}

//int  main() {
//    InitializeList(mylist);
//    menu();
//    int n;
//    cin >> n;//接受键入数字n，以下为数字对应功能情况
//    while (n) {
//        switch (n) {
//        case 111:cout << "清空链表..." << endl;
//            delall(mylist);
//            cout << "链表已清空" << endl << "链表已销毁" << endl;
//            return 0;
//        case 1:add(mylist); break;//完成新增联系人操作后跳出此次功能
//        case 2:searchone(mylist, name); break; //完成查找联系人操作后跳出此次功能
//        case 3:updateone(mylist, updPosition); break;//完成修改联系人操作后跳出此次功能
//        case 4:delone(mylist, delPosition); break;//完成删除联系人操作后跳出此次功能
//        case 5:display(mylist); break;//完成显示联系人操作后跳出此次功能
//        case 6:cout << "清空链表..." << endl;
//            delall(mylist); cout << "链表已清空" << endl;
//            InitializeList(mylist);
//            break;
//        case 7: {
//            InitializeList(mylist);
//            cout << "按回车键继续..."; cin.ignore(); cin.get();
//            ExecuteThreads(mylist, InsertThread, "插入");
//            cout << "总人数：" << mylist.length << endl;
//            cout << "测试插入总人数：" << insertCount1 << endl;
//            if (mylist.length == insertCount1) {
//                cout << "测试成功!" << endl;
//            }
//            else {
//                cout << "测试失败" << endl;
//            }
//            break;
//        }
//        case 8: {
//            DeleteInitialization(mylist);
//            cout << "按回车键继续..."; cin.ignore(); cin.get();
//            ExecuteThreads(mylist, DelThread, "删除");
//            int count = 0;
//            Node* current = mylist.head;
//            while (current != nullptr) {
//                count++;
//                current = current->next;
//            }
//            cout << "总人数：" << mylist.length << endl;
//            cout << "总人数：" << count << endl;
//            cout << "测试删除后剩余人数：" << delRemain << endl;
//            if (count == delRemain) {
//                cout << "测试成功!" << endl;
//            }
//            else {
//                cout << "测试失败" << endl;
//            }
//            break;
//        }
//        case 9: {
//            InitializeList(mylist);
//            cout << "按回车键继续..."; cin.ignore(); cin.get();
//            ExecuteThreads(mylist, InsertThread_Safe, "安全插入");
//            cout << "总人数：" << mylist.length << endl;
//            cout << "测试插入总人数：" << insertCount1 << endl;
//            if (mylist.length == insertCount1) {
//                cout << "测试成功!" << endl;
//            }
//            else {
//                cout << "测试失败" << endl;
//            }
//            break;
//        }
//        case 10: {
//            DeleteInitialization(mylist);
//            cout << "按回车键继续..."; cin.ignore(); cin.get();
//            ExecuteThreads(mylist, DelThread_Safe, "安全删除");
//            cout << "总人数：" << mylist.length << endl;
//            cout << "测试删除后剩余人数：" << delRemain << endl;
//            if (mylist.length == delRemain) {
//                cout << "测试成功!" << endl;
//            }
//            else {
//                cout << "测试失败" << endl;
//            }
//            break;
//        }
//        case 11: {
//            RWInitialization(mylist);
//            cout << "按回车键继续..."; cin.ignore(); cin.get();
//            hwriteblock = CreateMutex(NULL, FALSE, NULL); hCoutMutex = CreateMutex(NULL, FALSE, NULL);
//            HANDLE threads[numThreads2] = { NULL };
//            int threadIds[numThreads2] = { 0, 1, 2, 3, 4 ,5,6,7,8,9,10,11,12,13,14 };  // 每个线程有一个唯一的ID
//            for (int i = 0; i < numThreads2; ++i) {
//                if (i % 3 == 0) {
//                    // 创建读者线程 数
//                    threads[i] = (HANDLE)_beginthreadex(NULL, 0, ReaderThread, &threadIds[i], 0, NULL);
//                }
//                else if (i % 3 == 1) {
//                    // 创建写者删除线程
//                    threads[i] = (HANDLE)_beginthreadex(NULL, 0, WriterThreadDel, &threadIds[i], 0, NULL);
//                }
//                else {
//                    // 创建写者修改线程
//                    threads[i] = (HANDLE)_beginthreadex(NULL, 0, WriterThreadUpd, &threadIds[i], 0, NULL);
//                }
//                if (threads[i] == NULL) {
//                    cerr << "Error creating thread " << i + 1 << endl;
//                    return 1;
//                }
//            }
//            WaitForMultipleObjects(numThreads2, threads, TRUE, INFINITE);// 等待所有线程完成
//            for (int i = 0; i < numThreads2; ++i) {
//                CloseHandle(threads[i]);
//            }
//            break;
//        }
//        }
//        menu();//回到主菜单函数
//        cin >> n;//等待下一个功能键入
//    }
//    system("pause");
//    return 0;
//}
