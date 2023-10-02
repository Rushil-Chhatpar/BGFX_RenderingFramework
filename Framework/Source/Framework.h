//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

// This header includes all files inside the framework project.
// It is meant for use by game projects ONLY.
// As new files are created, include them here so the game has easy access to them.
// It should not be included by any files inside the framework project.

#include "CoreHeaders.h"

#include "Math/Vector.h"
#include "Imgui/ImGuiConfig.h"
#include "../Libraries/imgui/imgui.h"
#include "../Libraries/nlohmann/json.hpp"

#include "bgfx/platform.h"

#include "FWCore.h"
#include "GameCore.h"
#include "EventSystem/Events.h"
#include "EventSystem/EventManager.h"
#include "Imgui/ImGuiManager.h"
#include "Math/MathHelpers.h"
#include "Math/Matrix.h"
#include "Math/Random.h"
#include "Objects/Camera.h"
#include "Objects/GameObject.h"
#include "Components/BaseComponent.h"
#include "Components/Components.h"
#include "Components/ComponentManager.h"
#include "Renderer/Mesh.h"
#include "Renderer/Material.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture.h"
#include "Renderer/Uniforms.h"
#include "Utility/Utility.h"
#include "Contact/Box2DContactListener.h"
#include "ResourceManager/ResourceManager.h"
#include "Scene/Scene.h"