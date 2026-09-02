#pragma once

#include "render/Material.h"
#include <vector>

class MaterialManager {
public:
    MaterialHandle add(std::shared_ptr<Material> material) {
        MaterialHandle id {
            static_cast<std::uint32_t>(materials_.size())
        };

        materials_.push_back(std::move(material));

        return id;
    }

    std::shared_ptr<Material> get(MaterialHandle id) {
        return materials_[id];
    }

private:
    std::vector<std::shared_ptr<Material> > materials_;

};
