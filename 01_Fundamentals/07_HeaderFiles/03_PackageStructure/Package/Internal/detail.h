#pragma once

#include <string>

// ============================================================
// Package/Internal/detail.h
// ============================================================
// INTERNAL header — not part of the public API
// Users should only include Package/API/package.h
// ============================================================

namespace Package::Internal {

    // Implementation helper — not meant for external use
    std::string formatValue(int v, const std::string& s) {
        return "[" + std::to_string(v) + "|" + s + "]";
    }

    constexpr int MAX_ITEMS = 100;

} // namespace Package::Internal
