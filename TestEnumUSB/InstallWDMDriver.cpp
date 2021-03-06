// WinInstallWin.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <windows.h>
#include <newdev.h>
#include <setupapi.h>

//#pragma comment(lib, "newdev.lib")
//#pragma comment(lib, "setupapi.lib")

#ifndef MAX_DEVICE_ID_LEN
#define MAX_DEVICE_ID_LEN     200
#define MAX_DEVNODE_ID_LEN    MAX_DEVICE_ID_LEN
#define MAX_GUID_STRING_LEN   39          // 38 chars + terminator null
#define MAX_CLASS_NAME_LEN    32
#endif


HDEVINFO _hDevInfo = 0L;
WORD g_wVender = 0;
WORD g_wHardware = 0;
TCHAR g_strVender[20][64] = {0};
TCHAR g_strHardware[20][64] = {0};
//TCHAR g_strHID[MAX_PATH+1] = {0};


VOID ShowErrorMsg(WORD Count,LPCTSTR szData)
{
	printf("%d\n%s",&Count,&szData);

}

VOID FindComma(LPSTR szData)
{
	WORD wLen = (WORD)strlen(szData);
	WORD wIdx;
	WORD wLoop; 
	CHAR szTmp[128] = {0};

	for (wIdx = 0, wLoop = 0; wLoop < wLen; wLoop++)
	{
		if (szData[wLoop] == ',')
			szData[wLoop] = '.';
		else if (szData[wLoop] == ' ')
			continue;
		szTmp[wIdx++] = szData[wLoop];
	}
	memcpy(szData, szTmp, wIdx*sizeof(char));
	szData[wIdx] = 0;
}

VOID StrLTrim(LPSTR szData)
{
	LPSTR ptr = szData;

	while (isspace(*ptr))
		//ptr = _tcsinc(ptr);
		ptr++;

	if (strcmp(ptr, szData))
	{
		WORD wLen = (WORD)(strlen(szData) - (ptr - szData));
		memmove(szData, ptr, (wLen+1)*sizeof(char));
	}
}

VOID StrRTrim(LPSTR szData)
{
	LPSTR ptr  = szData;
	LPSTR pTmp = NULL;

	while (*ptr != 0)
	{
		if (isspace(*ptr))
		{
			if (!pTmp)
				pTmp = ptr;
		}
		else
			pTmp = NULL;
		//ptr = _tcsinc(ptr);
		ptr++;
	}

	if (pTmp)
	{
		*pTmp = 0;
		memmove(szData, szData, strlen(szData) - strlen(pTmp));
	}
}

VOID StrRight(LPSTR szData, WORD wCount)
{
	WORD wLen = (WORD)strlen(szData) - wCount;

	if (wCount > 0x7FFF)//负数
		wCount = 0;
	if (wCount >= (WORD)strlen(szData))
		return;

	memmove(szData, szData + wLen, wCount * sizeof(char));
	szData[wCount] = 0;
}

VOID ConvertGUIDToString(const GUID guid, LPSTR pData)
{
	CHAR szData[30] = {0};
	CHAR szTmp[3]   = {0};
	WORD wLoop;

	sprintf(pData, "%04X-%02X-%02X-", guid.Data1, guid.Data2, guid.Data3);
	for (wLoop = 0; wLoop < 8; wLoop++)
	{
		if (wLoop == 2)
			strcat(szData, "-");
		sprintf(szTmp, "%02X", guid.Data4[wLoop]);
		strcat(szData, szTmp);
	}

	memcpy(pData + strlen(pData), szData, strlen(szData));
}

BOOL AnsiToUnicode(LPCSTR Source, const WORD sLen, LPWSTR Destination, const WORD wLen)
{
	return MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Source, sLen, Destination, wLen);
}

BOOL UnicodeToAnsi(LPCWSTR Source, const WORD wLen, LPSTR Destination, const WORD sLen)
{
	return WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, Source, wLen, Destination, sLen, 0L, 0L);
}



// 初始化全局变量
__inline VOID InitialGlobalVar()
{
	WORD wLoop;

	g_wVender = g_wHardware = 0;
	for (wLoop = 0; wLoop < 20; wLoop++)
	{
		RtlZeroMemory(g_strVender[wLoop], sizeof(TCHAR)*64);
		RtlZeroMemory(g_strHardware[wLoop], sizeof(TCHAR)*64);
	}
}

__inline BOOL IsInstalled()
{
	HDEVINFO hDevInfo = 0L;
	SP_DEVINFO_DATA spDevInfoData = {0L};
	WORD wIdx;
	BOOL bIsFound;

	hDevInfo = SetupDiGetClassDevs(0L, 0, 0, DIGCF_ALLCLASSES | DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		ShowErrorMsg(GetLastError(), _T("SetupDiGetClassDevs"));
		return FALSE;
	}

	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	wIdx = 0;
	bIsFound = 0;
	while (++wIdx)
	{
		if (SetupDiEnumDeviceInfo(hDevInfo, wIdx, &spDevInfoData))
		{
			LPTSTR ptr;
			LPBYTE pBuffer = NULL;
			DWORD dwData  = 0L;
			DWORD dwRetVal;
			DWORD dwBufSize = 0L;

			while (TRUE)
			{
				dwRetVal = SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, SPDRP_HARDWAREID,
					&dwData, (PBYTE)pBuffer, dwBufSize, &dwBufSize);
				if (!dwRetVal)
					dwRetVal = GetLastError();
				else
					break;
				if (dwRetVal == ERROR_INVALID_DATA)
					break;
				else if (dwRetVal == ERROR_INSUFFICIENT_BUFFER)
				{
					if (pBuffer)
						LocalFree(pBuffer);
					pBuffer = (LPBYTE)LocalAlloc(LPTR, dwBufSize);
				}
				else
				{
					ShowErrorMsg(dwRetVal, _T("SetupDiGetDeviceRegistryProperty"));
					SetupDiDestroyDeviceInfoList(hDevInfo);
					return FALSE;
				}
			}

			if (dwRetVal == ERROR_INVALID_DATA) 
				continue;

			for (ptr = (LPTSTR)pBuffer; *ptr && (ptr < (LPTSTR)&pBuffer[dwBufSize]); ptr += _tcslen(ptr) + sizeof(TCHAR))
			{
				WORD wLoop;

				for (wLoop = 0; wLoop < g_wHardware; wLoop++)
				{
					if (!_tcscmp(g_strHardware[wLoop], ptr))
					{
						bIsFound = TRUE;
						break;
					}
				}
			}
			if (pBuffer)
				LocalFree(pBuffer);
			if (bIsFound)
				break;
		}
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return bIsFound;
}

BOOL FindSectionName(FILE *pFile, const char *szKey)
{
	char szData[256] = {0};

	if (!pFile)
		return FALSE;

	rewind(pFile);
	while (!feof(pFile))
	{
		fgets(szData, 255, pFile);
		szData[strlen(szData)-1] = 0;
		if (strcmp(szKey, szData) == 0)
			return TRUE;
	}
	return FALSE;
}

__inline BOOL GetSectionData(FILE* pFile, const char* szKey, const char bIsVender)
{
	char szData[128] = {0};

	if (bIsVender)
		strcpy(szData, szKey);
	else
		sprintf(szData, "[%s]", szKey);

	if (FindSectionName(pFile, szData) == FALSE)
		return FALSE;

	RtlZeroMemory(szData, sizeof(char)*128);
	while (!feof(pFile))
	{
		char *str = NULL;
		fgets(szData, 127, pFile);
		szData[strlen(szData)-1] = 0;
		StrLTrim(szData);
		StrRTrim(szData);
		if (!*szData)
			continue;
		if (szData[0] == ';')
			continue;

		if (strchr(szData, '['))
		{
			StrLTrim(szData);
			if (szData[0] != ';')
				return 1;
			else
				continue;
		}

		if (bIsVender)
			str = strchr(szData, '=');
		else
			str = strchr(szData, ',');

		if (*str)
		{
			char szTmp[128] = {0};
			WORD pos = (WORD)(str - szData + 1);

			StrRight(szData, (short)(strlen(szData)-pos));
			StrLTrim(szData);
			StrRTrim(szData);
			FindComma(szData);
			if (bIsVender)
			{
				//AnsiToUnicode(szData, strlen(szData), g_strVender[g_wVender++], 64);
				strcpy(g_strVender[g_wVender++], szData);
			}
			else
			{
				//AnsiToUnicode(szData, strlen(szData), g_strHardware[g_wHardware++], 64);
				strcpy(g_strHardware[g_wHardware++], szData);
			}
		}/* end if */
	}
	return TRUE;
}

BOOL GetINFData(FILE *pFile)
{
	WORD wLoop;

	if (!g_wVender || !g_wHardware)
		InitialGlobalVar();
	if (GetSectionData(pFile, "[Manufacturer]", TRUE) == FALSE)
		return 0;

	for (wLoop = 0; wLoop < g_wVender; wLoop++)
	{
		CHAR szVender[64] = {0};
		//UnicodeToAnsi(g_strVender[wLoop], _tcslen(g_strVender[wLoop]), szVender, 64);
		strcpy( szVender, g_strVender[wLoop]);
		GetSectionData(pFile, szVender, FALSE);
	}
	if (g_wHardware != 0)
	{
		if (IsInstalled() == TRUE)//如果已经安装
			return FALSE;
		else
			return TRUE;
	}
	return FALSE;
}

__inline BOOL InstallClassDriver(LPCTSTR theINFName)
{
	GUID guid = {0};
	SP_DEVINFO_DATA spDevData = {0};
	HDEVINFO hDevInfo = 0L;
	TCHAR className[MAX_CLASS_NAME_LEN] = {0};
	LPTSTR pHID = NULL;
	WORD wLoop;
	BOOL bRebootRequired;

	if (!SetupDiGetINFClass(theINFName, &guid, className, MAX_CLASS_NAME_LEN, 0))
	{
		ShowErrorMsg(GetLastError(), _T("SetupDiGetINFClass"));
		return FALSE;
	}

	hDevInfo = SetupDiCreateDeviceInfoList(&guid, 0);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		ShowErrorMsg(GetLastError(), _T("SetupDiCreateDeviceInfoList"));
		return FALSE;
	}

	spDevData.cbSize = sizeof(SP_DEVINFO_DATA);
	if (!SetupDiCreateDeviceInfo(hDevInfo, className, &guid, 0L, 0L, DICD_GENERATE_ID, &spDevData))
	{
		ShowErrorMsg(GetLastError(), _T("SetupDiCreateDeviceInfo"));
		SetupDiDestroyDeviceInfoList(hDevInfo);
		return FALSE;
	}

	for (wLoop = 0; wLoop < g_wHardware; wLoop++)
	{
		printf("Installing for [%d] %s\n", wLoop, g_strHardware[wLoop] );

		if (pHID)
			LocalFree(pHID);

		pHID = (LPTSTR)LocalAlloc(LPTR, _tcslen(g_strHardware[wLoop])*2*sizeof(TCHAR));
		if (!pHID)
		{
			ShowErrorMsg(GetLastError(), _T("LocalAlloc"));
			SetupDiDestroyDeviceInfoList(hDevInfo);
			return FALSE;
		}

		_tcscpy(pHID, g_strHardware[wLoop]);
		if (!SetupDiSetDeviceRegistryProperty(hDevInfo, &spDevData, SPDRP_HARDWAREID, (PBYTE)pHID,
			(DWORD)(_tcslen(g_strHardware[wLoop])*2*sizeof(TCHAR))))
		{
			ShowErrorMsg(GetLastError(), _T("SetupDiSetDeviceRegistryProperty"));
			SetupDiDestroyDeviceInfoList(hDevInfo);
			LocalFree(pHID);
			return FALSE;
		}

		if (!SetupDiCallClassInstaller(DIF_REGISTERDEVICE, hDevInfo, &spDevData))
		{
			ShowErrorMsg(GetLastError(), _T("SetupDiCallClassInstaller"));
			SetupDiDestroyDeviceInfoList(hDevInfo);
			LocalFree(pHID);
			return FALSE;
		}

		bRebootRequired = FALSE;
		if (!UpdateDriverForPlugAndPlayDevices(0L, g_strHardware[wLoop], theINFName, 
			INSTALLFLAG_FORCE, &bRebootRequired))
		{
			DWORD dwErrorCode = GetLastError();
			// ERROR_NO_CATALOG_FOR_OEM_INF
			if (!SetupDiCallClassInstaller(DIF_REMOVE, hDevInfo, &spDevData))
				ShowErrorMsg(GetLastError(), _T("SetupDiCallClassInstaller(Remove)"));
			ShowErrorMsg(dwErrorCode, _T("UpdateDriverForPlugAndPlayDevices"));
			SetupDiDestroyDeviceInfoList(hDevInfo);
			LocalFree(pHID);
			return FALSE;
		}
		LocalFree(pHID);
		pHID = NULL;
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);
	_tprintf(_T("Install Successed\n"));
	return TRUE;
}

// 安装WDM驱动
BOOL StartInstallWDMDriver(LPCTSTR theInfName)
{
	HDEVINFO hDevInfo = 0L;
	GUID guid = {0L};
	SP_DEVINSTALL_PARAMS spDevInst = {0L};
	TCHAR strClass[MAX_CLASS_NAME_LEN] = {0L};

	if (!SetupDiGetINFClass(theInfName, &guid, strClass, MAX_CLASS_NAME_LEN, 0))
	{
		ShowErrorMsg(GetLastError(), _T("SetupDiGetINFClass"));
		return FALSE;
	}

	hDevInfo = SetupDiGetClassDevs(&guid, 0L, 0L, DIGCF_PRESENT | DIGCF_ALLCLASSES | DIGCF_PROFILE);
	if (!hDevInfo)
	{
		ShowErrorMsg(GetLastError(), _T("SetupDiGetClassDevs"));
		return FALSE;
	}

	spDevInst.cbSize = sizeof(SP_DEVINSTALL_PARAMS);
	if (!SetupDiGetDeviceInstallParams(hDevInfo, 0L, &spDevInst))
	{
		ShowErrorMsg(GetLastError(), _T("SetupDiGetDeviceInstallParams"));
		return FALSE;
	}

	spDevInst.Flags   = DI_ENUMSINGLEINF;
	spDevInst.FlagsEx = DI_FLAGSEX_ALLOWEXCLUDEDDRVS;
	_tcscpy(spDevInst.DriverPath, theInfName);
	if (!SetupDiSetDeviceInstallParams(hDevInfo, 0, &spDevInst))
	{
		ShowErrorMsg(GetLastError(), _T("SetupDiSetDeviceInstallParams"));
		return FALSE;
	}

	if (!SetupDiBuildDriverInfoList(hDevInfo, 0, SPDIT_CLASSDRIVER))
	{
		ShowErrorMsg(GetLastError(), _T("SetupDiDeviceInstallParams"));
		return FALSE;
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);
	return InstallClassDriver(theInfName);
}

// 卸载WDM驱动
VOID UninstallWDMDriver(LPCTSTR theHardware)
{
	SP_DEVINFO_DATA spDevInfoData = {0};
	HDEVINFO hDevInfo = 0L;
	WORD wIdx, wCount = 0;

	hDevInfo = SetupDiGetClassDevs(0L, 0L, 0L, DIGCF_ALLCLASSES | DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		ShowErrorMsg(GetLastError(), _T("SetupDiGetClassDevs"));
		return;
	}

	wIdx = 0;
	while (TRUE)
	{
		spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
		if (SetupDiEnumDeviceInfo(hDevInfo, wIdx, &spDevInfoData))
		{
			char Buffer[2048] = {0};

			if (SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, SPDRP_HARDWAREID,
				0L, (PBYTE)Buffer, 2048, 0L))
			{
				if (!_tcscmp(theHardware, (LPTSTR)Buffer))
				{
					if (!SetupDiRemoveDevice(hDevInfo, &spDevInfoData))
						ShowErrorMsg(GetLastError(), _T("SetupDiRemoveDevice"));          
					wCount++;
				}
			}
		}
		else
			break;
		wIdx++;
	}

	if (wCount != 0)
		_tprintf(_T("UnInstall Successed\n"));

	SetupDiDestroyDeviceInfoList(hDevInfo);
	InitialGlobalVar();
	return;
}

short InstallWDMFromInf(const LPTSTR g_pInfPath)
{
	short wSts =0;
	CHAR szInfPath[MAX_PATH] = {0};
	//UnicodeToAnsi(g_pInfPath, _tcslen(g_pInfPath), szInfPath, MAX_PATH);
	strcpy(szInfPath, g_pInfPath);
	FILE *pInf = fopen(szInfPath, "r");
	CHK_ASSERT (pInf);

	// 获取INF文件数据
	GetINFData(pInf);fclose(pInf);

	CHK_ASSERT(g_wVender && g_wHardware);

	printf("Vender:\n");
	for(int i=0; i<g_wVender; i++)
		printf("[%d] %s\n", i, g_strVender[i] );

	printf("HardWareID:\n");
	for(int i=0; i<g_wHardware; i++)
		printf("[%d] %s\n", i, g_strHardware[i] );

	// 安装WDM驱动
	CHK_ASSERT (StartInstallWDMDriver(g_pInfPath) );

Exit:
	return wSts;

		// 与WDM驱动通讯,等待破解完毕

		// 卸载WDM驱动
		/* WORD wLoop;
		for (wLoop = 0; wLoop < g_wHardware; wLoop++)
		UninstallWDMDriver(g_strHardware[wLoop]);

		return 1;  */

}

