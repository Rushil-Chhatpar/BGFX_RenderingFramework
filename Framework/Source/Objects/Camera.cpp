//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
    
#include "CoreHeaders.h"

#include "Camera.h"
#include "FWCore.h"
#include "GameCore.h"
#include "Renderer/Uniforms.h"

namespace fw {

Camera::Camera(GameCore* pGameCore, vec3 pos)
    : GameObject( pGameCore, "Camera"/*, pos, nullptr, nullptr*/ )
    , m_Position(pos)
{
    FWCore* pFW = pGameCore->GetFramework();
    SetAspectRatio( (float)pFW->GetWindowClientWidth()/pFW->GetWindowClientHeight() );
}

Camera::~Camera()
{
}

void Camera::Update(float32 deltaTime)
{
}

void Camera::Enable(int viewID)
{
    Uniforms* pUniforms = m_pGameCore->GetUniforms();
    vec2 scaleWithAspectRatio = m_ProjectionScale * vec2( 1.0f/m_AspectRatio, 1 );
    
    mat4 viewMatrix;
    vec3 eye( 0, 4, -10 );
    vec3 up( 0, 1, 0 );
    vec3 at( 0, 0, -5 );
    viewMatrix.CreateLookAtView( eye, up, at );

    mat4 projMatrix;
    //projMatrix.CreateOrtho( -5, 5, -5, 5, 0, 100 );
    projMatrix.CreatePerspectiveVFoV( 45, m_AspectRatio, 0.01f, 100.0f );

    // Setup uniforms.
    bgfx::setUniform( pUniforms->GetUniform("u_ViewMatrix"), &viewMatrix );
    bgfx::setUniform( pUniforms->GetUniform("u_ProjMatrix"), &projMatrix );
    bgfx::setUniform( pUniforms->GetUniform("u_CameraPosition"), &m_Position, 1 );
    bgfx::setUniform( pUniforms->GetUniform("u_ProjectionScale"), &scaleWithAspectRatio, 1 );
}

} // namespace fw
