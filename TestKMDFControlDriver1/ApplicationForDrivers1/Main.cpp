//http://www.e-reading.ws/chapter.php/147098/147/Cant_-_Writing_Windows_WDM_Device_Drivers.html

#define INITGUID

#include <Windows.h>
#include <stdio.h>
#include <Setupapi.h>
#include "../TestKMDFControlDriver1/Public.h"

#pragma comment(lib, "Setupapi.lib")

HANDLE GetDeviceViaInterface(GUID* pGuid, DWORD instance) {

	HANDLE rv = NULL;
	// Get handle to relevant device information set
	PSP_INTERFACE_DEVICE_DETAIL_DATA ifDetail = NULL;
	HDEVINFO info = SetupDiGetClassDevs(pGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);

	if (info == INVALID_HANDLE_VALUE) {

		printf("No HDEVINFO available for this GUID\n");

		return NULL;

	}

	// Get interface data for the requested instance

	SP_INTERFACE_DEVICE_DATA ifdata;

	ifdata.cbSize = sizeof(ifdata);

	if (!SetupDiEnumDeviceInterfaces(info, NULL, pGuid, instance, &ifdata)) {

		printf("No SP_INTERFACE_DEVICE_DATA available for this GUID instance \n");

		goto Exit;

	}

	// Get size of symbolic link name

	DWORD ReqLen;

	SetupDiGetDeviceInterfaceDetail(info, &ifdata, NULL, 0, &ReqLen, NULL);

	ifDetail = (PSP_INTERFACE_DEVICE_DETAIL_DATA)(new char[ReqLen]);

	if (ifDetail == NULL) {

		goto Exit;

	}

	// Get symbolic link name

	ifDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

	if (!SetupDiGetDeviceInterfaceDetail(info, &ifdata, ifDetail, ReqLen, NULL, NULL)) {

		goto Exit;

	}

	printf("Symbolic link is %s\n", ifDetail->DevicePath);

		// Open file

	rv = CreateFile(ifDetail->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

Exit:
	if (ifDetail)
		delete ifDetail;

	SetupDiDestroyDeviceInfoList(info);

	return rv;

}

#define	VERIFY(x)	if(!(x)) {printf("Error %s\n", #x);}

int main()
{
	GUID guid = GUID_DEVINTERFACE_TestKMDFControlDriver1;

	for (int i = 0;; i++)
	{
		HANDLE handle = GetDeviceViaInterface(&guid, i);
		if (!handle)
			break;
		char inbuffer[10]="Input";
		char outbuffer[20] = { 0 };
		DWORD byteReturn = 0;
		VERIFY(DeviceIoControl(handle, 0x1234, inbuffer, sizeof(inbuffer), outbuffer, sizeof(outbuffer), &byteReturn, 0));
		printf("return: %s\n", outbuffer);
	}
}