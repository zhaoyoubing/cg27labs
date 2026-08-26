// include/ecs/components/ComponentIdCounter.h

#pragma once
#include <cstddef>

class ComponentIdCounter {
private:
    static std::size_t GetUniqueId() {
        static std::size_t counter = 0;
        return counter++;
    }

public:
    template <typename T>
    static std::size_t GetId() {
        static std::size_t id = GetUniqueId();
        return id;
    }
};