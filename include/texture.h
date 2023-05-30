#pragma once

#include <string>
#include <optional>

struct Texture{
  uint32_t id;
};

std::optional<Texture> load_texture(const std::string& filename);
