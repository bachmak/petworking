#include "packet.h"

namespace ese {

Packet::Packet(PacketType type, std::string body)
    : type_(type), body_(std::move(body)) {}

PacketType Packet::Type() const { return type_; }

const std::string& Packet::Body() const { return body_; }

void to_json(nlohmann::json& json, const Packet& packet) {
  json = {{"type", packet.type_}, {"body", packet.body_}};
}

void from_json(const nlohmann::json& json, Packet& packet) {
  json.at("type").get_to(packet.type_);
  json.at("body").get_to(packet.body_);
}
}  // namespace ese