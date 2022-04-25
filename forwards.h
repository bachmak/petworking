#pragma once

#include "stdafx.h"

#define ESE_FWD(args) std::forward<decltype(args)>(args)...

namespace ese {
class Logger;

using Context = boost::asio::io_context;
using Socket = boost::asio::ip::tcp::socket;
using Acceptor = boost::asio::ip::tcp::acceptor;
using Endpoint = boost::asio::ip::tcp::endpoint;
using Resolver = boost::asio::ip::tcp::resolver;
using ResolveResults = Resolver::results_type;
using Buffer = boost::asio::streambuf;
using ErrorCode = boost::system::error_code;

using Ip = boost::asio::ip::address;
using Port = boost::asio::ip::port_type;

inline constexpr const auto gMsgTerminator = std::string_view("\n");
inline constexpr const auto gLogDelim = std::string_view(" ");
}  // namespace ese