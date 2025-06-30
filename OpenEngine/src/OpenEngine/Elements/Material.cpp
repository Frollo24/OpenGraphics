#include "ogpch.h"
#include "Material.h"

namespace OpenGraphics
{
    float Material::GetFloat(const std::string& name) const
    {
        return GetProperty<MaterialPropertyType::Float, float>(name);
    }

    Color Material::GetColor(const std::string& name) const
    {
        return GetProperty<MaterialPropertyType::Color, Color>(name);
    }

    void Material::SetFloat(const std::string& name, const float& value)
    {
        SetProperty<MaterialPropertyType::Float>(name, value);
    }

    void Material::SetColor(const std::string& name, const Color& color)
    {
        SetProperty<MaterialPropertyType::Color>(name, color);
    }
}
