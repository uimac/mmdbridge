#pragma once

#pragma warning(disable : 4819)
#include <d3d9.h>
#include <d3dx9shader.h>
#include <d3d9types.h>
#include <d3dx9tex.h>
#pragma warning(default : 4819)

bool d3d9_initialize(HINSTANCE hInst, DWORD reason, LPVOID);

bool d3d9_dispose();
