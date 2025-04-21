#pragma once

// Core elements
#include "OpenEngine/Core/Application.h"
#include "OpenEngine/Core/Build.h"
#include "OpenEngine/Core/Logger.h"

// Maths
#include "OpenEngine/Math/Mathf.h"
#include "OpenEngine/Math/Vector2D.h"
#include "OpenEngine/Math/Vector3D.h"
#include "OpenEngine/Math/Vector4D.h"
#include "OpenEngine/Math/Color.h"
#include "OpenEngine/Math/Color32.h"
#include "OpenEngine/Math/Random.h"

// Rendering
#include "OpenEngine/Render/Buffer.h"
#include "OpenEngine/Render/VertexArray.h"
#include "OpenEngine/Render/Shader.h"
#include "OpenEngine/Render/Texture.h"
#include "OpenEngine/Render/RenderCommand.h"
#include "OpenEngine/Render/RenderEntity.h"
#include "OpenEngine/Render/RenderCamera.h"
#include "OpenEngine/Render/RenderSystem.h"

// Asset Loading
#include "OpenEngine/Asset/TextureImporter.h"

// Elements
#include "OpenEngine/Elements/VectorGizmo.h"
#include "OpenEngine/Elements/PointGizmo.h"
#include "OpenEngine/Elements/Model.h"

// Window system
#include "OpenEngine/Windowing/WindowSystem.h"

// Entry point
#ifdef OG_ENTRY_POINT
#include "OpenEngine/Core/EntryPoint.h"
#endif