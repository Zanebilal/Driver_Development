#include <Windows.h>
#include <stdio.h>

#define IOCTL_GET_DATA CTL_CODE(0x8000, 0x900, METHOD_BUFFERED, FILE_ANY_ACCESS)

int main() {
    HANDLE hDriver = CreateFileW(
        L"\\\\.\\GetDriver",    
        GENERIC_READ,              
        0,                        
        NULL,                      
        OPEN_EXISTING,             
        0,                         
        NULL                     
    );

    if (hDriver == INVALID_HANDLE_VALUE) {
        printf("Failed to open driver. Error code: %lu\n", GetLastError());
        return 1; 
    }


    char buffer[256] = { 0 }; 
    DWORD bytesRead; 

    if (!ReadFile(hDriver, buffer, sizeof(buffer), &bytesRead, NULL)) {
        printf("Failed to read from driver with Error code: %lu\n", GetLastError());
        CloseHandle(hFile); 
        return 1; 
    }

    printf("Received Data from driver: %s\n", buffer);

    CloseHandle(hDriver);
    return 0; 
}
