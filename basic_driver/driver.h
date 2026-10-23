#pragma once
#include <ntddk.h>

constexpr auto DRIVER_TAG = '1GAT';

void Cleanup(PDRIVER_OBJECT DriverObject);