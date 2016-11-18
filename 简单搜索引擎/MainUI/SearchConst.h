#pragma once
#include <cwctype>
using namespace std;

const DWORD SR_SUCCESS = 0;
const DWORD SR_FAILURE = 1;

//const BYTE SR_STRATEGY_TRAVERSAL = 0;

typedef enum
{
	SR_STRATEGY_NONE,
	SR_STRATEGY_TRAVERSAL,//以宽度优先、深度优先或启发式方式循环地在互联网中发现信息
	SR_STRATEGY_NOLIMIT_BY_REGION,//将Web空间按照域名、IP地址或国家域名划分，每个搜索器负责一个子空间的穷尽搜索
}SR_STRATEGY_TYPE;

typedef string URL;
typedef string PAGE;

const string IMG_TYPES[] = { ".bmp",".jpg",".tiff",".gif",".pcx",".tga",".exif",".fpx",".svg",".psd",".cdr",".pcd",".dxf",".ufo",".eps",".ai",".raw",".wmf", ".png"};

//extern CRITICAL_SECTION g_cs; // 临界区结构对象

