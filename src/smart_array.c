//
// Created by evanc on 10/26/26.
//

#include "../include/smart_array.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define DIE(...)                      \
    do {                              \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n");        \
        abort();                      \
    } while (0)

void* smartArrayCheckedRealloc(void* ptr, size_t oldSize, size_t newSize)
{
    if (newSize == 0) {
        free(ptr);
        return NULL;
    }
    void* result = realloc(ptr, newSize);
    if (result == NULL) {
        DIE("Failed to reallocate smart array from %zu bytes to %zu bytes",
            oldSize, newSize);
    }
    return result;
}

void initSmartArray(SmartArray* restrict smartArray, ReallocateFn reallocater,
    size_t elementSize)
{
    if (NULL == smartArray)
        return;
    smartArray->count = smartArray->capacity = 0;
    smartArray->elementSize = elementSize;
    smartArray->reallocater = reallocater;
    smartArray->data = NULL;
}

void smartArrayAppend(SmartArray* restrict smartArray,
    void const* restrict element)
{
    assert(NULL != smartArray && NULL != element);

    if (smartArray->capacity <= smartArray->count) {
        size_t oldCapacity = smartArray->capacity;
        smartArray->capacity = GROW_CAPACITY(oldCapacity);
        smartArray->data = smartArray->reallocater(
            smartArray->data, oldCapacity * smartArray->elementSize,
            smartArray->capacity * smartArray->elementSize);
    }

    smartArray->count++;
    memcpy(accessSmartArrayElementAt(smartArray, smartArray->count - 1),
        element, smartArray->elementSize);
}

size_t getSmartArrayCount(SmartArray const* smartArray)
{
    assert(NULL != smartArray);
    return smartArray->count;
}

size_t getSmartArrayCapacity(SmartArray const* smartArray)
{
    assert(NULL != smartArray);
    return smartArray->capacity;
}

void* accessSmartArrayElementAt(SmartArray* smartArray, size_t index)
{
#define INCREMENT_POINTER(ptr, bytes) ((char*)(ptr) + (bytes))
    if (index >= smartArray->count) {
        DIE("Tried to access smart array element at index %zu of smart array "
            "%p, but count is %zu",
            index, (void*)smartArray, smartArray->count);
    }
    return INCREMENT_POINTER(smartArray->data, smartArray->elementSize * index);
#undef INCREMENT_POINTER
}

bool smartArrayIsEmpty(SmartArray const* smartArray)
{
    return 0 == getSmartArrayCount(smartArray);
}

bool smartArrayPopFromEnd(SmartArray* restrict smartArray, void* restrict out)
{
    assert(NULL != smartArray);

    if (smartArrayIsEmpty(smartArray))
        return false;

    if (NULL != out) {
        void* lastElement = accessSmartArrayElementAt(smartArray, smartArray->count - 1);
        memcpy(out, lastElement, smartArray->elementSize);
    }

    smartArray->count--;
    return true;
}

void freeSmartArray(SmartArray* smartArray)
{
    assert(NULL != smartArray);
    smartArray->reallocater(smartArray->data,
        smartArray->capacity * smartArray->elementSize, 0);
    smartArray->count = smartArray->capacity = 0;
    smartArray->data = NULL;
}