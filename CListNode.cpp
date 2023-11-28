#include "CListNode.h"
BEGIN_MESSAGE_MAP(CListNode, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &CListNode::OnLvnGetdispinfo)
END_MESSAGE_MAP()


void CListNode::OnLvnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LV_ITEM* pItem = &(pDispInfo)->item;
	if (pItem->mask & LVIF_TEXT)
	{
		switch (pItem->iSubItem)
		{
		case 0:
		{
			CString text;
			text.Format(_T("%d"), nodeList[pItem->iItem].index);
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		}
		case 1:
		{
			lstrcpyn(pItem->pszText, CString(nodeList[pItem->iItem].item.contact.name.c_str()), pItem->cchTextMax);
			break;
		}
		case 2:
		{
			lstrcpyn(pItem->pszText, CString(nodeList[pItem->iItem].item.contact.tel.c_str()), pItem->cchTextMax);
			break;
		}
		}
	}
	*pResult = 0;
}

void CListNode::UpItemData()
{
	SetItemCountEx(nodeList.size(), LVSICF_NOSCROLL | LVSICF_NOINVALIDATEALL);
	Invalidate();
}

void CListNode::ClearData()
{
	nodeList.clear();
}
