/*++

Module Name:

    FSD.c

Abstract:

    This is the main module of the FSD miniFilter driver.

Environment:

    Kernel mode

--*/

#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>
#include "FSDFilterUtils.h"
#include "FSDCommonDefs.h"
#include "AutoPtr.h"
#include "CFSDefender.h"

#pragma prefast(disable:__WARNING_ENCODE_MEMBER_FUNCTION_POINTER, "Not valid for kernel mode drivers")

ULONG gTraceFlags = 7;

CFSDefender* g;

/*************************************************************************
    Prototypes
*************************************************************************/

EXTERN_C_START

DRIVER_INITIALIZE DriverEntry;
NTSTATUS
DriverEntry (
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
    );

EXTERN_C_END

/*************************************************************************
    MiniFilter initialization and unload routines.
*************************************************************************/

NTSTATUS
DriverEntry (
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
    )
/*++

Routine Description:

    This is the initialization routine for this miniFilter driver.  This
    registers with FltMgr and initializes all global data structures.

Arguments:

    DriverObject - Pointer to driver object created by the system to
        represent this driver.

    RegistryPath - Unicode string identifying where the parameters for this
        driver are located in the registry.

Return Value:

    Routine can return non success error codes.

--*/
{
    NTSTATUS hr;

    UNREFERENCED_PARAMETER(RegistryPath);

    DbgPrint("FSD!DriverEntry: Entered\n");

    DbgPrint("FSD!DriverEntry: About to create CFSDefender instance\n");
    CAutoPtr<CFSDefender> pDefender;
    hr = NewInstanceOf<CFSDefender>(&pDefender, DriverObject);
    if (FAILED(hr)) {
        DbgPrint("FSD!DriverEntry: Failed to create CFSDefender instance. hr = 0x%X\n", hr);
        return hr;
    }
    DbgPrint("FSD!DriverEntry: CFSDefender instance created successfully\n");

    DbgPrint("FSD!DriverEntry: About to detach pDefender to global pointer\n");
    pDefender.Detach(&g);
    DbgPrint("FSD!DriverEntry: Global pointer set successfully\n");

    DbgPrint("FSD!DriverEntry: Exiting successfully\n");
    return S_OK;
}