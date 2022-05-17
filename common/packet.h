#pragma once

#include "forwards.h"

namespace ese {

enum class PacketType { Message, Command };

NLOHMANN_JSON_SERIALIZE_ENUM(
    PacketType, {{PacketType::Message, BOOST_PP_STRINGIZE(Message)},
                  {PacketType::Command, BOOST_PP_STRINGIZE(Command)}})

class Packet {
 public:
  explicit Packet() = default;
  explicit Packet(PacketType type, std::string body);

 public:
  PacketType Type() const;

  const std::string& Body() const;

  friend void to_json(nlohmann::json& json, const Packet& packet);

  friend void from_json(const nlohmann::json& json, Packet& packet);

 private:
  PacketType type_;
  std::string body_;
};
}  // namespace ese