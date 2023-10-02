#include "GameComponents.h"
#include "Components/Components.h"
#include "Meshes/VertexFormats.h"
#include "Math/Random.h"
#include "Objects/Controller.h"
#include "stb/stb_image.h"


void HeightMapComponent::CreateHeightMap(const char* filename)
{
    //texture stuff
    uint32 length;
    char* fileContents = fw::LoadCompleteFile(filename, &length);

    int width;
    int height;
    int channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixels = stbi_load_from_memory((unsigned char*)fileContents, length, &width, &height, &channels, 1);
    assert(pixels != nullptr);

    m_VertScale.x = width;
    m_VertScale.y = height;

    fw::Transform* transform = m_pGameObject->GetComponent<fw::Transform>();

    vec3 startPos;
    fw::Render* render = m_pGameObject->GetComponent<fw::Render>();

    m_Verts = new VertexFormat_Pos3dUVNormal[m_VertScale.x * m_VertScale.y];

    float xOffset = m_WorldScale.x / (m_VertScale.x - 1);
    float zOffset = m_WorldScale.y / (m_VertScale.y - 1);

    int c = 0;

    for (int j = 0; j < m_VertScale.y; j++)
    {
        for (int i = 0; i < m_VertScale.x; i++)
        {
            float xpos = startPos.x + (i * xOffset);
            float zpos = startPos.z + (j * zOffset);
            float u = (1.0f / (m_VertScale.x - 1)) * i;
            float v = (1.0f / (m_VertScale.y - 1)) * j;
            int test = pixels[c];
            float ypos = (float)(test / 255.0f) * 15.0f;
            //ypos = 0.0f;
            c++;
            //fw::Random::Generator rand;
            //float ypos = rand.Float(-0.05f, 0.05f);
            m_Verts[i + (j * m_VertScale.x)] = { vec3(xpos, ypos, zpos), vec2(u,v) };
        }
    }


    int indSize = 6 * (m_VertScale.x - 1) * (m_VertScale.y - 1);
    g_Indices = new uint16[indSize];

    c = 0;
    for (int j = 0; j < m_VertScale.y - 1; j++)
    {
        for (int i = 0; i < m_VertScale.x - 1; i++)
        {
            g_Indices[c] = ((i)+((j)*m_VertScale.x));
            c++;
            g_Indices[c] = ((i + 1) + ((j + 1) * m_VertScale.x));
            c++;
            g_Indices[c] = ((i + 1) + ((j)*m_VertScale.x));
            c++;
            g_Indices[c] = ((i)+((j)*m_VertScale.x));
            c++;
            g_Indices[c] = ((i)+((j + 1) * m_VertScale.x));
            c++;
            g_Indices[c] = ((i + 1) + ((j + 1) * m_VertScale.x));
            c++;

        }
    }

    //calculate normals

    for (int y = 0; y < m_VertScale.y; y++)
    {
        for (int x = 0; x < m_VertScale.x; x++)
        {
            int index = x + (y * m_VertScale.x);//stride

            vec3 normal = vec3(0, 1, 0);

            if (x > 0 && x < (m_VertScale.x - 1) && y > 0 && y < (m_VertScale.y - 1))
            {
                vec3 v0 = m_Verts[index].pos;
                vec3 v1 = m_Verts[index - 1].pos;
                vec3 v2 = m_Verts[index + 1].pos;
                vec3 v3 = m_Verts[index - m_VertScale.x].pos;
                vec3 v4 = m_Verts[index + m_VertScale.x].pos;

                vec3 left = v1 - v0;
                vec3 right = v2 - v0;
                vec3 down = v3 - v0;
                vec3 up = v4 - v0;

                vec3 tr = up.Cross(right).GetNormalized();
                vec3 br = right.Cross(down).GetNormalized();
                vec3 bl = down.Cross(left).GetNormalized();
                vec3 tl = left.Cross(up).GetNormalized();

                normal = (tr + br + bl + tl).GetNormalized();


            }

            m_Verts[index].normal = normal;
        }
    }



    ///////////////////////////

    m_pMesh = new fw::Mesh(VertexFormat_Pos3dUVNormal::format, m_Verts, sizeof(VertexFormat_Pos3dUVNormal) * m_VertScale.x * m_VertScale.y, g_Indices, sizeof(uint16) * indSize);
}

void HeightMapComponent::Rebuild()
{
    delete m_pMesh;
    int indSize = 6 * (m_VertScale.x - 1) * (m_VertScale.y - 1);
    m_pMesh = new fw::Mesh(VertexFormat_Pos3dUVNormal::format, m_Verts, sizeof(VertexFormat_Pos3dUVNormal) * m_VertScale.x * m_VertScale.y, g_Indices, sizeof(uint16) * indSize);
}

float HeightMapComponent::GetHeightAtXZ(float x, float z)
{
    float height = 0;

    // Convert x,z to heightmap space. Only required if your heightmap is centered.
    //x += m_Size.x / 2;
    //z += m_Size.y / 2;

    // Convert x,z to heightmap indices.
    float xTileSize = m_WorldScale.x / (m_VertScale.x - 1);
    float zTileSize = m_WorldScale.y / (m_VertScale.y - 1);
    float xIndexFloat = x / xTileSize;
    float zIndexFloat = z / zTileSize;
    int xIndex = (int)xIndexFloat;
    int zIndex = (int)zIndexFloat;
    float xPercentInQuad = xIndexFloat - xIndex;
    float zPercentInQuad = zIndexFloat - zIndex;

    // Get the 4 vertices that make up the quad.
    int stride = m_VertScale.x;
    int index = zIndex * stride + xIndex;
    vec3 bottomLeft = m_Verts[index].pos;
    vec3 bottomRight = m_Verts[index + 1].pos;
    vec3 topLeft = m_Verts[index + stride].pos;
    vec3 topRight = m_Verts[index + stride + 1].pos;

    // Get the height at x,z.
    vec3 v1, v2, v3;
    if (xPercentInQuad < zPercentInQuad)
    {
        // Top left triangle.
        v1 = vec3(0, bottomLeft.y, 0);
        v2 = vec3(0, topLeft.y, 1);
        v3 = vec3(1, topRight.y, 1);
    }
    else
    {
        // Bottom right triangle.
        v1 = vec3(0, bottomLeft.y, 0);
        v2 = vec3(1, topRight.y, 1);
        v3 = vec3(1, bottomRight.y, 0);
    }

    // Barycentric interpolation.
    vec2 v4 = vec2(xPercentInQuad, zPercentInQuad);
    float areaTriangle = (v2.XZ() - v1.XZ()).Cross(v3.XZ() - v1.XZ());
    float area1 = (v4 - v3.XZ()).Cross(v2.XZ() - v3.XZ());
    float area2 = (v4 - v1.XZ()).Cross(v3.XZ() - v1.XZ());

    float perc1 = area1 / areaTriangle;
    float perc2 = area2 / areaTriangle;
    float perc3 = 1.0f - perc1 - perc2;

    height = perc1 * v1.y + perc2 * v2.y + perc3 * v3.y;

    return height;
}

ivec2 HeightMapComponent::GetVertIndexAtXZ(float x, float z)
{
    // Convert x,z to heightmap indices.
    float xTileSize = m_WorldScale.x / (m_VertScale.x - 1);
    float zTileSize = m_WorldScale.y / (m_VertScale.y - 1);
    float xIndexFloat = x / xTileSize;
    float zIndexFloat = z / zTileSize;
    int xIndex = (int)xIndexFloat;
    int zIndex = (int)zIndexFloat;

    return ivec2(xIndex, zIndex);
}



// Player Logic Component ############################################################

void PlayerLogicComponent::AddtoInspector()
{
}

void PlayerLogicComponent::SaveToJSON(nlohmann::json& json)
{
}

void PlayerLogicComponent::LoadFromJSON(nlohmann::json& json)
{
}

void PlayerLogicComponent::Update(float deltaTime)
{
    float speed = 5.0f;
    float angularSpeed = 90.0f;

    float updir = 0.0f;
    float rot = 0.0f;
    if (m_pController->IsHeld(Controller::Mask::Up))
        updir += 1.0f;
    else if (m_pController->IsHeld(Controller::Mask::Down))
        updir -= 1.0f;

    if (m_pController->IsHeld(Controller::Mask::Left))
        rot += 1.0f;
    else if (m_pController->IsHeld(Controller::Mask::Right))
        rot -= 1.0f;

    fw::Transform* pTransform = m_pGameObject->GetComponent<fw::Transform>();

    pTransform->m_Position += m_pGameObject->GetForwardVector() * updir * speed * deltaTime;
    pTransform->m_Rotation.y += rot * deltaTime * angularSpeed;
    


    //Set y pos on height map

    float ypos = m_pHeightMap->GetHeightAtXZ(pTransform->m_Position.x, pTransform->m_Position.z);
    pTransform->m_Position.y = ypos;


    //Raising the heightmap

    /*
    * 
FOR FLAT
- make a rebuild function with the mesh function Create
- inside the loop check if the index is within the radius
- inside the loop get the verts vertices and change the y pos of the verts inside the radius

FOR ROUND
- measure the distance from the center and multiply it my an exponent(figure that shit out later with Jimmy)
    */

    float radius = 10.0f;

    if (m_pController->WasPressed(Controller::Mask::Raise) || m_pController->WasPressed(Controller::Mask::Lower))
    {
        ivec2 indexXZ = m_pHeightMap->GetVertIndexAtXZ(pTransform->m_Position.x, pTransform->m_Position.z);
        float xRange = pTransform->m_Position.x - radius;
        float zRange = pTransform->m_Position.z - radius;

        ivec2 indexBound = m_pHeightMap->GetVertIndexAtXZ(xRange, zRange);

        for (int j = indexBound.y; j < (indexXZ.y - indexBound.y) + indexXZ.y; j++)
        {
            for (int i = indexBound.x; i < (indexXZ.x - indexBound.x) + indexXZ.x; i++)
            {
                if (i >= 0 && i < m_pHeightMap->m_VertScale.x && j >= 0 && j < m_pHeightMap->m_VertScale.y)
                {
                    vec2 center = vec2(pTransform->m_Position.x, pTransform->m_Position.z);
                    vec2 point = vec2(m_pHeightMap->m_Verts[i + (j * m_pHeightMap->m_VertScale.x)].pos.x, m_pHeightMap->m_Verts[i + (j * m_pHeightMap->m_VertScale.x)].pos.z);
                    vec2 diff = point - center;
                    if (diff.Length() <= radius)
                    {
                        float factor = 1 / diff.Length();
                        float distFactor = diff.Length() / radius;
                        factor = (cos(distFactor * PI / 2.0f)) / 2.0f;
                        //factor += distFactor;
                        if (factor >= 1.0f)
                            factor = 1.0f;
                        if (m_pController->WasPressed(Controller::Mask::Raise))
                            m_pHeightMap->m_Verts[i + (j * m_pHeightMap->m_VertScale.x)].pos.y += (5.0f * factor);
                        if (m_pController->WasPressed(Controller::Mask::Lower))
                            m_pHeightMap->m_Verts[i + (j * m_pHeightMap->m_VertScale.x)].pos.y -= (5.0f * factor);
                    }
                }
            }
        }

        m_pHeightMap->Rebuild();


    }

    //if (m_pController->WasPressed(Controller::Mask::Lower))
    //{
    //    ivec2 indexXZ = m_pHeightMap->GetVertIndexAtXZ(pTransform->m_Position.x, pTransform->m_Position.z);
    //    float xRange = pTransform->m_Position.x - radius;
    //    float zRange = pTransform->m_Position.z - radius;

    //    ivec2 indexBound = m_pHeightMap->GetVertIndexAtXZ(xRange, zRange);

    //    for (int j = indexBound.y; j < (indexXZ.y - indexBound.y) + indexXZ.y; j++)
    //    {
    //        for (int i = indexBound.x; i < (indexXZ.x - indexBound.x) + indexXZ.x; i++)
    //        {
    //            if (i >= 0 && i < m_pHeightMap->m_VertScale.x && j >= 0 && j < m_pHeightMap->m_VertScale.y)
    //            {
    //                vec2 center = vec2(pTransform->m_Position.x, pTransform->m_Position.z);
    //                vec2 point = vec2(m_pHeightMap->m_Verts[i + (j * m_pHeightMap->m_VertScale.x)].pos.x, m_pHeightMap->m_Verts[i + (j * m_pHeightMap->m_VertScale.x)].pos.z);
    //                vec2 diff = point - center;
    //                if (diff.Length() <= radius)
    //                {
    //                    float factor = 1 / diff.Length();
    //                    float distFactor = diff.Length() / radius;
    //                    //factor += distFactor;
    //                    if (factor >= 1.0f)
    //                        factor = 1.0f;
    //                    m_pHeightMap->m_Verts[i + (j * m_pHeightMap->m_VertScale.x)].pos.y -= (5.0f * factor);
    //                }
    //            }
    //        }
    //    }

    //    m_pHeightMap->Rebuild();


    //}

}

// Orbit Camera Component ############################################################

void OrbitCameraComponent::AddtoInspector()
{
    ImGui::DragFloat3("Eye Pos", &m_Eye.x);
    ImGui::DragFloat("Arm length", &m_TargetArmLength);
}

void OrbitCameraComponent::SaveToJSON(nlohmann::json& json)
{
}

void OrbitCameraComponent::LoadFromJSON(nlohmann::json& json)
{
}

void OrbitCameraComponent::UpdateCameraView(float deltaTime)
{
    //Set pos to target obj
    m_pGameObject->SetPosition(m_TargetObject->GetPosition());

    //Perform input rotation
    float rot = 0.0f;
    float rotx = 0.0f;
    float angularSpeed = 90.0f;

    if (m_pController->IsHeld(Controller::Mask::RotUp))
        rotx -= 1.0f;
    else if (m_pController->IsHeld(Controller::Mask::RotDown))
        rotx += 1.0f;

    if (m_pController->IsHeld(Controller::Mask::RotLeft))
        rot -= 1.0f;
    else if (m_pController->IsHeld(Controller::Mask::RotRight))
        rot += 1.0f;

    if (m_pController->IsHeld(Controller::Mask::ZoomIn))
        m_TargetArmLength -= deltaTime * 10.0f;
    else if (m_pController->IsHeld(Controller::Mask::ZoomOut))
        m_TargetArmLength += deltaTime * 10.0f;

    fw::Transform* pTransform = m_pGameObject->GetComponent<fw::Transform>();
    pTransform->m_Rotation.y += rot * deltaTime * angularSpeed;
    pTransform->m_Rotation.x += rotx * deltaTime * angularSpeed;


    //Set View
    vec3 forward = m_pGameObject->GetForwardVector();

    float sx = sin(pTransform->m_Rotation.x / 180 * PI);
    float sy = sin(pTransform->m_Rotation.y / 180 * PI);
    float cx = cos(pTransform->m_Rotation.x / 180 * PI);
    float cy = cos(pTransform->m_Rotation.y / 180 * PI);
    vec3 offset = vec3(cx * sy * m_TargetArmLength, -sx * m_TargetArmLength, -1 * cx * cy * m_TargetArmLength);

    //m_Eye = m_pGameObject->GetPosition() - (forward * m_TargetArmLength);
    // 
    //m_Eye.x = cos(pTransform->m_Rotation.y / 180 * PI);
    //m_Eye.z = sin(pTransform->m_Rotation.y / 180 * PI);
    //m_Eye += vec3(0, m_yOffset, 0);

    m_Eye = m_TargetObject->GetPosition() + offset;

    //m_Eye.y = m_yOffset;

    m_Up = vec3(0, 1, 0);

    m_At = m_TargetObject->GetPosition();

    fw::Uniforms* pUniforms = m_pGameObject->GetGameCore()->GetUniforms();

    bgfx::setUniform(pUniforms->GetUniform("u_CameraEyePosition"), &m_Eye);

    mat4 viewMatrix;
    viewMatrix.CreateLookAtView(m_Eye, m_Up, m_At);
    bgfx::setUniform(pUniforms->GetUniform("u_ViewMatrix"), &viewMatrix);

}

void WaterComponent::CreateWater(ivec2 vertScale)
{
    m_VertScale = vertScale;

    fw::Transform* transform = m_pGameObject->GetComponent<fw::Transform>();

    vec3 startPos;
    fw::Render* render = m_pGameObject->GetComponent<fw::Render>();

    m_Verts = new VertexFormat_Pos3dUVNormal[m_VertScale.x * m_VertScale.y];

    float xOffset = m_WorldScale.x / (m_VertScale.x - 1);
    float zOffset = m_WorldScale.y / (m_VertScale.y - 1);


    for (int j = 0; j < m_VertScale.y; j++)
    {
        for (int i = 0; i < m_VertScale.x; i++)
        {
            float xpos = startPos.x + (i * xOffset);
            float zpos = startPos.z + (j * zOffset);
            float u = (1.0f / (m_VertScale.x - 1)) * i;
            float v = (1.0f / (m_VertScale.y - 1)) * j;

            //fw::Random::Generator rand;
            //float ypos = rand.Float(-0.05f, 0.05f);
            m_Verts[i + (j * m_VertScale.x)] = { vec3(xpos, 0.0f, zpos), vec2(u,v) };
        }
    }


    int indSize = 6 * (m_VertScale.x - 1) * (m_VertScale.y - 1);
    g_Indices = new uint16[indSize];

    int c = 0;
    for (int j = 0; j < m_VertScale.y - 1; j++)
    {
        for (int i = 0; i < m_VertScale.x - 1; i++)
        {
            g_Indices[c] = ((i)+((j)*m_VertScale.x));
            c++;
            g_Indices[c] = ((i + 1) + ((j + 1) * m_VertScale.x));
            c++;
            g_Indices[c] = ((i + 1) + ((j)*m_VertScale.x));
            c++;
            g_Indices[c] = ((i)+((j)*m_VertScale.x));
            c++;
            g_Indices[c] = ((i)+((j + 1) * m_VertScale.x));
            c++;
            g_Indices[c] = ((i + 1) + ((j + 1) * m_VertScale.x));
            c++;

        }
    }

    //calculate normals

    for (int y = 0; y < m_VertScale.y; y++)
    {
        for (int x = 0; x < m_VertScale.x; x++)
        {
            int index = x + (y * m_VertScale.x);//stride

            vec3 normal = vec3(0, 1, 0);

            if (x > 0 && x < (m_VertScale.x - 1) && y > 0 && y < (m_VertScale.y - 1))
            {
                vec3 v0 = m_Verts[index].pos;
                vec3 v1 = m_Verts[index - 1].pos;
                vec3 v2 = m_Verts[index + 1].pos;
                vec3 v3 = m_Verts[index - m_VertScale.x].pos;
                vec3 v4 = m_Verts[index + m_VertScale.x].pos;

                vec3 left = v1 - v0;
                vec3 right = v2 - v0;
                vec3 down = v3 - v0;
                vec3 up = v4 - v0;

                vec3 tr = up.Cross(right).GetNormalized();
                vec3 br = right.Cross(down).GetNormalized();
                vec3 bl = down.Cross(left).GetNormalized();
                vec3 tl = left.Cross(up).GetNormalized();

                normal = (tr + br + bl + tl).GetNormalized();


            }

            m_Verts[index].normal = normal;
        }
    }

    m_pMesh = new fw::Mesh(VertexFormat_Pos3dUVNormal::format, m_Verts, sizeof(VertexFormat_Pos3dUVNormal) * m_VertScale.x * m_VertScale.y, g_Indices, sizeof(uint16) * indSize);
}

void LightComponent::AddtoInspector()
{
}

void LightComponent::SaveToJSON(nlohmann::json& json)
{
}

void LightComponent::LoadFromJSON(nlohmann::json& json)
{
}
