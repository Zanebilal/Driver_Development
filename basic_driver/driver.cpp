#include <ntddk.h>

#define DRIVER_NAME "basic-driver"

void UnloadDriver(_In_ PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
	DbgPrintEx(0, 0, " [%s] Driver unloaded successfully.\n", DRIVER_NAME);

}

extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	DriverObject->DriverUnload = UnloadDriver;

	DbgPrintEx(0, 0, " [%s] Driver loaded successfully.\n", DRIVER_NAME);

	return STATUS_SUCCESS;

}