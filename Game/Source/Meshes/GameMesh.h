#pragma once

#include "../../Framework/Source/Renderer/Mesh.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"

class GameMesh : public fw::Mesh
{
public:
    GameMesh(const bgfx::VertexLayout& vertexFormat, const char* filename);

protected:
};