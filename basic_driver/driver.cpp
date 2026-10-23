/*
 * -----------------------------------------------------------
 * this code demonstrate the basic and simplest way to create
 * a simple driver that can be loaded to kernel 
 * you can see the debug messages by loading the driver to 
 * the kernel using servie control manager sc.exe or using a 
 * graphical interface tool like OSR Online, otr you can make
 * the driver load itself using winapi like demonstrated in 
 * the code in : Driver_Development/basic_driver/self_load.cpp
 * -----------------------------------------------------------
 */


#include <ntddk.h>

#define DRIVER_NAME "Your_driver_name"

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
