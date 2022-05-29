#pragma once

#include "stdafx.h"

#define ESE_FWD(args) std::forward<decltype(args)>(args)...

namespace ese {

class Packet;

class Logger;
struct ConnectionSettings;

using Context = boost::asio::io_context;
using StreamBuf = boost::asio::streambuf;
using StaticBuf = std::array<char, 8192>;
using ErrorCode = boost::system::error_code;

using Ip = boost::asio::ip::address;
using Port = boost::asio::ip::port_type;

inline constexpr const auto gMsgTerminator = std::string_view("\n");
inline constexpr const auto gLogDelim = std::string_view(" ");

namespace tcp {

using Socket = boost::asio::ip::tcp::socket;
using Acceptor = boost::asio::ip::tcp::acceptor;
using Endpoint = boost::asio::ip::tcp::endpoint;

namespace server {

class Connection;

using OnConnected = std::function<void(Connection&)>;
using OnPacketReceived = std::function<void(Packet, Connection&)>;
}  // namespace server

namespace client {

class Client;

using OnConnected = std::function<void(Client&)>;
using OnPacketSent = std::function<void(Client&)>;
using OnPacketReceived = std::function<void(Packet, Client&)>;
}  // namespace client
}  // namespace tcp

namespace udp {

using Socket = boost::asio::ip::udp::socket;
using Endpoint = boost::asio::ip::udp::endpoint;

namespace server {

class Server;

using OnPacketReceived = std::function<void(Packet, Server&)>;
}  // namespace server

namespace client {

class Client;

using OnPacketSent = std::function<void(Client&)>;
using OnPacketReceived = std::function<void(Packet, Client&)>;
}  // namespace client
}  // namespace udp
}  // namespace ese