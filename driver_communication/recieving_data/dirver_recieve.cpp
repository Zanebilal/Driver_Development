/*
 * -----------------------------------------------------------
 * This code demonstrates how can a driver device from kernel
 * recieve data that has been sent from a user-mode app
 * the main code walk through creating the driver device , 
 * setting up the IRP stack and Handles IOCTL requests from 
 * user-mode applications
 * -----------------------------------------------------------
 */




#include <ntddk.h>

#define DRIVER_NAME "Your_Driver_Name"

#define IOCTL_SEND_DATA	CTL_CODE(0x8000, 0x900, METHOD_BUFFERED, FILE_ANY_ACCESS)

VOID UnloadDriver(_In_ PDRIVER_OBJECT DriverObject);
NTSTATUS DriverCreateClose(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);
NTSTATUS DriverDeviceIoControl(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);

extern "C" {
    NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {

        UNREFERENCED_PARAMETER(RegistryPath);

        DriverObject->DriverUnload = UnloadDriver;

        DriverObject->MajorFunction[IRP_MJ_CREATE] = DriverCreateClose;
        DriverObject->MajorFunction[IRP_MJ_CLOSE] = DriverCreateClose;
        DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DriverDeviceIoControl;

        UNICODE_STRING devName = RTL_CONSTANT_STRING(L"\\Device\\Your_Driver_Name");

        PDEVICE_OBJECT DeviceObject;
        NTSTATUS status = IoCreateDevice(DriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);
        if (!NT_SUCCESS(status)) {
            DbgPrintEx(0, 0, "[%s] PCKC Driver: Failed to create device (0x%08X)\n", DRIVER_NAME, status);
            return status;
        }

        DeviceObject->Flags |= DO_BUFFERED_IO;

        UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\Your_Driver_Name");
        status = IoCreateSymbolicLink(&symLink, &devName);
        if (!NT_SUCCESS(status)) {
            DbgPrintEx(0, 0, "[%s] PCKC Driver: Failed to create symbolic link (0x%08X)\n", DRIVER_NAME, status);
            IoDeleteDevice(DeviceObject); 
            return status;
        }

        DbgPrintEx(0, 0, "[%s] Driver loaded!", DRIVER_NAME);

        return STATUS_SUCCESS;
    }
}

VOID UnloadDriver(_In_ PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrintEx(0, 0, "[%s] Driver unloaded!\n", DRIVER_NAME);
}

_Use_decl_annotations_
NTSTATUS DriverCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

_Use_decl_annotations_
NTSTATUS DriverDeviceIoControl(PDEVICE_OBJECT, PIRP Irp) {
    NTSTATUS status = STATUS_SUCCESS;
    PIO_STACK_LOCATION CurrentStackLocation = IoGetCurrentIrpStackLocation(Irp);
    CHAR* data = (CHAR*)Irp->AssociatedIrp.SystemBuffer; 

    switch (CurrentStackLocation->Parameters.DeviceIoControl.IoControlCode) {
    case IOCTL_SEND_DATA:
    {
        DbgPrintEx(0, 0, "[%s] Inside IOCTL_SEND_DATA\n", DRIVER_NAME);
        DbgPrintEx(0, 0, "[%s] Data from User-mode process is %s\n", DRIVER_NAME, data);
    }
    break;

    default:
        status = STATUS_INVALID_DEVICE_REQUEST;
        break;
    }

    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}
