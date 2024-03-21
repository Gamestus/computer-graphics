#include "AssimpMesh.h"
#include "PhysicsServer.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

AssimpMesh::AssimpMesh(std::string filepath)
	: AssimpMesh(filepath, nullptr) {}

AssimpMesh::AssimpMesh(std::string filepath, LPCWSTR texpath)
	: AssimpMesh(filepath, texpath, 1.0f) {}

AssimpMesh::AssimpMesh(std::string filepath, LPCWSTR texpath, float importScale, bool isCollision)
{
	IsCollision = isCollision;
	MeshFile = filepath;
	game = Game::Instance;
	if (isCollision)
	{
		game->PhysServer->RegisterMesh(this);
	}

	ShaderFile = L"./Shaders/ShaderTexture.hlsl";
	if (texpath != nullptr)
	{
		TextureFile = texpath;
	}

	Assimp::Importer importer;
	unsigned int flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs;
	const aiScene* pScene = importer.ReadFile(MeshFile, flags);
	
	const auto pMesh = pScene->mMeshes[0];

	// center mesh
	aiVector3D center(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < pMesh->mNumVertices; ++i) {
		center += pMesh->mVertices[i];
	}
	center /= pMesh->mNumVertices;

	for (unsigned int i = 0; i < pMesh->mNumVertices; ++i) {
		pMesh->mVertices[i] -= center;
	}


	std::vector<Vertex> vertices;
	vertices.reserve(pMesh->mNumVertices);

	bool hasUV = pMesh->HasTextureCoords(0);

	for (int i = 0; i < pMesh->mNumVertices; i++)
	{
		vertices.push_back(
			Vertex(pMesh->mVertices[i].x * importScale,
				pMesh->mVertices[i].y * importScale,
				pMesh->mVertices[i].z * importScale,
				1.0f,
				1.0f,
				1.0f,
				1.0f,
				1.0f,
				hasUV ? pMesh->mTextureCoords[0][i].x : 0.0f,
				hasUV ? pMesh->mTextureCoords[0][i].y : 0.0f
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
