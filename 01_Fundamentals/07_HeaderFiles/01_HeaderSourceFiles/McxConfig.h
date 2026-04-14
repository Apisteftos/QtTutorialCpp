#pragma once

#include <string>
#include <cstdint>

// ============================================================
// McxConfig.h — MCX session configuration
// ============================================================
// Shows a real-world struct header — data shared across files
// ============================================================

// Constants declared in header — available everywhere that includes this
constexpr int    MCX_MAX_SESSIONS   = 8;
constexpr int    MCX_DEFAULT_PORT   = 5060;
constexpr double MCX_FLOOR_TIMEOUT  = 30.0;

// Enum declared in header — shared type
enum class McxCodec {
    AMR_NB,
    AMR_WB,
    OPUS,
    EVRC
};

// Struct declared in header — shared data layout
struct McxSessionConfig {
    std::string serverHost;
    uint16_t    serverPort;
    std::string groupId;
    McxCodec    codec;
    bool        encrypted;
    double      floorTimeout;

    // Constructor declaration — implementation in .cpp
    McxSessionConfig();
    McxSessionConfig(const std::string& host,
                     uint16_t           port,
                     const std::string& group,
                     McxCodec           c = McxCodec::AMR_NB,
                     bool               enc = false);

    // Method declarations
    std::string toString() const;
    bool        isValid()  const;
};
