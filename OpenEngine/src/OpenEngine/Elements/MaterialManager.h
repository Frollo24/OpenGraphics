#pragma once

#include "OpenEngine/Core/Base.h"
#include "Material.h"

namespace OpenGraphics
{
    class MaterialManager
    {
    public:
        static Ref<MaterialInstance> Get(const std::string& name);
        static Ref<MaterialInstance> GetOrCreate(const std::string& name, const Material& material);

    private:
        inline static DynamicRefArray<MaterialType> s_MaterialRefArray{50};
        inline static std::unordered_map<std::string, size_t> s_IndexMap;
    };
}
