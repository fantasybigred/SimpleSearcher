
// MainUIDlg.h : ͷ�ļ�
//

#pragma once
#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include "Searcher.h"
#include "ShareObject.h"

using namespace std;

// CMainUIDlg �Ի���
class CMainUIDlg : public CDialogEx
{
// ����
public:
	CMainUIDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	// ��������
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
//	//	if (m_pInstance == NULL)  //�ж��Ƿ��һ�ε���  
//	//		m_pInstance = new Searcher;
//	//	return m_pInstance;
//	//}
//private:
//	ShareObject()   //���캯����˽�е�  
//	{
//	}
//	static Searcher *m_pInstance;
//
//};
//Searcher * ShareObject::GetInstance()
//{
//	if (m_pInstance == NULL)  //�ж��Ƿ��һ�ε���  
//		m_pInstance = new Searcher;
//	return m_pInstance;
//}