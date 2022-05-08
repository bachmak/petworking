#pragma once

#include "client/client.h"

namespace ese {

std::unique_ptr<Client> CreateClient(Context& context,
                                     const ConnectionSettings& settings,
                                     Logger& logger);
}