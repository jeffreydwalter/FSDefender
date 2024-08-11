#include "FSDMemoryUtils.h"
#include <ntddk.h>

void* __cdecl operator new(size_t size) {
    void* ptr = ExAllocatePool2(POOL_FLAG_NON_PAGED, size, 'FSDm');
    if (ptr == nullptr) {
        // Log the allocation failure
        DbgPrint("FSD: Memory allocation failed for size %zu\n", size);
        // Throw std::bad_alloc() equivalent for kernel mode
        ExRaiseStatus(STATUS_NO_MEMORY);
    }
    return ptr;
}

void* __cdecl operator new[](size_t size) {
    void* ptr = ExAllocatePool2(POOL_FLAG_NON_PAGED, size, 'FSDm');
    if (ptr == nullptr) {
        // Log the allocation failure
        DbgPrint("FSD: Array memory allocation failed for size %zu\n", size);
        // Throw std::bad_alloc() equivalent for kernel mode
        ExRaiseStatus(STATUS_NO_MEMORY);
    }
    return ptr;
}

void __cdecl operator delete(void* object, size_t size) {
    UNREFERENCED_PARAMETER(size);
    if (object != nullptr) {
        ExFreePoolWithTag(object, 'FSDm');
    }
}

void __cdecl operator delete[](void* object) {
    if (object != nullptr) {
        ExFreePoolWithTag(object, 'FSDm');
    }
}

// Add these two overloads for C++14 compatibility
void __cdecl operator delete(void* object) {
    if (object != nullptr) {
        ExFreePoolWithTag(object, 'FSDm');
    }
}

void __cdecl operator delete[](void* object, size_t size) {
    UNREFERENCED_PARAMETER(size);
    if (object != nullptr) {
        ExFreePoolWithTag(object, 'FSDm');
    }
}