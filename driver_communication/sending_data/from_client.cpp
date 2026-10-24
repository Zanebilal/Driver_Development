/*
 * -----------------------------------------------------------
 * This code demonstrate how can a client app from user mode
 * communicate with a driver from kernel mode using winapi by
 * sending a text string which can be viewed from the driver
 * code in the recieveDriver code in :
 * Driver_Development/driver_communication/recieving_data/recieve_driver.cpp

 * -----------------------------------------------------------
 */




#include <windows.h>
#include <stdio.h>

#define MY_DRIVER_DEVICE  0x8000
#define IOCTL_SEND_DATA CTL_CODE(MY_DRIVER_DEVICE, 0x900, METHOD_BUFFERED, FILE_ANY_ACCESS)

int main() {
    
    HANDLE hDevice = CreateFile(
        L"\\\\.\\kernelDriver",        
        GENERIC_WRITE,                
        FILE_SHARE_WRITE,             
        NULL,                        
        OPEN_EXISTING,                
        0,                            
        NULL                     
    );

    if (hDevice == INVALID_HANDLE_VALUE) {
        printf("Failed to open the device with Error code: %d\n", GetLastError());
        return 1; 
    }

    char data[] = "Hello from user mode";
    DWORD bytesReturned; 

    BOOL success = DeviceIoControl(
        hDevice,                      
        IOCTL_SEND_DATA,             
        data,                        
        sizeof(data),                
        NULL,                         
        0,                           
        &bytesReturned,              
        NULL                          
    );

    if (!success) {
        printf("DeviceIoControl failed with Error code: %d\n", GetLastError());
        CloseHandle(hDevice); 
        return 1; 
    }

    printf("Data sent successfully to the driver: %s\n", data);

    CloseHandle(hDevice);
    return 0; 
}
