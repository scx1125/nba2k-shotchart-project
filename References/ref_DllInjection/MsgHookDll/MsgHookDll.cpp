// MsgHookDll.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"


extern "C" __declspec(dllexport) // �����������
LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam)
{
	return CallNextHookEx(NULL, code, wParam, lParam);
}
