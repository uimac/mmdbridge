#pragma once

#include <d3d9.h>
#include <string>
#include <vector>
#include <map>
#include "UMMath.h"

#include "texture_info.h"

typedef std::map<IDirect3DVertexBuffer9*, UINT > VertexBuffers;
typedef std::map<IDirect3DTexture9*, TextureInfo > TextureBuffers;
typedef std::map<DWORD, IDirect3DTexture9* > TextureSamplers;

struct RenderData {
	// all vertexbuffer
	VertexBuffers vertexBuffers;
	// all texture buffer
	TextureBuffers textureBuffers;
	// texture sampler
	TextureSamplers textureSamplers;
	// streamsource
	UINT streamNumber = 0;
	IDirect3DVertexBuffer9 *pStreamData = nullptr;
	UINT offsetInBytes = 0;
	UINT stride = 0;

	// index buffer
	IDirect3DIndexBuffer9 * pIndexData = nullptr;


	// fvf
	DWORD fvf = 0;
	bool pos = false;
	bool pos_xyz = false;
	bool pos_rhw = false;
	bool pos_xyzb[5] = { false, false, false, false, false };
	bool pos_last_beta_ubyte4 = false;

	bool normal = false;
	bool psize = false;
	bool diffuse = false;
	bool specular = false;

	int texcount = 0;

	RenderData() {}

	void dispose() {
		vertexBuffers.clear();
		textureBuffers.clear();
		textureSamplers.clear();
	}
};
