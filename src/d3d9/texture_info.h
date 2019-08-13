#pragma once

#include <d3d9.h>
#include <string>
#include <vector>
#include "UMMath.h"
#include "UMMatrix.h"
#include "UMVector.h"

struct TextureInfo
{
	umbase::UMVec2i wh;
	D3DFORMAT format;
};

