/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that app can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_TestKMDFControlDriver1,
    0xbe5a5076,0xc70d,0x4b01,0x94,0xea,0x00,0xd3,0x63,0x73,0x27,0x3e);
// {be5a5076-c70d-4b01-94ea-00d36373273e}
