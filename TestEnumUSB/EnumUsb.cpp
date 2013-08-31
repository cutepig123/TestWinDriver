#include "stdafx.h"
#include <INITGUID.H>
#include <windows.h>
#include <SetupAPI.h>
#include <Usbioctl.h>
#include <Strsafe.h>
#include <Usbiodef.h>


//bool GetDevName(TCHAR *pSzDrvPath)
//{
//	bool		fFinalRes = false;
//
//	HANDLE		hPort;
//	TCHAR		buf[512];
//	TCHAR		*pSzDrvKeyName = NULL;
//	BOOL		fRes = FALSE;
//	DWORD		dwRes = 0;
//
//	pSzDrvKeyName = pSzDrvPath;
//	pSzDrvKeyName += 4;
//
//	RtlZeroMemory(buf, sizeof(buf));
//
//	::StringCbCopy(buf, sizeof(buf), TEXT("\\DosDevices\\Global\\"));
//	::StringCbCat(buf, sizeof(buf), pSzDrvKeyName);
//
//	hPort = ::CreateFile(DKPORT_NAME_STR, GENERIC_ALL, 0, NULL, OPEN_EXISTING, 0, NULL);
//	if (hPort == INVALID_HANDLE_VALUE){
//		return fFinalRes;
//	}
//
//	::RtlZeroMemory(m_szRootHubDevName, sizeof(m_szRootHubDevName));
//
//	fRes = ::DeviceIoControl(hPort, IOCTL_DKSYSPORT_GET_TGTHUB,
//		buf, sizeof(buf), m_szRootHubDevName, sizeof(m_szRootHubDevName), &dwRes, NULL);
//
//	if (fRes){
//		fFinalRes = true;
//	}
//
//	::CloseHandle(hPort);
//
//	return fFinalRes;
//}

void GetPortDetail(TCHAR *pSzDrvPath, PUSB_NODE_CONNECTION_INFORMATION_EX pUsbNodeInfo, UCHAR iPort)
{
	TCHAR		szSpeed[16];
	
	//::RtlZeroMemory(m_szDetectMsg, sizeof(m_szDetectMsg));
	::RtlZeroMemory(szSpeed, sizeof(szSpeed));
	
	//GetDevName(pSzDrvPath);
	
	switch (pUsbNodeInfo->Speed){
		case UsbLowSpeed:
			::StringCbPrintf(szSpeed, sizeof(szSpeed), TEXT("Low Speed"));
			break;
		case UsbHighSpeed:
			::StringCbPrintf(szSpeed, sizeof(szSpeed), TEXT("High Speed"));
			break;
		case UsbFullSpeed:
			::StringCbPrintf(szSpeed, sizeof(szSpeed), TEXT("Full Speed"));
			break;
		default:
			::StringCbPrintf(szSpeed, sizeof(szSpeed), TEXT("Unknown Speed"));
			break;
	}

	_tprintf(
		TEXT("USB device attached to Root Hub: %s, port: %d. Detail: Vendor Id = 0x%X, Product Id = 0x%X, Speed = %s, Is hub device = 0x%X, Device address = 0x%X\n"),
		pSzDrvPath, iPort, pUsbNodeInfo->DeviceDescriptor.idVendor, pUsbNodeInfo->DeviceDescriptor.idProduct,
		szSpeed, pUsbNodeInfo->DeviceIsHub, pUsbNodeInfo->DeviceAddress);

	//TriggerOnDetectedDevice(m_szDetectMsg);

}

void EnumUSBPorts(TCHAR *pSzHubDevPath)
{
	HANDLE					hUsbHub;
	BOOL					bRes = FALSE;
	USB_NODE_INFORMATION	usbNodeInfo;
	DWORD					dwRes = 0;

	UCHAR								ucPortNumbs = 0;
	PUSB_NODE_CONNECTION_INFORMATION_EX	pBuf = NULL;
	DWORD								dwBufLen = 0;

	// 1. Open USB hub device
	hUsbHub = ::CreateFile(pSzHubDevPath, GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hUsbHub == INVALID_HANDLE_VALUE){
		return;
	}
	
	// 2. Get USB Node information (get ports number in USB Hub)
	bRes = ::DeviceIoControl(hUsbHub, IOCTL_USB_GET_NODE_INFORMATION,
		(LPVOID) &usbNodeInfo, sizeof(USB_NODE_INFORMATION), 
		(LPVOID) &usbNodeInfo, sizeof(USB_NODE_INFORMATION),
		&dwRes, NULL);
	if (!bRes){
		goto EndFunc;
	}

	ucPortNumbs = usbNodeInfo.u.HubInformation.HubDescriptor.bNumberOfPorts;

	// 3. Check to see if there is a USB device on each port
	for (UCHAR idx = 1; idx <= ucPortNumbs; idx++){

		dwBufLen = sizeof(USB_NODE_CONNECTION_INFORMATION_EX) + (sizeof(USB_PIPE_INFO) * 30);
		pBuf = (PUSB_NODE_CONNECTION_INFORMATION_EX) ::GlobalAlloc(GPTR, dwBufLen);

		pBuf->ConnectionIndex = idx;
		
		bRes = ::DeviceIoControl(hUsbHub, 
			IOCTL_USB_GET_NODE_CONNECTION_INFORMATION_EX,
			pBuf, dwBufLen, pBuf, dwBufLen, &dwRes, NULL);
		if (bRes){
			if (pBuf->ConnectionStatus == DeviceConnected){
				GetPortDetail(pSzHubDevPath, pBuf, idx);
			}
		}

		::GlobalFree((HGLOBAL) pBuf);
	}
	
EndFunc:
	::CloseHandle(hUsbHub);
}

void EnumUSBHubs()
{
	HDEVINFO					hDevInfo;
	
	int							idx = 0;
	SP_DEVICE_INTERFACE_DATA	spDevIntDat = {0};
	//BOOL						bRes = FALSE;

	PSP_DEVICE_INTERFACE_DETAIL_DATA	pSpDevIntDetDat = NULL;
	DWORD								dwBytes = 0;

	// 1. Get class devices
	hDevInfo = ::SetupDiGetClassDevs(
		&GUID_CLASS_USBHUB, NULL, NULL,
		(DIGCF_DEVICEINTERFACE | DIGCF_PRESENT));
	if (hDevInfo == INVALID_HANDLE_VALUE){
		return;
	}

	RtlZeroMemory(&spDevIntDat, sizeof(SP_DEVICE_INTERFACE_DATA));
	spDevIntDat.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	// 2. Enumerate USB Root Hubs
	for (;::SetupDiEnumDeviceInterfaces(hDevInfo, NULL,&GUID_CLASS_USBHUB, idx, &spDevIntDat);idx++){

		
		//bRes = ::SetupDiEnumDeviceInterfaces(hDevInfo, NULL,&GUID_CLASS_USBHUB, idx, &spDevIntDat);
		//if (bRes == FALSE)
		//	break;

		dwBytes = 0;

		::SetupDiGetInterfaceDeviceDetail(hDevInfo, &spDevIntDat, NULL, 0, &dwBytes, NULL);

		if (dwBytes > 0){

			pSpDevIntDetDat = (PSP_DEVICE_INTERFACE_DETAIL_DATA) ::GlobalAlloc(GPTR, dwBytes);
			pSpDevIntDetDat->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			BOOL bRes = ::SetupDiGetInterfaceDeviceDetail(hDevInfo, &spDevIntDat,
				pSpDevIntDetDat, dwBytes, &dwBytes, NULL);
			if (bRes){

				_tprintf(TEXT("[%d] %s\n"), idx, pSpDevIntDetDat->DevicePath );
				// 3. Enumerate USB Hub Ports
				EnumUSBPorts(pSpDevIntDetDat->DevicePath);

			}

			::GlobalFree((HGLOBAL) pSpDevIntDetDat);
		}

		
	}


	// 5. Destroy class devices.
	::SetupDiDestroyDeviceInfoList(hDevInfo);

}
