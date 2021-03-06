/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.


Module Name:

    driver.c

Abstract:

    This modules contains the Windows Driver Framework Driver object
    handlers for the fireshock filter driver.

Environment:

    Kernel mode

--*/

#include "FireShock.h"

//
// Main driver entry, initialize the framework, register
// driver event handlers
//
NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING RegistryPath
)
{
    WDF_DRIVER_CONFIG params;
    NTSTATUS  status;

    KdPrint((DRIVERNAME "FireShock: DriverEntry - WDF version built on %s %s\n",
        __DATE__, __TIME__));

    WDF_DRIVER_CONFIG_INIT(
        &params,
        FireShockEvtDeviceAdd
    );

    //
    // Create the framework WDFDRIVER object, with the handle
    // to it returned in Driver.
    //
    status = WdfDriverCreate(DriverObject,
        RegistryPath,
        WDF_NO_OBJECT_ATTRIBUTES,
        &params,
        WDF_NO_HANDLE);
    if (!NT_SUCCESS(status)) {
        //
        // Framework will automatically cleanup on error Status return
        //
        KdPrint((DRIVERNAME "FireShock: Error Creating WDFDRIVER 0x%x\n", status));
    }

    status = WdfCollectionCreate(WDF_NO_OBJECT_ATTRIBUTES,
        &FilterDeviceCollection);
    if (!NT_SUCCESS(status))
    {
        KdPrint((DRIVERNAME "WdfCollectionCreate failed with status 0x%x\n", status));
        return status;
    }

    //
    // The wait-lock object has the driver object as a default parent.
    //

    status = WdfWaitLockCreate(WDF_NO_OBJECT_ATTRIBUTES,
        &FilterDeviceCollectionLock);
    if (!NT_SUCCESS(status))
    {
        KdPrint((DRIVERNAME "WdfWaitLockCreate failed with status 0x%x\n", status));
        return status;
    }

    return status;
}
