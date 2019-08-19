#pragma once

#include <d3d9.h>
#include <string>
#include <vector>
#include "UMMath.h"
#include "UMMatrix.h"
#include "UMVector.h"

struct RenderedSurface {
	std::vector<umbase::UMVec3i> faces;
};

struct RenderedMaterial {
	umbase::UMVec4f   diffuse;        /* Diffuse color RGBA */
	umbase::UMVec3f   ambient;        /* Ambient color RGB */
	umbase::UMVec3f   specular;       /* Specular 'shininess' */
	umbase::UMVec3f   emissive;       /* Emissive color RGB */
	float        power;
	IDirect3DTexture9 *tex;
	std::string texture;
	std::string memoryTexture;
	RenderedSurface surface;
};

struct RenderedBuffer {
	typedef std::vector<umbase::UMVec2f>		UVList;
	typedef std::vector<umbase::UMVec3f>	VertexList;
	typedef std::vector<umbase::UMVec3f>	NormalList;
	typedef	std::vector<DWORD>			DiffuseList;
	typedef std::vector<RenderedMaterial* >		MaterialList;
	typedef std::map<int, RenderedMaterial* >	MaterialMap;

	VertexList		vertecies;
	NormalList		normals;
	UVList			uvs;
	DiffuseList		diffuses;
	MaterialList	materials;
	MaterialMap		material_map;

	bool isAccessory;
	int accessory;

	virtual ~RenderedBuffer()
	{
		//for (size_t i = 0; i < materials.size(); ++i) {
		//	if (materials[i]) { delete materials[i]; }
		//}
	}
	umbase::UMVec3f light;
	umbase::UMVec3f light_color;
	umbase::UMMat44f world;
	umbase::UMMat44f view;
	umbase::UMMat44f projection;
	umbase::UMMat44f world_inv;
};

struct RenderedTexture {
	umbase::UMVec2i size;
	std::string name;
	std::vector<umbase::UMVec4f> texture;
};

typedef std::vector<IDirect3DVertexBuffer9* > VertexBufferList;
typedef std::map<IDirect3DVertexBuffer9*, RenderedBuffer> RenderBufferMap;
