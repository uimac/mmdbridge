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
};
