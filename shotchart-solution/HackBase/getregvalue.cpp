#include "hackbase.h"
#include "trainerbase.h"

std::string GetRegValue(int nKeyType, const std::string& strUrl, const std::string& strKey) {
	std::string strValue("");
	HKEY hKey = NULL;
	HKEY  hKeyResult = NULL;
	DWORD dwSize = 0;
	DWORD dwDataType = 0;

	switch (nKeyType)
	{
	case 0:
	{
		hKey = HKEY_CLASSES_ROOT;
		break;
	}
	case 1:
	{
		hKey = HKEY_CURRENT_USER;
		break;
	}
	case 2:
	{
		hKey = HKEY_LOCAL_MACHINE;
		break;
	}
	case 3:
	{
		hKey = HKEY_USERS;
		break;
	}
	case 4:
	{
		hKey = HKEY_PERFORMANCE_DATA;
		break;
	}
	case 5:
	{
		hKey = HKEY_CURRENT_CONFIG;
		break;
	}
	case 6:
	{
		hKey = HKEY_DYN_DATA;
		break;
	}
	case 7:
	{
		hKey = HKEY_CURRENT_USER_LOCAL_SETTINGS;
		break;
	}
	case 8:
	{
		hKey = HKEY_PERFORMANCE_TEXT;
		break;
	}
	case 9:
	{
		hKey = HKEY_PERFORMANCE_NLSTEXT;
		break;
	}
	default:
	{
		return strValue;
	}
	}

	//��ע���
	if (ERROR_SUCCESS == RegOpenKeyEx(hKey, strUrl.c_str(), 0, KEY_QUERY_VALUE, &hKeyResult))
	{
		// ��ȡ����ĳ���dwSize������dwDataType
		RegQueryValueEx(hKeyResult, strKey.c_str(), 0, &dwDataType, NULL, &dwSize);
		switch (dwDataType)
		{
		case REG_MULTI_SZ:
		{
			//�����ڴ��С
			BYTE* lpValue = new BYTE[dwSize];
			//��ȡע�����ָ���ļ�����Ӧ��ֵ
			LONG lRet = RegQueryValueEx(hKeyResult, strKey.c_str(), 0, &dwDataType, lpValue, &dwSize);
			delete[] lpValue;
			break;
		}
		case REG_SZ:
		{
			//�����ڴ��С
			char* lpValue = new char[dwSize];
			memset(lpValue, 0, dwSize * sizeof(char));
			//��ȡע�����ָ���ļ�����Ӧ��ֵ
			if (ERROR_SUCCESS == RegQueryValueEx(hKeyResult, strKey.c_str(), 0, &dwDataType, (LPBYTE)lpValue, &dwSize)) {
				strValue = lpValue;
			}
			delete[] lpValue;
			break;
		}
		default:
			break;
		}
	}

	//�ر�ע���
	RegCloseKey(hKeyResult);

	return strValue;
}