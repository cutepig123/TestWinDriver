本程序为user mode console program

功能：
安装，运行，停止，卸载NT驱动
列举usb hub设备
安装wdm驱动

参考资料

*驱动的安装
[http://www.codeproject.com/Articles/43461/Driver-Loader-DLoad-from-Scratch 列了一堆方法]
[http://www.codeproject.com/Articles/43586/File-System-Filter-Driver-Tutorial#_Toc244423419 使用SC.EXE]

*Simple URB (USB Request Block) Monitor
监视usb消息

*列举所有usb hub信息
 --SetupDiGetClassDevs--》 HDEVINFO --SetupDiEnumDeviceInterfaces--》 SP_DEVICE_INTERFACE_DATA --SetupDiGetInterfaceDeviceDetail--》 PSP_DEVICE_INTERFACE_DETAIL_DATA

logging example
C:\WINDOWS\setupapi.log

[2013/09/01 11:35:47 9632.4506]
#-199 执行 "G:\Backup\我的文档\GitHub\TestWinDriver\TestEnumUSB\Debug\TestEnumUSB.exe"，用命令行: TestEnumUSB.exe -debug -mode InstallWDMFromInf -INFFile "F:\android-sdks\extras\google\usb_driver\android_winusb.inf"
#I060 设置所选的驱动器。
#-019 正在查找硬件 ID(s): usb\vid_18d1&pid_4ee6&mi_01
#I022 在 "f:\android-sdks\extras\google\usb_driver\android_winusb.inf" 中发现了 "USB\VID_18D1&PID_4EE6&MI_01"；设备: "Android Composite ADB Interface"；驱动程序: "Android Composite ADB Interface"；提供程序: "Google, Inc."；制造商: "Google, Inc."；段: "USB_Install"
#I087 驱动程序节点不受信任的，名次从 0x00000000 更改到 0x00008000。
#I023 实际安装部分: [USB_Install]。等级: 0x00008000。驱动程序有效日期: 08/27/2012。
#-166 设备安装函数: DIF_SELECTBESTCOMPATDRV。
#I063 从 [USB_Install] 中的 "f:\android-sdks\extras\google\usb_driver\android_winusb.inf" 选择驱动器安装服务。
#I320 设备的类别 GUID 依旧为: {3F966BD9-FA04-4EC5-991C-D326973B5128}。
#I060 设置所选的驱动器。
#I058 选择最兼容的驱动器。
#-124 正在做“仅复制”安装 "ROOT\ANDROIDUSBDEVICECLASS\0005"。
#E360 驱动程序 "Android Composite ADB Interface" 的一个未经过签署或签署不正确的文件 "f:\android-sdks\extras\google\usb_driver\androidwinusb86.cat" 将得到安装(策略=忽略)。 错误 0x800b0110: 证书对请求的用法无效。
#-024 正在将文件 "f:\android-sdks\extras\google\usb_driver\i386\WinUSBCoInstaller2.dll" 复制到 "C:\WINDOWS\system32\WinUSBCoInstaller2.dll"。
#E360 驱动程序 "Android Composite ADB Interface" 的一个未经过签署或签署不正确的文件 "f:\android-sdks\extras\google\usb_driver\i386\WinUSBCoInstaller2.dll" 将得到安装(策略=忽略)。 错误 0x800b0110: 证书对请求的用法无效。
#-336 正在通过临时文件 "C:\WINDOWS\system32\SET3CF3.tmp" 将文件 "f:\android-sdks\extras\google\usb_driver\i386\WdfCoInstaller01009.dll" 复制到 "C:\WINDOWS\system32\WdfCoInstaller01009.dll"。
#E360 驱动程序 "Android Composite ADB Interface" 的一个未经过签署或签署不正确的文件 "f:\android-sdks\extras\google\usb_driver\i386\WdfCoInstaller01009.dll" 将得到安装(策略=忽略)。 错误 0x800b0110: 证书对请求的用法无效。
#-166 设备安装函数: DIF_REGISTER_COINSTALLERS。
#I056 注册了共同安装程序。
#-166 设备安装函数: DIF_INSTALLINTERFACES。
#-011 正在从 "f:\android-sdks\extras\google\usb_driver\android_winusb.inf" 安装段 [USB_Install.Interfaces]。
#I054 安装接口。
#-166 设备安装函数: DIF_INSTALLDEVICE。
#I123 进行 "ROOT\ANDROIDUSBDEVICECLASS\0005" 的完整安装。
#E360 驱动程序 "Android Composite ADB Interface" 的一个未经过签署或签署不正确的文件 "f:\android-sdks\extras\google\usb_driver\androidwinusb86.cat" 将得到安装(策略=忽略)。 错误 0x800b0110: 证书对请求的用法无效。
#I163 设备未开始: 设备有问题: 0x0a: CM_PROB_FAILED_START.
#I121 "ROOT\ANDROIDUSBDEVICECLASS\0005" 的设备安装成功完成。

如果安装程序试图在系统中添加未经签署的驱动程序，利用下述过程可以控制这种情况的处理方法：

　　1，  在“my compr”单击右键

　　2，  在“hardware”面板下，点击“driver signining”，然后可以选择如下三种选项：

　　“ignore”--忽略，安装所有设备驱动程序，不管它们是不是有数字签署。

　　“warn”---警告，当检测到未经数字签署的设备驱动程序时，显示一个警告信息。

　　“block”---阻止，阻止用户安装未经数字签署的设备驱动程序。

　　对于标识未经签署的文件，则需要运行如下命令行：