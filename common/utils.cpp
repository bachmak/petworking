#include "common/utils.h"

#include "common/packet.h"

namespace ese {
namespace utils {

std::size_t WritePacketWithSize(StreamBuf& buffer, const Packet& packet) {
  std::string body = nlohmann::json(packet).dump();
  auto body_size = body.size();

  auto os = std::ostream(&buffer);
  os.write(reinterpret_cast<char*>(&body_size), sizeof(body_size)) << body;

  return sizeof(body_size) + body_size;
}

std::size_t ReadSize(StreamBuf& buffer) {
  auto is = std::istream(&buffer);
  auto body_size = std::size_t();
  is.read(reinterpret_cast<char*>(&body_size), sizeof(body_size));

  return body_size;
}

Packet ReadPacket(StreamBuf& buffer) {
  auto is = std::istream(&buffer);
  auto json = nlohmann::json();
  is >> json;
  return json.get<Packet>();
}
}  // namespace utils
}  // namespace ese