#include "stdafx.h"

short TestWriteFile_old(const char *file)
{
	short wSts =0;
	FILE *fp =fopen(file, "w");
	CHK_ASSERT(fp);
	fprintf(fp,"Test");
	fclose(fp);
Exit:
	return wSts;
}

short TestWriteFile(const char *file)
{
	short wSts =0;
	HANDLE hDevice = CreateFile("\\\\.\\HelloDDK",  
		GENERIC_WRITE | GENERIC_READ,  
		0,  
		NULL,  
		OPEN_EXISTING,  
		0,  
		NULL);  
	CHK_ASSERT( hDevice != INVALID_HANDLE_VALUE );
	DWORD dwByteW;
	CHK_ASSERT(WriteFile(hDevice, "Test",4, &dwByteW , NULL));

	CloseHandle( hDevice );
Exit:
	return wSts;
}