
// MainUIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MainUI.h"
#include "MainUIDlg.h"
#include "afxdialogex.h"

#include <afxinet.h>
#include <afxstr.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern UINT g_StartSearch(LPVOID pParam);
Searcher *g_pSearcher;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMainUIDlg �Ի���



CMainUIDlg::CMainUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAINUI_DIALOG, pParent)
	, m_ParseValue(_T(""))
	, m_cstrSeed(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_pSearcher = new Searcher;
}

void CMainUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_ParseValue);
	DDX_Text(pDX, IDC_EDIT_SEED, m_cstrSeed);
}

BEGIN_MESSAGE_MAP(CMainUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMainUIDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_AddSeed, &CMainUIDlg::OnBnClickedBtnAddseed)
	ON_BN_CLICKED(IDC_BTN_Start, &CMainUIDlg::OnBnClickedBtnStart)
END_MESSAGE_MAP()


// CMainUIDlg ��Ϣ�������

BOOL CMainUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMainUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMainUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMainUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CMainUIDlg::GetHTML(CString url) {
	CInternetSession session;
	CHttpFile* pfile = (CHttpFile*)session.OpenURL(url, 1, INTERNET_FLAG_TRANSFER_ASCII || INTERNET_FLAG_RELOAD, NULL, 0);
	CString strHtml;
	DWORD dwStatusCode;
	pfile->QueryInfoStatusCode(dwStatusCode);
	if (dwStatusCode == HTTP_STATUS_OK) {
		char strBuff[1025] = { 0 };
		//CString strHtml;
		while ((pfile->Read((void*)strBuff, 1024)) > 0) {
			CString strCode1(strBuff);
			strHtml += strCode1;
		}
	}
	pfile->Close();
	return strHtml;
}

void CMainUIDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strHtml = GetHTML(CString("http://www.163.com"));
	vector<string> links;
	CString buffer(strHtml);
	GetLinesFromBuffer(buffer, links);
	string strLinks = "";
	CString cstrLinks;

	for (auto item : links)
	{
		strLinks += item;
		strLinks += "\r\n";
	}
	//cstrLinks.Format(L"%s", strLinks.c_str());
	//cstrLinks(strLinks.c_str());
	cstrLinks = strLinks.c_str();
	MessageBox(cstrLinks);
	//UpdateData(TRUE);
	// m_ParseValue = GetHTML(CString("http://www.163.com"));
	m_ParseValue.Format(_T("%d"), strHtml.GetLength());
	UpdateData(FALSE);
	GetFile(CString("http://www.163.com"), CString("C:\\Users\\bigre\\Desktop\\163.html"));


	//MessageBox(strHtml, L"��Ϣ��ʾ", MB_OK);
	//MessageBox(strHtml,0,MB_OK);
	//CInternetSession MySession;
	//CString StrURL("http://www.163.com");
	//CString StrInfo("��ѯ������£�\n");
	//CInternetFile* MyFile = NULL;
	//try
	//{
	//	MyFile = (CInternetFile*)MySession.OpenURL(StrURL);
	//}
	//catch (CInternetException* pEx)
	//{
	//	MyFile = NULL;
	//	pEx->Delete();
	//}
	//if (MyFile)
	//{
	//	CString StrLine;
	//	for (int i = 0; i < 20 && MyFile->ReadString(StrLine); i++)
	//	{
	//		CString tmp = StrLine + "\r\n";
	//		StrInfo += tmp;
	//	}
	//	MyFile->Close();
	//	delete MyFile;
	//}
	//else
	//{
	//	StrInfo += " û�в�ѯ���!\r\n";
	//}
	////CString str;
	//GetDlgItem(IDC_EDIT2)->SetWindowText(strHtml);
	//MessageBox(StrInfo, L"��Ϣ��ʾ", MB_OK);
	//MessageBox(strHtml, L"��Ϣ��ʾ", MB_OK);
	//CView::OnRButtonDown(nFlags, point);
}

INT CMainUIDlg::GetFile(const CString strUrl, const CString strSavePath)
{
	//��鴫�����������   
	if (strUrl.IsEmpty())
		return -5;
	if (strSavePath.IsEmpty())
		return -6;

	unsigned short nPort;       //���ڱ���Ŀ��HTTP����˿�   
	CString strServer, strObject;   //strServer���ڱ����������ַ��strObject���ڱ����ļ���������   
	DWORD dwServiceType, dwRet;      //dwServiceType���ڱ���������ͣ�dwRet���ڱ����ύGET���󷵻ص�״̬��   

									 //����URL����ȡ��Ϣ   
	if (!AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort))
	{
		//����ʧ�ܣ���Url����ȷ   
		return -1;
	}
	CInternetSession session;
	CHttpFile* pfile = (CHttpFile*)session.OpenURL(strUrl, 1, INTERNET_FLAG_TRANSFER_ASCII || INTERNET_FLAG_RELOAD, NULL, 0);
	CString strHtml;
	DWORD dwStatusCode;
	pfile->QueryInfoStatusCode(dwStatusCode);
	//��ȡ���������ص�״̬��   
	//pHtFile->QueryInfoStatusCode(dwRet);
	if (dwStatusCode != HTTP_STATUS_OK)
	{
		return -4;
	}
	//��ȡ�ļ���С   
	UINT nFileLen = (UINT)pfile->GetLength();
	DWORD dwRead = 1;       //���ڱ�ʶ���˶��٣�Ϊ1��Ϊ�˽���ѭ��   
							//����������   
	CHAR *szBuffer = new CHAR[nFileLen + 1];
	TRY
	{
		//�����ļ�   
		CFile PicFile(strSavePath,CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive);
	while (dwRead>0)
	{
		//��ջ�����   
		memset(szBuffer,0,(size_t)(nFileLen + 1));
		//��ȡ��������   
		dwRead = pfile->Read(szBuffer,nFileLen);
		//д�뵽�ļ�   
		PicFile.Write(szBuffer,dwRead);
	}
	//�ر��ļ�   
	PicFile.Close();
	//�ͷ��ڴ�   
	delete[]szBuffer;
	delete pfile;
	}
	CATCH(CFileException, e)
	{
		//�ͷ��ڴ�   
		delete[]szBuffer;
		delete pfile;
		return -7;          //��д�ļ��쳣   
	}
	END_CATCH
		return 0;
}

void CMainUIDlg::GetLinesFromBuffer(CString strUrlBuffer, vector<string>& vecLines)
{
	//CString tmp = CStringA(strUrlBuffer);
	string buffer= CStringA(strUrlBuffer);
	//string pattern("http(s)?://([\\w-]+\\.)+[\\w-]+(/[\\w- ./?%&=]*)?");    //ƥ�����ܼ򵥣�������ɻ󣬿��Զ����﷨�鿴
	string pattern("((http|ftp|https)://)(([a-zA-Z0-9\._-]+\.[a-zA-Z]{2,6})|([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}))(:[0-9]{1,4})*(/[a-zA-Z0-9\&%_\./-~-]*)?");
	regex r(pattern);
	smatch results; 
	regex_match(buffer, results, r);
	for (std::sregex_iterator it(buffer.begin(), buffer.end(), r), end;     //end��β���������regex_iterator��regex_iterator��string���͵İ汾
		it != end;
		++it)
	{
		//std::cout << it->str() << std::endl;
		vecLines.push_back(it->str());
	}
}

void CMainUIDlg::OnBnClickedBtnAddseed()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//if(NULL == m_pSearcher)
	//	m_pSearcher = new Searcher;
	////UpdateData(FALSE); //������ؼ���������
	//UpdateData(TRUE); //���Դӿؼ�������� 
	//string strSeed = CStringA(m_cstrSeed);
	//if (!m_pSearcher->CheckUrl(strSeed))
	//{
	//	MessageBox(L"url�Ƿ���");
	//	return;
	//}
	//m_pSearcher->SetSeed(strSeed);
	//MessageBox(m_cstrSeed);
	CString strUrl = L"http://app.sina.com.cn/app_index.php?f=p_binfen&w=p_binfen";
	CInternetSession session;
	CHttpFile* pfile = (CHttpFile*)session.OpenURL(strUrl, 1, INTERNET_FLAG_TRANSFER_ASCII || INTERNET_FLAG_RELOAD, NULL, 0);
	CString strHtml;
	DWORD dwStatusCode;
	pfile->QueryInfoStatusCode(dwStatusCode);
	//��ȡ���������ص�״̬��   
	//pHtFile->QueryInfoStatusCode(dwRet);
	if (dwStatusCode != HTTP_STATUS_OK)
	{
		return ;
	}
	//��ȡ�ļ���С   
	UINT nFileLen = (UINT)pfile->GetLength();
	DWORD dwRead = 1;       //���ڱ�ʶ���˶��٣�Ϊ1��Ϊ�˽���ѭ��   
							//����������   
	CHAR *szBuffer = new CHAR[nFileLen + 1];
	CString cstrContent;
	//�����ļ�   
	CFile PicFile(L"C:\\Users\\bigre\\Desktop\\test.html",CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive);
	while (dwRead>0)
	{
		//��ջ�����   
		memset(szBuffer, 0, (size_t)(nFileLen + 1));
		//��ȡ��������   
		dwRead = pfile->Read(szBuffer, nFileLen);
		//д�뵽�ļ�   
		PicFile.Write(szBuffer, dwRead);
		cstrContent += szBuffer;
	}
}


void CMainUIDlg::OnBnClickedBtnStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//if (NULL == m_pSearcher)
	//	MessageBox(L"��������������");
	//vector<URL> seeds;
	//m_pSearcher->GetSeeds(seeds);
	//if(seeds.size()==0)
	//	MessageBox(L"��������������");
	//Searcher* pSearcher = ShareObject::GetInstance();
	g_pSearcher->InitPreURLList();
	//m_pSearcher->StartSearch(NULL);

	//CWinThread* AfxBeginThread(

	//	AFX_THREADPROC pfnThreadProc,
	//	LPVOID pParam,
	//	int nPriority = THREAD_PRIORITY_NORMAL,
	//	UINT nStackSize = 0,
	//	DWORD dwCreateFlags = 0,
	//	LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL
	//);//���ڴ����������߳�

	CWinThread *thread = AfxBeginThread(g_StartSearch,(LPVOID)NULL, 0,0,NULL,0);
	CWinThread *thread1 = AfxBeginThread(g_StartSearch, (LPVOID)NULL, 0, 0, NULL, 0);
	Sleep(1000 * 60);
	CWinThread *arrThread[10] = { 0 };
	for (DWORD dwIdx = 0; dwIdx < 10; ++dwIdx)
	{
		arrThread[dwIdx]= AfxBeginThread(g_StartSearch, (LPVOID)NULL, 0, 0, NULL, 0);
	}
}

UINT g_StartSearch(LPVOID pParam)
{
	//Searcher* pSearcher = ShareObject::GetInstance();
	g_pSearcher->StartSearch(pParam);
	return 0;
}
