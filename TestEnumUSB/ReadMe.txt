������Ϊuser mode console program

���ܣ�
��װ�����У�ֹͣ��ж��NT����
�о�usb hub�豸
��װwdm����

�ο�����

*�����İ�װ
[http://www.codeproject.com/Articles/43461/Driver-Loader-DLoad-from-Scratch ����һ�ѷ���]
[http://www.codeproject.com/Articles/43586/File-System-Filter-Driver-Tutorial#_Toc244423419 ʹ��SC.EXE]

*Simple URB (USB Request Block) Monitor
����usb��Ϣ

*�о�����usb hub��Ϣ
 --SetupDiGetClassDevs--�� HDEVINFO --SetupDiEnumDeviceInterfaces--�� SP_DEVICE_INTERFACE_DATA --SetupDiGetInterfaceDeviceDetail--�� PSP_DEVICE_INTERFACE_DETAIL_DATA

logging example
C:\WINDOWS\setupapi.log

[2013/09/01 11:35:47 9632.4506]
#-199 ִ�� "G:\Backup\�ҵ��ĵ�\GitHub\TestWinDriver\TestEnumUSB\Debug\TestEnumUSB.exe"����������: TestEnumUSB.exe -debug -mode InstallWDMFromInf -INFFile "F:\android-sdks\extras\google\usb_driver\android_winusb.inf"
#I060 ������ѡ����������
#-019 ���ڲ���Ӳ�� ID(s): usb\vid_18d1&pid_4ee6&mi_01
#I022 �� "f:\android-sdks\extras\google\usb_driver\android_winusb.inf" �з����� "USB\VID_18D1&PID_4EE6&MI_01"���豸: "Android Composite ADB Interface"����������: "Android Composite ADB Interface"���ṩ����: "Google, Inc."��������: "Google, Inc."����: "USB_Install"
#I087 ��������ڵ㲻�����εģ����δ� 0x00000000 ���ĵ� 0x00008000��
#I023 ʵ�ʰ�װ����: [USB_Install]���ȼ�: 0x00008000������������Ч����: 08/27/2012��
#-166 �豸��װ����: DIF_SELECTBESTCOMPATDRV��
#I063 �� [USB_Install] �е� "f:\android-sdks\extras\google\usb_driver\android_winusb.inf" ѡ����������װ����
#I320 �豸����� GUID ����Ϊ: {3F966BD9-FA04-4EC5-991C-D326973B5128}��
#I060 ������ѡ����������
#I058 ѡ������ݵ���������
#-124 �������������ơ���װ "ROOT\ANDROIDUSBDEVICECLASS\0005"��
#E360 �������� "Android Composite ADB Interface" ��һ��δ����ǩ���ǩ����ȷ���ļ� "f:\android-sdks\extras\google\usb_driver\androidwinusb86.cat" ���õ���װ(����=����)�� ���� 0x800b0110: ֤���������÷���Ч��
#-024 ���ڽ��ļ� "f:\android-sdks\extras\google\usb_driver\i386\WinUSBCoInstaller2.dll" ���Ƶ� "C:\WINDOWS\system32\WinUSBCoInstaller2.dll"��
#E360 �������� "Android Composite ADB Interface" ��һ��δ����ǩ���ǩ����ȷ���ļ� "f:\android-sdks\extras\google\usb_driver\i386\WinUSBCoInstaller2.dll" ���õ���װ(����=����)�� ���� 0x800b0110: ֤���������÷���Ч��
#-336 ����ͨ����ʱ�ļ� "C:\WINDOWS\system32\SET3CF3.tmp" ���ļ� "f:\android-sdks\extras\google\usb_driver\i386\WdfCoInstaller01009.dll" ���Ƶ� "C:\WINDOWS\system32\WdfCoInstaller01009.dll"��
#E360 �������� "Android Composite ADB Interface" ��һ��δ����ǩ���ǩ����ȷ���ļ� "f:\android-sdks\extras\google\usb_driver\i386\WdfCoInstaller01009.dll" ���õ���װ(����=����)�� ���� 0x800b0110: ֤���������÷���Ч��
#-166 �豸��װ����: DIF_REGISTER_COINSTALLERS��
#I056 ע���˹�ͬ��װ����
#-166 �豸��װ����: DIF_INSTALLINTERFACES��
#-011 ���ڴ� "f:\android-sdks\extras\google\usb_driver\android_winusb.inf" ��װ�� [USB_Install.Interfaces]��
#I054 ��װ�ӿڡ�
#-166 �豸��װ����: DIF_INSTALLDEVICE��
#I123 ���� "ROOT\ANDROIDUSBDEVICECLASS\0005" ��������װ��
#E360 �������� "Android Composite ADB Interface" ��һ��δ����ǩ���ǩ����ȷ���ļ� "f:\android-sdks\extras\google\usb_driver\androidwinusb86.cat" ���õ���װ(����=����)�� ���� 0x800b0110: ֤���������÷���Ч��
#I163 �豸δ��ʼ: �豸������: 0x0a: CM_PROB_FAILED_START.
#I121 "ROOT\ANDROIDUSBDEVICECLASS\0005" ���豸��װ�ɹ���ɡ�

�����װ������ͼ��ϵͳ�����δ��ǩ����������������������̿��Կ�����������Ĵ�������

����1��  �ڡ�my compr�������Ҽ�

����2��  �ڡ�hardware������£������driver signining����Ȼ�����ѡ����������ѡ�

������ignore��--���ԣ���װ�����豸�������򣬲��������ǲ���������ǩ��

������warn��---���棬����⵽δ������ǩ����豸��������ʱ����ʾһ��������Ϣ��

������block��---��ֹ����ֹ�û���װδ������ǩ����豸��������

�������ڱ�ʶδ��ǩ����ļ�������Ҫ�������������У�