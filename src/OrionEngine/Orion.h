#pragma once

// For use by Orion applications.
// Umbrella header: every include is re-exported to the client (IWYU tools must not flag them as unused).
// IWYU pragma: begin_exports

#include "Orion/Core/Application.h"
#include "Orion/Core/Base.h"
#include "Orion/Core/Input.h"
#include "Orion/Core/KeyCodes.h"
#include "Orion/Core/Layer.h"
#include "Orion/Core/MouseCodes.h"
#include "Orion/Core/Timestep.h"
#include "Orion/ImGui/ImGuiLayer.h"

// --- File System ------------
#include "Orion/Core/FileSystem/DiskMountSource.h"
#include "Orion/Core/FileSystem/FileSystem.h"

// --- Renderer ------------
#include "Orion/Renderer/AssimpMeshImporter.h"
#include "Orion/Renderer/Buffer.h"
#include "Orion/Renderer/Camera.h"
#include "Orion/Renderer/FlyCamera.h"
#include "Orion/Renderer/MeshRenderData.h"
#include "Orion/Renderer/MeshSource.h"
#include "Orion/Renderer/RenderCommand.h"
#include "Orion/Renderer/Renderer.h"
#include "Orion/Renderer/Renderer2D.h"
#include "Orion/Renderer/Shader.h"
#include "Orion/Renderer/Texture.h"
#include "Orion/Renderer/VertexArray.h"
// -------------------------

// IWYU pragma: end_exports
