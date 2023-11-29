
// kesheDlg.h: 头文件
//

#pragma once
#include "CListNode.h"


// CkesheDlg 对话框
class CkesheDlg : public CDialogEx
{
// 构造
public:
	CkesheDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KESHE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListNode m_list_show;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonShow();
	afx_msg void OnBnClickedButtonMulthreadInsert();
	afx_msg void OnBnClickedButtonMulthreadDelete();
	afx_msg void OnBnClickedButtonSafeThreadInsert();
	afx_msg void OnBnClickedButtonSafeThreadDelete();
	afx_msg void OnBnClickedButtonSafeThreadReadWrite();
	afx_msg void OnBnClickedButtonClear();
	CListBox m_list_box;
};
