#pragma once
#include <cwctype>
using namespace std;

const DWORD SR_SUCCESS = 0;
const DWORD SR_FAILURE = 1;

//const BYTE SR_STRATEGY_TRAVERSAL = 0;

typedef enum
{
	SR_STRATEGY_NONE,
	SR_STRATEGY_TRAVERSAL,//�Կ�����ȡ�������Ȼ�����ʽ��ʽѭ�����ڻ������з�����Ϣ
	SR_STRATEGY_NOLIMIT_BY_REGION,//��Web�ռ䰴��������IP��ַ������������֣�ÿ������������һ���ӿռ�������
}SR_STRATEGY_TYPE;

typedef string URL;
typedef string PAGE;

const string IMG_TYPES[] = { ".bmp",".jpg",".tiff",".gif",".pcx",".tga",".exif",".fpx",".svg",".psd",".cdr",".pcd",".dxf",".ufo",".eps",".ai",".raw",".wmf", ".png"};

//extern CRITICAL_SECTION g_cs; // �ٽ����ṹ����

