
// MainUIDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMainUIDlg 对话框



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


// CMainUIDlg 消息处理程序

BOOL CMainUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMainUIDlg::OnPaint()
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
	// TODO: 在此添加控件通知处理程序代码
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


	//MessageBox(strHtml, L"信息提示", MB_OK);
	//MessageBox(strHtml,0,MB_OK);
	//CInternetSession MySession;
	//CString StrURL("http://www.163.com");
	//CString StrInfo("查询情况如下：\n");
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
	//	StrInfo += " 没有查询结果!\r\n";
	//}
	////CString str;
	//GetDlgItem(IDC_EDIT2)->SetWindowText(strHtml);
	//MessageBox(StrInfo, L"信息提示", MB_OK);
	//MessageBox(strHtml, L"信息提示", MB_OK);
	//CView::OnRButtonDown(nFlags, point);
}

INT CMainUIDlg::GetFile(const CString strUrl, const CString strSavePath)
{
	//检查传入的两个参数   
	if (strUrl.IsEmpty())
		return -5;
	if (strSavePath.IsEmpty())
		return -6;

	unsigned short nPort;       //用于保存目标HTTP服务端口   
	CString strServer, strObject;   //strServer用于保存服务器地址，strObject用于保存文件对象名称   
	DWORD dwServiceType, dwRet;      //dwServiceType用于保存服务类型，dwRet用于保存提交GET请求返回的状态号   

									 //解析URL，获取信息   
	if (!AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort))
	{
		//解析失败，该Url不正确   
		return -1;
	}
	CInternetSession session;
	CHttpFile* pfile = (CHttpFile*)session.OpenURL(strUrl, 1, INTERNET_FLAG_TRANSFER_ASCII || INTERNET_FLAG_RELOAD, NULL, 0);
	CString strHtml;
	DWORD dwStatusCode;
	pfile->QueryInfoStatusCode(dwStatusCode);
	//获取服务器返回的状态号   
	//pHtFile->QueryInfoStatusCode(dwRet);
	if (dwStatusCode != HTTP_STATUS_OK)
	{
		return -4;
	}
	//获取文件大小   
	UINT nFileLen = (UINT)pfile->GetLength();
	DWORD dwRead = 1;       //用于标识读了多少，为1是为了进入循环   
							//创建缓冲区   
	CHAR *szBuffer = new CHAR[nFileLen + 1];
	TRY
	{
		//创建文件   
		CFile PicFile(strSavePath,CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive);
	while (dwRead>0)
	{
		//清空缓冲区   
		memset(szBuffer,0,(size_t)(nFileLen + 1));
		//读取到缓冲区   
		dwRead = pfile->Read(szBuffer,nFileLen);
		//写入到文件   
		PicFile.Write(szBuffer,dwRead);
	}
	//关闭文件   
	PicFile.Close();
	//释放内存   
	delete[]szBuffer;
	delete pfile;
	}
	CATCH(CFileException, e)
	{
		//释放内存   
		delete[]szBuffer;
		delete pfile;
		return -7;          //读写文件异常   
	}
	END_CATCH
		return 0;
}

void CMainUIDlg::GetLinesFromBuffer(CString strUrlBuffer, vector<string>& vecLines)
{
	//CString tmp = CStringA(strUrlBuffer);
	string buffer= CStringA(strUrlBuffer);
	//string pattern("http(s)?://([\\w-]+\\.)+[\\w-]+(/[\\w- ./?%&=]*)?");    //匹配规则很简单，如果有疑惑，可以对照语法查看
	string pattern("((http|ftp|https)://)(([a-zA-Z0-9\._-]+\.[a-zA-Z]{2,6})|([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}))(:[0-9]{1,4})*(/[a-zA-Z0-9\&%_\./-~-]*)?");
	regex r(pattern);
	smatch results; 
	regex_match(buffer, results, r);
	for (std::sregex_iterator it(buffer.begin(), buffer.end(), r), end;     //end是尾后迭代器，regex_iterator是regex_iterator的string类型的版本
		it != end;
		++it)
	{
		//std::cout << it->str() << std::endl;
		vecLines.push_back(it->str());
	}
}

void CMainUIDlg::OnBnClickedBtnAddseed()
{
	// TODO: 在此添加控件通知处理程序代码
	//if(NULL == m_pSearcher)
	//	m_pSearcher = new Searcher;
	////UpdateData(FALSE); //可以向控件传递数据
	//UpdateData(TRUE); //可以从控件获得数据 
	//string strSeed = CStringA(m_cstrSeed);
	//if (!m_pSearcher->CheckUrl(strSeed))
	//{
	//	MessageBox(L"url非法！");
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
	//获取服务器返回的状态号   
	//pHtFile->QueryInfoStatusCode(dwRet);
	if (dwStatusCode != HTTP_STATUS_OK)
	{
		return ;
	}
	//获取文件大小   
	UINT nFileLen = (UINT)pfile->GetLength();
	DWORD dwRead = 1;       //用于标识读了多少，为1是为了进入循环   
							//创建缓冲区   
	CHAR *szBuffer = new CHAR[nFileLen + 1];
	CString cstrContent;
	//创建文件   
	CFile PicFile(L"C:\\Users\\bigre\\Desktop\\test.html",CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive);
	while (dwRead>0)
	{
		//清空缓冲区   
		memset(szBuffer, 0, (size_t)(nFileLen + 1));
		//读取到缓冲区   
		dwRead = pfile->Read(szBuffer, nFileLen);
		//写入到文件   
		PicFile.Write(szBuffer, dwRead);
		cstrContent += szBuffer;
	}
}


void CMainUIDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	//if (NULL == m_pSearcher)
	//	MessageBox(L"请设置种子链接");
	//vector<URL> seeds;
	//m_pSearcher->GetSeeds(seeds);
	//if(seeds.size()==0)
	//	MessageBox(L"请设置种子链接");
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
	//);//用于创建工作者线程

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
