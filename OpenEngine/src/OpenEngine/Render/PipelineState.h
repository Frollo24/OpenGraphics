#pragma once

#include "OpenEngine/Core/Base.h"

namespace OpenGraphics
{
#pragma region PipelineDepthState
	enum class DepthComparison : uint8_t
	{
		None,
		Less,
		Equal,
		LessOrEqual,
		Greater,
		NotEqual,
		GreaterOrEqual,
		Always
	};

	struct DepthBoundaries
	{
		float MinDepth = 0.0f;
		float MaxDepth = 1.0f;
	};

	struct PipelineDepthState
	{
		DepthBoundaries DepthRange{};
		DepthComparison DepthFunc = DepthComparison::Less;
		bool DepthTest = true;
		bool DepthWrite = true;
	};
#pragma endregion

#pragma region PipelineBlendState
	enum class ColorWriteMask : uint8_t
	{
		ColorWriteMaskR = OG_BIT(0), // 1 (0x0001)
		ColorWriteMaskG = OG_BIT(1), // 2 (0x0010)
		ColorWriteMaskB = OG_BIT(2), // 4 (0x0100)
		ColorWriteMaskA = OG_BIT(3), // 8 (0x1000)
		ColorWriteMaskAll = ColorWriteMaskR | ColorWriteMaskG | ColorWriteMaskB | ColorWriteMaskA, // 15 (0x1111)
	};

	enum class BlendFactor : uint8_t
	{
		Zero, One,
		SrcColor, OneMinusSrcColor, DstColor, OneMinusDstColor,
		SrcAlpha, OneMinusSrcAlpha, DstAlpha, OneMinusDstAlpha,
		ConstantColor, OneMinusConstantColor,
		ConstantAlpha, OneMinusConstantAlpha,
	};

	enum class BlendOperation : uint8_t
	{
		Add, Subtract, ReverseSubtract, Minimum, Maximum,
		SrcMinusDst = Subtract,
		DstMinusSrc = ReverseSubtract,
	};

	struct BlendingEquation
	{
		BlendFactor SrcFactor = BlendFactor::One;
		BlendFactor DstFactor = BlendFactor::Zero;
		BlendOperation Operation = BlendOperation::Add;
	};

	struct BlendAttachment
	{
		BlendingEquation ColorEquation{};
		BlendingEquation AlphaEquation{};
		ColorWriteMask ColorWriteMask = ColorWriteMask::ColorWriteMaskAll;
		bool BlendEnable = false;
	};

	struct BlendConstants
	{
		float R = 0.0f;
		float G = 0.0f;
		float B = 0.0f;
		float A = 0.0f;
	};

	inline constexpr int MAX_COLOR_ATTACHMENTS = 8;
	struct PipelineBlendState
	{
		std::array<BlendAttachment, MAX_COLOR_ATTACHMENTS> BlendAttachments{};
		BlendConstants ConstantColor{};
	};

	OG_DEFINE_ENUM_FLAG_OPERATORS(ColorWriteMask);
#pragma endregion

#pragma region PipelinePolygonState
	enum class PolygonRasterMode : uint8_t
	{
		Fill,
		Line,
		Point
	};

	enum class CullingMode : uint8_t
	{
		Front,
		Back,
		FrontAndBack
	};

	enum class FrontFaceMode : uint8_t
	{
		CounterClockwise,
		Clockwise
	};

	struct PipelinePolygonState
	{
		PolygonRasterMode PolygonMode = PolygonRasterMode::Fill;
		CullingMode CullMode = CullingMode::Back;
		FrontFaceMode FrontFace = FrontFaceMode::CounterClockwise;
		bool CullEnable = true;
	};
#pragma endregion
}