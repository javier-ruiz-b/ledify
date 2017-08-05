#pragma once

#ifndef size_t
#define size_t int
#endif

#define SIZE_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#define SIZE_OF_STRING(x) (SIZE_OF(x) - 1)
