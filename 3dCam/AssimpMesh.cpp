#include "AssimpMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

AssimpMesh::AssimpMesh(std::string filepath)
	: AssimpMesh(filepath, nullptr) {}

AssimpMesh::AssimpMesh(std::string filepath, LPCWSTR texpath)
	: AssimpMesh(filepath, nullptr, 1.0f) {}


AssimpMesh::AssimpMesh(std::string filepath, LPCWSTR texpath, float importScale)
{
	ShaderFile = L"./Shaders/ShaderTexture.hlsl";
	if (texpath != nullptr)
	{
		TextureFile = texpath;
	}

	Assimp::Importer importer;
	unsigned int flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;
	const aiScene* pScene = importer.ReadFile(filepath, flags);
	
	const auto pMesh = pScene->mMeshes[0];

	std::vector<Vertex> vertices;
	vertices.reserve(pMesh->mNumVertices);

	bool hasUV = pMesh->HasTextureCoords(0);

	for (int i = 0; i < pMesh->mNumVertices; i++)
	{
		vertices.push_back(
			Vertex(pMesh->mVertices[i].x,
				pMesh->mVertices[i].y,
				pMesh->mVertices[i].z,
				1.0f,
				1.0f,
				1.0f,
				1.0f,
				1.0f,
				hasUV ? pMesh->mTextureCoords[0][i].x : 0.0f,
				hasUV ? 1.0f - pMesh->mTextureCoords[0][i].y : 0.0f
				)
			);
	}

	std::vector<int> indices;

	indices.reserve(pMesh->mNumFaces * 3);

	for (int i = 0; i < pMesh->mNumFaces; i++)
	{
		const auto& face = pMesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	points = vertices;
	Initialize(indices);
}
