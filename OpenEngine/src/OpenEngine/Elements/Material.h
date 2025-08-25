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

    class OPEN_API MaterialType
    {
    public:
        MaterialType() = default;
        MaterialType(const Material& material) : m_Material(material) {}

        operator Material&() { return m_Material; }
        operator const Material&() const { return m_Material; }

    private:
        friend class MaterialInstance;
        Material m_Material{};
    };

    class OPEN_API MaterialInstance
    {
    public:
        MaterialInstance() = default;
        MaterialInstance(const Material& material) : m_Material(material), m_Type(nullptr) {}

        MaterialInstance(const Ref<MaterialType>& material)
            : m_Material(*material), m_Type(material) {}

        operator Material&() { return m_Material; }
        operator const Material&() const { return m_Material; }

    private:
        Material m_Material{};
        Ref<MaterialType> m_Type = nullptr;
    };
}

template<> struct std::hash<OpenGraphics::Material>
{
    size_t operator()(OpenGraphics::Material const& material) const noexcept
    {
        return hash<glm::vec4>()(material.MainColor.toGlmVector());
    }
};

#include "Material.inl"
