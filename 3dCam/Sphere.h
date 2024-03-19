#pragma once
#include <DirectXMath.h>


class Sphere
{
public:
    static std::vector<DirectX::XMFLOAT4> GenerateSphereVertices(float radius, int slices, int stacks)
    {
        std::vector<DirectX::XMFLOAT4> vertices;

        for (int i = 0; i <= stacks; ++i)
        {
            float phi = DirectX::XM_PI * static_cast<float>(i) / stacks;

            for (int j = 0; j <= slices; ++j)
            {
                float theta = 2.0f * DirectX::XM_PI * static_cast<float>(j) / slices;

                float x = radius * sinf(phi) * cosf(theta);
                float y = radius * cosf(phi);
                float z = radius * sinf(phi) * sinf(theta);

                vertices.emplace_back(x, y, z, 1.0f);
            }
        }

        return vertices;
    }

    static std::vector<int> GenerateSphereIndices(int slices, int stacks)
    {
        std::vector<int> indices;

        for (int i = 0; i < stacks; ++i)
        {
            for (int j = 0; j < slices; ++j)
            {
                int currentIndex = i * (slices + 1) + j;

                indices.push_back(currentIndex);
                indices.push_back(currentIndex + 1);
                indices.push_back(currentIndex + slices + 1);

                indices.push_back(currentIndex + 1);
                indices.push_back(currentIndex + slices + 2);
                indices.push_back(currentIndex + slices + 1);
            }
        }

        return indices;
    }
};