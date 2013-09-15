// TestEnumUSB.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <algorithm>

void EnumUSBHubs();
short InstallDriver(LPCTSTR driverName,LPCTSTR driverExec,DWORD startType);
short OpenService(LPCTSTR driverName);
short RemoveService(LPCTSTR driverName);
short StopService(LPCTSTR driverName);
short TestWriteFile(const char *file);

short InstallWDMDriver(IN LPCTSTR HardwareId,IN LPCTSTR INFFile,OUT PBOOL RebootRequired OPTIONAL);
short InstallWDMFromInf(const LPTSTR g_pInfPath);

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int _tmain(int argc, _TCHAR* argv[])
{
	short wSts =0;

	_set_error_mode(_OUT_TO_MSGBOX);

	if(cmdOptionExists(argv, argv+argc, "-debug"))
	{
		system("pause");
	}

	char* pcMode =getCmdOption(argv, argv+argc, "-mode");
	CHK_ASSERT(pcMode);

	if(stricmp(pcMode,"EnumUSBHubs")==0)
		EnumUSBHubs();
	else if(stricmp(pcMode,"InstallDriver")==0)
	{
		char* pcdriverName =getCmdOption(argv, argv+argc, "-driverName");
		CHK_ASSERT(pcdriverName);

		char* pcdriverPath=getCmdOption(argv, argv+argc, "-driverPath");
		CHK_ASSERT(pcdriverPath);

		char* pcstartType=getCmdOption(argv, argv+argc, "-startType");
		CHK_ASSERT(pcstartType);

		DWORD dwStartType =SERVICE_AUTO_START;

		char *acStartTypes[]={"SERVICE_AUTO_START", "SERVICE_BOOT_START", "SERVICE_DEMAND_START", "SERVICE_DISABLED", "SERVICE_SYSTEM_START"};
		DWORD adwstartTypes[]={SERVICE_AUTO_START, SERVICE_BOOT_START, SERVICE_DEMAND_START, SERVICE_DISABLED, SERVICE_SYSTEM_START};

		int i=0;
		for(; i<sizeof(adwstartTypes)/sizeof(adwstartTypes[0]);i++)
		{
			if(stricmp(pcstartType,acStartTypes[i])==0)
			{
				dwStartType =adwstartTypes[i];
				break;
			}
		}

		CHK_ASSERT( i<sizeof(adwstartTypes)/sizeof(adwstartTypes[0]) );

		CHK_ASSERT( 0==InstallDriver(pcdriverName, pcdriverPath, dwStartType ));
	}
	else if(stricmp(pcMode,"OpenService")==0)
	{
		char* pcdriverName =getCmdOption(argv, argv+argc, "-driverName");
		CHK_ASSERT(pcdriverName);

		CHK_ASSERT( 0==OpenService(pcdriverName));
	}
	else if(stricmp(pcMode,"StopService")==0)
	{
		char* pcdriverName =getCmdOption(argv, argv+argc, "-driverName");
		CHK_ASSERT(pcdriverName);

		CHK_ASSERT( 0==StopService(pcdriverName));
	}
	else if(stricmp(pcMode,"RemoveService")==0)
	{
		char* pcdriverName =getCmdOption(argv, argv+argc, "-driverName");
		CHK_ASSERT(pcdriverName);

		CHK_ASSERT( 0==RemoveService(pcdriverName));
	}
	else if(stricmp(pcMode,"InstallWDMDriver")==0)
	{
		char* pcHardwareId =getCmdOption(argv, argv+argc, "-HardwareId");
		CHK_ASSERT(pcHardwareId);

		char* pcINFFile =getCmdOption(argv, argv+argc, "-INFFile");
		CHK_ASSERT(pcINFFile);

		BOOL RebootRequired =0;
		CHK_ASSERT( 0== InstallWDMDriver( pcHardwareId, pcINFFile,&RebootRequired ));

		printf( "RebootRequired %d\n", RebootRequired );
	}
	else if(stricmp(pcMode,"InstallWDMFromInf")==0)
	{
		char* pcINFFile =getCmdOption(argv, argv+argc, "-INFFile");
		CHK_ASSERT(pcINFFile);

		CHK_ASSERT( 0== InstallWDMFromInf(  pcINFFile));

	}
	else if(stricmp(pcMode,"WriteFile")==0)
	{
		char* pcFile =getCmdOption(argv, argv+argc, "-File");
		CHK_ASSERT(pcFile);

		CHK_ASSERT( 0== TestWriteFile( pcFile));

	}
	else
		CHK_ASSERT(0);
	
Exit:
	return wSts;
}

