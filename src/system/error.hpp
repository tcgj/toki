#pragma once

#ifndef TK_ENABLE_ASSERTS
#define tkAssert(expr) ((void)0)
#else
#define tkAssert(expr) assert(expr)
#endif // TK_ENABLE_ASSERTS
