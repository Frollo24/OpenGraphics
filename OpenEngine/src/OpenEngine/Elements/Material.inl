#pragma once

namespace OpenGraphics
{
    template<MaterialPropertyType propType, typename T>
    T Material::GetProperty(const std::string& name) const
    {
        const auto propertyLocation = Properties.find(name);
        if (propertyLocation == Properties.end())
            return T{};

        const auto materialProperty = propertyLocation->second;
        if (materialProperty.Type != propType)
            return T{};

        return *reinterpret_cast<T*>(materialProperty.Value);
    }

    template<MaterialPropertyType propType, typename T>
    void Material::SetProperty(const std::string& name, const T& value)
    {
        if (const auto propertyLocation = Properties.find(name); propertyLocation != Properties.end())
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
            const MaterialProperty newProperty{
                .Key = name,
                .Type = propType,
                .Value = new T(value)
            };
            Properties[name] = newProperty;
        }
    }
}
