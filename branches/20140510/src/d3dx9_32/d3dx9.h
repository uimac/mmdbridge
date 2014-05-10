
#ifndef D3DX9_H
#define D3DX9_H

#ifndef CINTERFACE
#define CINTERFACE
#endif //CINTERFACE

#pragma comment(lib, "d3dx9.lib")

#include <d3dx9shader.h>
#include <d3dx9math.h>
#include <d3dx9mesh.h>
#include <d3dx9anim.h>
#include <d3dx9shape.h>
#include <d3dx9tex.h>
#include <d3dx9effect.h>
#include <d3dx9core.h>
#include <d3dx9xof.h>

extern "C" {
    void UMSetFlag(int flag);
    LPD3DXEFFECT* UMGetEffect();
    void UMSync();
    int UMGetEffectType();
    void UMGetCameraFovLH(D3DXVECTOR4 *dst);
    void UMGetCameraEye(D3DXVECTOR3 *dst);
    void UMGetCameraAt(D3DXVECTOR3 *dst);
    void UMGetCameraUp(D3DXVECTOR3 *dst);
    BOOL UMCopyTexture(LPCWSTR dstDir, LPDIRECT3DTEXTURE9 tex);
    LPWSTR UMGetTextureName(LPDIRECT3DTEXTURE9 tex);
    BOOL UMIsAlphaTexture(LPDIRECT3DTEXTURE9 tex);
}

#define EgColor	(1 << 0)
#define ToonColor (1 << 1)
#define LightColor (1 << 2)
#define SpcColor (1 << 3)
#define DifColor (1 << 4)
#define LightDir (1 << 5)

#endif //D3DX9_H