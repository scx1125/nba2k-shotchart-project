// DllHijack.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "DllHijact.h"


Direct3DCreate9Type RealDirect3DCreate9 = NULL;


// ��MyDirect3DCreate9����ΪDirect3DCreate9����__declspec(dllexport)�Ļ�ʵ���ϵ�������_MyDirect3DCreate9@4
#ifndef _WIN64
#pragma comment(linker, "/EXPORT:Direct3DCreate9=_MyDirect3DCreate9@4")
#else
#pragma comment(linker, "/EXPORT:Direct3DCreate9=MyDirect3DCreate9")
#endif
extern "C" void* WINAPI MyDirect3DCreate9(UINT SDKVersion)
{
	MessageBox(NULL, _T("������Direct3DCreate9"), _T(""), MB_OK);
	return RealDirect3DCreate9(SDKVersion);
}
