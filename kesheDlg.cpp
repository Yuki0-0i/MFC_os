
// kesheDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "keshe.h"
#include "kesheDlg.h"
#include "afxdialogex.h"
#include "lianbiao.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//#include "lianbiao.cpp"


CkesheDlg::CkesheDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KESHE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CkesheDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOW, m_list_show);
}

BEGIN_MESSAGE_MAP(CkesheDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CkesheDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CkesheDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CkesheDlg::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CkesheDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &CkesheDlg::OnBnClickedButtonShow)
	ON_BN_CLICKED(IDC_BUTTON_MULTHREAD_INSERT, &CkesheDlg::OnBnClickedButtonMulthreadInsert)
	ON_BN_CLICKED(IDC_BUTTON_MULTHREAD_DELETE, &CkesheDlg::OnBnClickedButtonMulthreadDelete)
	ON_BN_CLICKED(IDC_BUTTON_SAFE_THREAD_INSERT, &CkesheDlg::OnBnClickedButtonSafeThreadInsert)
	ON_BN_CLICKED(IDC_BUTTON_SAFE_THREAD_DELETE, &CkesheDlg::OnBnClickedButtonSafeThreadDelete)
	ON_BN_CLICKED(IDC_BUTTON_SAFE_THREAD_READ_WRITE, &CkesheDlg::OnBnClickedButtonSafeThreadReadWrite)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CkesheDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CkesheDlg 消息处理程序

BOOL CkesheDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//AllocConsole();//控制台调试窗口开启
	//freopen("CONOUT$", "w", stdout);//开启中文控制台输出支持

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	RECT  m_rect;
	m_list_show.GetClientRect(&m_rect);
	m_list_show.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	m_list_show.InsertColumn(0, _T("序号"), LVCFMT_LEFT, (m_rect.right - 50) / 5);
	m_list_show.InsertColumn(1, _T("姓名"), LVCFMT_CENTER, (m_rect.right - 50) / 5 * 2);
	m_list_show.InsertColumn(1, _T("电话"), LVCFMT_CENTER, (m_rect.right - 50) / 5 * 2);

	InitializeList(mylist);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CkesheDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CkesheDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



HBRUSH CkesheDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_SHOW)
	{
		pDC->SetTextColor(RGB(0, 122, 204));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CkesheDlg::OnBnClickedButtonAdd()
{
	add(mylist);
	CString temp;
	temp.Format(_T("添加完成"));
	SetDlgItemText(IDC_STATIC_SHOW, temp);
}


void CkesheDlg::OnBnClickedButtonSearch()
{
	CString temp;
	m_list_show.ClearData();

	Node* p = mylist.head;
	int count = 0; int position = 0;  // 记录链表中的序号
	while (p != NULL) {
		position++;
		if (p->contact.name == name) {
			m_list_show.nodeList.push_back({ position , *p });
			count++;
		}
		p = p->next;
	}
	m_list_show.UpItemData();

	if (count == 0) {
		temp.Format(_T("对不起，通讯录中没有找到姓名为 %s 的联系人."), CString(name.c_str()));
	}
	else {
		temp.Format(_T("共找到 %d 个姓名为 %s 的联系人."), count, CString(name.c_str()));
	}

	SetDlgItemText(IDC_STATIC_SHOW, temp);
}


void CkesheDlg::OnBnClickedButtonModify()
{
	updateone(mylist, updPosition);
	CString temp;
	temp.Format(_T("修改完成"), mylist.length);
	SetDlgItemText(IDC_STATIC_SHOW, temp);
}


void CkesheDlg::OnBnClickedButtonDelete()
{
	delone(mylist, delPosition);
	CString temp;
	temp.Format(_T("删除完成"), mylist.length);
	SetDlgItemText(IDC_STATIC_SHOW, temp);
}


void CkesheDlg::OnBnClickedButtonShow()
{
	//清空数据
	m_list_show.ClearData();

	Node* p = mylist.head; int i = 1;//让p从第一个节点开始
	while (p != NULL)
	{
		m_list_show.nodeList.push_back({ i , *p });
		p = p->next; i++;
	}
	m_list_show.UpItemData();

	CString temp;
	temp.Format(_T("显示完成\r\n总人数：%d\r\n"), i - 1);
	SetDlgItemText(IDC_STATIC_SHOW, temp);
}


void CkesheDlg::OnBnClickedButtonMulthreadInsert()
{
	CString temp;
	//清空数据
	m_list_show.ClearData();

	InitializeList(mylist);
	temp.Format(_T("初始化通讯录链表成功！\r\n"));
	temp.Format(_T("%s初始时总人数：%d\r\n"), temp, mylist.length);
	ExecuteThreads(mylist, InsertThread, "插入");
	temp.Format(_T("%s所有线程执行完毕！\r\n"), temp);
	temp.Format(_T("%s总人数：%d\r\n"), temp, mylist.length);
	temp.Format(_T("%s测试插入总人数：%d\r\n"), temp, insertCount1);
	if (mylist.length == insertCount1) {
		temp.Format(_T("%s测试成功!\r\n"), temp);
	}
	else {
		temp.Format(_T("%s测试失败!\r\n"), temp);
	}
	m_list_show.UpItemData();
	SetDlgItemText(IDC_STATIC_SHOW, temp);
}


void CkesheDlg::OnBnClickedButtonMulthreadDelete()
{
	DeleteInitialization(mylist);
	ExecuteThreads(mylist, DelThread, "删除");
	int count = 0;
	Node* current = mylist.head;
	while (current != nullptr) {
		count++;
		current = current->next;
	}
	CString temp;
	temp.Format(_T("总人数：%d\r\n"), mylist.length);
	temp.Format(_T("%s总人数：%d\r\n"), temp, count);
	temp.Format(_T("%s测试删除后剩余人数：%d\r\n"), temp, delRemain);
	if (count == delRemain) {
		temp.Format(_T("%s测试成功!"), temp);
	}
	else {
		temp.Format(_T("%s测试失败!"), temp);
	}
	SetDlgItemText(IDC_STATIC_SHOW, temp);
}


void CkesheDlg::OnBnClickedButtonSafeThreadInsert()
{
	CString temp;
	InitializeList(mylist);
	ExecuteThreads(mylist, InsertThread_Safe, "安全插入");
	temp.Format(_T("总人数：%d\r\n"), mylist.length);
	temp.Format(_T("%s测试插入总人数：%d\r\n"), temp, mylist.length);
	if (mylist.length == insertCount1) {
		temp.Format(_T("%s测试成功!\r\n"), temp);
	}
	else {
		temp.Format(_T("%s测试失败!\r\n"), temp);
	}
	SetDlgItemText(IDC_STATIC_SHOW, temp);
}


void CkesheDlg::OnBnClickedButtonSafeThreadDelete()
{
	CString temp;
	DeleteInitialization(mylist);
	ExecuteThreads(mylist, DelThread_Safe, "安全删除");
	temp.Format(_T("安全删除完成\r\n"));
	temp.Format(_T("%s总人数：%d\r\n"), temp, mylist.length);
	temp.Format(_T("%s测试删除后剩余人数：%d\r\n"), temp, delRemain);
	if (mylist.length == delRemain) {
		temp.Format(_T("%s测试成功!\r\n"), temp);
	}
	else {
		temp.Format(_T("%s测试失败!\r\n"), temp);
	}
	SetDlgItemText(IDC_STATIC_SHOW, temp);
}


void CkesheDlg::OnBnClickedButtonSafeThreadReadWrite()
{
	CString temp;
	RWInitialization(mylist);
	hwriteblock = CreateMutex(NULL, FALSE, NULL); 
	hCoutMutex = CreateMutex(NULL, FALSE, NULL);
	HANDLE threads[numThreads2] = { NULL };
	int threadIds[numThreads2] = { 0, 1, 2, 3, 4 ,5,6,7,8,9,10,11,12,13,14 };  // 每个线程有一个唯一的ID
	for (int i = 0; i < numThreads2; ++i) {
		if (i % 3 == 0) {
			// 创建读者线程 数
			threads[i] = (HANDLE)_beginthreadex(NULL, 0, ReaderThread, &threadIds[i], 0, NULL);
		}
		else if (i % 3 == 1) {
			// 创建写者删除线程
			threads[i] = (HANDLE)_beginthreadex(NULL, 0, WriterThreadDel, &threadIds[i], 0, NULL);
		}
		else {
			// 创建写者修改线程
			threads[i] = (HANDLE)_beginthreadex(NULL, 0, WriterThreadUpd, &threadIds[i], 0, NULL);
		}
		if (threads[i] == NULL) {
			cerr << "Error creating thread " << i + 1 << endl;
			return;
		}
	}
	WaitForMultipleObjects(numThreads2, threads, TRUE, INFINITE);// 等待所有线程完成
	for (int i = 0; i < numThreads2; ++i) {
		CloseHandle(threads[i]);
	}
	temp.Format(_T("测试完成!\r\n"), temp);
	SetDlgItemText(IDC_STATIC_SHOW, temp);
}


void CkesheDlg::OnBnClickedButtonClear()
{
	CString temp;
	m_list_show.ClearData();
	m_list_show.UpItemData();
	delall(mylist);
	temp.Format(_T("清空初始化完成!\r\n"), temp);
	SetDlgItemText(IDC_STATIC_SHOW, temp);
}
