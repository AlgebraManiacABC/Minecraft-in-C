//
// Created by evanc on 10/26/26.
//

#ifndef MINECRAFT_IN_C_SMART_ARRAY_H
#define MINECRAFT_IN_C_SMART_ARRAY_H
#include <stddef.h>
#include <stdbool.h>

typedef void *(*ReallocateFn)(void *, size_t, size_t);

void * smartArrayCheckedRealloc(void * data, size_t _oldSize, size_t newSize);

typedef struct {
    size_t count, capacity, elementSize;
    ReallocateFn reallocater;
    void *data;
} SmartArray;

void initSmartArray(SmartArray *restrict smartArray, ReallocateFn reallocater,
                    size_t elementSize);

void smartArrayAppend(SmartArray *restrict smartArray,
                      void const *restrict element);

#define SMART_ARRAY_AT(smartArray, index, type) \
    (*((type *)accessSmartArrayElementAt((smartArray), (index))))

size_t getSmartArrayCount(SmartArray const *smartArray);
size_t getSmartArrayCapacity(SmartArray const *smartArray);

void *accessSmartArrayElementAt(SmartArray *smartArray, size_t index);

bool smartArrayIsEmpty(SmartArray const *smartArray);
bool smartArrayPopFromEnd(SmartArray *restrict smartArray, void *restrict out);

void freeSmartArray(SmartArray *smartArray);

#endif //MINECRAFT_IN_C_SMART_ARRAY_H
