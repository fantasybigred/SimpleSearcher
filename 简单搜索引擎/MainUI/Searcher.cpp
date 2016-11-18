#include "stdafx.h"
#include "Searcher.h"
#include <afxinet.h>
#include <afxstr.h>

//extern CRITICAL_SECTION g_cs; // �ٽ����ṹ����
CRITICAL_SECTION g_cs; // �ٽ����ṹ����

Searcher::Searcher()
{
	m_strSeeds.insert("http://www.163.com");
	m_strSeeds.insert("http://www.sina.com");
	for (auto item : IMG_TYPES)
	{
		m_ImgTypeSet.insert(item);
	}
	InitializeCriticalSection(&g_cs);
}

UINT Searcher::StartSearch(LPVOID pParam)
{
	//InitPreURLList();
	while (m_lstPreURLs.size() != 0)
	{
		EnterCriticalSection(&g_cs);// �����ٽ���
		URL curUrl = m_lstPreURLs.front();
		m_lstPreURLs.pop_front();
		CString cstrPageContent;
		//GetHTML(CString(curUrl.c_str()), cstrPageContent);
		CString filepath;
		filepath.Format(L"HTMLS/page%d.html", m_pageCnt);
		m_pageCnt++;
		LeaveCriticalSection(&g_cs);// �뿪�ٽ���

		ParseURL(CString(curUrl.c_str()), filepath, cstrPageContent);
		//m_lstPostURLs.push_back(curUrl);
		m_setPostURLs.insert(curUrl);
		//vector<URL> vecLinks;
		list<URL> lstLinks;
		GetLinksFromBuffer(cstrPageContent, lstLinks);
		DeleteUselessLinks(lstLinks);
		DeleteRepeatLinks(lstLinks);
		AddUnAccessLinks(lstLinks);
	}
	return 0;
}

DWORD Searcher::AddUnAccessLinks(const list<URL>& lstLinks)
{
	EnterCriticalSection(&g_cs);// �����ٽ���
	for (auto item : lstLinks)
	{
		m_lstPreURLs.push_back(item);
	}
	LeaveCriticalSection(&g_cs);// �뿪�ٽ���
	return SR_SUCCESS;
}

DWORD Searcher::InitPreURLList()
{
	if (m_strSeeds.size() == 0)
		return SR_FAILURE;
	for (auto item : m_strSeeds)
	{
		m_lstPreURLs.push_back(item);
	}
	return SR_SUCCESS;
}

DWORD Searcher::DeleteRepeatLinks(list<URL>& lstLinks)
{
	set<URL> setTmp;
	for (auto item : lstLinks)//ȥ��ҳ���е��ظ�����
	{
		setTmp.insert(item);
	}

	for (set<URL>::iterator it = setTmp.begin(); it != setTmp.end();)//ȥ���Ѿ����ʹ�������
	{
		if (m_setPostURLs.count(*it) == 1)
			setTmp.erase(it++);
		else
			it++;
	}

	lstLinks.clear();
	for (auto item : setTmp)
	{
		lstLinks.push_back(item);
	}
	return SR_SUCCESS;
}

DWORD Searcher::SetSeed(const string & seed)
{
	if (m_strSeeds.count(seed) == 1)
		return SR_FAILURE;
	m_strSeeds.insert(seed);
	return 0;
}

DWORD Searcher::GetSeeds(vector<string>& vecSeeds)
{
	if (m_strSeeds.size() == 0)
		return SR_FAILURE;
	for (auto seed : m_strSeeds)
	{
		vecSeeds.push_back(seed);
	}
	return SR_SUCCESS;
}

DWORD Searcher::SetSearchStrategy(BYTE byStategy)
{
	m_strategyType = (SR_STRATEGY_TYPE)byStategy;
	return SR_SUCCESS;
}

DWORD Searcher::GetCurrentStrategy(BYTE & byStategy)
{
	byStategy = m_strategyType;
	return SR_SUCCESS;
}

DWORD Searcher::GetPageContent(URL url, PAGE & page)
{
	CInternetSession MySession;
	CString StrURL("http://www.163.com");
	CString StrInfo("��ѯ������£�\n");
	CInternetFile* MyFile = NULL;
	try
	{
		MyFile = (CInternetFile*)MySession.OpenURL(StrURL);
	}
	catch (CInternetException* pEx)
	{
		MyFile = NULL;
		pEx->Delete();
	}
	if (MyFile)
	{
		CString StrLine;
		for (int i = 0; i < 20 && MyFile->ReadString(StrLine); i++)
		{
			CString tmp = StrLine + "\r\n";
			StrInfo += tmp;
		}
		MyFile->Close();
		delete MyFile;
	}
	else
	{
		StrInfo += " û�в�ѯ���!\r\n";
	}
	return 0;
}

DWORD Searcher::SearchPages()
{

	return 0;
}

DWORD Searcher::SavePages(const vector<string>& vecPages)
{
	return 0;
}

DWORD Searcher::UpdateOldPages()
{
	return 0;
}

DWORD Searcher::SvaeFile(CHttpFile* pFile, const CString strSavePath)
{
	//��ȡ�ļ���С   
	UINT nFileLen = (UINT)pFile->GetLength();
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
		dwRead = pFile->Read(szBuffer,nFileLen);
		//д�뵽�ļ�   
		PicFile.Write(szBuffer,dwRead);
	}
	//�ر��ļ�   
	PicFile.Close();
	//�ͷ��ڴ�   
	delete[]szBuffer;
	delete pFile;
	}
		CATCH(CFileException, e)
	{
		//�ͷ��ڴ�   
		delete[]szBuffer;
		//delete pFile;
		return SR_FAILURE;          //��д�ļ��쳣   
	}
	END_CATCH
		return SR_SUCCESS;
	return SR_SUCCESS;
}

DWORD Searcher::GetLinksFromBuffer(const CString strUrlBuffer, list<URL>& lstLinks)
{
	string buffer = CStringA(strUrlBuffer);
	//string pattern("http(s)?://([\\w-]+\\.)+[\\w-]+(/[\\w- ./?%&=]*)?");
	string pattern("((http|ftp|https)://)(([a-zA-Z0-9\._-]+\.[a-zA-Z]{2,6})|([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}))(:[0-9]{1,4})*(/[a-zA-Z0-9\&%_\./-~-]*)?");
	regex r(pattern);
	smatch results;
	regex_match(buffer, results, r);
	for (std::sregex_iterator it(buffer.begin(), buffer.end(), r), end;     //end��β���������regex_iterator��regex_iterator��string���͵İ汾
		it != end;
		++it)
	{
		//std::cout << it->str() << std::endl;
		lstLinks.push_back(it->str());
	}
	return 0;
}

DWORD Searcher::GetHTML(CString url, CString & cstrContent)
{
	CInternetSession session;
	CHttpFile* pfile = (CHttpFile*)session.OpenURL(url, 1, INTERNET_FLAG_TRANSFER_ASCII || INTERNET_FLAG_RELOAD, NULL, 0);
	//CString strHtml;
	DWORD dwStatusCode;
	pfile->QueryInfoStatusCode(dwStatusCode);
	if (dwStatusCode != HTTP_STATUS_OK)
		return SR_FAILURE;

	char strBuff[1025] = { 0 };
	//CString strHtml;
	while ((pfile->Read((void*)strBuff, 1024)) > 0) {
		CString strCode1(strBuff);
		cstrContent += strCode1;
	}
	//CString filepath = L"../../HTMLS/" + url;
	//CString filepath = L"HTMLS/page" + m_pageCnt;
	//filepath += L".html";
	CString filepath;
	filepath.Format(L"HTMLS/page%d.html", m_pageCnt);
	//CString filepath = L"C:\\Users\\bigre\\Desktop\\" + url;
	//SvaeFile(pfile, filepath);
	SaveFile(cstrContent, filepath);
	pfile->Close();
	return SR_SUCCESS;
}

bool Searcher::CheckUrl(URL url)
{
	//string pattern("((http|ftp|https)://){0,1}(([a-zA-Z0-9\._-]+\.[a-zA-Z]{2,6})|([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}))(:[0-9]{1,4})*(/[a-zA-Z0-9\&%_\./-~-]*)?");
	string pattern("((http[s]{0,1}|ftp)://){0,1}[a-zA-Z0-9\\.\\-]+\\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?");
	regex r(pattern);
	smatch results;
	if (regex_search(url, results, r))
	{
		return true;
	}
	return false;
}

DWORD Searcher::SaveFile(const CString& content, CString filepath)
{
	CStdioFile file;//ͷ�ļ������  
	CString filename_routelist��szFilePath; 
	//filename_routelist="key.txt";    
	if(file.Open(filepath,CFile::modeCreate|CFile::typeText|CFile::modeReadWrite)) //���û��key.txt�ļ� ���ڸ�·���½������ļ�  
	{    
		file.WriteString(content);
		file.Close();
		return SR_SUCCESS;
	} 
	return SR_FAILURE;
}

Searcher::~Searcher()
{
}

DWORD Searcher::m_pageCnt = 0;

INT Searcher::ParseURL(const CString strUrl, const CString strSavePath, CString& cstrContent)
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
		cstrContent += szBuffer;
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
DWORD Searcher::DeleteUselessLinks(list<URL>& links)
{
	for (list<URL>::iterator it = links.begin(); it != links.end();)
	{
		string tmpLink = *it;
		if (it->size() <= 4)
		{
			it = links.erase(it);
			continue;
		}
		string tail = tmpLink.substr(tmpLink.size() - 4);
		Str2Lower(tail);
		if (m_ImgTypeSet.count(tail) != 0 || tail == "tiff" || tail == ".jsp" || tail.substr(1) == ".js")
		{

			it = links.erase(it);
			continue;
		}
		++it;
	}
	return SR_SUCCESS;
}

void Searcher::Str2Lower(string& str)
{
	for (auto& item : str)
	{
		tolower(item);
	}
}