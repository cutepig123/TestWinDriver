#include "stdafx.h"

#include <SetupAPI.h>
#include <Newdev.h>

// startType: SERVICE_AUTO_START, SERVICE_BOOT_START, SERVICE_DEMAND_START, SERVICE_DISABLED, SERVICE_SYSTEM_START
short InstallDriver(LPCTSTR driverName,LPCTSTR driverExec,DWORD startType)
{
	short wSts =0;
	SC_HANDLE hService =0;
    SC_HANDLE hSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    CHK_ASSERT(hSCManager != NULL);

    hService =CreateService ( hSCManager,    
                        driverName,                
                        driverName,                
                        SERVICE_ALL_ACCESS,        
                        SERVICE_KERNEL_DRIVER,    
                        startType,                
                        SERVICE_ERROR_NORMAL,    
                        driverExec,                
                        NULL, NULL, NULL, NULL, NULL);
    CHK_ASSERT (hService != NULL) ;

    
Exit:
	if(hService) CloseServiceHandle(hService);

	if(hSCManager)
		CloseServiceHandle(hSCManager);

	return wSts;
}

short OpenService(LPCTSTR driverName)
{
	short wSts =0;
	SC_HANDLE hService =0;
    SC_HANDLE hSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    CHK_ASSERT(hSCManager != NULL);

    hService = OpenService (hSCManager, driverName, SERVICE_ALL_ACCESS);
    CHK_ASSERT (hService!= NULL) ;

    CHK_ASSERT ( StartService(hService,0,NULL) ) ;
Exit:
	if(hService) 
		CloseServiceHandle(hService);

	if(hSCManager)
		CloseServiceHandle(hSCManager);

	return wSts;
}


short RemoveService(LPCTSTR driverName)
{
	short wSts =0;
	SC_HANDLE hService =0;
    SC_HANDLE hSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    CHK_ASSERT(hSCManager != NULL);

    hService = OpenService (hSCManager, driverName, SERVICE_ALL_ACCESS);
    CHK_ASSERT (hService!= NULL) ;

    CHK_ASSERT ( DeleteService(hService) ) ;
Exit:
	if(hService) 
		CloseServiceHandle(hService);

	if(hSCManager)
		CloseServiceHandle(hSCManager);

	return wSts;
}


short StopService(LPCTSTR driverName)
{
	short wSts =0;
	SC_HANDLE hService =0;
    SC_HANDLE hSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    CHK_ASSERT(hSCManager != NULL);

    hService = OpenService (hSCManager, driverName, SERVICE_ALL_ACCESS);
    CHK_ASSERT (hService!= NULL) ;

	SERVICE_STATUS serviceStatus;
    CHK_ASSERT ( ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus) ) ;
Exit:
	if(hService) 
		CloseServiceHandle(hService);

	if(hSCManager)
		CloseServiceHandle(hSCManager);

	return wSts;
}

#define MAX_CLASS_NAME_LEN 300

short InstallWDMDriver(IN LPCTSTR HardwareId,IN LPCTSTR INFFile,OUT PBOOL RebootRequired OPTIONAL)
{
	short wSts =0;
	HDEVINFO DeviceInfoSet = 0;
	SP_DEVINFO_DATA DeviceInfoData;
	GUID ClassGUID;
	TCHAR ClassName[MAX_CLASS_NAME_LEN];
	BOOL isDevInfoCreated =0;
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// 首先从Inf文件中得到设备类GUID
	CHK_ASSERT (SetupDiGetINFClass((LPTSTR)INFFile,&ClassGUID,ClassName,sizeof(ClassName),0));

	DeviceInfoSet = SetupDiCreateDeviceInfoList(&ClassGUID,0);
	CHK_ASSERT(DeviceInfoSet != INVALID_HANDLE_VALUE);

	isDevInfoCreated =1;

	// 打开该设备所属的类
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	CHK_ASSERT (SetupDiCreateDeviceInfo(DeviceInfoSet,
		ClassName,
		&ClassGUID,
		NULL,
		0,
		DICD_GENERATE_ID,
		&DeviceInfoData));

	// 添加该设备
	CHK_ASSERT(SetupDiSetDeviceRegistryProperty(DeviceInfoSet,
		&DeviceInfoData,
		SPDRP_HARDWAREID,
		(LPBYTE)HardwareId,
		(lstrlen(HardwareId)+1+1)*sizeof(TCHAR)));

	
	// 建立关联
	CHK_ASSERT(!SetupDiCallClassInstaller(DIF_REGISTERDEVICE,
		DeviceInfoSet,
		&DeviceInfoData));

	// 关联信息都已建立，安装本驱动（以前相当于建立了一个未知的设备）
	if (!UpdateDriverForPlugAndPlayDevices(0,
		HardwareId,
		INFFile,
		INSTALLFLAG_FORCE,
		RebootRequired))
	{
		// 安装失败，需要把以前的创建的信息删除，否则将会留下一个未知设备
		DWORD err = GetLastError();
		//TRACE( "UpdateDriverForPlugAndPlayDevices failedn" );
		//TRACE( "the errcode is 0x%x", err );

		CHK_ASSERT (SetupDiCallClassInstaller(
			DIF_REMOVE,
			DeviceInfoSet,
			&DeviceInfoData));
	}

Exit:
	if(isDevInfoCreated)
		SetupDiDestroyDeviceInfoList(DeviceInfoSet);

	return wSts;
}