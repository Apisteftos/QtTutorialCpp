#pragma once

#include <string>
#include <vector>

// ============================================================
// MyDynLib — Dynamic Library Header
// ============================================================
// Export macros for cross-platform visibility
// ============================================================

#if defined(_WIN32)
    #ifdef MYDYNLIB_EXPORTS
        #define MYDYNLIB_API __declspec(dllexport)
    #else
        #define MYDYNLIB_API __declspec(dllimport)
    #endif
#else
    // GCC/Clang — use visibility attribute
    #define MYDYNLIB_API __attribute__((visibility("default")))
#endif

namespace MyDynLib {

    // ---- Math utilities ----
    MYDYNLIB_API int    add       (int a, int b);
    MYDYNLIB_API int    multiply  (int a, int b);
    MYDYNLIB_API double divide    (double a, double b);

    // ---- String utilities ----
    MYDYNLIB_API std::string toUpperCase(const std::string& s);
    MYDYNLIB_API std::string trim       (const std::string& s);

    // ---- Collection utilities ----
    MYDYNLIB_API double avgVector(const std::vector<int>& v);

    // ---- Library info ----
    MYDYNLIB_API std::string version();
    MYDYNLIB_API void        printBanner();

} // namespace MyDynLib
