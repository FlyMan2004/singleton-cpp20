#pragma once

#if __cplusplus < 202002L
# error "C++20 or higher is required"
#endif

#if !defined(__clang__) || __clang_major__ < 20
# error "Clang 20 or higher is required"
#endif

#define export_symbol$ [[gnu::visibility("default")]]
