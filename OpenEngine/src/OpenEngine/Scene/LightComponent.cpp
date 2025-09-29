#include "ogpch.h"
#include "LightComponent.h"

#include "GameObject.h"
#include "RenderComponent.h"
#include "OpenEngine/Math/Mathf.h"

namespace OpenGraphics
{
    namespace Utils
    {
        static void AdjustLightData(LightData& lightData)
        {
            switch (lightData.Type)
            {
                // The target unit for directional lights is Lux
                case LightType::Directional:
                    lightData.Unit = PhotometricUnit::Lux;
                    break;
                // The target unit for point & spot lights is Candela
                case LightType::Point:
                    if (lightData.Unit == PhotometricUnit::Lumen)
                        lightData.Intensity *= Mathf::OneOverFourPi;
                    lightData.Unit = PhotometricUnit::Candela;
                    break;
                case LightType::Spot:
                    if (lightData.Unit == PhotometricUnit::Lumen)
                        lightData.Intensity *= Mathf::OneOverPi;  // Technically not physically correct, but it allows
                                                                  // decoupling of the cone from the luminous power
                    lightData.Unit = PhotometricUnit::Candela;
                    break;
                default:
                    break;
            }
        }
    }

    LightComponent::LightComponent(const Color& lightColor, const float intensity, const PhotometricUnit unit, const LightType type)
        : m_LightData({lightColor, intensity, unit, type})
    {
    }

    LightComponent::LightComponent(const LightData& lightData)
        : m_LightData(lightData)
    {
    }

    void LightComponent::OnUpdate()
    {
        Utils::AdjustLightData(m_LightData);
    }

    void LightComponent::OnRender()
    {
        // TODO: upload data to a Uniform Buffer when available
    }
}
