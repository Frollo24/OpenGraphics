#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Scene/Component.h"
#include "OpenEngine/Math/Color.h"
#include "OpenEngine/Render/Buffer.h"

namespace OpenGraphics
{
    enum class PhotometricUnit : uint8_t
    {
        Lumen,   // Luminous power
        Candela, // Luminous intensity
        Lux,     // Illuminance
        Nit      // Luminance
    };

    enum class LightType : uint8_t
    {
        Directional,
        Point,
        Spot
    };

    struct LightData
    {
        Color LightColor = Color::white;
        float Intensity = 1.0f;
        PhotometricUnit Unit = PhotometricUnit::Lumen;
        LightType Type = LightType::Directional;
    };

    class OPEN_API LightComponent final : public Component
    {
    public:
        LightComponent() = default;
        ~LightComponent() = default;

        LightComponent(const Color& lightColor, float intensity, PhotometricUnit unit, LightType type);
        LightComponent(const LightData& lightData);

        [[nodiscard]] inline const LightData& GetLightData() const { return m_LightData; }
        inline void SetLightData(const LightData& lightData) { m_LightData = lightData; }

        void OnCreate() override;
        void OnUpdate() override;
        void OnRender() override;
        void OnDestroy() override;

    private:
        LightData m_LightData{};
    };
}
