
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
	DDX_Control(pDX, IDC_LIST_MES, m_list_box);
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
	m_list_box.ResetContent();
	CString temp;
	temp.Format(_T("添加完成"));
	m_list_box.AddString(temp);
}


void CkesheDlg::OnBnClickedButtonSearch()
{
	CString temp;
	GetDlgItemText(IDC_EDIT1, temp);
	USES_CONVERSION;
	name = T2A(temp);
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
	m_list_box.AddString(temp);
}


void CkesheDlg::OnBnClickedButtonModify()
{
	m_list_box.ResetContent();
	updateone(mylist, updPosition);
	CString temp;
	temp.Format(_T("修改完成"), mylist.length);
	m_list_box.AddString(temp);
}


void CkesheDlg::OnBnClickedButtonDelete()
{
	m_list_box.ResetContent();
	delone(mylist, delPosition);
	CString temp;
	temp.Format(_T("删除完成"), mylist.length);
	m_list_box.AddString(temp);
}


void CkesheDlg::OnBnClickedButtonShow()
{
	m_list_box.ResetContent();
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
	temp.Format(_T("显示完成"));
	m_list_box.AddString(temp);
	temp.Format(_T("总人数：%d"), i - 1);
	m_list_box.AddString(temp);
}


void CkesheDlg::OnBnClickedButtonMulthreadInsert()
{
	m_list_box.ResetContent();
	CString temp;
	//清空数据
	m_list_show.ClearData();

	InitializeList(mylist);
	temp.Format(_T("初始化通讯录链表成功"));
	m_list_box.AddString(temp);
	temp.Format(_T("初始时总人数：%d"), mylist.length);
	m_list_box.AddString(temp);
	ExecuteThreads(mylist, InsertThread, "插入");
	temp.Format(_T("所有线程执行完毕"));
	m_list_box.AddString(temp);
	temp.Format(_T("总人数：%d"), mylist.length);
	m_list_box.AddString(temp);
	temp.Format(_T("测试插入总人数：%d"), insertCount1);
	m_list_box.AddString(temp);
	if (mylist.length == insertCount1) {
		temp.Format(_T("测试成功!"));
	}
	else {
		temp.Format(_T("测试失败!"));
	}
	m_list_box.AddString(temp);
	m_list_show.UpItemData();
}


void CkesheDlg::OnBnClickedButtonMulthreadDelete()
{
	m_list_box.ResetContent();
	DeleteInitialization(mylist);
	ExecuteThreads(mylist, DelThread, "删除");
	int count = 0;
	Node* current = mylist.head;
	while (current != nullptr) {
		count++;
		current = current->next;
	}
	CString temp;
	temp.Format(_T("总人数：%d"), mylist.length);
	m_list_box.AddString(temp);
	temp.Format(_T("总人数：%d"), count);
	m_list_box.AddString(temp);
	temp.Format(_T("测试删除后剩余人数：%d"), delRemain);
	m_list_box.AddString(temp);
	if (count == delRemain) {
		temp.Format(_T("测试成功!"));
	}
	else {
		temp.Format(_T("测试失败!"));
	}
	m_list_box.AddString(temp);
}


void CkesheDlg::OnBnClickedButtonSafeThreadInsert()
{
	m_list_box.ResetContent();
	CString temp;
	InitializeList(mylist);
	ExecuteThreads(mylist, InsertThread_Safe, "安全插入");
	temp.Format(_T("总人数：%d"), mylist.length);
	m_list_box.AddString(temp);
	temp.Format(_T("测试插入总人数：%d"), mylist.length);
	m_list_box.AddString(temp);
	if (mylist.length == insertCount1) {
		temp.Format(_T("测试成功!"), temp);
	}
	else {
		temp.Format(_T("测试失败!"), temp);
	}
	m_list_box.AddString(temp);
}


void CkesheDlg::OnBnClickedButtonSafeThreadDelete()
{
	m_list_box.ResetContent();
	CString temp;
	DeleteInitialization(mylist);
	ExecuteThreads(mylist, DelThread_Safe, "安全删除");
	temp.Format(_T("安全删除完成"));
	m_list_box.AddString(temp);
	temp.Format(_T("总人数：%d"), mylist.length);
	m_list_box.AddString(temp);
	temp.Format(_T("测试删除后剩余人数：%d"), delRemain);
	m_list_box.AddString(temp);
	if (mylist.length == delRemain) {
		temp.Format(_T("测试成功!"), temp);
	}
	else {
		temp.Format(_T("测试失败!"), temp);
	}
	m_list_box.AddString(temp);
}


void CkesheDlg::OnBnClickedButtonSafeThreadReadWrite()
{
	m_list_box.ResetContent();
	message.clear();
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
	for (int i = 0; i < message.size(); i++)
	{
		m_list_box.AddString(CString(message[i].c_str()));
	}
	message.clear();
	temp.Format(_T("测试完成!\r\n"), temp);
	m_list_box.AddString(temp);
}


void CkesheDlg::OnBnClickedButtonClear()
{
	m_list_box.ResetContent();
	CString temp;
	m_list_show.ClearData();
	m_list_show.UpItemData();
	delall(mylist);
	temp.Format(_T("清空初始化完成!"), temp);
	m_list_box.AddString(temp);
}
