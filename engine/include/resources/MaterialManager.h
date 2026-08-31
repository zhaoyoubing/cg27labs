#pragma once

#include "render/Material.h"
#include <vector>

class MaterialManager {
public:
    MaterialHandle add(Material material) {
        MaterialHandle id{
            static_cast<std::uint32_t>(materials_.size())
        };

        materials_.push_back(std::move(material));

        return id;
    }

    Material& get(MaterialHandle id) {
        return materials_[id];
    }

private:
    std::vector<Material> materials_;

};
