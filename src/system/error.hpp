#pragma once

#ifndef ENABLE_ASSERTS
#define tkAssert(expr) ((void)0)
#else
#define tkAssert(expr) assert(expr)
#endif // ENABLE_ASSERTS
