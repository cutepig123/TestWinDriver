#include "stdafx.h"
#include <windows.h>
#include <assert.h>

#define	CHK_STS	if(wSts<0) {assert(0);goto Exit;}
#define	CHK_ASSERT(x)	if(!(x)) {wSts=-1; assert(0);goto Exit;}

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
