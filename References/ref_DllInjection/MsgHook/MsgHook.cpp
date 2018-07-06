// MsgHook.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <tlhelp32.h>


// ����������Ȩ������ĳЩ������ʧ��
BOOL EnablePrivilege(BOOL enable)
{
	// �õ����ƾ��
	HANDLE hToken = NULL;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY | TOKEN_READ, &hToken))
		return FALSE;

	// �õ���Ȩֵ
	LUID luid;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
		return FALSE;

	// �������ƾ��Ȩ��
	TOKEN_PRIVILEGES tp = {};
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL))
		return FALSE;

	// �ر����ƾ��
	CloseHandle(hToken);

	return TRUE;
}

DWORD GetProcessThreadID(DWORD pid)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
	THREADENTRY32 threadentry;
	threadentry.dwSize = sizeof(threadentry);

	BOOL hasNext = Thread32First(snapshot, &threadentry);
	DWORD threadID = 0;
	do
	{
		if (threadentry.th32OwnerProcessID == pid)
		{
			threadID = threadentry.th32ThreadID;
			break;
		}
		hasNext = Thread32Next(snapshot, &threadentry);
	} while (hasNext);

	CloseHandle(snapshot);
	return threadID;
}

// ע��DLL�����ع��Ӿ����DLL���뵼��CallWndProc���ӹ��̣�pid = 0��װȫ�ֹ���
HHOOK InjectDll(DWORD pid, LPCTSTR dllPath)
{
	// ����DLL
	HMODULE module = LoadLibrary(dllPath);
	if (module == NULL)
	{
		printf("����DLLʧ�ܣ�������룺%u\n", GetLastError());
		return NULL;
	}
	// ȡ���ӹ��̵�ַ
	HOOKPROC proc = (HOOKPROC)GetProcAddress(module, "CallWndProc");
	if (proc == NULL)
	{
		printf("ȡ���ӹ��̵�ַʧ�ܣ�������룺%u\n", GetLastError());
		return NULL;
	}

	// ȡ�߳�ID
	DWORD threadID = 0;
	if (pid != 0)
	{
		threadID = GetProcessThreadID(pid);
		if (threadID == 0)
		{
			printf("ȡ�߳�IDʧ��\n");
			return NULL;
		}
	}

	// ��װ����
	HHOOK hook = SetWindowsHookEx(WH_GETMESSAGE, proc, module, threadID);

	// �ͷ�
	FreeLibrary(module);

	return hook;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// ����Ȩ�ޣ���ʵ������Ҳ���ԣ��Թ���Ա������о��У�
	EnablePrivilege(TRUE);

	// ȡPID
	//DWORD pid = 0; // ȫ�ֹ��ӣ�����ȫ�ֹ��Ӳ�Ȼ�������...
	HWND hwnd = FindWindow(NULL, _T("���������"));
	if (hwnd == NULL)
	{
		printf("Ѱ�Ҵ���ʧ�ܣ�������룺%u\n", GetLastError());
		return 1;
	}
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);

	// ע��DLL
	// Ҫ��MsgHookDll.dll���ڱ�����ǰĿ¼��
	HHOOK hook = InjectDll(pid, _T("MsgHookDll.dll"));
	if (hook == NULL)
		return 2;

	// ��ͣ
	printf("���س�ж��DLL\n");
	getchar();

	// ж��DLL
	UnhookWindowsHookEx(hook);

	return 0;
}

