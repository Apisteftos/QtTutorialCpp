#include "McxConfig.h"
#include <sstream>

// ============================================================
// McxConfig.cpp — McxSessionConfig IMPLEMENTATIONS
// ============================================================

McxSessionConfig::McxSessionConfig()
    : serverHost("localhost")
    , serverPort(MCX_DEFAULT_PORT)
    , groupId("DEFAULT")
    , codec(McxCodec::AMR_NB)
    , encrypted(false)
    , floorTimeout(MCX_FLOOR_TIMEOUT)
{}

McxSessionConfig::McxSessionConfig(const std::string& host,
                                   uint16_t           port,
                                   const std::string& group,
                                   McxCodec           c,
                                   bool               enc)
    : serverHost(host)
    , serverPort(port)
    , groupId(group)
    , codec(c)
    , encrypted(enc)
    , floorTimeout(MCX_FLOOR_TIMEOUT)
{}

std::string McxSessionConfig::toString() const
{
    std::ostringstream oss;
    oss << serverHost << ":" << serverPort
        << " group=" << groupId
        << " enc="   << (encrypted ? "yes" : "no");
    return oss.str();
}

bool McxSessionConfig::isValid() const
{
    return !serverHost.empty()
        && serverPort > 0
        && !groupId.empty();
}
