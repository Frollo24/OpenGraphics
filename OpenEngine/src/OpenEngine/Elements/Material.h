#pragma once

#include "OpenEngine/Core/Base.h"

#include "OpenEngine/Math/Color.h"

namespace OpenGraphics
{
    enum class MaterialPropertyType
    {
        None = 0,
        Float,
        Color,
    };

    static std::string_view MaterialPropertyTypeToString(const MaterialPropertyType type)
    {
        switch (type)
        {
            case MaterialPropertyType::Float: return "MaterialPropertyType::Float";
            case MaterialPropertyType::Color: return "MaterialPropertyType::Color";
            default:
                return "MaterialPropertyType::None";
        }
    }

    struct MaterialProperty
    {
        std::string Key;
        MaterialPropertyType Type = MaterialPropertyType::None;
        void* Value = nullptr;
    };

    // HACK: we should distinguish between material type and material instance
    struct OPEN_API Material
    {
        std::string Name;
        Color MainColor;
        std::unordered_map<std::string, MaterialProperty> Properties;

        template<MaterialPropertyType propType, typename T>
        T GetProperty(const std::string& name) const;

        [[nodiscard]] float GetFloat(const std::string& name) const;
        [[nodiscard]] Color GetColor(const std::string& name) const;

        template<MaterialPropertyType propType, typename T>
        void SetProperty(const std::string& name, const T& value);

        void SetFloat(const std::string& name, const float& value);
        void SetColor(const std::string& name, const Color& color);

        inline bool operator==(const Material& other) const
        {
            return MainColor == other.MainColor;
        }
    };

    template<MaterialPropertyType propType, typename T>
    T Material::GetProperty(const std::string& name) const
    {
        auto propertyLocation = Properties.find(name);
        if (propertyLocation == Properties.end())
            return T{};

        auto materialProperty = propertyLocation->second;
        if (materialProperty.Type != propType)
            return T{};

        return *reinterpret_cast<T*>(materialProperty.Value);
    }

    template<MaterialPropertyType propType, typename T>
    void Material::SetProperty(const std::string& name, const T& value)
    {
        if (auto propertyLocation = Properties.find(name); propertyLocation != Properties.end())
        {
            if (Properties[name].Type != propType)
            {
                Logger::Warn("{} property of material {} is not of type {}",
                    name, Name, MaterialPropertyTypeToString(propType));
                return;
            }
            auto oldValue = reinterpret_cast<T*>(Properties[name].Value);
            Properties[name].Value = new T(value);
            delete oldValue;
        }
        else
        {
            MaterialProperty newProperty{
                .Key = name,
                .Type = propType,
                .Value = new T(value)
            };
            Properties[name] = newProperty;
        }
    }
}

template<> struct std::hash<OpenGraphics::Material>
{
    size_t operator()(OpenGraphics::Material const& material) const noexcept
    {
        return hash<glm::vec4>()(material.MainColor.toGlmVector());
    }
};