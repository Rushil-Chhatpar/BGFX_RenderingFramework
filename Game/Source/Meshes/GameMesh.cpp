#include "GameMesh.h"
#include "Framework.h"
#include "DataTypes.h"
#include "Meshes/VertexFormats.h"

GameMesh::GameMesh(const bgfx::VertexLayout& vertexFormat, const char* filename)
{
    char* buffer = fw::LoadCompleteFile(filename, nullptr);
    assert(buffer);

    std::vector<vec3> normals;
    std::vector<vec2> textCoords;
    std::vector<vec3> verts;

    std::vector<VertexFormat_Pos3dUVNormal> vertices;

    std::vector<uint16> indices;

    char* nextToken = nullptr;
    char* line = strtok_s(buffer, "\n", &nextToken);
    while (line)
    {
        vec3 normal, vert;
        vec2 textCoord;

        if (sscanf_s(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z))
            normals.push_back(normal);

        if (sscanf_s(line, "vt %f %f", &textCoord.x, &textCoord.y))
            textCoords.push_back(textCoord);

        if (sscanf_s(line, "v %f %f %f", &vert.x, &vert.y, &vert.z))
            verts.push_back(vert);

        int v1, v2, v3, t1, t2, t3, n1, n2, n3;
        if (sscanf_s(line, "f %i/%i/%i %i/%i/%i %i/%i/%i", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3))
        {
            VertexFormat_Pos3dUVNormal temp;

            temp.pos = verts[v1 - 1];
            temp.uv = textCoords[t1 - 1];
            temp.normal = normals[n1 - 1];
            vertices.push_back(temp);

            temp.pos = verts[v2 - 1];
            temp.uv = textCoords[t2 - 1];
            temp.normal = normals[n2 - 1];
            vertices.push_back(temp);

            temp.pos = verts[v3 - 1];
            temp.uv = textCoords[t3 - 1];
            temp.normal = normals[n3 - 1];
            vertices.push_back(temp);
        }

        line = strtok_s(0, "\n", &nextToken);
    } 

    for (int i = 0; i < vertices.size(); i++)
    {
        indices.push_back(i);
    }

    Create(vertexFormat, vertices.data(), vertices.size() * sizeof(VertexFormat_Pos3dUVNormal), indices.data(), indices.size() * sizeof(uint16));


}
