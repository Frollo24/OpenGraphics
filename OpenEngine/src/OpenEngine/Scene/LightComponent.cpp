#include "ogpch.h"
#include "LightComponent.h"

#include "GameObject.h"
#include "RenderComponent.h"
#include "OpenEngine/Math/Mathf.h"
#include "OpenEngine/Render/RenderCommand.h"

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

    void LightComponent::OnCreate()
    {
        BufferDescription lightDataBufferDesc{};
        lightDataBufferDesc.Type = BufferType::Uniform;
        lightDataBufferDesc.Size = sizeof(Vector4D) * 4; // 2 for directional light + 2 for point light
        lightDataBufferDesc.Usage = BufferUsage::DynamicDraw;
        m_LightDataBuffer = new Buffer(lightDataBufferDesc);
    }

    void LightComponent::OnUpdate()
    {
        Utils::AdjustLightData(m_LightData);
    }

    void LightComponent::OnRender()
    {
        std::array<Vector4D, 4> lightData{Vector4D::zero};
        const auto& [color, intensity, unit, type] = m_LightData;
        if (type == LightType::Directional)
        {
            lightData[0] = Vector4D(color.r, color.g, color.b, intensity);
            lightData[1] = Vector4D(-1.0, -1.0, -1.0).Normalized();
        }
        else if (type == LightType::Point)
        {
            Vector4D lastColumn = GetGameObject().GetTransform()->GetModelMatrix()[3];
            lastColumn.w = 20.0f;
            lightData[2] = Vector4D(color.r, color.g, color.b, intensity);
            lightData[3] = lastColumn;
        }

        m_LightDataBuffer->SetData(0, sizeof(Vector4D) * 4, lightData.data());
        RenderCommand::BindUniformBuffer(m_LightDataBuffer, 0);
    }

    void LightComponent::OnDestroy()
    {
        delete m_LightDataBuffer;
    }
}
