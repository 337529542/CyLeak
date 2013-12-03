#ifndef __MEMORY_LEAK_M_H__
#define __MEMORY_LEAK_M_H__

#include "ml.h"

void* operator new (size_t size, const char* file, int line);
void* operator new [](size_t size, const char* file, int line);

#define new new(__FILE__, __LINE__)

#endif
