#pragma once
#include <afxcmn.h>
#include "lianbiao.h"

struct NODEITEM
{
    int index;
    Node item;
    NODEITEM(int _index, Node _item) :index(_index), item(_item) {};
};

class CListNode :
    public CListCtrl
{
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnLvnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
public:
    std::vector<NODEITEM> nodeList;
    void UpItemData();
    void ClearData();
};

