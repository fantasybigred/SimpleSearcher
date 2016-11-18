
// MainUIDlg.h : 头文件
//

#pragma once
#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include "Searcher.h"
#include "ShareObject.h"

using namespace std;

// CMainUIDlg 对话框
class CMainUIDlg : public CDialogEx
{
// 构造
public:
	CMainUIDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	INT GetFile(const CString strUrl, const CString strSavePath);
	void GetLinesFromBuffer(const CString strUrlBuffer, vector<string>& vecLines);
	CString GetHTML(CString url);
	CString m_ParseValue;
	afx_msg void OnBnClickedBtnAddseed();
	// 种子链接
	CString m_cstrSeed;
	afx_msg void OnBnClickedBtnStart();
	//static INT g_StartSearch(LPVOID pParam);
	//Searcher* m_pSearcher;
};



//class ShareObject
//{
//public:
//	static Searcher * GetInstance();
//	//{
//	//	if (m_pInstance == NULL)  //判断是否第一次调用  
//	//		m_pInstance = new Searcher;
//	//	return m_pInstance;
//	//}
//private:
//	ShareObject()   //构造函数是私有的  
//	{
//	}
//	static Searcher *m_pInstance;
//
//};
//Searcher * ShareObject::GetInstance()
//{
//	if (m_pInstance == NULL)  //判断是否第一次调用  
//		m_pInstance = new Searcher;
//	return m_pInstance;
//}