// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "DllHijact.h"


HMODULE g_d3d9Module = NULL;


// DLL�����ء�ж��ʱ����
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	TCHAR processPath[MAX_PATH];
	TCHAR msg[MAX_PATH + 20];

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		GetModuleFileName(GetModuleHandle(NULL), processPath, MAX_PATH);
		_tcscpy_s(msg, _T("ע���˽��� "));
		_tcscat_s(msg, processPath);
		MessageBox(NULL, msg, _T(""), MB_OK);

		// ����ԭDLL����ȡ������Direct3DCreate9��ַ
		g_d3d9Module = LoadLibrary(_T("C:\\Windows\\System32\\d3d9.dll"));
		RealDirect3DCreate9 = (Direct3DCreate9Type)GetProcAddress(g_d3d9Module, "Direct3DCreate9");
		if (RealDirect3DCreate9 == NULL)
		{
			MessageBox(NULL, _T("��ȡDirect3DCreate9��ַʧ��"), _T(""), MB_OK);
			return FALSE;
		}

		break;

	case DLL_PROCESS_DETACH:
		MessageBox(NULL, _T("DLLж����"), _T(""), MB_OK);

		// �ֶ�ж��ԭDLL
		FreeLibrary(g_d3d9Module);

		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

