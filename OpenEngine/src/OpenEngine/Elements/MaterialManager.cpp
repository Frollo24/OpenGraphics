#include "ogpch.h"
#include "MaterialManager.h"

namespace OpenGraphics
{
    Ref<MaterialInstance> MaterialManager::Get(const std::string& name)
    {
        if (const auto& it = s_IndexMap.find(name); it != s_IndexMap.end())
        {
            Ref<MaterialType> materialType = s_MaterialRefArray.CreateRefAt(it->second);
            Ref<MaterialInstance> instance = CreateRef<MaterialInstance>(materialType);
            return instance;
        }

        return nullptr;
    }

    Ref<MaterialInstance> MaterialManager::GetOrCreate(const std::string& name, const Material& material)
    {
        Ref<MaterialInstance> instance = Get(name);
        if (!instance)
        {
            s_MaterialRefArray.EmplaceNew(material);
            Ref<MaterialType> materialType = s_MaterialRefArray.CreateRefLast();
            instance = CreateRef<MaterialInstance>(materialType);
        }

        return instance;
    }
}
