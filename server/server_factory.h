#pragma once

#include "server/server.h"

namespace ese {

std::unique_ptr<Server> CreateServer(Context& context,
                                     const ConnectionSettings& settings,
                                     Logger& logger);
}