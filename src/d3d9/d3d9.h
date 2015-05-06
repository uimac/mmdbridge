
#pragma once

#ifndef CINTERFACE
#define CINTERFACE
#endif //CINTERFACE

#pragma comment(lib, "d3d9.lib")
#pragma warning(disable : 4819)
#include <d3d9.h>
#include <d3dx9shader.h>
#include <d3d9types.h>
#include <d3dx9tex.h>
#pragma warning(default : 4819)

#include <vector>
#include <map>

#include "UMMathTypes.h"
#include "UMMath.h"
#include "UMVector.h"

typedef struct TextureInfo
{
	UMVec2i wh;
	D3DFORMAT format;
} TextureInfo;


typedef std::map<IDirect3DVertexBuffer9*, UINT > VertexBuffers;
typedef std::map<IDirect3DTexture9*, TextureInfo > TextureBuffers;
typedef std::map<DWORD, IDirect3DTexture9* > TextureSamplers;

typedef struct RenderData {
	// all vertexbuffer
	VertexBuffers vertexBuffers;
	// all texture buffer
	TextureBuffers textureBuffers;
	// texture sampler
	TextureSamplers textureSamplers;
	// streamsource
	UINT streamNumber;
	IDirect3DVertexBuffer9 *pStreamData;
	UINT offsetInBytes;
	UINT stride;

	// index buffer
	IDirect3DIndexBuffer9 * pIndexData;


	// fvf
	DWORD fvf;
	bool pos;
	bool pos_xyz;
	bool pos_rhw;
	bool pos_xyzb[5];
	bool pos_last_beta_ubyte4;

	bool normal;
	bool psize;
	bool diffuse;
	bool specular;

	int texcount;

	RenderData() {
		streamNumber = 0;
		pStreamData = NULL;
		offsetInBytes = 0;
		stride = 0;

		pIndexData = NULL;

		fvf = 0;
	    pos
		= pos_xyz
	    = pos_rhw
		= normal
	    = psize
	    = diffuse 
	    = specular
		= false;
	    
		for (int i = 0; i < 5; ++i) {
			pos_xyzb[i] = false;
		}
		pos_last_beta_ubyte4 = false;
		texcount = 0;
	}
	void dispose() {
		vertexBuffers.clear();
		textureBuffers.clear();
		textureSamplers.clear();
	}
} RenderData;

typedef struct RenderedSurface {
	std::vector<UMVec3i> faces;
} RenderedSurface;

typedef struct RenderedMaterial {
    UMVec4f   diffuse;        /* Diffuse color RGBA */
    UMVec3f   ambient;        /* Ambient color RGB */
    UMVec3f   specular;       /* Specular 'shininess' */
    UMVec3f   emissive;       /* Emissive color RGB */
    float        power; 
	IDirect3DTexture9 *tex;
	std::string texture;
	std::string memoryTexture;
	RenderedSurface surface;
} RenderedMaterial;

typedef struct RenderedBuffer {
	typedef std::vector<UMVec2f>		UVList;
	typedef std::vector<D3DXVECTOR3>	VertexList;
	typedef std::vector<D3DXVECTOR3>	NormalList;
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
	UMVec3f light;
	UMVec3f light_color;
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
	D3DXMATRIX world_inv;
} RenderedBuffer;


typedef struct RenderedTexture {
	UMVec2i size;
	std::string name;
	std::vector<UMVec4f> texture;
} RenderedTexture;


struct TextureParameter
{
	TextureParameter():
		texture(NULL), 
		hasTextureSampler0(FALSE),
		hasTextureSampler1(FALSE),
		hasTextureSampler2(FALSE),
		hasAlphaTexture(FALSE) {}

	IDirect3DTexture9* texture;
	bool hasTextureSampler0;
	bool hasTextureSampler1;
	bool hasTextureSampler2;
	bool hasAlphaTexture;
	std::string textureName;
	std::string textureMemoryName;
};

typedef std::vector<IDirect3DVertexBuffer9* > VertexBufferList;
typedef std::map<IDirect3DVertexBuffer9*, RenderedBuffer> RenderBufferMap;
