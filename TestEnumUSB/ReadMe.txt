±¾³ÌĞòÎªuser mode console program

¹¦ÄÜ£º
°²×°£¬ÔËĞĞ£¬Í£Ö¹£¬Ğ¶ÔØNTÇı¶¯
ÁĞ¾Ùusb hubÉè±¸
°²×°wdmÇı¶¯

²Î¿¼×ÊÁÏ

*Çı¶¯µÄ°²×°
[http://www.codeproject.com/Articles/43461/Driver-Loader-DLoad-from-Scratch ÁĞÁËÒ»¶Ñ·½·¨]
[http://www.codeproject.com/Articles/43586/File-System-Filter-Driver-Tutorial#_Toc244423419 Ê¹ÓÃSC.EXE]

*Simple URB (USB Request Block) Monitor
¼àÊÓusbÏûÏ¢

*ÁĞ¾ÙËùÓĞusb hubĞÅÏ¢
 --SetupDiGetClassDevs--¡· HDEVINFO --SetupDiEnumDeviceInterfaces--¡· SP_DEVICE_INTERFACE_DATA --SetupDiGetInterfaceDeviceDetail--¡· PSP_DEVICE_INTERFACE_DETAIL_DATA

logging example
C:\WINDOWS\setupapi.log

[2013/09/01 11:35:47 9632.4506]
#-199 Ö´ĞĞ "G:\Backup\ÎÒµÄÎÄµµ\GitHub\TestWinDriver\TestEnumUSB\Debug\TestEnumUSB.exe"£¬ÓÃÃüÁîĞĞ: TestEnumUSB.exe -debug -mode InstallWDMFromInf -INFFile "F:\android-sdks\extras\google\usb_driver\android_winusb.inf"
#I060 ÉèÖÃËùÑ¡µÄÇı¶¯Æ÷¡£
#-019 ÕıÔÚ²éÕÒÓ²¼ş ID(s): usb\vid_18d1&pid_4ee6&mi_01
#I022 ÔÚ "f:\android-sdks\extras\google\usb_driver\android_winusb.inf" ÖĞ·¢ÏÖÁË "USB\VID_18D1&PID_4EE6&MI_01"£»Éè±¸: "Android Composite ADB Interface"£»Çı¶¯³ÌĞò: "Android Composite ADB Interface"£»Ìá¹©³ÌĞò: "Google, Inc."£»ÖÆÔìÉÌ: "Google, Inc."£»¶Î: "USB_Install"
#I087 Çı¶¯³ÌĞò½Úµã²»ÊÜĞÅÈÎµÄ£¬Ãû´Î´Ó 0x00000000 ¸ü¸Äµ½ 0x00008000¡£
#I023 Êµ¼Ê°²×°²¿·Ö: [USB_Install]¡£µÈ¼¶: 0x00008000¡£Çı¶¯³ÌĞòÓĞĞ§ÈÕÆÚ: 08/27/2012¡£
#-166 Éè±¸°²×°º¯Êı: DIF_SELECTBESTCOMPATDRV¡£
#I063 ´Ó [USB_Install] ÖĞµÄ "f:\android-sdks\extras\google\usb_driver\android_winusb.inf" Ñ¡ÔñÇı¶¯Æ÷°²×°·şÎñ¡£
#I320 Éè±¸µÄÀà±ğ GUID ÒÀ¾ÉÎª: {3F966BD9-FA04-4EC5-991C-D326973B5128}¡£
#I060 ÉèÖÃËùÑ¡µÄÇı¶¯Æ÷¡£
#I058 Ñ¡Ôñ×î¼æÈİµÄÇı¶¯Æ÷¡£
#-124 ÕıÔÚ×ö¡°½ö¸´ÖÆ¡±°²×° "ROOT\ANDROIDUSBDEVICECLASS\0005"¡£
#E360 Çı¶¯³ÌĞò "Android Composite ADB Interface" µÄÒ»¸öÎ´¾­¹ıÇ©Êğ»òÇ©Êğ²»ÕıÈ·µÄÎÄ¼ş "f:\android-sdks\extras\google\usb_driver\androidwinusb86.cat" ½«µÃµ½°²×°(²ßÂÔ=ºöÂÔ)¡£ ´íÎó 0x800b0110: Ö¤Êé¶ÔÇëÇóµÄÓÃ·¨ÎŞĞ§¡£
#-024 ÕıÔÚ½«ÎÄ¼ş "f:\android-sdks\extras\google\usb_driver\i386\WinUSBCoInstaller2.dll" ¸´ÖÆµ½ "C:\WINDOWS\system32\WinUSBCoInstaller2.dll"¡£
#E360 Çı¶¯³ÌĞò "Android Composite ADB Interface" µÄÒ»¸öÎ´¾­¹ıÇ©Êğ»òÇ©Êğ²»ÕıÈ·µÄÎÄ¼ş "f:\android-sdks\extras\google\usb_driver\i386\WinUSBCoInstaller2.dll" ½«µÃµ½°²×°(²ßÂÔ=ºöÂÔ)¡£ ´íÎó 0x800b0110: Ö¤Êé¶ÔÇëÇóµÄÓÃ·¨ÎŞĞ§¡£
#-336 ÕıÔÚÍ¨¹ıÁÙÊ±ÎÄ¼ş "C:\WINDOWS\system32\SET3CF3.tmp" ½«ÎÄ¼ş "f:\android-sdks\extras\google\usb_driver\i386\WdfCoInstaller01009.dll" ¸´ÖÆµ½ "C:\WINDOWS\system32\WdfCoInstaller01009.dll"¡£
#E360 Çı¶¯³ÌĞò "Android Composite ADB Interface" µÄÒ»¸öÎ´¾­¹ıÇ©Êğ»òÇ©Êğ²»ÕıÈ·µÄÎÄ¼ş "f:\android-sdks\extras\google\usb_driver\i386\WdfCoInstaller01009.dll" ½«µÃµ½°²×°(²ßÂÔ=ºöÂÔ)¡£ ´íÎó 0x800b0110: Ö¤Êé¶ÔÇëÇóµÄÓÃ·¨ÎŞĞ§¡£
#-166 Éè±¸°²×°º¯Êı: DIF_REGISTER_COINSTALLERS¡£
#I056 ×¢²áÁË¹²Í¬°²×°³ÌĞò¡£
#-166 Éè±¸°²×°º¯Êı: DIF_INSTALLINTERFACES¡£
#-011 ÕıÔÚ´Ó "f:\android-sdks\extras\google\usb_driver\android_winusb.inf" °²×°¶Î [USB_Install.Interfaces]¡£
#I054 °²×°½Ó¿Ú¡£
#-166 Éè±¸°²×°º¯Êı: DIF_INSTALLDEVICE¡£
#I123 ½øĞĞ "ROOT\ANDROIDUSBDEVICECLASS\0005" µÄÍêÕû°²×°¡£
#E360 Çı¶¯³ÌĞò "Android Composite ADB Interface" µÄÒ»¸öÎ´¾­¹ıÇ©Êğ»òÇ©Êğ²»ÕıÈ·µÄÎÄ¼ş "f:\android-sdks\extras\google\usb_driver\androidwinusb86.cat" ½«µÃµ½°²×°(²ßÂÔ=ºöÂÔ)¡£ ´íÎó 0x800b0110: Ö¤Êé¶ÔÇëÇóµÄÓÃ·¨ÎŞĞ§¡£
#I163 Éè±¸Î´¿ªÊ¼: Éè±¸ÓĞÎÊÌâ: 0x0a: CM_PROB_FAILED_START.
#I121 "ROOT\ANDROIDUSBDEVICECLASS\0005" µÄÉè±¸°²×°³É¹¦Íê³É¡£

Èç¹û°²×°³ÌĞòÊÔÍ¼ÔÚÏµÍ³ÖĞÌí¼ÓÎ´¾­Ç©ÊğµÄÇı¶¯³ÌĞò£¬ÀûÓÃÏÂÊö¹ı³Ì¿ÉÒÔ¿ØÖÆÕâÖÖÇé¿öµÄ´¦Àí·½·¨£º

¡¡¡¡1£¬  ÔÚ¡°my compr¡±µ¥»÷ÓÒ¼ü

¡¡¡¡2£¬  ÔÚ¡°hardware¡±Ãæ°åÏÂ£¬µã»÷¡°driver signining¡±£¬È»ºó¿ÉÒÔÑ¡ÔñÈçÏÂÈıÖÖÑ¡Ïî£º

¡¡¡¡¡°ignore¡±--ºöÂÔ£¬°²×°ËùÓĞÉè±¸Çı¶¯³ÌĞò£¬²»¹ÜËüÃÇÊÇ²»ÊÇÓĞÊı×ÖÇ©Êğ¡£

¡¡¡¡¡°warn¡±---¾¯¸æ£¬µ±¼ì²âµ½Î´¾­Êı×ÖÇ©ÊğµÄÉè±¸Çı¶¯³ÌĞòÊ±£¬ÏÔÊ¾Ò»¸ö¾¯¸æĞÅÏ¢¡£

¡¡¡¡¡°block¡±---×èÖ¹£¬×èÖ¹ÓÃ»§°²×°Î´¾­Êı×ÖÇ©ÊğµÄÉè±¸Çı¶¯³ÌĞò¡£

¡¡¡¡¶ÔÓÚ±êÊ¶Î´¾­Ç©ÊğµÄÎÄ¼ş£¬ÔòĞèÒªÔËĞĞÈçÏÂÃüÁîĞĞ£º