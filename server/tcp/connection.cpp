#include "server/tcp/connection.h"

#include "common/logger.h"
#include "common/packet.h"
#include "common/utils.h"

namespace ese {
namespace tcp {

Connection::Connection(Socket socket, Logger& logger,
                       ServerCallbackPtr on_packet_received)
    : socket_(std::move(socket)),
      on_packet_received_(std::move(on_packet_received)),
      logger_(logger) {
  logger_.LogLine("new connection:", socket_);
}

Connection::~Connection() { logger_.LogLine("closed", socket_); }

void Connection::Start() { Read(sizeof(nlohmann::json::size_type)); }

void Connection::OnRead(ErrorCode ec, std::size_t bytes_read,
                        std::size_t bytes_left) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  static auto packetPart = PacketPart::Size;
  if (std::exchange(packetPart, PacketPart::Body) == PacketPart::Size) {
    auto request_body_size = nlohmann::json::size_type();
    if (bytes_read >= sizeof(request_body_size)) {
      auto is = std::istream(&buffer_);
      is.read(reinterpret_cast<char*>(&request_body_size),
              sizeof(request_body_size));

      if (bytes_read -= sizeof(request_body_size);
          bytes_read == request_body_size) {
        auto json = nlohmann::json();
        is >> json;
        static int i = 0;
        std::ofstream ofs(std::to_string(++i) + ".txt");
        ofs << "Request: " << json << std::endl;
        auto request = json.get<Packet>();
        auto response = on_packet_received_->operator()(std::move(request));

        json = nlohmann::json(response);
        auto response_body_size = json.size();

        auto os = std::ostream(&buffer_);
        os.write(reinterpret_cast<char*>(&response_body_size),
                 sizeof(response_body_size))
            << json;
        return Write(sizeof(response_body_size) + response_body_size);
      }
    } else {
      bytes_left = sizeof(request_body_size) - bytes_read;
      packetPart = PacketPart::Size;
      return Read(bytes_left);
    }
  }
  return Read(bytes_left - bytes_read);
}

void Connection::OnWrite(ErrorCode ec, std::size_t bytes_written,
                         std::size_t bytes_left) {
  if (ec) {
    ESE_LOG_EC(logger_, ec)
    return;
  }

  if (bytes_left -= bytes_written; bytes_left == 0) {
    Read(sizeof(nlohmann::json::size_type));
  } else {
    Write(bytes_left);
  }
}

void Connection::Read(std::size_t bytes) {
  boost::asio::async_read(
      socket_, buffer_,
      [connection = shared_from_this(), bytes](auto&&... args) {
        connection->OnRead(ESE_FWD(args), bytes);
      });
}

void Connection::Write(std::size_t bytes) {
  boost::asio::async_write(
      socket_, buffer_,
      [connection = shared_from_this(), bytes](auto&&... args) {
        connection->OnWrite(ESE_FWD(args), bytes);
      });
}
}  // namespace tcp
}  // namespace ese