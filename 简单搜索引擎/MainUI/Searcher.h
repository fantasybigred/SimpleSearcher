#pragma once
#include <cwctype>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <regex>
#include <iostream>
#include "SearchConst.h"
#include <afxinet.h>
#include <afxstr.h>

using namespace std;

class Searcher
{
public:
	Searcher();
	UINT StartSearch(LPVOID pParam);

	DWORD AddUnAccessLinks(const list<URL>& lstLinks);

	DWORD SetSeed(const string& seed);
	DWORD GetSeeds(vector<string>& vecSeeds);
	DWORD SetSearchStrategy(BYTE byStategy);
	DWORD GetCurrentStrategy(BYTE& byStategy);
	DWORD GetPageContent(URL url, PAGE& page);
	bool CheckUrl(URL url);
	DWORD SaveFile(const CString & content, CString filepath);
	virtual ~Searcher();
	INT ParseURL(const CString strUrl, const CString strSavePath, CString& cstrContent);
	DWORD DeleteUselessLinks(list<URL>& links);
	DWORD InitPreURLList();
private:
	
	DWORD DeleteRepeatLinks(list<URL>& vecLinks);
	DWORD SearchPages();
	DWORD SavePages(const vector<string>& vecPages);
	DWORD UpdateOldPages();
	DWORD SvaeFile(CHttpFile* pFile, const CString strSavePath);
	DWORD GetLinksFromBuffer(const CString strUrlBuffer, list<string>& vecLinks);
	DWORD GetHTML(CString url, CString& cstrContent);

	void Str2Lower(string& str);
	
private:
	set<string> m_strSeeds;
	SR_STRATEGY_TYPE m_strategyType;
	list<URL> m_lstPreURLs;//待抓取URL队列
	//list<URL> m_lstPostURLs;//已抓取URL队列
	set<URL> m_setPostURLs;//已抓取URL队列
	map<URL, PAGE> m_MapPages;//已下载网页库
	static DWORD m_pageCnt;
	set<string> m_ImgTypeSet;
};

