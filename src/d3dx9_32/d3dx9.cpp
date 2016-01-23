// このファイルはpythonで適当に作ったファイルを適当に書き換えたものです

#define CINTERFACE

#include "d3dx9.h"

#include <windows.h>
#include <string>
#include <sstream>
#include <tchar.h>
#include <fstream>
#include <algorithm>
#include <shlwapi.h>
#include <vector>
#include <map>

#include <Python.h>

template <class T> std::string to_string(T value)
{
	std::stringstream converter;
	std::string  str;
	converter << value;
	converter >> str;
	return str;
}

HRESULT (WINAPI *original_SetFloatArray)(
	ID3DXEffect* pEffect,
	D3DXHANDLE hParameter,
	CONST FLOAT* pf,
	UINT Count)(NULL);

D3DXHANDLE (WINAPI *original_GetParameter)(
	ID3DXEffect* pEffect,
	D3DXHANDLE hParent,
	UINT index)(NULL);

HRESULT (WINAPI *original_SetTexture)(
	ID3DXEffect* pEffect,
	D3DXHANDLE hParameter,
	LPDIRECT3DBASETEXTURE9 pTexture
	)(NULL);

///-----------------------------------------------------D3DX------------------------------------------------

HRESULT (WINAPI *original_D3DXAssembleShader)(
	LPCSTR                          pSrcData,
	UINT                            SrcDataLen,
	CONST D3DXMACRO*                pDefines,
	LPD3DXINCLUDE                   pInclude,
	DWORD                           Flags,
	LPD3DXBUFFER*                   ppShader,
	LPD3DXBUFFER*                   ppErrorMsgs)(NULL);

HRESULT (WINAPI *original_D3DXAssembleShaderFromFileA)(
	LPCSTR                          pSrcFile,
	CONST D3DXMACRO*                pDefines,
	LPD3DXINCLUDE                   pInclude,
	DWORD                           Flags,
	LPD3DXBUFFER*                   ppShader,
	LPD3DXBUFFER*                   ppErrorMsgs)(NULL);

HRESULT (WINAPI *original_D3DXAssembleShaderFromFileW)(
	LPCWSTR                         pSrcFile,
	CONST D3DXMACRO*                pDefines,
	LPD3DXINCLUDE                   pInclude,
	DWORD                           Flags,
	LPD3DXBUFFER*                   ppShader,
	LPD3DXBUFFER*                   ppErrorMsgs)(NULL);

HRESULT (WINAPI *original_D3DXAssembleShaderFromResourceA)(
	HMODULE                         hSrcModule,
	LPCSTR                          pSrcResource,
	CONST D3DXMACRO*                pDefines,
	LPD3DXINCLUDE                   pInclude,
	DWORD                           Flags,
	LPD3DXBUFFER*                   ppShader,
	LPD3DXBUFFER*                   ppErrorMsgs)(NULL);

HRESULT (WINAPI *original_D3DXAssembleShaderFromResourceW)(
	HMODULE                         hSrcModule,
	LPCWSTR                         pSrcResource,
	CONST D3DXMACRO*                pDefines,
	LPD3DXINCLUDE                   pInclude,
	DWORD                           Flags,
	LPD3DXBUFFER*                   ppShader,
	LPD3DXBUFFER*                   ppErrorMsgs)(NULL);

BOOL (WINAPI *original_D3DXBoxBoundProbe)(
	CONST D3DXVECTOR3 *pMin,
	CONST D3DXVECTOR3 *pMax,
	CONST D3DXVECTOR3 *pRayPosition,
	CONST D3DXVECTOR3 *pRayDirection)(NULL);

HRESULT (WINAPI *original_D3DXCheckCubeTextureRequirements)(
	LPDIRECT3DDEVICE9         pDevice,
	UINT*                     pSize,
	UINT*                     pNumMipLevels,
	DWORD                     Usage,
	D3DFORMAT*                pFormat,
	D3DPOOL                   Pool)(NULL);

HRESULT (WINAPI *original_D3DXCheckTextureRequirements)(
	LPDIRECT3DDEVICE9         pDevice,
	UINT*                     pWidth,
	UINT*                     pHeight,
	UINT*                     pNumMipLevels,
	DWORD                     Usage,
	D3DFORMAT*                pFormat,
	D3DPOOL                   Pool)(NULL);

BOOL (WINAPI *original_D3DXCheckVersion)(
	UINT D3DSdkVersion,
	UINT D3DXSdkVersion)(NULL);

HRESULT (WINAPI *original_D3DXCheckVolumeTextureRequirements)(
	LPDIRECT3DDEVICE9         pDevice,
	UINT*                     pWidth,
	UINT*                     pHeight,
	UINT*                     pDepth,
	UINT*                     pNumMipLevels,
	DWORD                     Usage,
	D3DFORMAT*                pFormat,
	D3DPOOL                   Pool)(NULL);

HRESULT (WINAPI *original_D3DXCleanMesh)(
	D3DXCLEANTYPE CleanType,
	LPD3DXMESH pMeshIn,
	CONST DWORD* pAdjacencyIn,
	LPD3DXMESH* ppMeshOut,
	DWORD* pAdjacencyOut,
	LPD3DXBUFFER* ppErrorsAndWarnings)(NULL);

D3DXCOLOR* (WINAPI *original_D3DXColorAdjustContrast)(
	D3DXCOLOR *pOut,
	CONST D3DXCOLOR *pC,
	FLOAT c)(NULL);

D3DXCOLOR* (WINAPI *original_D3DXColorAdjustSaturation)(
	D3DXCOLOR *pOut,
	CONST D3DXCOLOR *pC,
	FLOAT s)(NULL);

HRESULT (WINAPI *original_D3DXCompileShader)(
	LPCSTR                          pSrcData,
	UINT                            SrcDataLen,
	CONST D3DXMACRO*                pDefines,
	LPD3DXINCLUDE                   pInclude,
	LPCSTR                          pFunctionName,
	LPCSTR                          pProfile,
	DWORD                           Flags,
	LPD3DXBUFFER*                   ppShader,
	LPD3DXBUFFER*                   ppErrorMsgs,
	LPD3DXCONSTANTTABLE*            ppConstantTable)(NULL);

HRESULT (WINAPI *original_D3DXCompileShaderFromFileA)(
	LPCSTR                          pSrcFile,
	CONST D3DXMACRO*                pDefines,
	LPD3DXINCLUDE                   pInclude,
	LPCSTR                          pFunctionName,
	LPCSTR                          pProfile,
	DWORD                           Flags,
	LPD3DXBUFFER*                   ppShader,
	LPD3DXBUFFER*                   ppErrorMsgs,
	LPD3DXCONSTANTTABLE*            ppConstantTable)(NULL);

HRESULT (WINAPI *original_D3DXCompileShaderFromFileW)(
	LPCWSTR                         pSrcFile,
	CONST D3DXMACRO*                pDefines,
	LPD3DXINCLUDE                   pInclude,
	LPCSTR                          pFunctionName,
	LPCSTR                          pProfile,
	DWORD                           Flags,
	LPD3DXBUFFER*                   ppShader,
	LPD3DXBUFFER*                   ppErrorMsgs,
	LPD3DXCONSTANTTABLE*            ppConstantTable)(NULL);

HRESULT (WINAPI *original_D3DXCompileShaderFromResourceA)(
	HMODULE                         hSrcModule,
	LPCSTR                          pSrcResource,
	CONST D3DXMACRO*                pDefines,
	LPD3DXINCLUDE                   pInclude,
	LPCSTR                          pFunctionName,
	LPCSTR                          pProfile,
	DWORD                           Flags,
	LPD3DXBUFFER*                   ppShader,
	LPD3DXBUFFER*                   ppErrorMsgs,
	LPD3DXCONSTANTTABLE*            ppConstantTable)(NULL);

HRESULT (WINAPI *original_D3DXCompileShaderFromResourceW)(
	HMODULE                         hSrcModule,
	LPCWSTR                         pSrcResource,
	CONST D3DXMACRO*                pDefines,
	LPD3DXINCLUDE                   pInclude,
	LPCSTR                          pFunctionName,
	LPCSTR                          pProfile,
	DWORD                           Flags,
	LPD3DXBUFFER*                   ppShader,
	LPD3DXBUFFER*                   ppErrorMsgs,
	LPD3DXCONSTANTTABLE*            ppConstantTable)(NULL);

HRESULT (WINAPI *original_D3DXComputeBoundingBox)(
	CONST D3DXVECTOR3 *pFirstPosition,
	DWORD NumVertices,
	DWORD dwStride,
	D3DXVECTOR3 *pMin,
	D3DXVECTOR3 *pMax)(NULL);

HRESULT (WINAPI *original_D3DXComputeBoundingSphere)(
	CONST D3DXVECTOR3 *pFirstPosition,
	DWORD NumVertices,
	DWORD dwStride,
	D3DXVECTOR3 *pCenter,
	FLOAT *pRadius)(NULL);

HRESULT (WINAPI *original_D3DXComputeIMTFromPerTexelSignal)(
	LPD3DXMESH pMesh,
	DWORD dwTextureIndex,
	FLOAT *pfTexelSignal,
	UINT uWidth,
	UINT uHeight,
	UINT uSignalDimension,
	UINT uComponents,
	DWORD dwOptions,
	LPD3DXUVATLASCB pStatusCallback,
	LPVOID pUserContext,
	LPD3DXBUFFER *ppIMTData)(NULL);

HRESULT (WINAPI *original_D3DXComputeIMTFromPerVertexSignal)(
	LPD3DXMESH pMesh,
	CONST FLOAT *pfVertexSignal,
	UINT uSignalDimension,
	UINT uSignalStride,
	DWORD dwOptions,
	LPD3DXUVATLASCB pStatusCallback,
	LPVOID pUserContext,
	LPD3DXBUFFER *ppIMTData)(NULL);

HRESULT (WINAPI *original_D3DXComputeIMTFromSignal)(
	LPD3DXMESH pMesh,
	DWORD dwTextureIndex,
	UINT uSignalDimension,
	FLOAT fMaxUVDistance,
	DWORD dwOptions,
	LPD3DXIMTSIGNALCALLBACK pSignalCallback,
	VOID *pUserData,
	LPD3DXUVATLASCB pStatusCallback,
	LPVOID pUserContext,
	LPD3DXBUFFER *ppIMTData         )(NULL);

HRESULT (WINAPI *original_D3DXComputeIMTFromTexture)(
	LPD3DXMESH pMesh,
	LPDIRECT3DTEXTURE9 pTexture,
	DWORD dwTextureIndex,
	DWORD dwOptions,
	LPD3DXUVATLASCB pStatusCallback,
	LPVOID pUserContext,
	LPD3DXBUFFER *ppIMTData)(NULL);

HRESULT (WINAPI *original_D3DXComputeNormalMap)(
	LPDIRECT3DTEXTURE9        pTexture,
	LPDIRECT3DTEXTURE9        pSrcTexture,
	CONST PALETTEENTRY*       pSrcPalette,
	DWORD                     Flags,
	DWORD                     Channel,
	FLOAT                     Amplitude)(NULL);

HRESULT (WINAPI *original_D3DXComputeNormals)(
	LPD3DXBASEMESH pMesh,
	CONST DWORD *pAdjacency)(NULL);

HRESULT (WINAPI *original_D3DXComputeTangent)(
	LPD3DXMESH Mesh,
	DWORD TexStage,
	DWORD TangentIndex,
	DWORD BinormIndex,
	DWORD Wrap,
	CONST DWORD *pAdjacency)(NULL);

HRESULT (WINAPI *original_D3DXComputeTangentFrame)(
	ID3DXMesh *pMesh,
	DWORD dwOptions)(NULL);

HRESULT (WINAPI *original_D3DXComputeTangentFrameEx)(
	ID3DXMesh *pMesh,
	DWORD dwTextureInSemantic,
	DWORD dwTextureInIndex,
	DWORD dwUPartialOutSemantic,
	DWORD dwUPartialOutIndex,
	DWORD dwVPartialOutSemantic,
	DWORD dwVPartialOutIndex,
	DWORD dwNormalOutSemantic,
	DWORD dwNormalOutIndex,
	DWORD dwOptions,
	CONST DWORD *pdwAdjacency,
	FLOAT fPartialEdgeThreshold,
	FLOAT fSingularPointThreshold,
	FLOAT fNormalEdgeThreshold,
	ID3DXMesh **ppMeshOut,
	ID3DXBuffer **ppVertexMapping)(NULL);

HRESULT (WINAPI *original_D3DXConcatenateMeshes)(
	LPD3DXMESH *ppMeshes,
	UINT NumMeshes,
	DWORD Options,
	CONST D3DXMATRIX *pGeomXForms,
	CONST D3DXMATRIX *pTextureXForms,
	CONST D3DVERTEXELEMENT9 *pDecl,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXMESH *ppMeshOut)(NULL);

HRESULT (WINAPI *original_D3DXConvertMeshSubsetToSingleStrip)(
	LPD3DXBASEMESH MeshIn,
	DWORD AttribId,
	DWORD IBOptions,
	LPDIRECT3DINDEXBUFFER9 *ppIndexBuffer,
	DWORD *pNumIndices)(NULL);

HRESULT (WINAPI *original_D3DXConvertMeshSubsetToStrips)(
	LPD3DXBASEMESH MeshIn,
	DWORD AttribId,
	DWORD IBOptions,
	LPDIRECT3DINDEXBUFFER9 *ppIndexBuffer,
	DWORD *pNumIndices,
	LPD3DXBUFFER *ppStripLengths,
	DWORD *pNumStrips)(NULL);

HRESULT (WINAPI *original_D3DXCreateAnimationController)(
	UINT MaxNumMatrices,
	UINT MaxNumAnimationSets,
	UINT MaxNumTracks,
	UINT MaxNumEvents,
	LPD3DXANIMATIONCONTROLLER *ppAnimController)(NULL);

HRESULT (WINAPI *original_D3DXCreateBox)(
	LPDIRECT3DDEVICE9   pDevice,
	FLOAT               Width,
	FLOAT               Height,
	FLOAT               Depth,
	LPD3DXMESH*         ppMesh,
	LPD3DXBUFFER*       ppAdjacency)(NULL);

HRESULT (WINAPI *original_D3DXCreateBuffer)(
	DWORD NumBytes,
	LPD3DXBUFFER *ppBuffer)(NULL);

HRESULT (WINAPI *original_D3DXCreateCompressedAnimationSet)(
	LPCSTR pName,
	DOUBLE TicksPerSecond,
	D3DXPLAYBACK_TYPE Playback,
	LPD3DXBUFFER pCompressedData,
	UINT NumCallbackKeys,
	CONST D3DXKEY_CALLBACK *pCallbackKeys,
	LPD3DXCOMPRESSEDANIMATIONSET *ppAnimationSet)(NULL);

HRESULT (WINAPI *original_D3DXCreateCubeTexture)(
	LPDIRECT3DDEVICE9         pDevice,
	UINT                      Size,
	UINT                      MipLevels,
	DWORD                     Usage,
	D3DFORMAT                 Format,
	D3DPOOL                   Pool,
	LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateCubeTextureFromFileA)(
	LPDIRECT3DDEVICE9         pDevice,
	LPCSTR                    pSrcFile,
	LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateCubeTextureFromFileExA)(
	LPDIRECT3DDEVICE9         pDevice,
	LPCSTR                    pSrcFile,
	UINT                      Size,
	UINT                      MipLevels,
	DWORD                     Usage,
	D3DFORMAT                 Format,
	D3DPOOL                   Pool,
	DWORD                     Filter,
	DWORD                     MipFilter,
	D3DCOLOR                  ColorKey,
	D3DXIMAGE_INFO*           pSrcInfo,
	PALETTEENTRY*             pPalette,
	LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateCubeTextureFromFileExW)(
	LPDIRECT3DDEVICE9         pDevice,
	LPCWSTR                   pSrcFile,
	UINT                      Size,
	UINT                      MipLevels,
	DWORD                     Usage,
	D3DFORMAT                 Format,
	D3DPOOL                   Pool,
	DWORD                     Filter,
	DWORD                     MipFilter,
	D3DCOLOR                  ColorKey,
	D3DXIMAGE_INFO*           pSrcInfo,
	PALETTEENTRY*             pPalette,
	LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateCubeTextureFromFileInMemory)(
	LPDIRECT3DDEVICE9         pDevice,
	LPCVOID                   pSrcData,
	UINT                      SrcDataSize,
	LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateCubeTextureFromFileInMemoryEx)(
	LPDIRECT3DDEVICE9         pDevice,
	LPCVOID                   pSrcData,
	UINT                      SrcDataSize,
	UINT                      Size,
	UINT                      MipLevels,
	DWORD                     Usage,
	D3DFORMAT                 Format,
	D3DPOOL                   Pool,
	DWORD                     Filter,
	DWORD                     MipFilter,
	D3DCOLOR                  ColorKey,
	D3DXIMAGE_INFO*           pSrcInfo,
	PALETTEENTRY*             pPalette,
	LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateCubeTextureFromFileW)(
	LPDIRECT3DDEVICE9		 pDevice,
	LPCWSTR				pSrcFile,
	LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateCubeTextureFromResourceA)(
	LPDIRECT3DDEVICE9		 pDevice,
	HMODULE				hSrcModule,
	LPCSTR				 pSrcResource,
	LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateCubeTextureFromResourceExA)(
	LPDIRECT3DDEVICE9		 pDevice,
	HMODULE				hSrcModule,
	LPCSTR				 pSrcResource,
	UINT				   Size,
	UINT				   MipLevels,
	DWORD				  Usage,
	D3DFORMAT				 Format,
	D3DPOOL				Pool,
	DWORD				  Filter,
	DWORD				  MipFilter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo,
	PALETTEENTRY*		  pPalette,
	LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateCubeTextureFromResourceExW)(
	LPDIRECT3DDEVICE9		 pDevice,
	HMODULE				hSrcModule,
	LPCWSTR				pSrcResource,
	UINT				   Size,
	UINT				   MipLevels,
	DWORD				  Usage,
	D3DFORMAT				 Format,
	D3DPOOL				Pool,
	DWORD				  Filter,
	DWORD				  MipFilter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo,
	PALETTEENTRY*		  pPalette,
	LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateCubeTextureFromResourceW)(
	LPDIRECT3DDEVICE9		 pDevice,
	HMODULE				hSrcModule,
	LPCWSTR				pSrcResource,
	LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateCylinder)(
	LPDIRECT3DDEVICE9   pDevice,
	FLOAT		    Radius1,
	FLOAT		    Radius2,
	FLOAT		    Length,
	UINT				Slices,
	UINT				Stacks,
	LPD3DXMESH*		 ppMesh,
	LPD3DXBUFFER*       ppAdjacency)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffect)(
	LPDIRECT3DDEVICE9		    pDevice,
	LPCVOID						 pSrcData,
	UINT						 SrcDataLen,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXEFFECTPOOL				pPool,
	LPD3DXEFFECT*				ppEffect,
	LPD3DXBUFFER*				ppCompilationErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectCompiler)(
	LPCSTR						  pSrcData,
	UINT						 SrcDataLen,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXEFFECTCOMPILER*		ppCompiler,
	LPD3DXBUFFER*				ppParseErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectCompilerFromFileA)(
	LPCSTR						  pSrcFile,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXEFFECTCOMPILER*		ppCompiler,
	LPD3DXBUFFER*				ppParseErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectCompilerFromFileW)(
	LPCWSTR						 pSrcFile,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXEFFECTCOMPILER*		ppCompiler,
	LPD3DXBUFFER*				ppParseErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectCompilerFromResourceA)(
	HMODULE						 hSrcModule,
	LPCSTR						  pSrcResource,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXEFFECTCOMPILER*		ppCompiler,
	LPD3DXBUFFER*				ppParseErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectCompilerFromResourceW)(
	HMODULE						 hSrcModule,
	LPCWSTR						 pSrcResource,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXEFFECTCOMPILER*		ppCompiler,
	LPD3DXBUFFER*				ppParseErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectEx)(
	LPDIRECT3DDEVICE9		    pDevice,
	LPCVOID						 pSrcData,
	UINT						 SrcDataLen,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	LPCSTR						  pSkipConstants,
	DWORD						Flags,
	LPD3DXEFFECTPOOL				pPool,
	LPD3DXEFFECT*				ppEffect,
	LPD3DXBUFFER*				ppCompilationErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectFromFileA)(
	LPDIRECT3DDEVICE9		    pDevice,
	LPCSTR						  pSrcFile,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXEFFECTPOOL				pPool,
	LPD3DXEFFECT*				ppEffect,
	LPD3DXBUFFER*				ppCompilationErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectFromFileExA)(
	LPDIRECT3DDEVICE9		    pDevice,
	LPCSTR						  pSrcFile,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	LPCSTR						  pSkipConstants,
	DWORD						Flags,
	LPD3DXEFFECTPOOL				pPool,
	LPD3DXEFFECT*				ppEffect,
	LPD3DXBUFFER*				ppCompilationErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectFromFileExW)(
	LPDIRECT3DDEVICE9		    pDevice,
	LPCWSTR						 pSrcFile,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	LPCSTR						  pSkipConstants,
	DWORD						Flags,
	LPD3DXEFFECTPOOL				pPool,
	LPD3DXEFFECT*				ppEffect,
	LPD3DXBUFFER*				ppCompilationErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectFromFileW)(
	LPDIRECT3DDEVICE9		    pDevice,
	LPCWSTR						 pSrcFile,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXEFFECTPOOL				pPool,
	LPD3DXEFFECT*				ppEffect,
	LPD3DXBUFFER*				ppCompilationErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectFromResourceA)(
	LPDIRECT3DDEVICE9		    pDevice,
	HMODULE						 hSrcModule,
	LPCSTR						  pSrcResource,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXEFFECTPOOL				pPool,
	LPD3DXEFFECT*				ppEffect,
	LPD3DXBUFFER*				ppCompilationErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectFromResourceExA)(
	LPDIRECT3DDEVICE9		    pDevice,
	HMODULE						 hSrcModule,
	LPCSTR						  pSrcResource,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	LPCSTR						  pSkipConstants,
	DWORD						Flags,
	LPD3DXEFFECTPOOL				pPool,
	LPD3DXEFFECT*				ppEffect,
	LPD3DXBUFFER*				ppCompilationErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectFromResourceExW)(
	LPDIRECT3DDEVICE9		    pDevice,
	HMODULE						 hSrcModule,
	LPCWSTR						 pSrcResource,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	LPCSTR						  pSkipConstants,
	DWORD						Flags,
	LPD3DXEFFECTPOOL				pPool,
	LPD3DXEFFECT*				ppEffect,
	LPD3DXBUFFER*				ppCompilationErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectFromResourceW)(
	LPDIRECT3DDEVICE9		    pDevice,
	HMODULE						 hSrcModule,
	LPCWSTR						 pSrcResource,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXEFFECTPOOL				pPool,
	LPD3DXEFFECT*				ppEffect,
	LPD3DXBUFFER*				ppCompilationErrors)(NULL);

HRESULT (WINAPI *original_D3DXCreateEffectPool)(
	LPD3DXEFFECTPOOL*		    ppPool)(NULL);

HRESULT (WINAPI *original_D3DXCreateFontA)(
	LPDIRECT3DDEVICE9       pDevice,
	INT				  Height,
	UINT				 Width,
	UINT				 Weight,
	UINT				 MipLevels,
	BOOL				 Italic,
	DWORD				CharSet,
	DWORD				OutputPrecision,
	DWORD				Quality,
	DWORD				PitchAndFamily,
	LPCSTR				  pFaceName,
	LPD3DXFONT*		  ppFont)(NULL);

HRESULT (WINAPI *original_D3DXCreateFontIndirectA)(
	LPDIRECT3DDEVICE9       pDevice,
	CONST D3DXFONT_DESCA*   pDesc,
	LPD3DXFONT*		  ppFont)(NULL);

HRESULT (WINAPI *original_D3DXCreateFontIndirectW)(
	LPDIRECT3DDEVICE9       pDevice,
	CONST D3DXFONT_DESCW*   pDesc,
	LPD3DXFONT*		  ppFont)(NULL);

HRESULT (WINAPI *original_D3DXCreateFontW)(
	LPDIRECT3DDEVICE9       pDevice,
	INT				  Height,
	UINT				 Width,
	UINT				 Weight,
	UINT				 MipLevels,
	BOOL				 Italic,
	DWORD				CharSet,
	DWORD				OutputPrecision,
	DWORD				Quality,
	DWORD				PitchAndFamily,
	LPCWSTR				 pFaceName,
	LPD3DXFONT*		  ppFont)(NULL);

/*
HRESULT (WINAPI *original_D3DXCreateFragmentLinker)(
	LPDIRECT3DDEVICE9		    pDevice,
	UINT						 ShaderCacheSize,
	LPD3DXFRAGMENTLINKER*		ppFragmentLinker)(NULL);
*/

HRESULT (WINAPI *original_D3DXCreateKeyframedAnimationSet)(
	LPCSTR pName,
	DOUBLE TicksPerSecond,
	D3DXPLAYBACK_TYPE Playback,
	UINT NumAnimations,
	UINT NumCallbackKeys,
	CONST D3DXKEY_CALLBACK *pCallbackKeys,
	LPD3DXKEYFRAMEDANIMATIONSET *ppAnimationSet)(NULL);

HRESULT (WINAPI *original_D3DXCreateLine)(
	LPDIRECT3DDEVICE9   pDevice,
	LPD3DXLINE*		 ppLine)(NULL);

HRESULT (WINAPI *original_D3DXCreateMatrixStack)(
	DWORD		    Flags,
	LPD3DXMATRIXSTACK*  ppStack)(NULL);

HRESULT (WINAPI *original_D3DXCreateMesh)(
	DWORD NumFaces,
	DWORD NumVertices,
	DWORD Options,
	CONST D3DVERTEXELEMENT9 *pDeclaration,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXMESH* ppMesh)(NULL);

HRESULT (WINAPI *original_D3DXCreateMeshFVF)(
	DWORD NumFaces,
	DWORD NumVertices,
	DWORD Options,
	DWORD FVF,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXMESH* ppMesh)(NULL);

HRESULT (WINAPI *original_D3DXCreateNPatchMesh)(
	LPD3DXMESH pMeshSysMem,
	LPD3DXPATCHMESH *pPatchMesh)(NULL);

HRESULT (WINAPI *original_D3DXCreatePMeshFromStream)(
	IStream *pStream,
	DWORD Options,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXBUFFER *ppMaterials,
	LPD3DXBUFFER *ppEffectInstances,
	DWORD* pNumMaterials,
	LPD3DXPMESH *ppPMesh)(NULL);

HRESULT (WINAPI *original_D3DXCreatePRTBuffer)(
	UINT NumSamples,
	UINT NumCoeffs,
	UINT NumChannels,
	LPD3DXPRTBUFFER* ppBuffer)(NULL);

HRESULT (WINAPI *original_D3DXCreatePRTBufferTex)(
	UINT Width,
	UINT Height,
	UINT NumCoeffs,
	UINT NumChannels,
	LPD3DXPRTBUFFER* ppBuffer)(NULL);

HRESULT (WINAPI *original_D3DXCreatePRTCompBuffer)(
	D3DXSHCOMPRESSQUALITYTYPE Quality,
	UINT NumClusters,
	UINT NumPCA,
	LPD3DXSHPRTSIMCB pCB,
	LPVOID lpUserContext,
	LPD3DXPRTBUFFER  pBufferIn,
	LPD3DXPRTCOMPBUFFER *ppBufferOut)(NULL);

HRESULT (WINAPI *original_D3DXCreatePRTEngine)(
	LPD3DXMESH pMesh,
	DWORD *pAdjacency,
	BOOL ExtractUVs,
	LPD3DXMESH pBlockerMesh,
	LPD3DXPRTENGINE* ppEngine)(NULL);

HRESULT (WINAPI *original_D3DXCreatePatchMesh)(
	CONST D3DXPATCHINFO *pInfo,
	DWORD dwNumPatches,
	DWORD dwNumVertices,
	DWORD dwOptions,
	CONST D3DVERTEXELEMENT9 *pDecl,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXPATCHMESH *pPatchMesh)(NULL);

HRESULT (WINAPI *original_D3DXCreatePolygon)(
	LPDIRECT3DDEVICE9   pDevice,
	FLOAT		    Length,
	UINT				Sides,
	LPD3DXMESH*		 ppMesh,
	LPD3DXBUFFER*       ppAdjacency)(NULL);

HRESULT (WINAPI *original_D3DXCreateRenderToEnvMap)(
	LPDIRECT3DDEVICE9       pDevice,
	UINT				 Size,
	UINT				 MipLevels,
	D3DFORMAT		    Format,
	BOOL				 DepthStencil,
	D3DFORMAT		    DepthStencilFormat,
	LPD3DXRenderToEnvMap*   ppRenderToEnvMap)(NULL);

HRESULT (WINAPI *original_D3DXCreateRenderToSurface)(
	LPDIRECT3DDEVICE9       pDevice,
	UINT				 Width,
	UINT				 Height,
	D3DFORMAT		    Format,
	BOOL				 DepthStencil,
	D3DFORMAT		    DepthStencilFormat,
	LPD3DXRENDERTOSURFACE*  ppRenderToSurface)(NULL);

HRESULT (WINAPI *original_D3DXCreateSPMesh)(
	LPD3DXMESH pMesh,
	CONST DWORD* pAdjacency,
	CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,
	CONST FLOAT *pVertexWeights,
	LPD3DXSPMESH* ppSMesh)(NULL);

HRESULT (WINAPI *original_D3DXCreateSkinInfo)(
	DWORD NumVertices,
	CONST D3DVERTEXELEMENT9 *pDeclaration,
	DWORD NumBones,
	LPD3DXSKININFO* ppSkinInfo)(NULL);

HRESULT (WINAPI *original_D3DXCreateSkinInfoFVF)(
	DWORD NumVertices,
	DWORD FVF,
	DWORD NumBones,
	LPD3DXSKININFO* ppSkinInfo)(NULL);

HRESULT (WINAPI *original_D3DXCreateSkinInfoFromBlendedMesh)(
	LPD3DXBASEMESH pMesh,
	DWORD NumBones,
	CONST D3DXBONECOMBINATION *pBoneCombinationTable,
	LPD3DXSKININFO* ppSkinInfo)(NULL);

HRESULT (WINAPI *original_D3DXCreateSphere)(
	LPDIRECT3DDEVICE9  pDevice,
	FLOAT		   Radius,
	UINT		    Slices,
	UINT		    Stacks,
	LPD3DXMESH*		ppMesh,
	LPD3DXBUFFER*      ppAdjacency)(NULL);

HRESULT (WINAPI *original_D3DXCreateSprite)(
	LPDIRECT3DDEVICE9   pDevice,
	LPD3DXSPRITE*       ppSprite)(NULL);

HRESULT (WINAPI *original_D3DXCreateTeapot)(
	LPDIRECT3DDEVICE9   pDevice,
	LPD3DXMESH*		 ppMesh,
	LPD3DXBUFFER*       ppAdjacency)(NULL);

HRESULT (WINAPI *original_D3DXCreateTextA)(
	LPDIRECT3DDEVICE9   pDevice,
	HDC				 hDC,
	LPCSTR		   pText,
	FLOAT		    Deviation,
	FLOAT		    Extrusion,
	LPD3DXMESH*		 ppMesh,
	LPD3DXBUFFER*       ppAdjacency,
	LPGLYPHMETRICSFLOAT pGlyphMetrics)(NULL);

HRESULT (WINAPI *original_D3DXCreateTextW)(
	LPDIRECT3DDEVICE9   pDevice,
	HDC				 hDC,
	LPCWSTR		  pText,
	FLOAT		    Deviation,
	FLOAT		    Extrusion,
	LPD3DXMESH*		 ppMesh,
	LPD3DXBUFFER*       ppAdjacency,
	LPGLYPHMETRICSFLOAT pGlyphMetrics)(NULL);

HRESULT (WINAPI *original_D3DXCreateTexture)(
	LPDIRECT3DDEVICE9		 pDevice,
	UINT				   Width,
	UINT				   Height,
	UINT				   MipLevels,
	DWORD				  Usage,
	D3DFORMAT				 Format,
	D3DPOOL				Pool,
	LPDIRECT3DTEXTURE9*       ppTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateTextureFromFileA)(
	LPDIRECT3DDEVICE9		 pDevice,
	LPCSTR				 pSrcFile,
	LPDIRECT3DTEXTURE9*       ppTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateTextureFromFileExA)(
	LPDIRECT3DDEVICE9		 pDevice,
	LPCSTR				 pSrcFile,
	UINT				   Width,
	UINT				   Height,
	UINT				   MipLevels,
	DWORD				  Usage,
	D3DFORMAT				 Format,
	D3DPOOL				Pool,
	DWORD				  Filter,
	DWORD				  MipFilter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo,
	PALETTEENTRY*		  pPalette,
	LPDIRECT3DTEXTURE9*       ppTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateTextureFromFileExW)(
	LPDIRECT3DDEVICE9		 pDevice,
	LPCWSTR				pSrcFile,
	UINT				   Width,
	UINT				   Height,
	UINT				   MipLevels,
	DWORD				  Usage,
	D3DFORMAT				 Format,
	D3DPOOL				Pool,
	DWORD				  Filter,
	DWORD				  MipFilter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo,
	PALETTEENTRY*		  pPalette,
	LPDIRECT3DTEXTURE9*       ppTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateTextureFromFileInMemory)(
	LPDIRECT3DDEVICE9		 pDevice,
	LPCVOID				pSrcData,
	UINT				   SrcDataSize,
	LPDIRECT3DTEXTURE9*       ppTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateTextureFromFileInMemoryEx)(
	LPDIRECT3DDEVICE9		 pDevice,
	LPCVOID				pSrcData,
	UINT				   SrcDataSize,
	UINT				   Width,
	UINT				   Height,
	UINT				   MipLevels,
	DWORD				  Usage,
	D3DFORMAT				 Format,
	D3DPOOL				Pool,
	DWORD				  Filter,
	DWORD				  MipFilter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo,
	PALETTEENTRY*		  pPalette,
	LPDIRECT3DTEXTURE9*       ppTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateTextureFromFileW)(
	LPDIRECT3DDEVICE9		 pDevice,
	LPCWSTR				pSrcFile,
	LPDIRECT3DTEXTURE9*       ppTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateTextureFromResourceA)(
	LPDIRECT3DDEVICE9		 pDevice,
	HMODULE				hSrcModule,
	LPCSTR				 pSrcResource,
	LPDIRECT3DTEXTURE9*       ppTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateTextureFromResourceExA)(
	LPDIRECT3DDEVICE9		 pDevice,
	HMODULE				hSrcModule,
	LPCSTR				 pSrcResource,
	UINT				   Width,
	UINT				   Height,
	UINT				   MipLevels,
	DWORD				  Usage,
	D3DFORMAT				 Format,
	D3DPOOL				Pool,
	DWORD				  Filter,
	DWORD				  MipFilter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo,
	PALETTEENTRY*		  pPalette,
	LPDIRECT3DTEXTURE9*       ppTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateTextureFromResourceExW)(
	LPDIRECT3DDEVICE9		 pDevice,
	HMODULE				hSrcModule,
	LPCWSTR				pSrcResource,
	UINT				   Width,
	UINT				   Height,
	UINT				   MipLevels,
	DWORD				  Usage,
	D3DFORMAT				 Format,
	D3DPOOL				Pool,
	DWORD				  Filter,
	DWORD				  MipFilter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo,
	PALETTEENTRY*		  pPalette,
	LPDIRECT3DTEXTURE9*       ppTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateTextureFromResourceW)(
	LPDIRECT3DDEVICE9		 pDevice,
	HMODULE				hSrcModule,
	LPCWSTR				pSrcResource,
	LPDIRECT3DTEXTURE9*       ppTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateTextureGutterHelper)(
	UINT Width,
	UINT Height,
	LPD3DXMESH pMesh,
	FLOAT GutterSize,
	LPD3DXTEXTUREGUTTERHELPER* ppBuffer)(NULL);

HRESULT (WINAPI *original_D3DXCreateTextureShader)(
	CONST DWORD*				 pFunction,
	LPD3DXTEXTURESHADER*		 ppTextureShader)(NULL);

HRESULT (WINAPI *original_D3DXCreateTorus)(
	LPDIRECT3DDEVICE9   pDevice,
	FLOAT		    InnerRadius,
	FLOAT		    OuterRadius,
	UINT				Sides,
	UINT				Rings,
	LPD3DXMESH*		 ppMesh,
	LPD3DXBUFFER*       ppAdjacency)(NULL);

HRESULT (WINAPI *original_D3DXCreateVolumeTexture)(
	LPDIRECT3DDEVICE9		 pDevice,
	UINT				   Width,
	UINT				   Height,
	UINT				   Depth,
	UINT				   MipLevels,
	DWORD				  Usage,
	D3DFORMAT				 Format,
	D3DPOOL				Pool,
	LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateVolumeTextureFromFileA)(
	LPDIRECT3DDEVICE9		 pDevice,
	LPCSTR				 pSrcFile,
	LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateVolumeTextureFromFileExA)(
	LPDIRECT3DDEVICE9		 pDevice,
	LPCSTR				 pSrcFile,
	UINT				   Width,
	UINT				   Height,
	UINT				   Depth,
	UINT				   MipLevels,
	DWORD				  Usage,
	D3DFORMAT				 Format,
	D3DPOOL				Pool,
	DWORD				  Filter,
	DWORD				  MipFilter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo,
	PALETTEENTRY*		  pPalette,
	LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateVolumeTextureFromFileExW)(
	LPDIRECT3DDEVICE9		 pDevice,
	LPCWSTR				pSrcFile,
	UINT				   Width,
	UINT				   Height,
	UINT				   Depth,
	UINT				   MipLevels,
	DWORD				  Usage,
	D3DFORMAT				 Format,
	D3DPOOL				Pool,
	DWORD				  Filter,
	DWORD				  MipFilter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo,
	PALETTEENTRY*		  pPalette,
	LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateVolumeTextureFromFileInMemory)(
	LPDIRECT3DDEVICE9		 pDevice,
	LPCVOID				pSrcData,
	UINT				   SrcDataSize,
	LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateVolumeTextureFromFileInMemoryEx)(
	LPDIRECT3DDEVICE9		 pDevice,
	LPCVOID				pSrcData,
	UINT				   SrcDataSize,
	UINT				   Width,
	UINT				   Height,
	UINT				   Depth,
	UINT				   MipLevels,
	DWORD				  Usage,
	D3DFORMAT				 Format,
	D3DPOOL				Pool,
	DWORD				  Filter,
	DWORD				  MipFilter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo,
	PALETTEENTRY*		  pPalette,
	LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateVolumeTextureFromFileW)(
	LPDIRECT3DDEVICE9		 pDevice,
	LPCWSTR				pSrcFile,
	LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateVolumeTextureFromResourceA)(
	LPDIRECT3DDEVICE9		 pDevice,
	HMODULE				hSrcModule,
	LPCSTR				 pSrcResource,
	LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateVolumeTextureFromResourceExA)(
	LPDIRECT3DDEVICE9		 pDevice,
	HMODULE				hSrcModule,
	LPCSTR				 pSrcResource,
	UINT				   Width,
	UINT				   Height,
	UINT				   Depth,
	UINT				   MipLevels,
	DWORD				  Usage,
	D3DFORMAT				 Format,
	D3DPOOL				Pool,
	DWORD				  Filter,
	DWORD				  MipFilter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo,
	PALETTEENTRY*		  pPalette,
	LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateVolumeTextureFromResourceExW)(
	LPDIRECT3DDEVICE9		 pDevice,
	HMODULE				hSrcModule,
	LPCWSTR				pSrcResource,
	UINT				   Width,
	UINT				   Height,
	UINT				   Depth,
	UINT				   MipLevels,
	DWORD				  Usage,
	D3DFORMAT				 Format,
	D3DPOOL				Pool,
	DWORD				  Filter,
	DWORD				  MipFilter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo,
	PALETTEENTRY*		  pPalette,
	LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)(NULL);

HRESULT (WINAPI *original_D3DXCreateVolumeTextureFromResourceW)(
	LPDIRECT3DDEVICE9		 pDevice,
	HMODULE				hSrcModule,
	LPCWSTR				pSrcResource,
	LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)(NULL);

BOOL (WINAPI *original_D3DXDebugMute)(
							  BOOL Mute)(NULL);

HRESULT (WINAPI *original_D3DXDeclaratorFromFVF)(
	DWORD FVF,
	D3DVERTEXELEMENT9 pDeclarator[MAX_FVF_DECL_SIZE])(NULL);

HRESULT (WINAPI *original_D3DXDisassembleEffect)(
	LPD3DXEFFECT pEffect,
	BOOL EnableColorCode,
	LPD3DXBUFFER *ppDisassembly)(NULL);

HRESULT (WINAPI *original_D3DXDisassembleShader)(
	CONST DWORD*				 pShader,
	BOOL						 EnableColorCode,
	LPCSTR						  pComments,
	LPD3DXBUFFER*				ppDisassembly)(NULL);

HRESULT (WINAPI *original_D3DXFVFFromDeclarator)(
	CONST D3DVERTEXELEMENT9 *pDeclarator,
	DWORD *pFVF)(NULL);

HRESULT (WINAPI *original_D3DXFileCreate)(
	ID3DXFile** lplpDirectXFile)(NULL);

HRESULT (WINAPI *original_D3DXFillCubeTexture)(
	LPDIRECT3DCUBETEXTURE9    pCubeTexture,
	LPD3DXFILL3D		   pFunction,
	LPVOID				 pData)(NULL);

HRESULT (WINAPI *original_D3DXFillCubeTextureTX)(
	LPDIRECT3DCUBETEXTURE9    pCubeTexture,
	LPD3DXTEXTURESHADER       pTextureShader)(NULL);

HRESULT (WINAPI *original_D3DXFillTexture)(
	LPDIRECT3DTEXTURE9		pTexture,
	LPD3DXFILL2D		   pFunction,
	LPVOID				 pData)(NULL);

HRESULT (WINAPI *original_D3DXFillTextureTX)(
	LPDIRECT3DTEXTURE9		pTexture,
	LPD3DXTEXTURESHADER       pTextureShader)(NULL);

HRESULT (WINAPI *original_D3DXFillVolumeTexture)(
	LPDIRECT3DVOLUMETEXTURE9  pVolumeTexture,
	LPD3DXFILL3D		   pFunction,
	LPVOID				 pData)(NULL);

HRESULT (WINAPI *original_D3DXFillVolumeTextureTX)(
	LPDIRECT3DVOLUMETEXTURE9  pVolumeTexture,
	LPD3DXTEXTURESHADER       pTextureShader)(NULL);

HRESULT (WINAPI *original_D3DXFilterTexture)(
	LPDIRECT3DBASETEXTURE9    pBaseTexture,
	CONST PALETTEENTRY*       pPalette,
	UINT				   SrcLevel,
	DWORD				  Filter)(NULL);

HRESULT (WINAPI *original_D3DXFindShaderComment)(
	CONST DWORD*				 pFunction,
	DWORD						FourCC,
	LPCVOID*						ppData,
	UINT*						pSizeInBytes)(NULL);

FLOAT* (WINAPI *original_D3DXFloat16To32Array)(
		FLOAT *pOut,
		CONST D3DXFLOAT16 *pIn,
		UINT n)(NULL);

D3DXFLOAT16* (WINAPI *original_D3DXFloat32To16Array)(
	D3DXFLOAT16 *pOut,
	CONST FLOAT *pIn,
	UINT n)(NULL); 

HRESULT (WINAPI *original_D3DXFrameAppendChild)(
	LPD3DXFRAME pFrameParent,
	CONST D3DXFRAME *pFrameChild)(NULL);

HRESULT (WINAPI *original_D3DXFrameCalculateBoundingSphere)(
	CONST D3DXFRAME *pFrameRoot,
	LPD3DXVECTOR3 pObjectCenter,
	FLOAT *pObjectRadius)(NULL);

HRESULT (WINAPI *original_D3DXFrameDestroy)(
	LPD3DXFRAME pFrameRoot,
	LPD3DXALLOCATEHIERARCHY pAlloc)(NULL);

LPD3DXFRAME (WINAPI *original_D3DXFrameFind
	)(
	CONST D3DXFRAME *pFrameRoot,
	LPCSTR Name
	)(NULL);

UINT (WINAPI *original_D3DXFrameNumNamedMatrices)(
	CONST D3DXFRAME *pFrameRoot)(NULL);

HRESULT (WINAPI *original_D3DXFrameRegisterNamedMatrices)(
	LPD3DXFRAME pFrameRoot,
	LPD3DXANIMATIONCONTROLLER pAnimController)(NULL);

FLOAT (WINAPI *original_D3DXFresnelTerm
	   )(
	   FLOAT CosTheta,
	   FLOAT RefractionIndex
	   )(NULL);

HRESULT (WINAPI *original_D3DXGatherFragments)(
	LPCSTR						  pSrcData,
	UINT						 SrcDataLen,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXBUFFER*				ppShader,
	LPD3DXBUFFER*				ppErrorMsgs)(NULL);

HRESULT (WINAPI *original_D3DXGatherFragmentsFromFileA)(
	LPCSTR						  pSrcFile,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXBUFFER*				ppShader,
	LPD3DXBUFFER*				ppErrorMsgs)(NULL);

HRESULT (WINAPI *original_D3DXGatherFragmentsFromFileW)(
	LPCWSTR						 pSrcFile,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXBUFFER*				ppShader,
	LPD3DXBUFFER*				ppErrorMsgs)(NULL);

HRESULT (WINAPI *original_D3DXGatherFragmentsFromResourceA)(
	HMODULE						 hSrcModule,
	LPCSTR						  pSrcResource,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXBUFFER*				ppShader,
	LPD3DXBUFFER*				ppErrorMsgs)(NULL);

HRESULT (WINAPI *original_D3DXGatherFragmentsFromResourceW)(
	HMODULE						 hSrcModule,
	LPCWSTR						 pSrcResource,
	CONST D3DXMACRO*				pDefines,
	LPD3DXINCLUDE				pInclude,
	DWORD						Flags,
	LPD3DXBUFFER*				ppShader,
	LPD3DXBUFFER*				ppErrorMsgs)(NULL);

HRESULT (WINAPI *original_D3DXGenerateOutputDecl)(
	D3DVERTEXELEMENT9 *pOutput,
	CONST D3DVERTEXELEMENT9 *pInput)(NULL);

HRESULT (WINAPI *original_D3DXGeneratePMesh)(
	LPD3DXMESH pMesh,
	CONST DWORD* pAdjacency,
	CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,
	CONST FLOAT *pVertexWeights,
	DWORD MinValue,
	DWORD Options,
	LPD3DXPMESH* ppPMesh)(NULL);

UINT (WINAPI *original_D3DXGetDeclLength)(
	CONST D3DVERTEXELEMENT9 *pDecl)(NULL);

UINT (WINAPI *original_D3DXGetDeclVertexSize)(
	CONST D3DVERTEXELEMENT9 *pDecl,DWORD Stream)(NULL);

UINT (WINAPI *original_D3DXGetDriverLevel)(
	LPDIRECT3DDEVICE9 pDevice)(NULL);

UINT (WINAPI *original_D3DXGetFVFVertexSize)(
	DWORD FVF)(NULL);

HRESULT (WINAPI *original_D3DXGetImageInfoFromFileA)(
	LPCSTR				 pSrcFile,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

HRESULT (WINAPI *original_D3DXGetImageInfoFromFileInMemory)(
	LPCVOID				pSrcData,
	UINT				   SrcDataSize,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

HRESULT (WINAPI *original_D3DXGetImageInfoFromFileW)(
	LPCWSTR				pSrcFile,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

HRESULT (WINAPI *original_D3DXGetImageInfoFromResourceA)(
	HMODULE				hSrcModule,
	LPCSTR				 pSrcResource,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

HRESULT (WINAPI *original_D3DXGetImageInfoFromResourceW)(
	HMODULE				hSrcModule,
	LPCWSTR				pSrcResource,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

LPCSTR (WINAPI *original_D3DXGetPixelShaderProfile)(
		LPDIRECT3DDEVICE9		    pDevice)(NULL);

HRESULT (WINAPI *original_D3DXGetShaderConstantTable)(
	CONST DWORD*				 pFunction,
	LPD3DXCONSTANTTABLE*		 ppConstantTable)(NULL);

HRESULT (WINAPI *original_D3DXGetShaderInputSemantics)(
	CONST DWORD*				 pFunction,
	D3DXSEMANTIC*				pSemantics,
	UINT*						pCount)(NULL);

HRESULT (WINAPI *original_D3DXGetShaderOutputSemantics)(
	CONST DWORD*				 pFunction,
	D3DXSEMANTIC*				pSemantics,
	UINT*						pCount)(NULL);

HRESULT (WINAPI *original_D3DXGetShaderSamplers)(
	CONST DWORD*				 pFunction,
	LPCSTR*						 pSamplers,
	UINT*						pCount)(NULL);

UINT (WINAPI *original_D3DXGetShaderSize)(
	CONST DWORD*				 pFunction)(NULL);

DWORD (WINAPI *original_D3DXGetShaderVersion)(
	CONST DWORD*				 pFunction)(NULL);

LPCSTR (WINAPI *original_D3DXGetVertexShaderProfile)(
	LPDIRECT3DDEVICE9		    pDevice)(NULL);

HRESULT (WINAPI *original_D3DXIntersect)(
	LPD3DXBASEMESH pMesh,
	CONST D3DXVECTOR3 *pRayPos,
	CONST D3DXVECTOR3 *pRayDir,
	BOOL    *pHit,
	DWORD   *pFaceIndex,
	FLOAT   *pU,
	FLOAT   *pV,
	FLOAT   *pDist,
	LPD3DXBUFFER *ppAllHits,
	DWORD   *pCountOfHits)(NULL);

HRESULT (WINAPI *original_D3DXIntersectSubset)(
	LPD3DXBASEMESH pMesh,
	DWORD AttribId,
	CONST D3DXVECTOR3 *pRayPos,
	CONST D3DXVECTOR3 *pRayDir,
	BOOL    *pHit,
	DWORD   *pFaceIndex,
	FLOAT   *pU,
	FLOAT   *pV,
	FLOAT   *pDist,
	LPD3DXBUFFER *ppAllHits,
	DWORD   *pCountOfHits)(NULL);

BOOL (WINAPI *original_D3DXIntersectTri)(
	CONST D3DXVECTOR3 *p0,
	CONST D3DXVECTOR3 *p1,
	CONST D3DXVECTOR3 *p2,
	CONST D3DXVECTOR3 *pRayPos,
	CONST D3DXVECTOR3 *pRayDir,
	FLOAT *pU,
	FLOAT *pV,
	FLOAT *pDist)(NULL);

HRESULT (WINAPI *original_D3DXLoadMeshFromXA)(
	LPCSTR pFilename,
	DWORD Options,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXBUFFER *ppAdjacency,
	LPD3DXBUFFER *ppMaterials,
	LPD3DXBUFFER *ppEffectInstances,
	DWORD *pNumMaterials,
	LPD3DXMESH *ppMesh)(NULL);

HRESULT (WINAPI *original_D3DXLoadMeshFromXInMemory)(
	LPCVOID Memory,
	DWORD SizeOfMemory,
	DWORD Options,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXBUFFER *ppAdjacency,
	LPD3DXBUFFER *ppMaterials,
	LPD3DXBUFFER *ppEffectInstances,
	DWORD *pNumMaterials,
	LPD3DXMESH *ppMesh)(NULL);

HRESULT (WINAPI *original_D3DXLoadMeshFromXResource)(
	HMODULE Module,
	LPCSTR Name,
	LPCSTR Type,
	DWORD Options,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXBUFFER *ppAdjacency,
	LPD3DXBUFFER *ppMaterials,
	LPD3DXBUFFER *ppEffectInstances,
	DWORD *pNumMaterials,
	LPD3DXMESH *ppMesh)(NULL);

HRESULT (WINAPI *original_D3DXLoadMeshFromXW)(
	LPCWSTR pFilename,
	DWORD Options,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXBUFFER *ppAdjacency,
	LPD3DXBUFFER *ppMaterials,
	LPD3DXBUFFER *ppEffectInstances,
	DWORD *pNumMaterials,
	LPD3DXMESH *ppMesh)(NULL);

HRESULT (WINAPI *original_D3DXLoadMeshFromXof)(
	LPD3DXFILEDATA pxofMesh,
	DWORD Options,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXBUFFER *ppAdjacency,
	LPD3DXBUFFER *ppMaterials,
	LPD3DXBUFFER *ppEffectInstances,
	DWORD *pNumMaterials,
	LPD3DXMESH *ppMesh)(NULL);

HRESULT (WINAPI *original_D3DXLoadMeshHierarchyFromXA)(
	LPCSTR Filename,
	DWORD MeshOptions,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXALLOCATEHIERARCHY pAlloc,
	LPD3DXLOADUSERDATA pUserDataLoader,
	LPD3DXFRAME *ppFrameHierarchy,
	LPD3DXANIMATIONCONTROLLER *ppAnimController)(NULL);

HRESULT (WINAPI *original_D3DXLoadMeshHierarchyFromXInMemory)(
	LPCVOID Memory,
	DWORD SizeOfMemory,
	DWORD MeshOptions,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXALLOCATEHIERARCHY pAlloc,
	LPD3DXLOADUSERDATA pUserDataLoader,
	LPD3DXFRAME *ppFrameHierarchy,
	LPD3DXANIMATIONCONTROLLER *ppAnimController)(NULL);

HRESULT (WINAPI *original_D3DXLoadMeshHierarchyFromXW)(
	LPCWSTR Filename,
	DWORD MeshOptions,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXALLOCATEHIERARCHY pAlloc,
	LPD3DXLOADUSERDATA pUserDataLoader,
	LPD3DXFRAME *ppFrameHierarchy,
	LPD3DXANIMATIONCONTROLLER *ppAnimController)(NULL);

HRESULT (WINAPI *original_D3DXLoadPRTBufferFromFileA)(
	LPCSTR pFilename,
	LPD3DXPRTBUFFER*       ppBuffer)(NULL);

HRESULT (WINAPI *original_D3DXLoadPRTBufferFromFileW)(
	LPCWSTR pFilename,
	LPD3DXPRTBUFFER*       ppBuffer)(NULL);

HRESULT (WINAPI *original_D3DXLoadPRTCompBufferFromFileA)(
	LPCSTR pFilename,
	LPD3DXPRTCOMPBUFFER*       ppBuffer)(NULL);

HRESULT (WINAPI *original_D3DXLoadPRTCompBufferFromFileW)(
	LPCWSTR pFilename,
	LPD3DXPRTCOMPBUFFER*       ppBuffer)(NULL);

HRESULT (WINAPI *original_D3DXLoadPatchMeshFromXof)(
	LPD3DXFILEDATA pXofObjMesh,
	DWORD Options,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXBUFFER *ppMaterials,
	LPD3DXBUFFER *ppEffectInstances,
	PDWORD pNumMaterials,
	LPD3DXPATCHMESH *ppMesh)(NULL);

HRESULT (WINAPI *original_D3DXLoadSkinMeshFromXof)(
	LPD3DXFILEDATA pxofMesh,
	DWORD Options,
	LPDIRECT3DDEVICE9 pD3DDevice,
	LPD3DXBUFFER* ppAdjacency,
	LPD3DXBUFFER* ppMaterials,
	LPD3DXBUFFER *ppEffectInstances,
	DWORD *pMatOut,
	LPD3DXSKININFO* ppSkinInfo,
	LPD3DXMESH* ppMesh)(NULL);

HRESULT (WINAPI *original_D3DXLoadSurfaceFromFileA)(
	LPDIRECT3DSURFACE9		pDestSurface,
	CONST PALETTEENTRY*       pDestPalette,
	CONST RECT*		    pDestRect,
	LPCSTR				 pSrcFile,
	CONST RECT*		    pSrcRect,
	DWORD				  Filter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

HRESULT (WINAPI *original_D3DXLoadSurfaceFromFileInMemory)(
	LPDIRECT3DSURFACE9		pDestSurface,
	CONST PALETTEENTRY*       pDestPalette,
	CONST RECT*		    pDestRect,
	LPCVOID				pSrcData,
	UINT				   SrcDataSize,
	CONST RECT*		    pSrcRect,
	DWORD				  Filter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

HRESULT (WINAPI *original_D3DXLoadSurfaceFromFileW)(
	LPDIRECT3DSURFACE9		pDestSurface,
	CONST PALETTEENTRY*       pDestPalette,
	CONST RECT*		    pDestRect,
	LPCWSTR				pSrcFile,
	CONST RECT*		    pSrcRect,
	DWORD				  Filter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

HRESULT (WINAPI *original_D3DXLoadSurfaceFromMemory)(
	LPDIRECT3DSURFACE9		pDestSurface,
	CONST PALETTEENTRY*       pDestPalette,
	CONST RECT*		    pDestRect,
	LPCVOID				pSrcMemory,
	D3DFORMAT				 SrcFormat,
	UINT				   SrcPitch,
	CONST PALETTEENTRY*       pSrcPalette,
	CONST RECT*		    pSrcRect,
	DWORD				  Filter,
	D3DCOLOR				  ColorKey)(NULL);

HRESULT (WINAPI *original_D3DXLoadSurfaceFromResourceA)(
	LPDIRECT3DSURFACE9		pDestSurface,
	CONST PALETTEENTRY*       pDestPalette,
	CONST RECT*		    pDestRect,
	HMODULE				hSrcModule,
	LPCSTR				 pSrcResource,
	CONST RECT*		    pSrcRect,
	DWORD				  Filter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

HRESULT (WINAPI *original_D3DXLoadSurfaceFromResourceW)(
	LPDIRECT3DSURFACE9		pDestSurface,
	CONST PALETTEENTRY*       pDestPalette,
	CONST RECT*		    pDestRect,
	HMODULE				hSrcModule,
	LPCWSTR				pSrcResource,
	CONST RECT*		    pSrcRect,
	DWORD				  Filter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

HRESULT (WINAPI *original_D3DXLoadSurfaceFromSurface)(
	LPDIRECT3DSURFACE9		pDestSurface,
	CONST PALETTEENTRY*       pDestPalette,
	CONST RECT*		    pDestRect,
	LPDIRECT3DSURFACE9		pSrcSurface,
	CONST PALETTEENTRY*       pSrcPalette,
	CONST RECT*		    pSrcRect,
	DWORD				  Filter,
	D3DCOLOR				  ColorKey)(NULL);

HRESULT (WINAPI *original_D3DXLoadVolumeFromFileA)(
	LPDIRECT3DVOLUME9		 pDestVolume,
	CONST PALETTEENTRY*       pDestPalette,
	CONST D3DBOX*		  pDestBox,
	LPCSTR				 pSrcFile,
	CONST D3DBOX*		  pSrcBox,
	DWORD				  Filter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

HRESULT (WINAPI *original_D3DXLoadVolumeFromFileInMemory)(
	LPDIRECT3DVOLUME9		 pDestVolume,
	CONST PALETTEENTRY*       pDestPalette,
	CONST D3DBOX*		  pDestBox,
	LPCVOID				pSrcData,
	UINT				   SrcDataSize,
	CONST D3DBOX*		  pSrcBox,
	DWORD				  Filter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

HRESULT (WINAPI *original_D3DXLoadVolumeFromFileW)(
	LPDIRECT3DVOLUME9		 pDestVolume,
	CONST PALETTEENTRY*       pDestPalette,
	CONST D3DBOX*		  pDestBox,
	LPCWSTR				pSrcFile,
	CONST D3DBOX*		  pSrcBox,
	DWORD				  Filter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

HRESULT (WINAPI *original_D3DXLoadVolumeFromMemory)(
	LPDIRECT3DVOLUME9		 pDestVolume,
	CONST PALETTEENTRY*       pDestPalette,
	CONST D3DBOX*		  pDestBox,
	LPCVOID				pSrcMemory,
	D3DFORMAT				 SrcFormat,
	UINT				   SrcRowPitch,
	UINT				   SrcSlicePitch,
	CONST PALETTEENTRY*       pSrcPalette,
	CONST D3DBOX*		  pSrcBox,
	DWORD				  Filter,
	D3DCOLOR				  ColorKey)(NULL);

HRESULT (WINAPI *original_D3DXLoadVolumeFromResourceA)(
	LPDIRECT3DVOLUME9		 pDestVolume,
	CONST PALETTEENTRY*       pDestPalette,
	CONST D3DBOX*		  pDestBox,
	HMODULE				hSrcModule,
	LPCSTR				 pSrcResource,
	CONST D3DBOX*		  pSrcBox,
	DWORD				  Filter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

HRESULT (WINAPI *original_D3DXLoadVolumeFromResourceW)(
	LPDIRECT3DVOLUME9		 pDestVolume,
	CONST PALETTEENTRY*       pDestPalette,
	CONST D3DBOX*		  pDestBox,
	HMODULE				hSrcModule,
	LPCWSTR				pSrcResource,
	CONST D3DBOX*		  pSrcBox,
	DWORD				  Filter,
	D3DCOLOR				  ColorKey,
	D3DXIMAGE_INFO*		pSrcInfo)(NULL);

HRESULT (WINAPI *original_D3DXLoadVolumeFromVolume)(
	LPDIRECT3DVOLUME9		 pDestVolume,
	CONST PALETTEENTRY*       pDestPalette,
	CONST D3DBOX*		  pDestBox,
	LPDIRECT3DVOLUME9		 pSrcVolume,
	CONST PALETTEENTRY*       pSrcPalette,
	CONST D3DBOX*		  pSrcBox,
	DWORD				  Filter,
	D3DCOLOR				  ColorKey)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixAffineTransformation)(
	D3DXMATRIX *pOut,
	FLOAT Scaling,
	CONST D3DXVECTOR3 *pRotationCenter,
	CONST D3DXQUATERNION *pRotation,
	CONST D3DXVECTOR3 *pTranslation)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixAffineTransformation2D)(
	D3DXMATRIX *pOut,
	FLOAT Scaling,
	CONST D3DXVECTOR2* pRotationCenter,
	FLOAT Rotation,
	CONST D3DXVECTOR2* pTranslation)(NULL);

HRESULT (WINAPI *original_D3DXMatrixDecompose)(
	D3DXVECTOR3 *pOutScale,
	D3DXQUATERNION *pOutRotation,
	D3DXVECTOR3 *pOutTranslation,
	CONST D3DXMATRIX *pM)(NULL);

FLOAT (WINAPI *original_D3DXMatrixDeterminant)(
	CONST D3DXMATRIX *pM
	)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixInverse)(
	D3DXMATRIX *pOut,
	FLOAT *pDeterminant,
	CONST D3DXMATRIX *pM
	)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixLookAtLH)(
	D3DXMATRIX *pOut,
	CONST D3DXVECTOR3 *pEye,
	CONST D3DXVECTOR3 *pAt,
	CONST D3DXVECTOR3 *pUp)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixLookAtRH)(
	D3DXMATRIX *pOut,
	CONST D3DXVECTOR3 *pEye,
	CONST D3DXVECTOR3 *pAt,
	CONST D3DXVECTOR3 *pUp)(NULL);

D3DXMATRIX * (WINAPI *original_D3DXMatrixMultiply
	 )	(D3DXMATRIX *pOut,
	 CONST D3DXMATRIX *pM1,
	 CONST D3DXMATRIX *pM2
	 )(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixMultiplyTranspose)(
	D3DXMATRIX *pOut,
	CONST D3DXMATRIX *pM1,
	CONST D3DXMATRIX *pM2)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixOrthoLH)(
	D3DXMATRIX *pOut,
	FLOAT w,
	FLOAT h,
	FLOAT zn,
	FLOAT zf)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixOrthoOffCenterLH)(
	D3DXMATRIX *pOut,
	FLOAT l,
	FLOAT r,
	FLOAT b,
	FLOAT t,
	FLOAT zn,
	FLOAT zf)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixOrthoOffCenterRH)(
	D3DXMATRIX *pOut,
	FLOAT l,
	FLOAT r,
	FLOAT b,
	FLOAT t,
	FLOAT zn,
	FLOAT zf)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixOrthoRH)(
	D3DXMATRIX *pOut,
	FLOAT w,
	FLOAT h,
	FLOAT zn,
	FLOAT zf)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixPerspectiveFovLH)(
	D3DXMATRIX *pOut,
	FLOAT fovy,
	FLOAT Aspect,
	FLOAT zn,
	FLOAT zf)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixPerspectiveFovRH)(
	D3DXMATRIX *pOut,
	FLOAT fovy,
	FLOAT Aspect,
	FLOAT zn,
	FLOAT zf)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixPerspectiveLH)(
	D3DXMATRIX *pOut,
	FLOAT w,
	FLOAT h,
	FLOAT zn,
	FLOAT zf)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixPerspectiveOffCenterLH)(
	D3DXMATRIX *pOut,
	FLOAT l,
	FLOAT r,
	FLOAT b,
	FLOAT t,
	FLOAT zn,
	FLOAT zf)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixPerspectiveOffCenterRH)(
	D3DXMATRIX *pOut,
	FLOAT l,
	FLOAT r,
	FLOAT b,
	FLOAT t,
	FLOAT zn,
	FLOAT zf)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixPerspectiveRH)(
	D3DXMATRIX *pOut,
	FLOAT w,
	FLOAT h,
	FLOAT zn,
	FLOAT zf)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixReflect)(
	D3DXMATRIX *pOut,
	CONST D3DXPLANE *pPlane)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixRotationAxis)(
	D3DXMATRIX *pOut,
	CONST D3DXVECTOR3 *pV,
	FLOAT Angle)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixRotationQuaternion)(
	D3DXMATRIX *pOut,
	CONST D3DXQUATERNION *pQ)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixRotationX)(
	D3DXMATRIX *pOut,
	FLOAT Angle)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixRotationY)(
	D3DXMATRIX *pOut,
	FLOAT Angle)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixRotationYawPitchRoll)(
	D3DXMATRIX *pOut,
	FLOAT Yaw,
	FLOAT Pitch,
	FLOAT Roll)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixRotationZ)(
	D3DXMATRIX *pOut,
	FLOAT Angle)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixScaling)(
	D3DXMATRIX *pOut,
	FLOAT sx,
	FLOAT sy,
	FLOAT sz)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixShadow)(
	D3DXMATRIX *pOut,
	CONST D3DXVECTOR4 *pLight,
	CONST D3DXPLANE *pPlane)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixTransformation)(
	D3DXMATRIX *pOut,
	CONST D3DXVECTOR3 *pScalingCenter,
	CONST D3DXQUATERNION *pScalingRotation,
	CONST D3DXVECTOR3 *pScaling,
	CONST D3DXVECTOR3 *pRotationCenter,
	CONST D3DXQUATERNION *pRotation,
	CONST D3DXVECTOR3 *pTranslation)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixTransformation2D)(
	D3DXMATRIX *pOut,
	CONST D3DXVECTOR2* pScalingCenter,
	FLOAT ScalingRotation,
	CONST D3DXVECTOR2* pScaling,
	CONST  D3DXVECTOR2* pRotationCenter,
	FLOAT Rotation,
	CONST  D3DXVECTOR2* pTranslation)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixTranslation)(
	D3DXMATRIX *pOut,
	FLOAT x,
	FLOAT y,
	FLOAT z)(NULL);

D3DXMATRIX* (WINAPI *original_D3DXMatrixTranspose)(
	D3DXMATRIX *pOut,
	CONST D3DXMATRIX *pM)(NULL);

HRESULT (WINAPI *original_D3DXOptimizeFaces)(
	LPCVOID pbIndices,
	UINT cFaces,
	UINT cVertices,
	BOOL b32BitIndices,
	DWORD* pFaceRemap)(NULL);

HRESULT (WINAPI *original_D3DXOptimizeVertices)(
	LPCVOID pbIndices,
	UINT cFaces,
	UINT cVertices,
	BOOL b32BitIndices,
	DWORD* pVertexRemap)(NULL);

D3DXPLANE* (WINAPI *original_D3DXPlaneFromPointNormal)(
	D3DXPLANE *pOut,
	CONST D3DXVECTOR3 *pPoint,
	CONST D3DXVECTOR3 *pNormal)(NULL);

D3DXPLANE* (WINAPI *original_D3DXPlaneFromPoints)(
	D3DXPLANE *pOut,
	CONST D3DXVECTOR3 *pV1,
	CONST D3DXVECTOR3 *pV2,
	CONST D3DXVECTOR3 *pV3)(NULL);

D3DXVECTOR3* (WINAPI *original_D3DXPlaneIntersectLine)(
	D3DXVECTOR3 *pOut,
	CONST D3DXPLANE *pP,
	CONST D3DXVECTOR3 *pV1,
	CONST D3DXVECTOR3 *pV2)(NULL); 

D3DXPLANE* (WINAPI *original_D3DXPlaneNormalize)(
	D3DXPLANE *pOut,
	CONST D3DXPLANE *pP)(NULL);

D3DXPLANE* (WINAPI *original_D3DXPlaneTransform)(
	D3DXPLANE *pOut,
	CONST D3DXPLANE *pP,
	CONST D3DXMATRIX *pM)(NULL);

D3DXPLANE* (WINAPI *original_D3DXPlaneTransformArray)(
	D3DXPLANE *pOut,
	UINT OutStride,
	CONST D3DXPLANE *pP,
	UINT PStride,
	CONST D3DXMATRIX *pM,
	UINT n)(NULL);

HRESULT (WINAPI *original_D3DXPreprocessShader)(
	LPCSTR				    pSrcData,
	UINT						 SrcDataSize,
	CONST D3DXMACRO*		  pDefines,
	LPD3DXINCLUDE				pInclude,
	LPD3DXBUFFER*				ppShaderText,
	LPD3DXBUFFER*				ppErrorMsgs)(NULL);

HRESULT (WINAPI *original_D3DXPreprocessShaderFromFileA)(
	LPCSTR				    pSrcFile,
	CONST D3DXMACRO*		  pDefines,
	LPD3DXINCLUDE				pInclude,
	LPD3DXBUFFER*				ppShaderText,
	LPD3DXBUFFER*				ppErrorMsgs)(NULL);

HRESULT (WINAPI *original_D3DXPreprocessShaderFromFileW)(
	LPCWSTR				   pSrcFile,
	CONST D3DXMACRO*		  pDefines,
	LPD3DXINCLUDE				pInclude,
	LPD3DXBUFFER*				ppShaderText,
	LPD3DXBUFFER*				ppErrorMsgs)(NULL);

HRESULT (WINAPI *original_D3DXPreprocessShaderFromResourceA)(
	HMODULE				   hSrcModule,
	LPCSTR				    pSrcResource,
	CONST D3DXMACRO*		  pDefines,
	LPD3DXINCLUDE				pInclude,
	LPD3DXBUFFER*				ppShaderText,
	LPD3DXBUFFER*				ppErrorMsgs)(NULL);

HRESULT (WINAPI *original_D3DXPreprocessShaderFromResourceW)(
	HMODULE				   hSrcModule,
	LPCWSTR				   pSrcResource,
	CONST D3DXMACRO*		  pDefines,
	LPD3DXINCLUDE				pInclude,
	LPD3DXBUFFER*				ppShaderText,
	LPD3DXBUFFER*				ppErrorMsgs)(NULL);

D3DXQUATERNION* (WINAPI *original_D3DXQuaternionBaryCentric)(
	D3DXQUATERNION *pOut,
	CONST D3DXQUATERNION *pQ1,
	CONST D3DXQUATERNION *pQ2,
	CONST D3DXQUATERNION *pQ3,
	FLOAT f,
	FLOAT g)(NULL);

D3DXQUATERNION* (WINAPI *original_D3DXQuaternionExp)(
	D3DXQUATERNION *pOut,
	CONST D3DXQUATERNION *pQ)(NULL);

D3DXQUATERNION* (WINAPI *original_D3DXQuaternionInverse)(
	D3DXQUATERNION *pOut,
	CONST D3DXQUATERNION *pQ)(NULL);

D3DXQUATERNION* (WINAPI *original_D3DXQuaternionLn)(
	D3DXQUATERNION *pOut,
	CONST D3DXQUATERNION *pQ)(NULL);

D3DXQUATERNION* (WINAPI *original_D3DXQuaternionMultiply)(
	D3DXQUATERNION *pOut,
	CONST D3DXQUATERNION *pQ1,
	CONST D3DXQUATERNION *pQ2)(NULL);

D3DXQUATERNION* (WINAPI *original_D3DXQuaternionNormalize)(
	D3DXQUATERNION *pOut,
	CONST D3DXQUATERNION *pQ)(NULL);

D3DXQUATERNION* (WINAPI *original_D3DXQuaternionRotationAxis)(
	D3DXQUATERNION *pOut,
	CONST D3DXVECTOR3 *pV,
	FLOAT Angle)(NULL);

D3DXQUATERNION* (WINAPI *original_D3DXQuaternionRotationMatrix)(
	D3DXQUATERNION *pOut,
	CONST D3DXMATRIX *pM)(NULL);

D3DXQUATERNION* (WINAPI *original_D3DXQuaternionRotationYawPitchRoll)(
	D3DXQUATERNION *pOut,
	FLOAT Yaw,
	FLOAT Pitch,
	FLOAT Roll)(NULL);

D3DXQUATERNION* (WINAPI *original_D3DXQuaternionSlerp)(
	D3DXQUATERNION *pOut,
	CONST D3DXQUATERNION *pQ1,
	CONST D3DXQUATERNION *pQ2,
	FLOAT t)(NULL);

D3DXQUATERNION* (WINAPI *original_D3DXQuaternionSquad)(
	D3DXQUATERNION *pOut,
	CONST D3DXQUATERNION *pQ1,
	CONST D3DXQUATERNION *pA,
	CONST D3DXQUATERNION *pB,
	CONST D3DXQUATERNION *pC,
	FLOAT t)(NULL);

void (WINAPI *original_D3DXQuaternionSquadSetup)(
	D3DXQUATERNION *pAOut,
	D3DXQUATERNION *pBOut,
	D3DXQUATERNION *pCOut,
	CONST D3DXQUATERNION *pQ0,
	CONST D3DXQUATERNION *pQ1,
	CONST D3DXQUATERNION *pQ2,
	CONST D3DXQUATERNION *pQ3)(NULL);

void (WINAPI *original_D3DXQuaternionToAxisAngle)(
	CONST D3DXQUATERNION *pQ,
	D3DXVECTOR3 *pAxis,
	FLOAT *pAngle)(NULL);

HRESULT (WINAPI *original_D3DXRectPatchSize)(
	CONST FLOAT *pfNumSegs,
	DWORD *pdwTriangles,
	DWORD *pdwVertices)(NULL);

FLOAT* (WINAPI *original_D3DXSHAdd)(
	FLOAT *pOut,
	UINT Order,
	CONST FLOAT *pA,
	CONST FLOAT *pB)(NULL);

FLOAT (WINAPI *original_D3DXSHDot)(
	UINT Order,
	CONST FLOAT *pA,
	CONST FLOAT *pB
	)(NULL);

HRESULT (WINAPI *original_D3DXSHEvalConeLight)(
	UINT Order,
	CONST D3DXVECTOR3 *pDir,
	FLOAT Radius,
	FLOAT RIntensity,
	FLOAT GIntensity,
	FLOAT BIntensity,
	FLOAT *pROut,
	FLOAT *pGOut,
	FLOAT *pBOut)(NULL);

FLOAT* (WINAPI *original_D3DXSHEvalDirection)(
		FLOAT *pOut,
		UINT Order,
		CONST D3DXVECTOR3 *pDir)(NULL);

HRESULT (WINAPI *original_D3DXSHEvalDirectionalLight)(
	UINT Order,
	CONST D3DXVECTOR3 *pDir,
	FLOAT RIntensity,
	FLOAT GIntensity,
	FLOAT BIntensity,
	FLOAT *pROut,
	FLOAT *pGOut,
	FLOAT *pBOut)(NULL);

HRESULT (WINAPI *original_D3DXSHEvalHemisphereLight)(
	UINT Order,
	CONST D3DXVECTOR3 *pDir,
	D3DXCOLOR Top,
	D3DXCOLOR Bottom,
	FLOAT *pROut,
	FLOAT *pGOut,
	FLOAT *pBOut)(NULL);

HRESULT (WINAPI *original_D3DXSHEvalSphericalLight)(
	UINT Order,
	CONST D3DXVECTOR3 *pPos,
	FLOAT Radius,
	FLOAT RIntensity,
	FLOAT GIntensity,
	FLOAT BIntensity,
	FLOAT *pROut,
	FLOAT *pGOut,
	FLOAT *pBOut)(NULL);

FLOAT* (WINAPI *original_D3DXSHMultiply2)(
	FLOAT *pOut,
	CONST FLOAT *pF,
	CONST FLOAT *pG)(NULL);

FLOAT* (WINAPI *original_D3DXSHMultiply3)(
	FLOAT *pOut,
	CONST FLOAT *pF,
	CONST FLOAT *pG)(NULL);

FLOAT* (WINAPI *original_D3DXSHMultiply4)(
	FLOAT *pOut,
	CONST FLOAT *pF,
	CONST FLOAT *pG)(NULL);

FLOAT* (WINAPI *original_D3DXSHMultiply5)(
	FLOAT *pOut,
	CONST FLOAT *pF,
	CONST FLOAT *pG)(NULL);

FLOAT* (WINAPI *original_D3DXSHMultiply6)(
	FLOAT *pOut,
	CONST FLOAT *pF,
	CONST FLOAT *pG)(NULL);

HRESULT (WINAPI *original_D3DXSHPRTCompSplitMeshSC)(
	UINT *pClusterIDs,
	UINT NumVertices,
	UINT NumClusters,
	UINT *pSuperClusterIDs,
	UINT NumSuperClusters,
	LPVOID pInputIB,
	BOOL InputIBIs32Bit,
	UINT NumFaces,
	LPD3DXBUFFER *ppIBData,
	UINT *pIBDataLength,
	BOOL OutputIBIs32Bit,
	LPD3DXBUFFER *ppFaceRemap,
	LPD3DXBUFFER *ppVertData,
	UINT *pVertDataLength,
	UINT *pSCClusterList,
	D3DXSHPRTSPLITMESHCLUSTERDATA *pSCData)(NULL);

HRESULT (WINAPI *original_D3DXSHPRTCompSuperCluster)(
	UINT *pClusterIDs,
	LPD3DXMESH pScene,
	UINT MaxNumClusters,
	UINT NumClusters,
	UINT *pSuperClusterIDs,
	UINT *pNumSuperClusters)(NULL);

HRESULT (WINAPI *original_D3DXSHProjectCubeMap)(
	UINT uOrder, LPDIRECT3DCUBETEXTURE9 pCubeMap,
	FLOAT *pROut,
	FLOAT *pGOut,
	FLOAT *pBOut)(NULL);

FLOAT* (WINAPI *original_D3DXSHRotate)(
	FLOAT *pOut,
	UINT Order,
	CONST D3DXMATRIX *pMatrix,
	CONST FLOAT *pIn)(NULL);

FLOAT* (WINAPI *original_D3DXSHRotateZ)(
	FLOAT *pOut,
	UINT Order,
	FLOAT Angle,
	CONST FLOAT *pIn)(NULL);

FLOAT* (WINAPI *original_D3DXSHScale)(
	FLOAT *pOut,
	UINT Order,
	CONST FLOAT *pIn,
	CONST FLOAT Scale)(NULL);

HRESULT (WINAPI *original_D3DXSaveMeshHierarchyToFileA)(
	LPCSTR Filename,
	DWORD XFormat,
	CONST D3DXFRAME *pFrameRoot,
	LPD3DXANIMATIONCONTROLLER pAnimcontroller,
	LPD3DXSAVEUSERDATA pUserDataSaver)(NULL);

HRESULT (WINAPI *original_D3DXSaveMeshHierarchyToFileW)(
	LPCWSTR Filename,
	DWORD XFormat,
	CONST D3DXFRAME *pFrameRoot,
	LPD3DXANIMATIONCONTROLLER pAnimController,
	LPD3DXSAVEUSERDATA pUserDataSaver)(NULL);

HRESULT (WINAPI *original_D3DXSaveMeshToXA)(
	LPCSTR pFilename,
	LPD3DXMESH pMesh,
	CONST DWORD* pAdjacency,
	CONST D3DXMATERIAL* pMaterials,
	CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials,
	DWORD Format)(NULL);

HRESULT (WINAPI *original_D3DXSaveMeshToXW)(
	LPCWSTR pFilename,
	LPD3DXMESH pMesh,
	CONST DWORD* pAdjacency,
	CONST D3DXMATERIAL* pMaterials,
	CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials,
	DWORD Format)(NULL);

HRESULT (WINAPI *original_D3DXSavePRTBufferToFileA)(
	LPCSTR pFileName,
	LPD3DXPRTBUFFER pBuffer)(NULL);

HRESULT (WINAPI *original_D3DXSavePRTBufferToFileW)(
	LPCWSTR pFileName,
	LPD3DXPRTBUFFER pBuffer)(NULL);

HRESULT (WINAPI *original_D3DXSavePRTCompBufferToFileA)(
	LPCSTR pFileName,
	LPD3DXPRTCOMPBUFFER pBuffer)(NULL);

HRESULT (WINAPI *original_D3DXSavePRTCompBufferToFileW)(
	LPCWSTR pFileName,
	LPD3DXPRTCOMPBUFFER pBuffer)(NULL);

HRESULT (WINAPI *original_D3DXSaveSurfaceToFileA)(
	LPCSTR				 pDestFile,
	D3DXIMAGE_FILEFORMAT      DestFormat,
	LPDIRECT3DSURFACE9		pSrcSurface,
	CONST PALETTEENTRY*       pSrcPalette,
	CONST RECT*		    pSrcRect)(NULL);

HRESULT (WINAPI *original_D3DXSaveSurfaceToFileInMemory)(
	LPD3DXBUFFER*		  ppDestBuf,
	D3DXIMAGE_FILEFORMAT      DestFormat,
	LPDIRECT3DSURFACE9		pSrcSurface,
	CONST PALETTEENTRY*       pSrcPalette,
	CONST RECT*		    pSrcRect)(NULL);

HRESULT (WINAPI *original_D3DXSaveSurfaceToFileW)(
	LPCWSTR				pDestFile,
	D3DXIMAGE_FILEFORMAT      DestFormat,
	LPDIRECT3DSURFACE9		pSrcSurface,
	CONST PALETTEENTRY*       pSrcPalette,
	CONST RECT*		    pSrcRect)(NULL);

HRESULT (WINAPI *original_D3DXSaveTextureToFileA)(
	LPCSTR				 pDestFile,
	D3DXIMAGE_FILEFORMAT      DestFormat,
	LPDIRECT3DBASETEXTURE9    pSrcTexture,
	CONST PALETTEENTRY*       pSrcPalette)(NULL);

HRESULT (WINAPI *original_D3DXSaveTextureToFileInMemory)(
	LPD3DXBUFFER*		  ppDestBuf,
	D3DXIMAGE_FILEFORMAT      DestFormat,
	LPDIRECT3DBASETEXTURE9    pSrcTexture,
	CONST PALETTEENTRY*       pSrcPalette)(NULL);

HRESULT (WINAPI *original_D3DXSaveTextureToFileW)(
	LPCWSTR				pDestFile,
	D3DXIMAGE_FILEFORMAT      DestFormat,
	LPDIRECT3DBASETEXTURE9    pSrcTexture,
	CONST PALETTEENTRY*       pSrcPalette)(NULL);

HRESULT (WINAPI *original_D3DXSaveVolumeToFileA)(
	LPCSTR				 pDestFile,
	D3DXIMAGE_FILEFORMAT      DestFormat,
	LPDIRECT3DVOLUME9		 pSrcVolume,
	CONST PALETTEENTRY*       pSrcPalette,
	CONST D3DBOX*		  pSrcBox)(NULL);

HRESULT (WINAPI *original_D3DXSaveVolumeToFileInMemory)(
	LPD3DXBUFFER*		  ppDestBuf,
	D3DXIMAGE_FILEFORMAT      DestFormat,
	LPDIRECT3DVOLUME9		 pSrcVolume,
	CONST PALETTEENTRY*       pSrcPalette,
	CONST D3DBOX*		  pSrcBox)(NULL);

HRESULT (WINAPI *original_D3DXSaveVolumeToFileW)(
	LPCWSTR				pDestFile,
	D3DXIMAGE_FILEFORMAT      DestFormat,
	LPDIRECT3DVOLUME9		 pSrcVolume,
	CONST PALETTEENTRY*       pSrcPalette,
	CONST D3DBOX*		  pSrcBox)(NULL);

HRESULT (WINAPI *original_D3DXSimplifyMesh)(
	LPD3DXMESH pMesh,
	CONST DWORD* pAdjacency,
	CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,
	CONST FLOAT *pVertexWeights,
	DWORD MinValue,
	DWORD Options,
	LPD3DXMESH* ppMesh)(NULL);

BOOL (WINAPI *original_D3DXSphereBoundProbe)(
	CONST D3DXVECTOR3 *pCenter,
	FLOAT Radius,
	CONST D3DXVECTOR3 *pRayPosition,
	CONST D3DXVECTOR3 *pRayDirection)(NULL);

HRESULT (WINAPI *original_D3DXSplitMesh)(
	LPD3DXMESH pMeshIn,
	CONST DWORD *pAdjacencyIn,
	CONST DWORD MaxSize,
	CONST DWORD Options,
	DWORD *pMeshesOut,
	LPD3DXBUFFER *ppMeshArrayOut,
	LPD3DXBUFFER *ppAdjacencyArrayOut,
	LPD3DXBUFFER *ppFaceRemapArrayOut,
	LPD3DXBUFFER *ppVertRemapArrayOut)(NULL);

HRESULT (WINAPI *original_D3DXTessellateNPatches)(
	LPD3DXMESH pMeshIn,
	CONST DWORD* pAdjacencyIn,
	FLOAT NumSegs,
	BOOL  QuadraticInterpNormals,
	LPD3DXMESH *ppMeshOut,
	LPD3DXBUFFER *ppAdjacencyOut)(NULL);

HRESULT (WINAPI *original_D3DXTessellateRectPatch)(
	LPDIRECT3DVERTEXBUFFER9 pVB,
	CONST FLOAT *pNumSegs,
	CONST D3DVERTEXELEMENT9 *pdwInDecl,
	CONST D3DRECTPATCH_INFO *pRectPatchInfo,
	LPD3DXMESH pMesh)(NULL);

HRESULT (WINAPI *original_D3DXTessellateTriPatch)(
	LPDIRECT3DVERTEXBUFFER9 pVB,
	CONST FLOAT *pNumSegs,
	CONST D3DVERTEXELEMENT9 *pInDecl,
	CONST D3DTRIPATCH_INFO *pTriPatchInfo,
	LPD3DXMESH pMesh)(NULL);

HRESULT (WINAPI *original_D3DXTriPatchSize)(
	CONST FLOAT *pfNumSegs,
	DWORD *pdwTriangles,
	DWORD *pdwVertices)(NULL);

HRESULT (WINAPI *original_D3DXUVAtlasCreate)(
	LPD3DXMESH pMesh,
	UINT uMaxChartNumber,
	FLOAT fMaxStretch,
	UINT uWidth,
	UINT uHeight,
	FLOAT fGutter,
	DWORD dwTextureIndex,
	CONST DWORD *pdwAdjacency,
	CONST DWORD *pdwFalseEdgeAdjacency,
	CONST FLOAT *pfIMTArray,
	LPD3DXUVATLASCB pStatusCallback,
	FLOAT fCallbackFrequency,
	LPVOID pUserContext,
	LPD3DXMESH *ppMeshOut,
	LPD3DXBUFFER *ppFacePartitioning,
	LPD3DXBUFFER *ppVertexRemapArray,
	FLOAT *pfMaxStretchOut,
	UINT *puNumChartsOut)(NULL);

HRESULT (WINAPI *original_D3DXUVAtlasPack)(
	ID3DXMesh *pMesh,
	UINT uWidth,
	UINT uHeight,
	FLOAT fGutter,
	DWORD dwTextureIndex,
	CONST DWORD *pdwPartitionResultAdjacency,
	LPD3DXUVATLASCB pStatusCallback,
	FLOAT fCallbackFrequency,
	LPVOID pUserContext,
	LPD3DXBUFFER pFacePartitioning)(NULL);

HRESULT (WINAPI *original_D3DXUVAtlasPartition)(
	LPD3DXMESH pMesh,
	UINT uMaxChartNumber,
	FLOAT fMaxStretch,
	DWORD dwTextureIndex,
	CONST DWORD *pdwAdjacency,
	CONST DWORD *pdwFalseEdgeAdjacency,
	CONST FLOAT *pfIMTArray,
	LPD3DXUVATLASCB pStatusCallback,
	FLOAT fCallbackFrequency,
	LPVOID pUserContext,
	LPD3DXMESH *ppMeshOut,
	LPD3DXBUFFER *ppFacePartitioning,
	LPD3DXBUFFER *ppVertexRemapArray,
	LPD3DXBUFFER *ppPartitionResultAdjacency,
	FLOAT *pfMaxStretchOut,
	UINT *puNumChartsOut)(NULL);

HRESULT (WINAPI *original_D3DXValidMesh)(
	LPD3DXMESH pMeshIn,
	CONST DWORD* pAdjacency,
	LPD3DXBUFFER* ppErrorsAndWarnings)(NULL);

HRESULT (WINAPI *original_D3DXValidPatchMesh)(
	LPD3DXPATCHMESH pMesh,
	DWORD *dwcDegenerateVertices,
	DWORD *dwcDegeneratePatches,
	LPD3DXBUFFER *ppErrorsAndWarnings)(NULL);

D3DXVECTOR2* (WINAPI *original_D3DXVec2BaryCentric)(
	D3DXVECTOR2 *pOut,
	CONST D3DXVECTOR2 *pV1,
	CONST D3DXVECTOR2 *pV2,
	CONST D3DXVECTOR2 *pV3,
	FLOAT f,
	FLOAT g)(NULL); 

D3DXVECTOR2* (WINAPI *original_D3DXVec2CatmullRom)(
	D3DXVECTOR2 *pOut,
	CONST D3DXVECTOR2 *pV0,
	CONST D3DXVECTOR2 *pV1,
	CONST D3DXVECTOR2 *pV2,
	CONST D3DXVECTOR2 *pV3,
	FLOAT s)(NULL); 

D3DXVECTOR2* (WINAPI *original_D3DXVec2Hermite)(
	D3DXVECTOR2 *pOut,
	CONST D3DXVECTOR2 *pV1,
	CONST D3DXVECTOR2 *pT1,
	CONST D3DXVECTOR2 *pV2,
	CONST D3DXVECTOR2 *pT2,
	FLOAT s)(NULL); 

D3DXVECTOR2* (WINAPI *original_D3DXVec2Normalize)(
	D3DXVECTOR2 *pOut,
	CONST D3DXVECTOR2 *pV)(NULL); 

D3DXVECTOR4* (WINAPI *original_D3DXVec2Transform)(
	D3DXVECTOR4 *pOut,
	CONST D3DXVECTOR2 *pV,
	CONST D3DXMATRIX *pM)(NULL); 

D3DXVECTOR4* (WINAPI *original_D3DXVec2TransformArray)(
	D3DXVECTOR4 *pOut,
	UINT OutStride,
	CONST D3DXVECTOR2 *pV,
	UINT VStride,
	CONST D3DXMATRIX *pM,
	UINT n)(NULL); 

D3DXVECTOR2* (WINAPI *original_D3DXVec2TransformCoord)(
	D3DXVECTOR2 *pOut,
	CONST D3DXVECTOR2 *pV,
	CONST D3DXMATRIX *pM)(NULL); 

D3DXVECTOR2* (WINAPI *original_D3DXVec2TransformCoordArray)(
	D3DXVECTOR2 *pOut,
	UINT OutStride,
	CONST D3DXVECTOR2 *pV,
	UINT VStride,
	CONST D3DXMATRIX *pM,
	UINT n)(NULL); 

D3DXVECTOR2* (WINAPI *original_D3DXVec2TransformNormal)(
	D3DXVECTOR2 *pOut,
	CONST D3DXVECTOR2 *pV,
	CONST D3DXMATRIX *pM)(NULL); 

D3DXVECTOR2* (WINAPI *original_D3DXVec2TransformNormalArray)(
	D3DXVECTOR2 *pOut,
	UINT OutStride,
	CONST D3DXVECTOR2 *pV,
	UINT VStride,
	CONST D3DXMATRIX *pM,
	UINT n)(NULL); 

D3DXVECTOR3* (WINAPI *original_D3DXVec3BaryCentric)(
	D3DXVECTOR3 *pOut,
	CONST D3DXVECTOR3 *pV1,
	CONST D3DXVECTOR3 *pV2,
	CONST D3DXVECTOR3 *pV3,
	FLOAT f,
	FLOAT g)(NULL); 

D3DXVECTOR3* (WINAPI *original_D3DXVec3CatmullRom)(
	D3DXVECTOR3 *pOut,
	CONST D3DXVECTOR3 *pV0,
	CONST D3DXVECTOR3 *pV1,
	CONST D3DXVECTOR3 *pV2,
	CONST D3DXVECTOR3 *pV3,
	FLOAT s)(NULL); 

D3DXVECTOR3* (WINAPI *original_D3DXVec3Hermite)(
	D3DXVECTOR3 *pOut,
	CONST D3DXVECTOR3 *pV1,
	CONST D3DXVECTOR3 *pT1,
	CONST D3DXVECTOR3 *pV2,
	CONST D3DXVECTOR3 *pT2,
	FLOAT s)(NULL); 

D3DXVECTOR3* (WINAPI *original_D3DXVec3Normalize)(
	D3DXVECTOR3 *pOut,
	CONST D3DXVECTOR3 *pV)(NULL); 

D3DXVECTOR3* (WINAPI *original_D3DXVec3Project)(
	D3DXVECTOR3 *pOut,
	CONST D3DXVECTOR3 *pV,
	CONST D3DVIEWPORT9 *pViewport,
	CONST D3DXMATRIX *pProjection,
	CONST D3DXMATRIX *pView,
	CONST D3DXMATRIX *pWorld)(NULL); 

D3DXVECTOR3* (WINAPI *original_D3DXVec3ProjectArray)(
	D3DXVECTOR3 *pOut,
	UINT OutStride,CONST D3DXVECTOR3 *pV,
	UINT VStride,
	CONST D3DVIEWPORT9 *pViewport,
	CONST D3DXMATRIX *pProjection,
	CONST D3DXMATRIX *pView,
	CONST D3DXMATRIX *pWorld,
	UINT n)(NULL); 

D3DXVECTOR4* (WINAPI *original_D3DXVec3Transform)(
	D3DXVECTOR4 *pOut,
	CONST D3DXVECTOR3 *pV,
	CONST D3DXMATRIX *pM)(NULL); 

D3DXVECTOR4* (WINAPI *original_D3DXVec3TransformArray)(
	D3DXVECTOR4 *pOut,
	UINT OutStride,
	CONST D3DXVECTOR3 *pV,
	UINT VStride,
	CONST D3DXMATRIX *pM,
	UINT n)(NULL); 

D3DXVECTOR3* (WINAPI *original_D3DXVec3TransformCoord)(
	D3DXVECTOR3 *pOut,
	CONST D3DXVECTOR3 *pV,
	CONST D3DXMATRIX *pM)(NULL); 

D3DXVECTOR3* (WINAPI *original_D3DXVec3TransformCoordArray)(
	D3DXVECTOR3 *pOut,
	UINT OutStride,
	CONST D3DXVECTOR3 *pV,
	UINT VStride,
	CONST D3DXMATRIX *pM,
	UINT n)(NULL); 

D3DXVECTOR3* (WINAPI *original_D3DXVec3TransformNormal)(
	D3DXVECTOR3 *pOut,
	CONST D3DXVECTOR3 *pV,
	CONST D3DXMATRIX *pM)(NULL); 

D3DXVECTOR3* (WINAPI *original_D3DXVec3TransformNormalArray)(
	D3DXVECTOR3 *pOut,
	UINT OutStride,
	CONST D3DXVECTOR3 *pV,
	UINT VStride,
	CONST D3DXMATRIX *pM,
	UINT n)(NULL); 

D3DXVECTOR3* (WINAPI *original_D3DXVec3Unproject)(
	D3DXVECTOR3 *pOut,
	CONST D3DXVECTOR3 *pV,
	CONST D3DVIEWPORT9 *pViewport,
	CONST D3DXMATRIX *pProjection,
	CONST D3DXMATRIX *pView,
	CONST D3DXMATRIX *pWorld)(NULL); 

D3DXVECTOR3* (WINAPI *original_D3DXVec3UnprojectArray)(
	D3DXVECTOR3 *pOut,
	UINT OutStride,
	CONST D3DXVECTOR3 *pV,
	UINT VStride,
	CONST D3DVIEWPORT9 *pViewport,
	CONST D3DXMATRIX *pProjection,
	CONST D3DXMATRIX *pView,
	CONST D3DXMATRIX *pWorld,
	UINT n)(NULL); 

D3DXVECTOR4* (WINAPI *original_D3DXVec4BaryCentric)(
	D3DXVECTOR4 *pOut,
	CONST D3DXVECTOR4 *pV1,
	CONST D3DXVECTOR4 *pV2,
	CONST  D3DXVECTOR4 *pV3,
	FLOAT f,
	FLOAT g)(NULL); 

D3DXVECTOR4* (WINAPI *original_D3DXVec4CatmullRom)(
	D3DXVECTOR4 *pOut,
	CONST D3DXVECTOR4 *pV0,
	CONST D3DXVECTOR4 *pV1,
	CONST D3DXVECTOR4 *pV2,
	CONST D3DXVECTOR4 *pV3,
	FLOAT s)(NULL); 

D3DXVECTOR4* (WINAPI *original_D3DXVec4Cross)(
	D3DXVECTOR4 *pOut,
	CONST D3DXVECTOR4 *pV1,
	CONST D3DXVECTOR4 *pV2,
	CONST D3DXVECTOR4 *pV3)(NULL); 

D3DXVECTOR4* (WINAPI *original_D3DXVec4Hermite)(
	D3DXVECTOR4 *pOut,
	CONST D3DXVECTOR4 *pV1,
	CONST D3DXVECTOR4 *pT1,
	CONST D3DXVECTOR4 *pV2,
	CONST D3DXVECTOR4 *pT2,
	FLOAT s)(NULL); 

D3DXVECTOR4* (WINAPI *original_D3DXVec4Normalize)(
	D3DXVECTOR4 *pOut,
	CONST D3DXVECTOR4 *pV)(NULL); 

D3DXVECTOR4* (WINAPI *original_D3DXVec4Transform)(
	D3DXVECTOR4 *pOut,
	CONST D3DXVECTOR4 *pV,
	CONST D3DXMATRIX *pM)(NULL); 

D3DXVECTOR4* (WINAPI *original_D3DXVec4TransformArray)(
	D3DXVECTOR4 *pOut,
	UINT OutStride,
	CONST D3DXVECTOR4 *pV,
	UINT VStride,
	CONST D3DXMATRIX *pM,
	UINT n)(NULL);

HRESULT (WINAPI *original_D3DXWeldVertices)(
	LPD3DXMESH pMesh,
	DWORD Flags,
	CONST D3DXWELDEPSILONS *pEpsilons,
	CONST DWORD *pAdjacencyIn,
	DWORD *pAdjacencyOut,
	DWORD *pFaceRemap,
	LPD3DXBUFFER *ppVertexRemap)(NULL);

///------------------------------------------------------------------------------------------------------------------------------------------------------------------------------///

int effectFlag = 0;
bool showFloatArray = false;

D3DXVECTOR4 fovlh(0.0f, 0.0f, 0.0f, 0.0f);

D3DXVECTOR3 v_eye(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 v_up(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 v_at(0.0f, 0.0f, 0.0f);

std::map<LPDIRECT3DTEXTURE9, std::pair<std::wstring, D3DFORMAT>> dxTextureMap;

void UMSetFlag(int flag)
{
	showFloatArray = (flag > 0) ;
}

void UMSync()
{
	effectFlag = 0;
}

int UMGetEffectType()
{
	return effectFlag;
}

void UMGetCameraFovLH(D3DXVECTOR4 *dst)
{
	dst->x = fovlh.x;
	dst->y = fovlh.y;
	dst->z = fovlh.z;
	dst->w = fovlh.w;
}

void UMGetCameraEye(D3DXVECTOR3 *dst)
{
	dst->x = v_eye.x;
	dst->y = v_eye.y;
	dst->z = v_eye.z;
}

void UMGetCameraAt(D3DXVECTOR3 *dst)
{
	dst->x = v_at.x;
	dst->y = v_at.y;
	dst->z = v_at.z;
}

void UMGetCameraUp(D3DXVECTOR3 *dst)
{
	dst->x = v_up.x;
	dst->y = v_up.y;
	dst->z = v_up.z;
}

BOOL UMCopyTexture(LPCWSTR dstDir, LPDIRECT3DTEXTURE9 tex)
{
	std::map<LPDIRECT3DTEXTURE9, std::pair<std::wstring, D3DFORMAT> >::iterator it = dxTextureMap.find(tex);

	if (it != dxTextureMap.end()) {
		LPCWSTR srcPath = (*it).second.first.c_str();
		if (PathFileExists(srcPath)) {
			WCHAR fileName[MAX_PATH];
			short size = GetFileTitle(srcPath, NULL, NULL);
			GetFileTitle(srcPath, fileName, size);

			std::wstring dst(std::wstring(dstDir) + std::wstring(fileName));

			CopyFile(srcPath, dst.c_str(), false);
		}

	} else {
		return false;
	}
	return true;
}

LPWSTR UMGetTextureName(LPDIRECT3DTEXTURE9 tex)
{
	std::map<LPDIRECT3DTEXTURE9, std::pair<std::wstring, D3DFORMAT> >::iterator it = dxTextureMap.find(tex);

	static WCHAR dst[MAX_PATH];

	if (it != dxTextureMap.end()) {
		LPCWSTR srcPath = (*it).second.first.c_str();
		if (PathFileExists(srcPath)) {
			short size = GetFileTitle(srcPath, NULL, NULL);
			GetFileTitle(srcPath, dst, size);
		}
	}
	return dst;
}

BOOL UMIsAlphaTexture(LPDIRECT3DTEXTURE9 tex)
{
	std::map<LPDIRECT3DTEXTURE9, std::pair<std::wstring, D3DFORMAT> >::iterator it = dxTextureMap.find(tex);

	if (it != dxTextureMap.end()) {
		if ((*it).second.second == D3DFMT_A8R8G8B8) {
			return true;
		}
	}
	return false;

}

HRESULT WINAPI setTexture(
						  ID3DXEffect* pEffect,
						  D3DXHANDLE hParameter,
						  LPDIRECT3DBASETEXTURE9 pTexture
						  )
{
	//::MessageBoxA(NULL, hParameter, "texture", MB_OK);
	return original_SetTexture(pEffect, hParameter, pTexture);
}

HRESULT WINAPI setFloatArray(
							 ID3DXEffect* pEffect,
							 D3DXHANDLE hParameter,
							 CONST FLOAT* pf,
							 UINT Count
							 )
{
	std::string param(hParameter);
	if (param == "EgColor")
		effectFlag |= EgColor;
	if (param == "ToonColor")
		effectFlag |= ToonColor;
	if (param == "SpcColor")
		effectFlag |= SpcColor;
	if (param == "DifColor")
		effectFlag |= DifColor;

	return original_SetFloatArray(pEffect, hParameter, pf, Count);
}

D3DXHANDLE WINAPI getParameter(
							   ID3DXEffect* pEffect,
							   D3DXHANDLE hParent,
							   UINT index
							   )
{
	//::MessageBoxA(NULL, hParent, "param", MB_OK);
	return original_GetParameter(pEffect, hParent, index);
}




extern "C" {


	HRESULT WINAPI D3DXAssembleShader

		(
		LPCSTR                          pSrcData,
		UINT                            SrcDataLen,
		CONST D3DXMACRO*                pDefines,
		LPD3DXINCLUDE                   pInclude,
		DWORD                           Flags,
		LPD3DXBUFFER*                   ppShader,
		LPD3DXBUFFER*                   ppErrorMsgs)
	{
		return (*original_D3DXAssembleShader)(pSrcData, SrcDataLen, pDefines, pInclude, Flags, ppShader, ppErrorMsgs);
	}

	HRESULT WINAPI D3DXAssembleShaderFromFileA(
		LPCSTR                          pSrcFile,
		CONST D3DXMACRO*                pDefines,
		LPD3DXINCLUDE                   pInclude,
		DWORD                           Flags,
		LPD3DXBUFFER*                   ppShader,
		LPD3DXBUFFER*                   ppErrorMsgs)
	{
		return (*original_D3DXAssembleShaderFromFileA)(pSrcFile, pDefines, pInclude, Flags, ppShader, ppErrorMsgs);
	}

	HRESULT WINAPI D3DXAssembleShaderFromFileW(
		LPCWSTR                         pSrcFile,
		CONST D3DXMACRO*                pDefines,
		LPD3DXINCLUDE                   pInclude,
		DWORD                           Flags,
		LPD3DXBUFFER*                   ppShader,
		LPD3DXBUFFER*                   ppErrorMsgs)
	{
		return (*original_D3DXAssembleShaderFromFileW)(pSrcFile, pDefines, pInclude, Flags, ppShader, ppErrorMsgs);
	}

	HRESULT WINAPI D3DXAssembleShaderFromResourceA(
		HMODULE                         hSrcModule,
		LPCSTR                          pSrcResource,
		CONST D3DXMACRO*                pDefines,
		LPD3DXINCLUDE                   pInclude,
		DWORD                           Flags,
		LPD3DXBUFFER*                   ppShader,
		LPD3DXBUFFER*                   ppErrorMsgs)
	{
		return (*original_D3DXAssembleShaderFromResourceA)(hSrcModule, pSrcResource, pDefines, pInclude, Flags, ppShader, ppErrorMsgs);
	}

	HRESULT WINAPI D3DXAssembleShaderFromResourceW(
		HMODULE                         hSrcModule,
		LPCWSTR                         pSrcResource,
		CONST D3DXMACRO*                pDefines,
		LPD3DXINCLUDE                   pInclude,
		DWORD                           Flags,
		LPD3DXBUFFER*                   ppShader,
		LPD3DXBUFFER*                   ppErrorMsgs)
	{
		return (*original_D3DXAssembleShaderFromResourceW)(hSrcModule, pSrcResource, pDefines, pInclude, Flags, ppShader, ppErrorMsgs);
	}

	BOOL WINAPI D3DXBoxBoundProbe(
		CONST D3DXVECTOR3 *pMin,
		CONST D3DXVECTOR3 *pMax,
		CONST D3DXVECTOR3 *pRayPosition,
		CONST D3DXVECTOR3 *pRayDirection)
	{
		return (*original_D3DXBoxBoundProbe)(pMin, pMax, pRayPosition, pRayDirection);
	}

	HRESULT WINAPI D3DXCheckCubeTextureRequirements(
		LPDIRECT3DDEVICE9         pDevice,
		UINT*                     pSize,
		UINT*                     pNumMipLevels,
		DWORD                     Usage,
		D3DFORMAT*                pFormat,
		D3DPOOL                   Pool)
	{
		return (*original_D3DXCheckCubeTextureRequirements)(pDevice, pSize, pNumMipLevels, Usage, pFormat, Pool);
	}

	HRESULT WINAPI D3DXCheckTextureRequirements(
		LPDIRECT3DDEVICE9         pDevice,
		UINT*                     pWidth,
		UINT*                     pHeight,
		UINT*                     pNumMipLevels,
		DWORD                     Usage,
		D3DFORMAT*                pFormat,
		D3DPOOL                   Pool)
	{
		return (*original_D3DXCheckTextureRequirements)(pDevice, pWidth, pHeight, pNumMipLevels, Usage, pFormat, Pool);
	}

	BOOL WINAPI D3DXCheckVersion(
		UINT D3DSdkVersion,
		UINT D3DXSdkVersion)
	{
		return (*original_D3DXCheckVersion)(D3DSdkVersion, D3DXSdkVersion);
	}

	HRESULT WINAPI D3DXCheckVolumeTextureRequirements(
		LPDIRECT3DDEVICE9         pDevice,
		UINT*                     pWidth,
		UINT*                     pHeight,
		UINT*                     pDepth,
		UINT*                     pNumMipLevels,
		DWORD                     Usage,
		D3DFORMAT*                pFormat,
		D3DPOOL                   Pool)
	{
		return (*original_D3DXCheckVolumeTextureRequirements)(pDevice, pWidth, pHeight, pDepth, pNumMipLevels, Usage, pFormat, Pool);
	}

	HRESULT WINAPI D3DXCleanMesh(
		D3DXCLEANTYPE CleanType,
		LPD3DXMESH pMeshIn,
		CONST DWORD* pAdjacencyIn,
		LPD3DXMESH* ppMeshOut,
		DWORD* pAdjacencyOut,
		LPD3DXBUFFER* ppErrorsAndWarnings)
	{
		return (*original_D3DXCleanMesh)(CleanType, pMeshIn, pAdjacencyIn, ppMeshOut, pAdjacencyOut, ppErrorsAndWarnings);
	}

	D3DXCOLOR* WINAPI D3DXColorAdjustContrast(
		D3DXCOLOR *pOut,
		CONST D3DXCOLOR *pC,
		FLOAT c)
	{
		return (*original_D3DXColorAdjustContrast)(pOut, pC, c);
	}

	D3DXCOLOR* WINAPI D3DXColorAdjustSaturation(
		D3DXCOLOR *pOut,
		CONST D3DXCOLOR *pC,
		FLOAT s)
	{
		return (*original_D3DXColorAdjustSaturation)(pOut, pC, s);
	}

	HRESULT WINAPI D3DXCompileShader(
		LPCSTR                          pSrcData,
		UINT                            SrcDataLen,
		CONST D3DXMACRO*                pDefines,
		LPD3DXINCLUDE                   pInclude,
		LPCSTR                          pFunctionName,
		LPCSTR                          pProfile,
		DWORD                           Flags,
		LPD3DXBUFFER*                   ppShader,
		LPD3DXBUFFER*                   ppErrorMsgs,
		LPD3DXCONSTANTTABLE*            ppConstantTable)
	{
		return (*original_D3DXCompileShader)(pSrcData, SrcDataLen, pDefines, pInclude, pFunctionName, pProfile, Flags, ppShader, ppErrorMsgs, ppConstantTable);
	}

	HRESULT WINAPI D3DXCompileShaderFromFileA(
		LPCSTR                          pSrcFile,
		CONST D3DXMACRO*                pDefines,
		LPD3DXINCLUDE                   pInclude,
		LPCSTR                          pFunctionName,
		LPCSTR                          pProfile,
		DWORD                           Flags,
		LPD3DXBUFFER*                   ppShader,
		LPD3DXBUFFER*                   ppErrorMsgs,
		LPD3DXCONSTANTTABLE*            ppConstantTable)
	{
		return (*original_D3DXCompileShaderFromFileA)(pSrcFile, pDefines, pInclude, pFunctionName, pProfile, Flags, ppShader, ppErrorMsgs, ppConstantTable);
	}

	HRESULT WINAPI D3DXCompileShaderFromFileW(
		LPCWSTR                         pSrcFile,
		CONST D3DXMACRO*                pDefines,
		LPD3DXINCLUDE                   pInclude,
		LPCSTR                          pFunctionName,
		LPCSTR                          pProfile,
		DWORD                           Flags,
		LPD3DXBUFFER*                   ppShader,
		LPD3DXBUFFER*                   ppErrorMsgs,
		LPD3DXCONSTANTTABLE*            ppConstantTable)
	{
		return (*original_D3DXCompileShaderFromFileW)(pSrcFile, pDefines, pInclude, pFunctionName, pProfile, Flags, ppShader, ppErrorMsgs, ppConstantTable);
	}

	HRESULT WINAPI D3DXCompileShaderFromResourceA(
		HMODULE                         hSrcModule,
		LPCSTR                          pSrcResource,
		CONST D3DXMACRO*                pDefines,
		LPD3DXINCLUDE                   pInclude,
		LPCSTR                          pFunctionName,
		LPCSTR                          pProfile,
		DWORD                           Flags,
		LPD3DXBUFFER*                   ppShader,
		LPD3DXBUFFER*                   ppErrorMsgs,
		LPD3DXCONSTANTTABLE*            ppConstantTable)
	{
		return (*original_D3DXCompileShaderFromResourceA)(hSrcModule, pSrcResource, pDefines, pInclude, pFunctionName, pProfile, Flags, ppShader, ppErrorMsgs, ppConstantTable);
	}

	HRESULT WINAPI D3DXCompileShaderFromResourceW(
		HMODULE                         hSrcModule,
		LPCWSTR                         pSrcResource,
		CONST D3DXMACRO*                pDefines,
		LPD3DXINCLUDE                   pInclude,
		LPCSTR                          pFunctionName,
		LPCSTR                          pProfile,
		DWORD                           Flags,
		LPD3DXBUFFER*                   ppShader,
		LPD3DXBUFFER*                   ppErrorMsgs,
		LPD3DXCONSTANTTABLE*            ppConstantTable)
	{
		return (*original_D3DXCompileShaderFromResourceW)(hSrcModule, pSrcResource, pDefines, pInclude, pFunctionName, pProfile, Flags, ppShader, ppErrorMsgs, ppConstantTable);
	}

	HRESULT WINAPI D3DXComputeBoundingBox(
		CONST D3DXVECTOR3 *pFirstPosition,
		DWORD NumVertices,
		DWORD dwStride,
		D3DXVECTOR3 *pMin,
		D3DXVECTOR3 *pMax)
	{
		return (*original_D3DXComputeBoundingBox)(pFirstPosition, NumVertices, dwStride, pMin, pMax);
	}

	HRESULT WINAPI D3DXComputeBoundingSphere(
		CONST D3DXVECTOR3 *pFirstPosition,
		DWORD NumVertices,
		DWORD dwStride,
		D3DXVECTOR3 *pCenter,
		FLOAT *pRadius)
	{
		return (*original_D3DXComputeBoundingSphere)(pFirstPosition, NumVertices, dwStride, pCenter, pRadius);
	}

	HRESULT WINAPI D3DXComputeIMTFromPerTexelSignal(
		LPD3DXMESH pMesh,
		DWORD dwTextureIndex,
		FLOAT *pfTexelSignal,
		UINT uWidth,
		UINT uHeight,
		UINT uSignalDimension,
		UINT uComponents,
		DWORD dwOptions,
		LPD3DXUVATLASCB pStatusCallback,
		LPVOID pUserContext,
		LPD3DXBUFFER *ppIMTData)
	{
		return (*original_D3DXComputeIMTFromPerTexelSignal)(pMesh, dwTextureIndex, pfTexelSignal, uWidth, uHeight, uSignalDimension, uComponents, dwOptions, pStatusCallback, pUserContext, ppIMTData);
	}

	HRESULT WINAPI D3DXComputeIMTFromPerVertexSignal(
		LPD3DXMESH pMesh,
		CONST FLOAT *pfVertexSignal,
		UINT uSignalDimension,
		UINT uSignalStride,
		DWORD dwOptions,
		LPD3DXUVATLASCB pStatusCallback,
		LPVOID pUserContext,
		LPD3DXBUFFER *ppIMTData)
	{
		return (*original_D3DXComputeIMTFromPerVertexSignal)(pMesh, pfVertexSignal, uSignalDimension, uSignalStride, dwOptions, pStatusCallback, pUserContext, ppIMTData);
	}

	HRESULT WINAPI D3DXComputeIMTFromSignal(
		LPD3DXMESH pMesh,
		DWORD dwTextureIndex,
		UINT uSignalDimension,
		FLOAT fMaxUVDistance,
		DWORD dwOptions,
		LPD3DXIMTSIGNALCALLBACK pSignalCallback,
		VOID *pUserData,
		LPD3DXUVATLASCB pStatusCallback,
		LPVOID pUserContext,
		LPD3DXBUFFER *ppIMTData)
	{
		return (*original_D3DXComputeIMTFromSignal)(pMesh, dwTextureIndex, uSignalDimension, fMaxUVDistance, dwOptions,
			pSignalCallback, pUserData, pStatusCallback, pUserContext, ppIMTData);
	}

	HRESULT WINAPI D3DXComputeIMTFromTexture(
		LPD3DXMESH pMesh,
		LPDIRECT3DTEXTURE9 pTexture,
		DWORD dwTextureIndex,
		DWORD dwOptions,
		LPD3DXUVATLASCB pStatusCallback,
		LPVOID pUserContext,
		LPD3DXBUFFER *ppIMTData)
	{
		return (*original_D3DXComputeIMTFromTexture)(pMesh, pTexture, dwTextureIndex, dwOptions, pStatusCallback, pUserContext, ppIMTData);
	}

	HRESULT WINAPI D3DXComputeNormalMap(
		LPDIRECT3DTEXTURE9        pTexture,
		LPDIRECT3DTEXTURE9        pSrcTexture,
		CONST PALETTEENTRY*       pSrcPalette,
		DWORD                     Flags,
		DWORD                     Channel,
		FLOAT                     Amplitude)
	{
		return (*original_D3DXComputeNormalMap)(pTexture, pSrcTexture, pSrcPalette, Flags, Channel, Amplitude);
	}

	HRESULT WINAPI D3DXComputeNormals(
		LPD3DXBASEMESH pMesh,
		CONST DWORD *pAdjacency)
	{
		return (*original_D3DXComputeNormals)(pMesh, pAdjacency);
	}

	HRESULT WINAPI D3DXComputeTangent
		(LPD3DXMESH Mesh,
		DWORD TexStage,
		DWORD TangentIndex,
		DWORD BinormIndex,
		DWORD Wrap,
		CONST DWORD *pAdjacency)
	{
		return (*original_D3DXComputeTangent)(Mesh,TexStage,TangentIndex,BinormIndex,Wrap,pAdjacency);
	}

	HRESULT WINAPI D3DXComputeTangentFrame(
		ID3DXMesh *pMesh,
		DWORD dwOptions)
	{
		return (*original_D3DXComputeTangentFrame)(pMesh, dwOptions);
	}

	HRESULT WINAPI D3DXComputeTangentFrameEx(
		ID3DXMesh *pMesh,
		DWORD dwTextureInSemantic,
		DWORD dwTextureInIndex,
		DWORD dwUPartialOutSemantic,
		DWORD dwUPartialOutIndex,
		DWORD dwVPartialOutSemantic,
		DWORD dwVPartialOutIndex,
		DWORD dwNormalOutSemantic,
		DWORD dwNormalOutIndex,
		DWORD dwOptions,
		CONST DWORD *pdwAdjacency,
		FLOAT fPartialEdgeThreshold,
		FLOAT fSingularPointThreshold,
		FLOAT fNormalEdgeThreshold,
		ID3DXMesh **ppMeshOut,
		ID3DXBuffer **ppVertexMapping)
	{
		return (*original_D3DXComputeTangentFrameEx)(pMesh, dwTextureInSemantic, dwTextureInIndex, dwUPartialOutSemantic, dwUPartialOutIndex, dwVPartialOutSemantic, dwVPartialOutIndex, dwNormalOutSemantic, dwNormalOutIndex, dwOptions, pdwAdjacency, fPartialEdgeThreshold, fSingularPointThreshold, fNormalEdgeThreshold, ppMeshOut, ppVertexMapping);
	}

	HRESULT WINAPI D3DXConcatenateMeshes(
		LPD3DXMESH *ppMeshes,
		UINT NumMeshes,
		DWORD Options,
		CONST D3DXMATRIX *pGeomXForms,
		CONST D3DXMATRIX *pTextureXForms,
		CONST D3DVERTEXELEMENT9 *pDecl,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXMESH *ppMeshOut)
	{
		return (*original_D3DXConcatenateMeshes)(ppMeshes, NumMeshes, Options, pGeomXForms, pTextureXForms, pDecl, pD3DDevice, ppMeshOut);
	}

	HRESULT WINAPI D3DXConvertMeshSubsetToSingleStrip(
		LPD3DXBASEMESH MeshIn,
		DWORD AttribId,
		DWORD IBOptions,
		LPDIRECT3DINDEXBUFFER9 *ppIndexBuffer,
		DWORD *pNumIndices)
	{
		return (*original_D3DXConvertMeshSubsetToSingleStrip)(MeshIn, AttribId, IBOptions, ppIndexBuffer, pNumIndices);
	}

	HRESULT WINAPI D3DXConvertMeshSubsetToStrips(
		LPD3DXBASEMESH MeshIn,
		DWORD AttribId,
		DWORD IBOptions,
		LPDIRECT3DINDEXBUFFER9 *ppIndexBuffer,
		DWORD *pNumIndices,
		LPD3DXBUFFER *ppStripLengths,
		DWORD *pNumStrips)
	{
		return (*original_D3DXConvertMeshSubsetToStrips)(MeshIn, AttribId, IBOptions, ppIndexBuffer, pNumIndices, ppStripLengths, pNumStrips);
	}

	HRESULT WINAPI D3DXCreateAnimationController(
		UINT MaxNumMatrices,
		UINT MaxNumAnimationSets,
		UINT MaxNumTracks,
		UINT MaxNumEvents,
		LPD3DXANIMATIONCONTROLLER *ppAnimController)
	{
		return (*original_D3DXCreateAnimationController)(MaxNumMatrices, MaxNumAnimationSets, MaxNumTracks, MaxNumEvents, ppAnimController);
	}

	HRESULT WINAPI D3DXCreateBox(
		LPDIRECT3DDEVICE9   pDevice,
		FLOAT               Width,
		FLOAT               Height,
		FLOAT               Depth,
		LPD3DXMESH*         ppMesh,
		LPD3DXBUFFER*       ppAdjacency)
	{
		return (*original_D3DXCreateBox)(pDevice, Width, Height, Depth, ppMesh, ppAdjacency);
	}

	HRESULT WINAPI D3DXCreateBuffer(
		DWORD NumBytes,
		LPD3DXBUFFER *ppBuffer)
	{
		return (*original_D3DXCreateBuffer)(NumBytes, ppBuffer);
	}

	HRESULT WINAPI D3DXCreateCompressedAnimationSet(
		LPCSTR pName,
		DOUBLE TicksPerSecond,
		D3DXPLAYBACK_TYPE Playback,
		LPD3DXBUFFER pCompressedData,
		UINT NumCallbackKeys,
		CONST D3DXKEY_CALLBACK *pCallbackKeys,
		LPD3DXCOMPRESSEDANIMATIONSET *ppAnimationSet)
	{
		return (*original_D3DXCreateCompressedAnimationSet)(pName, TicksPerSecond, Playback, pCompressedData, NumCallbackKeys, pCallbackKeys, ppAnimationSet);
	}

	HRESULT WINAPI D3DXCreateCubeTexture(
		LPDIRECT3DDEVICE9         pDevice,
		UINT                      Size,
		UINT                      MipLevels,
		DWORD                     Usage,
		D3DFORMAT                 Format,
		D3DPOOL                   Pool,
		LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)
	{
		return (*original_D3DXCreateCubeTexture)(pDevice, Size, MipLevels, Usage, Format, Pool, ppCubeTexture);
	}

	HRESULT WINAPI D3DXCreateCubeTextureFromFileA(
		LPDIRECT3DDEVICE9         pDevice,
		LPCSTR                    pSrcFile,
		LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)
	{
		return (*original_D3DXCreateCubeTextureFromFileA)(pDevice, pSrcFile, ppCubeTexture);
	}

	HRESULT WINAPI D3DXCreateCubeTextureFromFileExA(
		LPDIRECT3DDEVICE9         pDevice,
		LPCSTR                    pSrcFile,
		UINT                      Size,
		UINT                      MipLevels,
		DWORD                     Usage,
		D3DFORMAT                 Format,
		D3DPOOL                   Pool,
		DWORD                     Filter,
		DWORD                     MipFilter,
		D3DCOLOR                  ColorKey,
		D3DXIMAGE_INFO*           pSrcInfo,
		PALETTEENTRY*             pPalette,
		LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)
	{
		return (*original_D3DXCreateCubeTextureFromFileExA)(pDevice, pSrcFile, Size, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppCubeTexture);
	}

	HRESULT WINAPI D3DXCreateCubeTextureFromFileExW(
		LPDIRECT3DDEVICE9         pDevice,
		LPCWSTR                   pSrcFile,
		UINT                      Size,
		UINT                      MipLevels,
		DWORD                     Usage,
		D3DFORMAT                 Format,
		D3DPOOL                   Pool,
		DWORD                     Filter,
		DWORD                     MipFilter,
		D3DCOLOR                  ColorKey,
		D3DXIMAGE_INFO*           pSrcInfo,
		PALETTEENTRY*             pPalette,
		LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)
	{
		return (*original_D3DXCreateCubeTextureFromFileExW)(pDevice, pSrcFile, Size, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppCubeTexture);
	}

	HRESULT WINAPI D3DXCreateCubeTextureFromFileInMemory(
		LPDIRECT3DDEVICE9         pDevice,
		LPCVOID                   pSrcData,
		UINT                      SrcDataSize,
		LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)
	{
		return (*original_D3DXCreateCubeTextureFromFileInMemory)(pDevice, pSrcData, SrcDataSize, ppCubeTexture);
	}

	HRESULT WINAPI D3DXCreateCubeTextureFromFileInMemoryEx(
		LPDIRECT3DDEVICE9         pDevice,
		LPCVOID                   pSrcData,
		UINT                      SrcDataSize,
		UINT                      Size,
		UINT                      MipLevels,
		DWORD                     Usage,
		D3DFORMAT                 Format,
		D3DPOOL                   Pool,
		DWORD                     Filter,
		DWORD                     MipFilter,
		D3DCOLOR                  ColorKey,
		D3DXIMAGE_INFO*           pSrcInfo,
		PALETTEENTRY*             pPalette,
		LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)
	{
		return (*original_D3DXCreateCubeTextureFromFileInMemoryEx)(pDevice, pSrcData, SrcDataSize, Size, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppCubeTexture);
	}

	HRESULT WINAPI D3DXCreateCubeTextureFromFileW(
		LPDIRECT3DDEVICE9		 pDevice,
		LPCWSTR				pSrcFile,
		LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)
	{
		return (*original_D3DXCreateCubeTextureFromFileW)(pDevice, pSrcFile, ppCubeTexture);
	}

	HRESULT WINAPI D3DXCreateCubeTextureFromResourceA(
		LPDIRECT3DDEVICE9		 pDevice,
		HMODULE				hSrcModule,
		LPCSTR				 pSrcResource,
		LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)
	{
		return (*original_D3DXCreateCubeTextureFromResourceA)(pDevice, hSrcModule, pSrcResource, ppCubeTexture);
	}

	HRESULT WINAPI D3DXCreateCubeTextureFromResourceExA(
		LPDIRECT3DDEVICE9		 pDevice,
		HMODULE				hSrcModule,
		LPCSTR				 pSrcResource,
		UINT				   Size,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		DWORD				  Filter,
		DWORD				  MipFilter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo,
		PALETTEENTRY*		  pPalette,
		LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)
	{
		return (*original_D3DXCreateCubeTextureFromResourceExA)(pDevice, hSrcModule, pSrcResource, Size, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppCubeTexture);
	}

	HRESULT WINAPI D3DXCreateCubeTextureFromResourceExW(
		LPDIRECT3DDEVICE9		 pDevice,
		HMODULE				hSrcModule,
		LPCWSTR				pSrcResource,
		UINT				   Size,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		DWORD				  Filter,
		DWORD				  MipFilter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo,
		PALETTEENTRY*		  pPalette,
		LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)
	{
		return (*original_D3DXCreateCubeTextureFromResourceExW)(pDevice, hSrcModule, pSrcResource, Size, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppCubeTexture);
	}

	HRESULT WINAPI D3DXCreateCubeTextureFromResourceW(
		LPDIRECT3DDEVICE9		 pDevice,
		HMODULE				hSrcModule,
		LPCWSTR				pSrcResource,
		LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)
	{
		return (*original_D3DXCreateCubeTextureFromResourceW)(pDevice, hSrcModule, pSrcResource, ppCubeTexture);
	}

	HRESULT WINAPI D3DXCreateCylinder(
		LPDIRECT3DDEVICE9   pDevice,
		FLOAT		    Radius1,
		FLOAT		    Radius2,
		FLOAT		    Length,
		UINT				Slices,
		UINT				Stacks,
		LPD3DXMESH*		 ppMesh,
		LPD3DXBUFFER*       ppAdjacency)
	{
		return (*original_D3DXCreateCylinder)(pDevice, Radius1, Radius2, Length, Slices, Stacks, ppMesh, ppAdjacency);
	}

	HRESULT WINAPI D3DXCreateEffect(
		LPDIRECT3DDEVICE9		    pDevice,
		LPCVOID						 pSrcData,
		UINT						 SrcDataLen,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXEFFECTPOOL				pPool,
		LPD3DXEFFECT*				ppEffect,
		LPD3DXBUFFER*				ppCompilationErrors)
	{


		return (*original_D3DXCreateEffect)(pDevice, pSrcData, SrcDataLen, pDefines, pInclude, Flags, pPool, ppEffect, ppCompilationErrors);
	}

	HRESULT WINAPI D3DXCreateEffectCompiler(
		LPCSTR						  pSrcData,
		UINT						 SrcDataLen,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXEFFECTCOMPILER*		ppCompiler,
		LPD3DXBUFFER*				ppParseErrors)
	{
		return (*original_D3DXCreateEffectCompiler)(pSrcData, SrcDataLen, pDefines, pInclude, Flags, ppCompiler, ppParseErrors);
	}

	HRESULT WINAPI D3DXCreateEffectCompilerFromFileA(
		LPCSTR						  pSrcFile,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXEFFECTCOMPILER*		ppCompiler,
		LPD3DXBUFFER*				ppParseErrors)
	{
		return (*original_D3DXCreateEffectCompilerFromFileA)(pSrcFile, pDefines, pInclude, Flags, ppCompiler, ppParseErrors);
	}

	HRESULT WINAPI D3DXCreateEffectCompilerFromFileW(
		LPCWSTR						 pSrcFile,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXEFFECTCOMPILER*		ppCompiler,
		LPD3DXBUFFER*				ppParseErrors)
	{
		return (*original_D3DXCreateEffectCompilerFromFileW)(pSrcFile, pDefines, pInclude, Flags, ppCompiler, ppParseErrors);
	}

	HRESULT WINAPI D3DXCreateEffectCompilerFromResourceA(
		HMODULE						 hSrcModule,
		LPCSTR						  pSrcResource,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXEFFECTCOMPILER*		ppCompiler,
		LPD3DXBUFFER*				ppParseErrors)
	{
		return (*original_D3DXCreateEffectCompilerFromResourceA)(hSrcModule, pSrcResource, pDefines, pInclude, Flags, ppCompiler, ppParseErrors);
	}

	HRESULT WINAPI D3DXCreateEffectCompilerFromResourceW(
		HMODULE						 hSrcModule,
		LPCWSTR						 pSrcResource,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXEFFECTCOMPILER*		ppCompiler,
		LPD3DXBUFFER*				ppParseErrors)
	{
		return (*original_D3DXCreateEffectCompilerFromResourceW)(hSrcModule, pSrcResource, pDefines, pInclude, Flags, ppCompiler, ppParseErrors);
	}

	HRESULT WINAPI D3DXCreateEffectEx(
		LPDIRECT3DDEVICE9		    pDevice,
		LPCVOID						 pSrcData,
		UINT						 SrcDataLen,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		LPCSTR						  pSkipConstants,
		DWORD						Flags,
		LPD3DXEFFECTPOOL				pPool,
		LPD3DXEFFECT*				ppEffect,
		LPD3DXBUFFER*				ppCompilationErrors)
	{
		return (*original_D3DXCreateEffectEx)(pDevice, pSrcData, SrcDataLen, pDefines, pInclude, pSkipConstants, Flags, pPool, ppEffect, ppCompilationErrors);
	}

	HRESULT WINAPI D3DXCreateEffectFromFileA(
		LPDIRECT3DDEVICE9		    pDevice,
		LPCSTR						  pSrcFile,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXEFFECTPOOL				pPool,
		LPD3DXEFFECT*				ppEffect,
		LPD3DXBUFFER*				ppCompilationErrors)
	{
		return (*original_D3DXCreateEffectFromFileA)(pDevice, pSrcFile, pDefines, pInclude, Flags, pPool, ppEffect, ppCompilationErrors);
	}

	HRESULT WINAPI D3DXCreateEffectFromFileExA(
		LPDIRECT3DDEVICE9		    pDevice,
		LPCSTR						  pSrcFile,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		LPCSTR						  pSkipConstants,
		DWORD						Flags,
		LPD3DXEFFECTPOOL				pPool,
		LPD3DXEFFECT*				ppEffect,
		LPD3DXBUFFER*				ppCompilationErrors)
	{
		return (*original_D3DXCreateEffectFromFileExA)(pDevice, pSrcFile, pDefines, pInclude, pSkipConstants, Flags, pPool, ppEffect, ppCompilationErrors);
	}

	HRESULT WINAPI D3DXCreateEffectFromFileExW(
		LPDIRECT3DDEVICE9		    pDevice,
		LPCWSTR						 pSrcFile,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		LPCSTR						  pSkipConstants,
		DWORD						Flags,
		LPD3DXEFFECTPOOL				pPool,
		LPD3DXEFFECT*				ppEffect,
		LPD3DXBUFFER*				ppCompilationErrors)
	{
		//::MessageBox(NULL, pSrcFile, _T("texture"), MB_OK);
		return (*original_D3DXCreateEffectFromFileExW)(pDevice, pSrcFile, pDefines, pInclude, pSkipConstants, Flags, pPool, ppEffect, ppCompilationErrors);
	}

	HRESULT WINAPI D3DXCreateEffectFromFileW(
		LPDIRECT3DDEVICE9		    pDevice,
		LPCWSTR						 pSrcFile,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXEFFECTPOOL				pPool,
		LPD3DXEFFECT*				ppEffect,
		LPD3DXBUFFER*				ppCompilationErrors)
	{
		return (*original_D3DXCreateEffectFromFileW)(pDevice, pSrcFile, pDefines, pInclude, Flags, pPool, ppEffect, ppCompilationErrors);
	}

	std::vector<LPD3DXEFFECT*> effects;

	LPD3DXEFFECT* UMGetEffect()
	{
		if (effects.size() > 0) {
			return effects[0];
		} else {
			return NULL;
		}

	}

	int UMGetEffectSize()
	{
		return effects.size();
	}

	HRESULT WINAPI D3DXCreateEffectFromResourceA(
		LPDIRECT3DDEVICE9		    pDevice,
		HMODULE						 hSrcModule,
		LPCSTR						  pSrcResource,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXEFFECTPOOL				pPool,
		LPD3DXEFFECT*				ppEffect,
		LPD3DXBUFFER*				ppCompilationErrors)
	{
		HRESULT res = (*original_D3DXCreateEffectFromResourceA)(pDevice, hSrcModule, pSrcResource, pDefines, pInclude, Flags, pPool, ppEffect, ppCompilationErrors);

		effects.push_back(ppEffect);


		original_SetFloatArray = (*ppEffect)->lpVtbl->SetFloatArray;
		//original_SetTexture = (*ppEffect)->lpVtbl->SetTexture;
		//original_GetParameter = 	(*ppEffect)->lpVtbl->GetParameter;

		// 書き込み属性付与
		DWORD old_protect;
		VirtualProtect(reinterpret_cast<void *>((*ppEffect)->lpVtbl), sizeof((*ppEffect)->lpVtbl), PAGE_EXECUTE_WRITECOPY, &old_protect);

		(*ppEffect)->lpVtbl->SetFloatArray = setFloatArray;
		//(*ppEffect)->lpVtbl->SetTexture = setTexture;
		//(*ppEffect)->lpVtbl->GetParameter = getParameter;

		// 書き込み属性元に戻す
		VirtualProtect(reinterpret_cast<void *>((*ppEffect)->lpVtbl), sizeof((*ppEffect)->lpVtbl), old_protect, &old_protect);

		return res;
	}

	HRESULT WINAPI D3DXCreateEffectFromResourceExA(
		LPDIRECT3DDEVICE9		    pDevice,
		HMODULE						 hSrcModule,
		LPCSTR						  pSrcResource,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		LPCSTR						  pSkipConstants,
		DWORD						Flags,
		LPD3DXEFFECTPOOL				pPool,
		LPD3DXEFFECT*				ppEffect,
		LPD3DXBUFFER*				ppCompilationErrors)
	{
		return (*original_D3DXCreateEffectFromResourceExA)(pDevice, hSrcModule, pSrcResource, pDefines, pInclude, pSkipConstants, Flags, pPool, ppEffect, ppCompilationErrors);
	}

	HRESULT WINAPI D3DXCreateEffectFromResourceExW(
		LPDIRECT3DDEVICE9		    pDevice,
		HMODULE						 hSrcModule,
		LPCWSTR						 pSrcResource,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		LPCSTR						  pSkipConstants,
		DWORD						Flags,
		LPD3DXEFFECTPOOL				pPool,
		LPD3DXEFFECT*				ppEffect,
		LPD3DXBUFFER*				ppCompilationErrors)
	{
		return (*original_D3DXCreateEffectFromResourceExW)(pDevice, hSrcModule, pSrcResource, pDefines, pInclude, pSkipConstants, Flags, pPool, ppEffect, ppCompilationErrors);
	}

	HRESULT WINAPI D3DXCreateEffectFromResourceW(
		LPDIRECT3DDEVICE9		    pDevice,
		HMODULE						 hSrcModule,
		LPCWSTR						 pSrcResource,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXEFFECTPOOL				pPool,
		LPD3DXEFFECT*				ppEffect,
		LPD3DXBUFFER*				ppCompilationErrors)
	{
		return (*original_D3DXCreateEffectFromResourceW)(pDevice, hSrcModule, pSrcResource, pDefines, pInclude, Flags, pPool, ppEffect, ppCompilationErrors);
	}

	HRESULT WINAPI D3DXCreateEffectPool(
		LPD3DXEFFECTPOOL*		    ppPool)
	{
		return (*original_D3DXCreateEffectPool)(ppPool);
	}

	HRESULT WINAPI D3DXCreateFontA(
		LPDIRECT3DDEVICE9       pDevice,
		INT				  Height,
		UINT				 Width,
		UINT				 Weight,
		UINT				 MipLevels,
		BOOL				 Italic,
		DWORD				CharSet,
		DWORD				OutputPrecision,
		DWORD				Quality,
		DWORD				PitchAndFamily,
		LPCSTR				  pFaceName,
		LPD3DXFONT*		  ppFont)
	{
		return (*original_D3DXCreateFontA)(pDevice, Height, Width, Weight, MipLevels, Italic, CharSet, OutputPrecision, Quality, PitchAndFamily, pFaceName, ppFont);
	}

	HRESULT WINAPI D3DXCreateFontIndirectA(
		LPDIRECT3DDEVICE9       pDevice,
		CONST D3DXFONT_DESCA*   pDesc,
		LPD3DXFONT*		  ppFont)
	{
		return (*original_D3DXCreateFontIndirectA)(pDevice, pDesc, ppFont);
	}

	HRESULT WINAPI D3DXCreateFontIndirectW(
		LPDIRECT3DDEVICE9       pDevice,
		CONST D3DXFONT_DESCW*   pDesc,
		LPD3DXFONT*		  ppFont)
	{
		return (*original_D3DXCreateFontIndirectW)(pDevice, pDesc, ppFont);
	}

	HRESULT WINAPI D3DXCreateFontW(
		LPDIRECT3DDEVICE9       pDevice,
		INT				  Height,
		UINT				 Width,
		UINT				 Weight,
		UINT				 MipLevels,
		BOOL				 Italic,
		DWORD				CharSet,
		DWORD				OutputPrecision,
		DWORD				Quality,
		DWORD				PitchAndFamily,
		LPCWSTR				 pFaceName,
		LPD3DXFONT*		  ppFont)
	{
		return (*original_D3DXCreateFontW)(pDevice, Height, Width, Weight, MipLevels, Italic, CharSet, OutputPrecision, Quality, PitchAndFamily, pFaceName, ppFont);
	}

	/*
	HRESULT WINAPI D3DXCreateFragmentLinker(
		LPDIRECT3DDEVICE9		    pDevice,
		UINT						 ShaderCacheSize,
		LPD3DXFRAGMENTLINKER*		ppFragmentLinker)
	{
		return (*original_D3DXCreateFragmentLinker)(pDevice, ShaderCacheSize, ppFragmentLinker);
	}
	*/

	HRESULT WINAPI D3DXCreateKeyframedAnimationSet(
		LPCSTR pName,
		DOUBLE TicksPerSecond,
		D3DXPLAYBACK_TYPE Playback,
		UINT NumAnimations,
		UINT NumCallbackKeys,
		CONST D3DXKEY_CALLBACK *pCallbackKeys,
		LPD3DXKEYFRAMEDANIMATIONSET *ppAnimationSet)
	{
		return (*original_D3DXCreateKeyframedAnimationSet)(pName, TicksPerSecond, Playback, NumAnimations, NumCallbackKeys, pCallbackKeys, ppAnimationSet);
	}

	HRESULT WINAPI D3DXCreateLine(
		LPDIRECT3DDEVICE9   pDevice,
		LPD3DXLINE*		 ppLine)
	{
		return (*original_D3DXCreateLine)(pDevice, ppLine);
	}

	HRESULT WINAPI D3DXCreateMatrixStack(
		DWORD		    Flags,
		LPD3DXMATRIXSTACK*  ppStack)
	{
		return (*original_D3DXCreateMatrixStack)(Flags, ppStack);
	}

	HRESULT WINAPI D3DXCreateMesh(
		DWORD NumFaces,
		DWORD NumVertices,
		DWORD Options,
		CONST D3DVERTEXELEMENT9 *pDeclaration,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXMESH* ppMesh)
	{
		return (*original_D3DXCreateMesh)(NumFaces, NumVertices, Options, pDeclaration, pD3DDevice, ppMesh);
	}

	HRESULT WINAPI D3DXCreateMeshFVF(
		DWORD NumFaces,
		DWORD NumVertices,
		DWORD Options,
		DWORD FVF,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXMESH* ppMesh)
	{
		return (*original_D3DXCreateMeshFVF)(NumFaces, NumVertices, Options, FVF, pD3DDevice, ppMesh);
	}

	HRESULT WINAPI D3DXCreateNPatchMesh(
		LPD3DXMESH pMeshSysMem,
		LPD3DXPATCHMESH *pPatchMesh)
	{
		return (*original_D3DXCreateNPatchMesh)(pMeshSysMem, pPatchMesh);
	}

	HRESULT WINAPI D3DXCreatePMeshFromStream(
		IStream *pStream,
		DWORD Options,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXBUFFER *ppMaterials,
		LPD3DXBUFFER *ppEffectInstances,
		DWORD* pNumMaterials,
		LPD3DXPMESH *ppPMesh)
	{
		return (*original_D3DXCreatePMeshFromStream)(pStream, Options, pD3DDevice, ppMaterials, ppEffectInstances, pNumMaterials, ppPMesh);
	}

	HRESULT WINAPI D3DXCreatePRTBuffer(
		UINT NumSamples,
		UINT NumCoeffs,
		UINT NumChannels,
		LPD3DXPRTBUFFER* ppBuffer)
	{
		return (*original_D3DXCreatePRTBuffer)(NumSamples, NumCoeffs, NumChannels, ppBuffer);
	}

	HRESULT WINAPI D3DXCreatePRTBufferTex(
		UINT Width,
		UINT Height,
		UINT NumCoeffs,
		UINT NumChannels,
		LPD3DXPRTBUFFER* ppBuffer)
	{
		return (*original_D3DXCreatePRTBufferTex)(Width, Height, NumCoeffs, NumChannels, ppBuffer);
	}

	HRESULT WINAPI D3DXCreatePRTCompBuffer(
		D3DXSHCOMPRESSQUALITYTYPE Quality,
		UINT NumClusters,
		UINT NumPCA,
		LPD3DXSHPRTSIMCB pCB,
		LPVOID lpUserContext,
		LPD3DXPRTBUFFER  pBufferIn,
		LPD3DXPRTCOMPBUFFER *ppBufferOut)
	{
		return (*original_D3DXCreatePRTCompBuffer)(Quality, NumClusters, NumPCA, pCB, lpUserContext, pBufferIn, ppBufferOut);
	}

	HRESULT WINAPI D3DXCreatePRTEngine(
		LPD3DXMESH pMesh,
		DWORD *pAdjacency,
		BOOL ExtractUVs,
		LPD3DXMESH pBlockerMesh,
		LPD3DXPRTENGINE* ppEngine)
	{
		return (*original_D3DXCreatePRTEngine)(pMesh, pAdjacency, ExtractUVs, pBlockerMesh, ppEngine);
	}

	HRESULT WINAPI D3DXCreatePatchMesh(
		CONST D3DXPATCHINFO *pInfo,
		DWORD dwNumPatches,
		DWORD dwNumVertices,
		DWORD dwOptions,
		CONST D3DVERTEXELEMENT9 *pDecl,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXPATCHMESH *pPatchMesh)
	{
		return (*original_D3DXCreatePatchMesh)(pInfo, dwNumPatches, dwNumVertices, dwOptions, pDecl, pD3DDevice, pPatchMesh);
	}

	HRESULT WINAPI D3DXCreatePolygon(
		LPDIRECT3DDEVICE9   pDevice,
		FLOAT		    Length,
		UINT				Sides,
		LPD3DXMESH*		 ppMesh,
		LPD3DXBUFFER*       ppAdjacency)
	{
		return (*original_D3DXCreatePolygon)(pDevice, Length, Sides, ppMesh, ppAdjacency);
	}

	HRESULT WINAPI D3DXCreateRenderToEnvMap(
		LPDIRECT3DDEVICE9       pDevice,
		UINT				 Size,
		UINT				 MipLevels,
		D3DFORMAT		    Format,
		BOOL				 DepthStencil,
		D3DFORMAT		    DepthStencilFormat,
		LPD3DXRenderToEnvMap*   ppRenderToEnvMap)
	{
		return (*original_D3DXCreateRenderToEnvMap)(pDevice, Size, MipLevels, Format, DepthStencil, DepthStencilFormat, ppRenderToEnvMap);
	}

	HRESULT WINAPI D3DXCreateRenderToSurface(
		LPDIRECT3DDEVICE9       pDevice,
		UINT				 Width,
		UINT				 Height,
		D3DFORMAT		    Format,
		BOOL				 DepthStencil,
		D3DFORMAT		    DepthStencilFormat,
		LPD3DXRENDERTOSURFACE*  ppRenderToSurface)
	{
		return (*original_D3DXCreateRenderToSurface)(pDevice, Width, Height, Format, DepthStencil, DepthStencilFormat, ppRenderToSurface);
	}

	HRESULT WINAPI D3DXCreateSPMesh(
		LPD3DXMESH pMesh,
		CONST DWORD* pAdjacency,
		CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,
		CONST FLOAT *pVertexWeights,
		LPD3DXSPMESH* ppSMesh)
	{
		return (*original_D3DXCreateSPMesh)(pMesh, pAdjacency, pVertexAttributeWeights, pVertexWeights, ppSMesh);
	}

	HRESULT WINAPI D3DXCreateSkinInfo(
		DWORD NumVertices,
		CONST D3DVERTEXELEMENT9 *pDeclaration,
		DWORD NumBones,
		LPD3DXSKININFO* ppSkinInfo)
	{
		return (*original_D3DXCreateSkinInfo)(NumVertices, pDeclaration, NumBones, ppSkinInfo);
	}

	HRESULT WINAPI D3DXCreateSkinInfoFVF(
		DWORD NumVertices,
		DWORD FVF,
		DWORD NumBones,
		LPD3DXSKININFO* ppSkinInfo)
	{
		return (*original_D3DXCreateSkinInfoFVF)(NumVertices, FVF, NumBones, ppSkinInfo);
	}

	HRESULT WINAPI D3DXCreateSkinInfoFromBlendedMesh(
		LPD3DXBASEMESH pMesh,
		DWORD NumBones,
		CONST D3DXBONECOMBINATION *pBoneCombinationTable,
		LPD3DXSKININFO* ppSkinInfo)
	{
		return (*original_D3DXCreateSkinInfoFromBlendedMesh)(pMesh, NumBones, pBoneCombinationTable, ppSkinInfo);
	}

	HRESULT WINAPI D3DXCreateSphere(
		LPDIRECT3DDEVICE9  pDevice,
		FLOAT		   Radius,
		UINT		    Slices,
		UINT		    Stacks,
		LPD3DXMESH*		ppMesh,
		LPD3DXBUFFER*      ppAdjacency)
	{
		return (*original_D3DXCreateSphere)(pDevice, Radius, Slices, Stacks, ppMesh, ppAdjacency);
	}

	HRESULT WINAPI D3DXCreateSprite(
		LPDIRECT3DDEVICE9   pDevice,
		LPD3DXSPRITE*       ppSprite)
	{
		return (*original_D3DXCreateSprite)(pDevice, ppSprite);
	}

	HRESULT WINAPI D3DXCreateTeapot(
		LPDIRECT3DDEVICE9   pDevice,
		LPD3DXMESH*		 ppMesh,
		LPD3DXBUFFER*       ppAdjacency)
	{
		return (*original_D3DXCreateTeapot)(pDevice, ppMesh, ppAdjacency);
	}

	HRESULT WINAPI D3DXCreateTextA(
		LPDIRECT3DDEVICE9   pDevice,
		HDC				 hDC,
		LPCSTR		   pText,
		FLOAT		    Deviation,
		FLOAT		    Extrusion,
		LPD3DXMESH*		 ppMesh,
		LPD3DXBUFFER*       ppAdjacency,
		LPGLYPHMETRICSFLOAT pGlyphMetrics)
	{
		return (*original_D3DXCreateTextA)(pDevice, hDC, pText, Deviation, Extrusion, ppMesh, ppAdjacency, pGlyphMetrics);
	}

	HRESULT WINAPI D3DXCreateTextW(
		LPDIRECT3DDEVICE9   pDevice,
		HDC				 hDC,
		LPCWSTR		  pText,
		FLOAT		    Deviation,
		FLOAT		    Extrusion,
		LPD3DXMESH*		 ppMesh,
		LPD3DXBUFFER*       ppAdjacency,
		LPGLYPHMETRICSFLOAT pGlyphMetrics)
	{
		return (*original_D3DXCreateTextW)(pDevice, hDC, pText, Deviation, Extrusion, ppMesh, ppAdjacency, pGlyphMetrics);
	}

	HRESULT WINAPI D3DXCreateTexture(
		LPDIRECT3DDEVICE9		 pDevice,
		UINT				   Width,
		UINT				   Height,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		LPDIRECT3DTEXTURE9*       ppTexture)
	{
		//::MessageBoxA(NULL, "D3DXCreateTexture", "d3dx", MB_OK);
		return (*original_D3DXCreateTexture)(pDevice, Width, Height, MipLevels, Usage, Format, Pool, ppTexture);
	}

	HRESULT WINAPI D3DXCreateTextureFromFileA(
		LPDIRECT3DDEVICE9		 pDevice,
		LPCSTR				 pSrcFile,
		LPDIRECT3DTEXTURE9*       ppTexture)
	{
		//::MessageBoxA(NULL, "D3DXCreateTextureFromFileA", "d3dx", MB_OK);
		return (*original_D3DXCreateTextureFromFileA)(pDevice, pSrcFile, ppTexture);
	}

	HRESULT WINAPI D3DXCreateTextureFromFileExA(
		LPDIRECT3DDEVICE9		 pDevice,
		LPCSTR				 pSrcFile,
		UINT				   Width,
		UINT				   Height,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		DWORD				  Filter,
		DWORD				  MipFilter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo,
		PALETTEENTRY*		  pPalette,
		LPDIRECT3DTEXTURE9*       ppTexture)
	{
		//::MessageBoxA(NULL, "D3DXCreateTextureFromFileExA", "d3dx", MB_OK);
		return (*original_D3DXCreateTextureFromFileExA)(pDevice, pSrcFile, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
	}

	HRESULT WINAPI D3DXCreateTextureFromFileExW(
		LPDIRECT3DDEVICE9		 pDevice,
		LPCWSTR				pSrcFile,
		UINT				   Width,
		UINT				   Height,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		DWORD				  Filter,
		DWORD				  MipFilter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo,
		PALETTEENTRY*		  pPalette,
		LPDIRECT3DTEXTURE9*       ppTexture)
	{
		HRESULT res = (*original_D3DXCreateTextureFromFileExW)(pDevice, pSrcFile, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);

		//{
		//	D3DXIMAGE_INFO info;
		//	LPDIRECT3DTEXTURE9 tmpTex;

		//	HRESULT res2 = (*original_D3DXCreateTextureFromFileExW)(pDevice, pSrcFile, Width, Height, MipLevels, Usage, D3DFMT_UNKNOWN, Pool, Filter, MipFilter, ColorKey, &info, pPalette, &tmpTex);
		//	if (SUCCEEDED(res2))
		//	{
		//		::MessageBoxA(NULL, to_string(info.).c_str(), "info", MB_OK);
		//		tmpTex->lpVtbl->Release(tmpTex);
		//	}
		//}

		if (SUCCEEDED(res)) {
			if (dxTextureMap.find(*ppTexture) != dxTextureMap.end()) {
				dxTextureMap.erase(*ppTexture);

			}
			dxTextureMap[*ppTexture] = std::pair<std::wstring, D3DFORMAT>(pSrcFile, pSrcInfo->Format);

		}
		return res;
	}

	HRESULT WINAPI D3DXCreateTextureFromFileInMemory(
		LPDIRECT3DDEVICE9		 pDevice,
		LPCVOID				pSrcData,
		UINT				   SrcDataSize,
		LPDIRECT3DTEXTURE9*       ppTexture)
	{
		//::MessageBoxA(NULL, "D3DXCreateTextureFromFileInMemory", "d3dx", MB_OK);
		return (*original_D3DXCreateTextureFromFileInMemory)(pDevice, pSrcData, SrcDataSize, ppTexture);
	}

	HRESULT WINAPI D3DXCreateTextureFromFileInMemoryEx(
		LPDIRECT3DDEVICE9		 pDevice,
		LPCVOID				pSrcData,
		UINT				   SrcDataSize,
		UINT				   Width,
		UINT				   Height,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		DWORD				  Filter,
		DWORD				  MipFilter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo,
		PALETTEENTRY*		  pPalette,
		LPDIRECT3DTEXTURE9*       ppTexture)
	{
		//::MessageBoxA(NULL, "D3DXCreateTextureFromFileInMemoryEx", "d3dx", MB_OK);
		return (*original_D3DXCreateTextureFromFileInMemoryEx)(pDevice, pSrcData, SrcDataSize, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
	}

	HRESULT WINAPI D3DXCreateTextureFromFileW(
		LPDIRECT3DDEVICE9		 pDevice,
		LPCWSTR				pSrcFile,
		LPDIRECT3DTEXTURE9*       ppTexture)
	{
		//::MessageBoxA(NULL, "D3DXCreateTextureFromFileW", "d3dx", MB_OK);
		return (*original_D3DXCreateTextureFromFileW)(pDevice, pSrcFile, ppTexture);
	}

	HRESULT WINAPI D3DXCreateTextureFromResourceA(
		LPDIRECT3DDEVICE9		 pDevice,
		HMODULE				hSrcModule,
		LPCSTR				 pSrcResource,
		LPDIRECT3DTEXTURE9*       ppTexture)
	{
		//::MessageBoxA(NULL, "D3DXCreateTextureFromResourceA", "d3dx", MB_OK);
		return (*original_D3DXCreateTextureFromResourceA)(pDevice, hSrcModule, pSrcResource, ppTexture);
	}

	HRESULT WINAPI D3DXCreateTextureFromResourceExA(
		LPDIRECT3DDEVICE9		 pDevice,
		HMODULE				hSrcModule,
		LPCSTR				 pSrcResource,
		UINT				   Width,
		UINT				   Height,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		DWORD				  Filter,
		DWORD				  MipFilter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo,
		PALETTEENTRY*		  pPalette,
		LPDIRECT3DTEXTURE9*       ppTexture)
	{
		//::MessageBoxA(NULL, "D3DXCreateTextureFromResourceExA", "d3dx", MB_OK);
		return (*original_D3DXCreateTextureFromResourceExA)(pDevice, hSrcModule, pSrcResource, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
	}

	HRESULT WINAPI D3DXCreateTextureFromResourceExW(
		LPDIRECT3DDEVICE9		 pDevice,
		HMODULE				hSrcModule,
		LPCWSTR				pSrcResource,
		UINT				   Width,
		UINT				   Height,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		DWORD				  Filter,
		DWORD				  MipFilter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo,
		PALETTEENTRY*		  pPalette,
		LPDIRECT3DTEXTURE9*       ppTexture)
	{
		//::MessageBoxA(NULL, "D3DXCreateTextureFromResourceExW", "d3dx", MB_OK);
		return (*original_D3DXCreateTextureFromResourceExW)(pDevice, hSrcModule, pSrcResource, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
	}

	HRESULT WINAPI D3DXCreateTextureFromResourceW(
		LPDIRECT3DDEVICE9		 pDevice,
		HMODULE				hSrcModule,
		LPCWSTR				pSrcResource,
		LPDIRECT3DTEXTURE9*       ppTexture)
	{
		//::MessageBoxA(NULL, "D3DXCreateTextureFromResourceW", "d3dx", MB_OK);
		return (*original_D3DXCreateTextureFromResourceW)(pDevice, hSrcModule, pSrcResource, ppTexture);
	}

	HRESULT WINAPI D3DXCreateTextureGutterHelper(
		UINT Width,
		UINT Height,
		LPD3DXMESH pMesh,
		FLOAT GutterSize,
		LPD3DXTEXTUREGUTTERHELPER* ppBuffer)
	{
		//::MessageBoxA(NULL, "D3DXCreateTextureGutterHelper", "d3dx", MB_OK);
		return (*original_D3DXCreateTextureGutterHelper)(Width, Height, pMesh, GutterSize, ppBuffer);
	}

	HRESULT WINAPI D3DXCreateTextureShader(
		CONST DWORD*				 pFunction,
		LPD3DXTEXTURESHADER*		 ppTextureShader)
	{
		return (*original_D3DXCreateTextureShader)(pFunction, ppTextureShader);
	}

	HRESULT WINAPI D3DXCreateTorus(
		LPDIRECT3DDEVICE9   pDevice,
		FLOAT		    InnerRadius,
		FLOAT		    OuterRadius,
		UINT				Sides,
		UINT				Rings,
		LPD3DXMESH*		 ppMesh,
		LPD3DXBUFFER*       ppAdjacency)
	{
		return (*original_D3DXCreateTorus)(pDevice, InnerRadius, OuterRadius, Sides, Rings, ppMesh, ppAdjacency);
	}

	HRESULT WINAPI D3DXCreateVolumeTexture(
		LPDIRECT3DDEVICE9		 pDevice,
		UINT				   Width,
		UINT				   Height,
		UINT				   Depth,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
	{
		return (*original_D3DXCreateVolumeTexture)(pDevice, Width, Height, Depth, MipLevels, Usage, Format, Pool, ppVolumeTexture);
	}

	HRESULT WINAPI D3DXCreateVolumeTextureFromFileA(
		LPDIRECT3DDEVICE9		 pDevice,
		LPCSTR				 pSrcFile,
		LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
	{
		return (*original_D3DXCreateVolumeTextureFromFileA)(pDevice, pSrcFile, ppVolumeTexture);
	}

	HRESULT WINAPI D3DXCreateVolumeTextureFromFileExA(
		LPDIRECT3DDEVICE9		 pDevice,
		LPCSTR				 pSrcFile,
		UINT				   Width,
		UINT				   Height,
		UINT				   Depth,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		DWORD				  Filter,
		DWORD				  MipFilter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo,
		PALETTEENTRY*		  pPalette,
		LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
	{
		return (*original_D3DXCreateVolumeTextureFromFileExA)(pDevice, pSrcFile, Width, Height, Depth, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppVolumeTexture);
	}

	HRESULT WINAPI D3DXCreateVolumeTextureFromFileExW(
		LPDIRECT3DDEVICE9		 pDevice,
		LPCWSTR				pSrcFile,
		UINT				   Width,
		UINT				   Height,
		UINT				   Depth,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		DWORD				  Filter,
		DWORD				  MipFilter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo,
		PALETTEENTRY*		  pPalette,
		LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
	{
		return (*original_D3DXCreateVolumeTextureFromFileExW)(pDevice, pSrcFile, Width, Height, Depth, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppVolumeTexture);
	}

	HRESULT WINAPI D3DXCreateVolumeTextureFromFileInMemory(
		LPDIRECT3DDEVICE9		 pDevice,
		LPCVOID				pSrcData,
		UINT				   SrcDataSize,
		LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
	{
		return (*original_D3DXCreateVolumeTextureFromFileInMemory)(pDevice, pSrcData, SrcDataSize, ppVolumeTexture);
	}

	HRESULT WINAPI D3DXCreateVolumeTextureFromFileInMemoryEx(
		LPDIRECT3DDEVICE9		 pDevice,
		LPCVOID				pSrcData,
		UINT				   SrcDataSize,
		UINT				   Width,
		UINT				   Height,
		UINT				   Depth,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		DWORD				  Filter,
		DWORD				  MipFilter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo,
		PALETTEENTRY*		  pPalette,
		LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
	{
		return (*original_D3DXCreateVolumeTextureFromFileInMemoryEx)(pDevice, pSrcData, SrcDataSize, Width, Height, Depth, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppVolumeTexture);
	}

	HRESULT WINAPI D3DXCreateVolumeTextureFromFileW(
		LPDIRECT3DDEVICE9		 pDevice,
		LPCWSTR				pSrcFile,
		LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
	{
		return (*original_D3DXCreateVolumeTextureFromFileW)(pDevice, pSrcFile, ppVolumeTexture);
	}

	HRESULT WINAPI D3DXCreateVolumeTextureFromResourceA(
		LPDIRECT3DDEVICE9		 pDevice,
		HMODULE				hSrcModule,
		LPCSTR				 pSrcResource,
		LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
	{
		return (*original_D3DXCreateVolumeTextureFromResourceA)(pDevice, hSrcModule, pSrcResource, ppVolumeTexture);
	}

	HRESULT WINAPI D3DXCreateVolumeTextureFromResourceExA(
		LPDIRECT3DDEVICE9		 pDevice,
		HMODULE				hSrcModule,
		LPCSTR				 pSrcResource,
		UINT				   Width,
		UINT				   Height,
		UINT				   Depth,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		DWORD				  Filter,
		DWORD				  MipFilter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo,
		PALETTEENTRY*		  pPalette,
		LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
	{
		return (*original_D3DXCreateVolumeTextureFromResourceExA)(pDevice, hSrcModule, pSrcResource, Width, Height, Depth, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppVolumeTexture);
	}

	HRESULT WINAPI D3DXCreateVolumeTextureFromResourceExW(
		LPDIRECT3DDEVICE9		 pDevice,
		HMODULE				hSrcModule,
		LPCWSTR				pSrcResource,
		UINT				   Width,
		UINT				   Height,
		UINT				   Depth,
		UINT				   MipLevels,
		DWORD				  Usage,
		D3DFORMAT				 Format,
		D3DPOOL				Pool,
		DWORD				  Filter,
		DWORD				  MipFilter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo,
		PALETTEENTRY*		  pPalette,
		LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
	{
		return (*original_D3DXCreateVolumeTextureFromResourceExW)(pDevice, hSrcModule, pSrcResource, Width, Height, Depth, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppVolumeTexture);
	}

	HRESULT WINAPI D3DXCreateVolumeTextureFromResourceW(
		LPDIRECT3DDEVICE9		 pDevice,
		HMODULE				hSrcModule,
		LPCWSTR				pSrcResource,
		LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
	{
		return (*original_D3DXCreateVolumeTextureFromResourceW)(pDevice, hSrcModule, pSrcResource, ppVolumeTexture);
	}

	BOOL WINAPI D3DXDebugMute(
		BOOL Mute)
	{
		return (*original_D3DXDebugMute)(Mute);
	}

	HRESULT WINAPI D3DXDeclaratorFromFVF(
		DWORD FVF,
		D3DVERTEXELEMENT9 pDeclarator[MAX_FVF_DECL_SIZE]
	)
	{
		return (*original_D3DXDeclaratorFromFVF)(FVF, pDeclarator);
	}

	HRESULT WINAPI D3DXDisassembleEffect(
		LPD3DXEFFECT pEffect,
		BOOL EnableColorCode,
		LPD3DXBUFFER *ppDisassembly)
	{
		return (*original_D3DXDisassembleEffect)(pEffect, EnableColorCode, ppDisassembly);
	}

	HRESULT WINAPI D3DXDisassembleShader(
		CONST DWORD*				 pShader,
		BOOL						 EnableColorCode,
		LPCSTR						  pComments,
		LPD3DXBUFFER*				ppDisassembly)
	{
		return (*original_D3DXDisassembleShader)(pShader, EnableColorCode, pComments, ppDisassembly);
	}

	HRESULT WINAPI D3DXFVFFromDeclarator(
		CONST D3DVERTEXELEMENT9 *pDeclarator,
		DWORD *pFVF)
	{
		return (*original_D3DXFVFFromDeclarator)(pDeclarator, pFVF);
	}

	HRESULT WINAPI D3DXFileCreate
		(
		ID3DXFile** lplpDirectXFile)
	{
		return (*original_D3DXFileCreate)(lplpDirectXFile);
	}

	HRESULT WINAPI D3DXFillCubeTexture(
		LPDIRECT3DCUBETEXTURE9    pCubeTexture,
		LPD3DXFILL3D		   pFunction,
		LPVOID				 pData)
	{
		return (*original_D3DXFillCubeTexture)(pCubeTexture, pFunction, pData);
	}

	HRESULT WINAPI D3DXFillCubeTextureTX(
		LPDIRECT3DCUBETEXTURE9    pCubeTexture,
		LPD3DXTEXTURESHADER       pTextureShader)
	{
		return (*original_D3DXFillCubeTextureTX)(pCubeTexture, pTextureShader);
	}

	HRESULT WINAPI D3DXFillTexture(
		LPDIRECT3DTEXTURE9		pTexture,
		LPD3DXFILL2D		   pFunction,
		LPVOID				 pData)
	{
		return (*original_D3DXFillTexture)(pTexture, pFunction, pData);
	}

	HRESULT WINAPI D3DXFillTextureTX(
		LPDIRECT3DTEXTURE9		pTexture,
		LPD3DXTEXTURESHADER       pTextureShader)
	{
		return (*original_D3DXFillTextureTX)(pTexture, pTextureShader);
	}

	HRESULT WINAPI D3DXFillVolumeTexture(
		LPDIRECT3DVOLUMETEXTURE9  pVolumeTexture,
		LPD3DXFILL3D		   pFunction,
		LPVOID				 pData)
	{
		return (*original_D3DXFillVolumeTexture)(pVolumeTexture, pFunction, pData);
	}

	HRESULT WINAPI D3DXFillVolumeTextureTX(
		LPDIRECT3DVOLUMETEXTURE9  pVolumeTexture,
		LPD3DXTEXTURESHADER       pTextureShader)
	{
		return (*original_D3DXFillVolumeTextureTX)(pVolumeTexture, pTextureShader);
	}

	HRESULT WINAPI D3DXFilterTexture(
		LPDIRECT3DBASETEXTURE9    pBaseTexture,
		CONST PALETTEENTRY*       pPalette,
		UINT				   SrcLevel,
		DWORD				  Filter)
	{
		return (*original_D3DXFilterTexture)(pBaseTexture, pPalette, SrcLevel, Filter);
	}

	HRESULT WINAPI D3DXFindShaderComment(
		CONST DWORD*				 pFunction,
		DWORD						FourCC,
		LPCVOID*						ppData,
		UINT*						pSizeInBytes)
	{
		return (*original_D3DXFindShaderComment)(pFunction, FourCC, ppData, pSizeInBytes);
	}

	FLOAT* WINAPI D3DXFloat16To32Array(
		FLOAT *pOut,
		CONST D3DXFLOAT16 *pIn,
		UINT n)
	{
		return (*original_D3DXFloat16To32Array)(pOut, pIn, n);
	}

	D3DXFLOAT16* WINAPI D3DXFloat32To16Array(
		D3DXFLOAT16 *pOut,
		CONST FLOAT *pIn,
		UINT n)
	{
		return (*original_D3DXFloat32To16Array)(pOut, pIn, n);
	}

	HRESULT WINAPI D3DXFrameAppendChild(
		LPD3DXFRAME pFrameParent,
		CONST D3DXFRAME *pFrameChild)
	{
		return (*original_D3DXFrameAppendChild)(pFrameParent, pFrameChild);
	}

	HRESULT WINAPI D3DXFrameCalculateBoundingSphere(
		CONST D3DXFRAME *pFrameRoot,
		LPD3DXVECTOR3 pObjectCenter,
		FLOAT *pObjectRadius)
	{
		return (*original_D3DXFrameCalculateBoundingSphere)(pFrameRoot, pObjectCenter, pObjectRadius);
	}

	HRESULT WINAPI D3DXFrameDestroy(
		LPD3DXFRAME pFrameRoot,
		LPD3DXALLOCATEHIERARCHY pAlloc)
	{
		return (*original_D3DXFrameDestroy)(pFrameRoot, pAlloc);
	}

	LPD3DXFRAME WINAPI D3DXFrameFind(
		CONST D3DXFRAME *pFrameRoot,
		LPCSTR Name)
	{
		return (*original_D3DXFrameFind)(pFrameRoot, Name);
	}

	UINT WINAPI D3DXFrameNumNamedMatrices(
		CONST D3DXFRAME *pFrameRoot)
	{
		return (*original_D3DXFrameNumNamedMatrices)(pFrameRoot);
	}

	HRESULT WINAPI D3DXFrameRegisterNamedMatrices(
		LPD3DXFRAME pFrameRoot,
		LPD3DXANIMATIONCONTROLLER pAnimController)
	{
		return (*original_D3DXFrameRegisterNamedMatrices)(pFrameRoot, pAnimController);
	}

	FLOAT WINAPI D3DXFresnelTerm(
		FLOAT CosTheta,
		FLOAT RefractionIndex)
	{
		return (*original_D3DXFresnelTerm)(CosTheta, RefractionIndex);
	}

	HRESULT WINAPI D3DXGatherFragments(
		LPCSTR						  pSrcData,
		UINT						 SrcDataLen,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXBUFFER*				ppShader,
		LPD3DXBUFFER*				ppErrorMsgs)
	{
		return (*original_D3DXGatherFragments)(pSrcData, SrcDataLen, pDefines, pInclude, Flags, ppShader, ppErrorMsgs);
	}

	HRESULT WINAPI D3DXGatherFragmentsFromFileA(
		LPCSTR						  pSrcFile,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXBUFFER*				ppShader,
		LPD3DXBUFFER*				ppErrorMsgs)
	{
		return (*original_D3DXGatherFragmentsFromFileA)(pSrcFile, pDefines, pInclude, Flags, ppShader, ppErrorMsgs);
	}

	HRESULT WINAPI D3DXGatherFragmentsFromFileW(
		LPCWSTR						 pSrcFile,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXBUFFER*				ppShader,
		LPD3DXBUFFER*				ppErrorMsgs)
	{
		return (*original_D3DXGatherFragmentsFromFileW)(pSrcFile, pDefines, pInclude, Flags, ppShader, ppErrorMsgs);
	}

	HRESULT WINAPI D3DXGatherFragmentsFromResourceA(
		HMODULE						 hSrcModule,
		LPCSTR						  pSrcResource,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXBUFFER*				ppShader,
		LPD3DXBUFFER*				ppErrorMsgs)
	{
		return (*original_D3DXGatherFragmentsFromResourceA)(hSrcModule, pSrcResource, pDefines, pInclude, Flags, ppShader, ppErrorMsgs);
	}

	HRESULT WINAPI D3DXGatherFragmentsFromResourceW(
		HMODULE						 hSrcModule,
		LPCWSTR						 pSrcResource,
		CONST D3DXMACRO*				pDefines,
		LPD3DXINCLUDE				pInclude,
		DWORD						Flags,
		LPD3DXBUFFER*				ppShader,
		LPD3DXBUFFER*				ppErrorMsgs)
	{
		return (*original_D3DXGatherFragmentsFromResourceW)(hSrcModule, pSrcResource, pDefines, pInclude, Flags, ppShader, ppErrorMsgs);
	}

	HRESULT WINAPI D3DXGenerateOutputDecl(
		D3DVERTEXELEMENT9 *pOutput,
		CONST D3DVERTEXELEMENT9 *pInput)
	{
		return (*original_D3DXGenerateOutputDecl)(pOutput, pInput);
	}

	HRESULT WINAPI D3DXGeneratePMesh(
		LPD3DXMESH pMesh,
		CONST DWORD* pAdjacency,
		CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,
		CONST FLOAT *pVertexWeights,
		DWORD MinValue,
		DWORD Options,
		LPD3DXPMESH* ppPMesh)
	{
		return (*original_D3DXGeneratePMesh)(pMesh, pAdjacency, pVertexAttributeWeights, pVertexWeights, MinValue, Options, ppPMesh);
	}

	UINT WINAPI D3DXGetDeclLength(
		CONST D3DVERTEXELEMENT9 *pDecl)
	{
		return (*original_D3DXGetDeclLength)(pDecl);
	}

	UINT WINAPI D3DXGetDeclVertexSize(
		CONST D3DVERTEXELEMENT9 *pDecl,DWORD Stream)
	{
		return (*original_D3DXGetDeclVertexSize)(pDecl, Stream);
	}

	UINT WINAPI D3DXGetDriverLevel(
		LPDIRECT3DDEVICE9 pDevice)
	{
		return (*original_D3DXGetDriverLevel)(pDevice);
	}

	UINT WINAPI D3DXGetFVFVertexSize(
		DWORD FVF)
	{
		return (*original_D3DXGetFVFVertexSize)(FVF);
	}

	HRESULT WINAPI D3DXGetImageInfoFromFileA(
		LPCSTR				 pSrcFile,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXGetImageInfoFromFileA)(pSrcFile, pSrcInfo);
	}

	HRESULT WINAPI D3DXGetImageInfoFromFileInMemory(
		LPCVOID				pSrcData,
		UINT				   SrcDataSize,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXGetImageInfoFromFileInMemory)(pSrcData, SrcDataSize, pSrcInfo);
	}

	HRESULT WINAPI D3DXGetImageInfoFromFileW(
		LPCWSTR				pSrcFile,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXGetImageInfoFromFileW)(pSrcFile, pSrcInfo);
	}

	HRESULT WINAPI D3DXGetImageInfoFromResourceA(
		HMODULE				hSrcModule,
		LPCSTR				 pSrcResource,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXGetImageInfoFromResourceA)(hSrcModule, pSrcResource, pSrcInfo);
	}

	HRESULT WINAPI D3DXGetImageInfoFromResourceW(
		HMODULE				hSrcModule,
		LPCWSTR				pSrcResource,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXGetImageInfoFromResourceW)(hSrcModule, pSrcResource, pSrcInfo);
	}

	LPCSTR WINAPI D3DXGetPixelShaderProfile(
		LPDIRECT3DDEVICE9		    pDevice)
	{
		return (*original_D3DXGetPixelShaderProfile)(pDevice);
	}

	HRESULT WINAPI D3DXGetShaderConstantTable(
		CONST DWORD*				 pFunction,
		LPD3DXCONSTANTTABLE*		 ppConstantTable)
	{
		return (*original_D3DXGetShaderConstantTable)(pFunction, ppConstantTable);
	}

	HRESULT WINAPI D3DXGetShaderInputSemantics(
		CONST DWORD*				 pFunction,
		D3DXSEMANTIC*				pSemantics,
		UINT*						pCount)
	{
		return (*original_D3DXGetShaderInputSemantics)(pFunction, pSemantics, pCount);
	}

	HRESULT WINAPI D3DXGetShaderOutputSemantics(
		CONST DWORD*				 pFunction,
		D3DXSEMANTIC*				pSemantics,
		UINT*						pCount)
	{
		return (*original_D3DXGetShaderOutputSemantics)(pFunction, pSemantics, pCount);
	}

	HRESULT WINAPI D3DXGetShaderSamplers(
		CONST DWORD*				 pFunction,
		LPCSTR*						 pSamplers,
		UINT*						pCount)
	{
		return (*original_D3DXGetShaderSamplers)(pFunction, pSamplers, pCount);
	}

	UINT WINAPI D3DXGetShaderSize(
		CONST DWORD*				 pFunction)
	{
		return (*original_D3DXGetShaderSize)(pFunction);
	}

	DWORD WINAPI D3DXGetShaderVersion(
		CONST DWORD*				 pFunction)
	{
		return (*original_D3DXGetShaderVersion)(pFunction);
	}

	LPCSTR WINAPI D3DXGetVertexShaderProfile(
		LPDIRECT3DDEVICE9		    pDevice)
	{
		return (*original_D3DXGetVertexShaderProfile)(pDevice);
	}

	HRESULT WINAPI D3DXIntersect(
		LPD3DXBASEMESH pMesh,
		CONST D3DXVECTOR3 *pRayPos,
		CONST D3DXVECTOR3 *pRayDir,
		BOOL    *pHit,
		DWORD   *pFaceIndex,
		FLOAT   *pU,
		FLOAT   *pV,
		FLOAT   *pDist,
		LPD3DXBUFFER *ppAllHits,
		DWORD   *pCountOfHits)
	{
		return (*original_D3DXIntersect)(pMesh, pRayPos, pRayDir, pHit, pFaceIndex, pU, pV, pDist, ppAllHits, pCountOfHits);
	}

	HRESULT WINAPI D3DXIntersectSubset(
		LPD3DXBASEMESH pMesh,
		DWORD AttribId,
		CONST D3DXVECTOR3 *pRayPos,
		CONST D3DXVECTOR3 *pRayDir,
		BOOL    *pHit,
		DWORD   *pFaceIndex,
		FLOAT   *pU,
		FLOAT   *pV,
		FLOAT   *pDist,
		LPD3DXBUFFER *ppAllHits,
		DWORD   *pCountOfHits)
	{
		return (*original_D3DXIntersectSubset)(pMesh, AttribId, pRayPos, pRayDir, pHit, pFaceIndex, pU, pV, pDist, ppAllHits, pCountOfHits);
	}

	BOOL WINAPI D3DXIntersectTri(
		CONST D3DXVECTOR3 *p0,
		CONST D3DXVECTOR3 *p1,
		CONST D3DXVECTOR3 *p2,
		CONST D3DXVECTOR3 *pRayPos,
		CONST D3DXVECTOR3 *pRayDir,
		FLOAT *pU,
		FLOAT *pV,
		FLOAT *pDist)
	{
		return (*original_D3DXIntersectTri)(p0, p1, p2, pRayPos, pRayDir, pU, pV, pDist);
	}

	HRESULT WINAPI D3DXLoadMeshFromXA(
		LPCSTR pFilename,
		DWORD Options,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXBUFFER *ppAdjacency,
		LPD3DXBUFFER *ppMaterials,
		LPD3DXBUFFER *ppEffectInstances,
		DWORD *pNumMaterials,
		LPD3DXMESH *ppMesh)
	{
		return (*original_D3DXLoadMeshFromXA)(pFilename, Options, pD3DDevice, ppAdjacency, ppMaterials, ppEffectInstances, pNumMaterials, ppMesh);
	}

	HRESULT WINAPI D3DXLoadMeshFromXInMemory(
		LPCVOID Memory,
		DWORD SizeOfMemory,
		DWORD Options,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXBUFFER *ppAdjacency,
		LPD3DXBUFFER *ppMaterials,
		LPD3DXBUFFER *ppEffectInstances,
		DWORD *pNumMaterials,
		LPD3DXMESH *ppMesh)
	{
		return (*original_D3DXLoadMeshFromXInMemory)(Memory, SizeOfMemory, Options, pD3DDevice, ppAdjacency, ppMaterials, ppEffectInstances, pNumMaterials, ppMesh);
	}

	HRESULT WINAPI D3DXLoadMeshFromXResource(
		HMODULE Module,
		LPCSTR Name,
		LPCSTR Type,
		DWORD Options,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXBUFFER *ppAdjacency,
		LPD3DXBUFFER *ppMaterials,
		LPD3DXBUFFER *ppEffectInstances,
		DWORD *pNumMaterials,
		LPD3DXMESH *ppMesh)
	{
		return (*original_D3DXLoadMeshFromXResource)(Module, Name, Type, Options, pD3DDevice, ppAdjacency, ppMaterials, ppEffectInstances, pNumMaterials, ppMesh);
	}

	HRESULT WINAPI D3DXLoadMeshFromXW(
		LPCWSTR pFilename,
		DWORD Options,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXBUFFER *ppAdjacency,
		LPD3DXBUFFER *ppMaterials,
		LPD3DXBUFFER *ppEffectInstances,
		DWORD *pNumMaterials,
		LPD3DXMESH *ppMesh)
	{
		return (*original_D3DXLoadMeshFromXW)(pFilename, Options, pD3DDevice, ppAdjacency, ppMaterials, ppEffectInstances, pNumMaterials, ppMesh);
	}

	HRESULT WINAPI D3DXLoadMeshFromXof(
		LPD3DXFILEDATA pxofMesh,
		DWORD Options,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXBUFFER *ppAdjacency,
		LPD3DXBUFFER *ppMaterials,
		LPD3DXBUFFER *ppEffectInstances,
		DWORD *pNumMaterials,
		LPD3DXMESH *ppMesh)
	{
		return (*original_D3DXLoadMeshFromXof)(pxofMesh, Options, pD3DDevice, ppAdjacency, ppMaterials, ppEffectInstances, pNumMaterials, ppMesh);
	}

	HRESULT WINAPI D3DXLoadMeshHierarchyFromXA(
		LPCSTR Filename,
		DWORD MeshOptions,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXALLOCATEHIERARCHY pAlloc,
		LPD3DXLOADUSERDATA pUserDataLoader,
		LPD3DXFRAME *ppFrameHierarchy,
		LPD3DXANIMATIONCONTROLLER *ppAnimController)
	{
		return (*original_D3DXLoadMeshHierarchyFromXA)(Filename, MeshOptions, pD3DDevice, pAlloc, pUserDataLoader, ppFrameHierarchy, ppAnimController);
	}

	HRESULT WINAPI D3DXLoadMeshHierarchyFromXInMemory(
		LPCVOID Memory,
		DWORD SizeOfMemory,
		DWORD MeshOptions,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXALLOCATEHIERARCHY pAlloc,
		LPD3DXLOADUSERDATA pUserDataLoader,
		LPD3DXFRAME *ppFrameHierarchy,
		LPD3DXANIMATIONCONTROLLER *ppAnimController)
	{
		return (*original_D3DXLoadMeshHierarchyFromXInMemory)(Memory, SizeOfMemory, MeshOptions, pD3DDevice, pAlloc, pUserDataLoader, ppFrameHierarchy, ppAnimController);
	}

	HRESULT WINAPI D3DXLoadMeshHierarchyFromXW(
		LPCWSTR Filename,
		DWORD MeshOptions,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXALLOCATEHIERARCHY pAlloc,
		LPD3DXLOADUSERDATA pUserDataLoader,
		LPD3DXFRAME *ppFrameHierarchy,
		LPD3DXANIMATIONCONTROLLER *ppAnimController)
	{
		return (*original_D3DXLoadMeshHierarchyFromXW)(Filename, MeshOptions, pD3DDevice, pAlloc, pUserDataLoader, ppFrameHierarchy, ppAnimController);
	}

	HRESULT WINAPI D3DXLoadPRTBufferFromFileA(
		LPCSTR pFilename,
		LPD3DXPRTBUFFER*       ppBuffer)
	{
		return (*original_D3DXLoadPRTBufferFromFileA)(pFilename, ppBuffer);
	}

	HRESULT WINAPI D3DXLoadPRTBufferFromFileW(
		LPCWSTR pFilename,
		LPD3DXPRTBUFFER*       ppBuffer)
	{
		return (*original_D3DXLoadPRTBufferFromFileW)(pFilename, ppBuffer);
	}

	HRESULT WINAPI D3DXLoadPRTCompBufferFromFileA
		(
		LPCSTR pFilename,
		LPD3DXPRTCOMPBUFFER*       ppBuffer)
	{
		return (*original_D3DXLoadPRTCompBufferFromFileA)(pFilename, ppBuffer);
	}

	HRESULT WINAPI D3DXLoadPRTCompBufferFromFileW(
		LPCWSTR pFilename,
		LPD3DXPRTCOMPBUFFER*       ppBuffer)
	{
		return (*original_D3DXLoadPRTCompBufferFromFileW)(pFilename, ppBuffer);
	}

	HRESULT WINAPI D3DXLoadPatchMeshFromXof(
		LPD3DXFILEDATA pXofObjMesh,
		DWORD Options,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXBUFFER *ppMaterials,
		LPD3DXBUFFER *ppEffectInstances,
		PDWORD pNumMaterials,
		LPD3DXPATCHMESH *ppMesh)
	{
		return (*original_D3DXLoadPatchMeshFromXof)(pXofObjMesh, Options, pD3DDevice, ppMaterials, ppEffectInstances, pNumMaterials, ppMesh);
	}

	HRESULT WINAPI D3DXLoadSkinMeshFromXof(
		LPD3DXFILEDATA pxofMesh,
		DWORD Options,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXBUFFER* ppAdjacency,
		LPD3DXBUFFER* ppMaterials,
		LPD3DXBUFFER *ppEffectInstances,
		DWORD *pMatOut,
		LPD3DXSKININFO* ppSkinInfo,
		LPD3DXMESH* ppMesh)
	{
		return (*original_D3DXLoadSkinMeshFromXof)(pxofMesh, Options, pD3DDevice, ppAdjacency, ppMaterials, ppEffectInstances, pMatOut, ppSkinInfo, ppMesh);
	}

	HRESULT WINAPI D3DXLoadSurfaceFromFileA(
		LPDIRECT3DSURFACE9		pDestSurface,
		CONST PALETTEENTRY*       pDestPalette,
		CONST RECT*		    pDestRect,
		LPCSTR				 pSrcFile,
		CONST RECT*		    pSrcRect,
		DWORD				  Filter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXLoadSurfaceFromFileA)(pDestSurface, pDestPalette, pDestRect, pSrcFile, pSrcRect, Filter, ColorKey, pSrcInfo);
	}

	HRESULT WINAPI D3DXLoadSurfaceFromFileInMemory(
		LPDIRECT3DSURFACE9		pDestSurface,
		CONST PALETTEENTRY*       pDestPalette,
		CONST RECT*		    pDestRect,
		LPCVOID				pSrcData,
		UINT				   SrcDataSize,
		CONST RECT*		    pSrcRect,
		DWORD				  Filter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXLoadSurfaceFromFileInMemory)(pDestSurface, pDestPalette, pDestRect, pSrcData, SrcDataSize, pSrcRect, Filter, ColorKey, pSrcInfo);
	}

	HRESULT WINAPI D3DXLoadSurfaceFromFileW(
		LPDIRECT3DSURFACE9		pDestSurface,
		CONST PALETTEENTRY*       pDestPalette,
		CONST RECT*		    pDestRect,
		LPCWSTR				pSrcFile,
		CONST RECT*		    pSrcRect,
		DWORD				  Filter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXLoadSurfaceFromFileW)(pDestSurface, pDestPalette, pDestRect, pSrcFile, pSrcRect, Filter, ColorKey, pSrcInfo);
	}

	HRESULT WINAPI D3DXLoadSurfaceFromMemory(
		LPDIRECT3DSURFACE9		pDestSurface,
		CONST PALETTEENTRY*       pDestPalette,
		CONST RECT*		    pDestRect,
		LPCVOID				pSrcMemory,
		D3DFORMAT				 SrcFormat,
		UINT				   SrcPitch,
		CONST PALETTEENTRY*       pSrcPalette,
		CONST RECT*		    pSrcRect,
		DWORD				  Filter,
		D3DCOLOR				  ColorKey)
	{
		return (*original_D3DXLoadSurfaceFromMemory)(pDestSurface, pDestPalette, pDestRect, pSrcMemory, SrcFormat, SrcPitch, pSrcPalette, pSrcRect, Filter, ColorKey);
	}

	HRESULT WINAPI D3DXLoadSurfaceFromResourceA(
		LPDIRECT3DSURFACE9		pDestSurface,
		CONST PALETTEENTRY*       pDestPalette,
		CONST RECT*		    pDestRect,
		HMODULE				hSrcModule,
		LPCSTR				 pSrcResource,
		CONST RECT*		    pSrcRect,
		DWORD				  Filter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXLoadSurfaceFromResourceA)(pDestSurface, pDestPalette, pDestRect, hSrcModule, pSrcResource, pSrcRect, Filter, ColorKey, pSrcInfo);
	}

	HRESULT WINAPI D3DXLoadSurfaceFromResourceW(
		LPDIRECT3DSURFACE9		pDestSurface,
		CONST PALETTEENTRY*       pDestPalette,
		CONST RECT*		    pDestRect,
		HMODULE				hSrcModule,
		LPCWSTR				pSrcResource,
		CONST RECT*		    pSrcRect,
		DWORD				  Filter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXLoadSurfaceFromResourceW)(pDestSurface, pDestPalette, pDestRect, hSrcModule, pSrcResource, pSrcRect, Filter, ColorKey, pSrcInfo);
	}

	HRESULT WINAPI D3DXLoadSurfaceFromSurface(
		LPDIRECT3DSURFACE9		pDestSurface,
		CONST PALETTEENTRY*       pDestPalette,
		CONST RECT*		    pDestRect,
		LPDIRECT3DSURFACE9		pSrcSurface,
		CONST PALETTEENTRY*       pSrcPalette,
		CONST RECT*		    pSrcRect,
		DWORD				  Filter,
		D3DCOLOR				  ColorKey)
	{
		return (*original_D3DXLoadSurfaceFromSurface)(pDestSurface, pDestPalette, pDestRect, pSrcSurface, pSrcPalette, pSrcRect, Filter, ColorKey);
	}

	HRESULT WINAPI D3DXLoadVolumeFromFileA(
		LPDIRECT3DVOLUME9		 pDestVolume,
		CONST PALETTEENTRY*       pDestPalette,
		CONST D3DBOX*		  pDestBox,
		LPCSTR				 pSrcFile,
		CONST D3DBOX*		  pSrcBox,
		DWORD				  Filter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXLoadVolumeFromFileA)(pDestVolume, pDestPalette, pDestBox, pSrcFile, pSrcBox, Filter, ColorKey, pSrcInfo);
	}

	HRESULT WINAPI D3DXLoadVolumeFromFileInMemory(
		LPDIRECT3DVOLUME9		 pDestVolume,
		CONST PALETTEENTRY*       pDestPalette,
		CONST D3DBOX*		  pDestBox,
		LPCVOID				pSrcData,
		UINT				   SrcDataSize,
		CONST D3DBOX*		  pSrcBox,
		DWORD				  Filter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXLoadVolumeFromFileInMemory)(pDestVolume, pDestPalette, pDestBox, pSrcData, SrcDataSize, pSrcBox, Filter, ColorKey, pSrcInfo);
	}

	HRESULT WINAPI D3DXLoadVolumeFromFileW(
		LPDIRECT3DVOLUME9		 pDestVolume,
		CONST PALETTEENTRY*       pDestPalette,
		CONST D3DBOX*		  pDestBox,
		LPCWSTR				pSrcFile,
		CONST D3DBOX*		  pSrcBox,
		DWORD				  Filter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXLoadVolumeFromFileW)(pDestVolume, pDestPalette, pDestBox, pSrcFile, pSrcBox, Filter, ColorKey, pSrcInfo);
	}

	HRESULT WINAPI D3DXLoadVolumeFromMemory(
		LPDIRECT3DVOLUME9		 pDestVolume,
		CONST PALETTEENTRY*       pDestPalette,
		CONST D3DBOX*		  pDestBox,
		LPCVOID				pSrcMemory,
		D3DFORMAT				 SrcFormat,
		UINT				   SrcRowPitch,
		UINT				   SrcSlicePitch,
		CONST PALETTEENTRY*       pSrcPalette,
		CONST D3DBOX*		  pSrcBox,
		DWORD				  Filter,
		D3DCOLOR				  ColorKey)
	{
		return (*original_D3DXLoadVolumeFromMemory)(pDestVolume, pDestPalette, pDestBox, pSrcMemory, SrcFormat, SrcRowPitch, SrcSlicePitch, pSrcPalette, pSrcBox, Filter, ColorKey);
	}

	HRESULT WINAPI D3DXLoadVolumeFromResourceA(
		LPDIRECT3DVOLUME9		 pDestVolume,
		CONST PALETTEENTRY*       pDestPalette,
		CONST D3DBOX*		  pDestBox,
		HMODULE				hSrcModule,
		LPCSTR				 pSrcResource,
		CONST D3DBOX*		  pSrcBox,
		DWORD				  Filter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXLoadVolumeFromResourceA)(pDestVolume, pDestPalette, pDestBox, hSrcModule, pSrcResource, pSrcBox, Filter, ColorKey, pSrcInfo);
	}

	HRESULT WINAPI D3DXLoadVolumeFromResourceW(
		LPDIRECT3DVOLUME9		 pDestVolume,
		CONST PALETTEENTRY*       pDestPalette,
		CONST D3DBOX*		  pDestBox,
		HMODULE				hSrcModule,
		LPCWSTR				pSrcResource,
		CONST D3DBOX*		  pSrcBox,
		DWORD				  Filter,
		D3DCOLOR				  ColorKey,
		D3DXIMAGE_INFO*		pSrcInfo)
	{
		return (*original_D3DXLoadVolumeFromResourceW)(pDestVolume, pDestPalette, pDestBox, hSrcModule, pSrcResource, pSrcBox, Filter, ColorKey, pSrcInfo);
	}

	HRESULT WINAPI D3DXLoadVolumeFromVolume(
		LPDIRECT3DVOLUME9		 pDestVolume,
		CONST PALETTEENTRY*       pDestPalette,
		CONST D3DBOX*		  pDestBox,
		LPDIRECT3DVOLUME9		 pSrcVolume,
		CONST PALETTEENTRY*       pSrcPalette,
		CONST D3DBOX*		  pSrcBox,
		DWORD				  Filter,
		D3DCOLOR				  ColorKey)
	{
		return (*original_D3DXLoadVolumeFromVolume)(pDestVolume, pDestPalette, pDestBox, pSrcVolume, pSrcPalette, pSrcBox, Filter, ColorKey);
	}

	D3DXMATRIX* WINAPI D3DXMatrixAffineTransformation
		(
		D3DXMATRIX *pOut,
		FLOAT Scaling,
		CONST D3DXVECTOR3 *pRotationCenter,
		CONST D3DXQUATERNION *pRotation,
		CONST D3DXVECTOR3 *pTranslation)
	{
		return (*original_D3DXMatrixAffineTransformation)(pOut, Scaling, pRotationCenter, pRotation, pTranslation);
	}

	D3DXMATRIX* WINAPI D3DXMatrixAffineTransformation2D(
		D3DXMATRIX *pOut,
		FLOAT Scaling,
		CONST D3DXVECTOR2* pRotationCenter,
		FLOAT Rotation,
		CONST D3DXVECTOR2* pTranslation)
	{
		return (*original_D3DXMatrixAffineTransformation2D)(pOut, Scaling, pRotationCenter, Rotation, pTranslation);
	}

	HRESULT WINAPI D3DXMatrixDecompose(
		D3DXVECTOR3 *pOutScale,
		D3DXQUATERNION *pOutRotation,
		D3DXVECTOR3 *pOutTranslation,
		CONST D3DXMATRIX *pM)
	{
		return (*original_D3DXMatrixDecompose)(pOutScale, pOutRotation, pOutTranslation, pM);
	}

	FLOAT WINAPI D3DXMatrixDeterminant(
		CONST D3DXMATRIX *pM)
	{
		return (*original_D3DXMatrixDeterminant)(pM);
	}

	D3DXMATRIX* WINAPI D3DXMatrixInverse(
		D3DXMATRIX *pOut,
		FLOAT *pDeterminant,
		CONST D3DXMATRIX *pM)
	{
		return (*original_D3DXMatrixInverse)(pOut, pDeterminant, pM);
	}


	D3DXMATRIX* WINAPI D3DXMatrixLookAtLH(
		D3DXMATRIX *pOut,
		CONST D3DXVECTOR3 *pEye,
		CONST D3DXVECTOR3 *pAt,
		CONST D3DXVECTOR3 *pUp)
	{
		if (pUp->y == 1) {
			v_eye.x = pEye->x;
			v_eye.y = pEye->y;
			v_eye.z = pEye->z;

			v_up.x = pUp->x;
			v_up.y = pUp->y;
			v_up.z = pUp->z;

			v_at.x = pAt->x;
			v_at.y = pAt->y;
			v_at.z = pAt->z;
		}
		//
		//::MessageBoxA(NULL,
		//	std::string(
		//	"up" + to_string(pUp->x)+" "+to_string(pUp->y)+" "+to_string(pUp->z)+"\n"
		//	+"at" + to_string(pAt->x)+" "+to_string(pAt->y)+" "+to_string(pAt->z)+"\n"
		//	+"eye" + to_string(pEye->x)+" "+to_string(pEye->y)+" "+to_string(pEye->z)+"\n").c_str(), "lookat", MB_OK);
		return (*original_D3DXMatrixLookAtLH)(pOut, pEye, pAt, pUp);
	}

	D3DXMATRIX* WINAPI D3DXMatrixLookAtRH(
		D3DXMATRIX *pOut,
		CONST D3DXVECTOR3 *pEye,
		CONST D3DXVECTOR3 *pAt,
		CONST D3DXVECTOR3 *pUp)
	{
		return (*original_D3DXMatrixLookAtRH)(pOut, pEye, pAt, pUp);
	}

	D3DXMATRIX * WINAPI D3DXMatrixMultiply(D3DXMATRIX *pOut,
		CONST D3DXMATRIX *pM1,
		CONST D3DXMATRIX *pM2)
	{
		return (*original_D3DXMatrixMultiply)(pOut, pM1, pM2);
	}

	D3DXMATRIX* WINAPI D3DXMatrixMultiplyTranspose(
		D3DXMATRIX *pOut,
		CONST D3DXMATRIX *pM1,
		CONST D3DXMATRIX *pM2)
	{
		return (*original_D3DXMatrixMultiplyTranspose)(pOut, pM1, pM2);
	}

	D3DXMATRIX* WINAPI D3DXMatrixOrthoLH(
		D3DXMATRIX *pOut,
		FLOAT w,
		FLOAT h,
		FLOAT zn,
		FLOAT zf)
	{
		return (*original_D3DXMatrixOrthoLH)(pOut, w, h, zn, zf);
	}

	D3DXMATRIX* WINAPI D3DXMatrixOrthoOffCenterLH(
		D3DXMATRIX *pOut,
		FLOAT l,
		FLOAT r,
		FLOAT b,
		FLOAT t,
		FLOAT zn,
		FLOAT zf)
	{
		return (*original_D3DXMatrixOrthoOffCenterLH)(pOut, l, r, b, t, zn, zf);
	}

	D3DXMATRIX* WINAPI D3DXMatrixOrthoOffCenterRH(
		D3DXMATRIX *pOut,
		FLOAT l,
		FLOAT r,
		FLOAT b,
		FLOAT t,
		FLOAT zn,
		FLOAT zf)
	{
		return (*original_D3DXMatrixOrthoOffCenterRH)(pOut, l, r, b, t, zn, zf);
	}

	D3DXMATRIX* WINAPI D3DXMatrixOrthoRH(
		D3DXMATRIX *pOut,
		FLOAT w,
		FLOAT h,
		FLOAT zn,
		FLOAT zf)
	{
		return (*original_D3DXMatrixOrthoRH)(pOut, w, h, zn, zf);
	}



	D3DXMATRIX* WINAPI D3DXMatrixPerspectiveFovLH(
		D3DXMATRIX *pOut,
		FLOAT fovy,
		FLOAT Aspect,
		FLOAT zn,
		FLOAT zf)
	{
		fovlh.x = fovy;
		fovlh.y = Aspect;
		fovlh.z = zn;
		fovlh.w = zf;
		return (*original_D3DXMatrixPerspectiveFovLH)(pOut, fovy, Aspect, zn, zf);
	}

	D3DXMATRIX* WINAPI D3DXMatrixPerspectiveFovRH(
		D3DXMATRIX *pOut,
		FLOAT fovy,
		FLOAT Aspect,
		FLOAT zn,
		FLOAT zf)
	{
		return (*original_D3DXMatrixPerspectiveFovRH)(pOut, fovy, Aspect, zn, zf);
	}

	D3DXMATRIX* WINAPI D3DXMatrixPerspectiveLH(
		D3DXMATRIX *pOut,
		FLOAT w,
		FLOAT h,
		FLOAT zn,
		FLOAT zf)
	{
		return (*original_D3DXMatrixPerspectiveLH)(pOut, w, h, zn, zf);
	}

	D3DXMATRIX* WINAPI D3DXMatrixPerspectiveOffCenterLH(
		D3DXMATRIX *pOut,
		FLOAT l,
		FLOAT r,
		FLOAT b,
		FLOAT t,
		FLOAT zn,
		FLOAT zf)
	{
		return (*original_D3DXMatrixPerspectiveOffCenterLH)(pOut, l, r, b, t, zn, zf);
	}

	D3DXMATRIX* WINAPI D3DXMatrixPerspectiveOffCenterRH(
		D3DXMATRIX *pOut,
		FLOAT l,
		FLOAT r,
		FLOAT b,
		FLOAT t,
		FLOAT zn,
		FLOAT zf)
	{
		return (*original_D3DXMatrixPerspectiveOffCenterRH)(pOut, l, r, b, t, zn, zf);
	}

	D3DXMATRIX* WINAPI D3DXMatrixPerspectiveRH(
		D3DXMATRIX *pOut,
		FLOAT w,
		FLOAT h,
		FLOAT zn,
		FLOAT zf)
	{
		return (*original_D3DXMatrixPerspectiveRH)(pOut, w, h, zn, zf);
	}

	D3DXMATRIX* WINAPI D3DXMatrixReflect(
		D3DXMATRIX *pOut,
		CONST D3DXPLANE *pPlane)
	{
		return (*original_D3DXMatrixReflect)(pOut, pPlane);
	}

	D3DXMATRIX* WINAPI D3DXMatrixRotationAxis(
		D3DXMATRIX *pOut,
		CONST D3DXVECTOR3 *pV,
		FLOAT Angle)
	{
		return (*original_D3DXMatrixRotationAxis)(pOut, pV, Angle);
	}

	D3DXMATRIX* WINAPI D3DXMatrixRotationQuaternion(
		D3DXMATRIX *pOut,
		CONST D3DXQUATERNION *pQ)
	{
		return (*original_D3DXMatrixRotationQuaternion)(pOut, pQ);
	}

	D3DXMATRIX* WINAPI D3DXMatrixRotationX(
		D3DXMATRIX *pOut,
		FLOAT Angle)
	{
		return (*original_D3DXMatrixRotationX)(pOut, Angle);
	}

	D3DXMATRIX* WINAPI D3DXMatrixRotationY(
		D3DXMATRIX *pOut,
		FLOAT Angle)
	{
		return (*original_D3DXMatrixRotationY)(pOut, Angle);
	}

	D3DXMATRIX* WINAPI D3DXMatrixRotationYawPitchRoll(
		D3DXMATRIX *pOut,
		FLOAT Yaw,
		FLOAT Pitch,
		FLOAT Roll)
	{
		return (*original_D3DXMatrixRotationYawPitchRoll)(pOut, Yaw, Pitch, Roll);
	}

	D3DXMATRIX* WINAPI D3DXMatrixRotationZ(
		D3DXMATRIX *pOut,
		FLOAT Angle)
	{
		return (*original_D3DXMatrixRotationZ)(pOut, Angle);
	}

	D3DXMATRIX* WINAPI D3DXMatrixScaling(
		D3DXMATRIX *pOut,
		FLOAT sx,
		FLOAT sy,
		FLOAT sz)
	{
		return (*original_D3DXMatrixScaling)(pOut, sx, sy, sz);
	}

	D3DXMATRIX* WINAPI D3DXMatrixShadow(
		D3DXMATRIX *pOut,
		CONST D3DXVECTOR4 *pLight,
		CONST D3DXPLANE *pPlane)
	{
		return (*original_D3DXMatrixShadow)(pOut, pLight, pPlane);
	}

	D3DXMATRIX* WINAPI D3DXMatrixTransformation(
		D3DXMATRIX *pOut,
		CONST D3DXVECTOR3 *pScalingCenter,
		CONST D3DXQUATERNION *pScalingRotation,
		CONST D3DXVECTOR3 *pScaling,
		CONST D3DXVECTOR3 *pRotationCenter,
		CONST D3DXQUATERNION *pRotation,
		CONST D3DXVECTOR3 *pTranslation)
	{
		return (*original_D3DXMatrixTransformation)(pOut, pScalingCenter, pScalingRotation, pScaling, pRotationCenter, pRotation, pTranslation);
	}

	D3DXMATRIX* WINAPI D3DXMatrixTransformation2D(
		D3DXMATRIX *pOut,
		CONST D3DXVECTOR2* pScalingCenter,
		FLOAT ScalingRotation,
		CONST D3DXVECTOR2* pScaling,
		CONST  D3DXVECTOR2* pRotationCenter,
		FLOAT Rotation,
		CONST  D3DXVECTOR2* pTranslation)
	{
		return (*original_D3DXMatrixTransformation2D)(pOut, pScalingCenter, ScalingRotation, pScaling, pRotationCenter, Rotation, pTranslation);
	}

	D3DXMATRIX* WINAPI D3DXMatrixTranslation(
		D3DXMATRIX *pOut,
		FLOAT x,
		FLOAT y,
		FLOAT z)
	{
		return (*original_D3DXMatrixTranslation)(pOut, x, y, z);
	}

	D3DXMATRIX* WINAPI D3DXMatrixTranspose(
		D3DXMATRIX *pOut,
		CONST D3DXMATRIX *pM)
	{
		return (*original_D3DXMatrixTranspose)(pOut, pM);
	}

	HRESULT WINAPI D3DXOptimizeFaces(
		LPCVOID pbIndices,
		UINT cFaces,
		UINT cVertices,
		BOOL b32BitIndices,
		DWORD* pFaceRemap)
	{
		return (*original_D3DXOptimizeFaces)(pbIndices, cFaces, cVertices, b32BitIndices, pFaceRemap);
	}

	HRESULT WINAPI D3DXOptimizeVertices(
		LPCVOID pbIndices,
		UINT cFaces,
		UINT cVertices,
		BOOL b32BitIndices,
		DWORD* pVertexRemap)
	{
		return (*original_D3DXOptimizeVertices)(pbIndices, cFaces, cVertices, b32BitIndices, pVertexRemap);
	}

	D3DXPLANE* WINAPI D3DXPlaneFromPointNormal(
		D3DXPLANE *pOut,
		CONST D3DXVECTOR3 *pPoint,
		CONST D3DXVECTOR3 *pNormal)
	{
		return (*original_D3DXPlaneFromPointNormal)(pOut, pPoint, pNormal);
	}

	D3DXPLANE* WINAPI D3DXPlaneFromPoints(
		D3DXPLANE *pOut,
		CONST D3DXVECTOR3 *pV1,
		CONST D3DXVECTOR3 *pV2,
		CONST D3DXVECTOR3 *pV3)
	{
		return (*original_D3DXPlaneFromPoints)(pOut, pV1, pV2, pV3);
	}

	D3DXVECTOR3* WINAPI D3DXPlaneIntersectLine(
		D3DXVECTOR3 *pOut,
		CONST D3DXPLANE *pP,
		CONST D3DXVECTOR3 *pV1,
		CONST D3DXVECTOR3 *pV2)
	{
		return (*original_D3DXPlaneIntersectLine)(pOut, pP, pV1, pV2);
	}

	D3DXPLANE* WINAPI D3DXPlaneNormalize(
		D3DXPLANE *pOut,
		CONST D3DXPLANE *pP)
	{
		return (*original_D3DXPlaneNormalize)(pOut, pP);
	}

	D3DXPLANE* WINAPI D3DXPlaneTransform(
		D3DXPLANE *pOut,
		CONST D3DXPLANE *pP,
		CONST D3DXMATRIX *pM)
	{
		return (*original_D3DXPlaneTransform)(pOut, pP, pM);
	}

	D3DXPLANE* WINAPI D3DXPlaneTransformArray(
		D3DXPLANE *pOut,
		UINT OutStride,
		CONST D3DXPLANE *pP,
		UINT PStride,
		CONST D3DXMATRIX *pM,
		UINT n)
	{
		return (*original_D3DXPlaneTransformArray)(pOut, OutStride, pP, PStride, pM, n);
	}

	HRESULT WINAPI D3DXPreprocessShader(
		LPCSTR				    pSrcData,
		UINT						 SrcDataSize,
		CONST D3DXMACRO*		  pDefines,
		LPD3DXINCLUDE				pInclude,
		LPD3DXBUFFER*				ppShaderText,
		LPD3DXBUFFER*				ppErrorMsgs)
	{
		return (*original_D3DXPreprocessShader)(pSrcData, SrcDataSize, pDefines, pInclude, ppShaderText, ppErrorMsgs);
	}

	HRESULT WINAPI D3DXPreprocessShaderFromFileA(
		LPCSTR				    pSrcFile,
		CONST D3DXMACRO*		  pDefines,
		LPD3DXINCLUDE				pInclude,
		LPD3DXBUFFER*				ppShaderText,
		LPD3DXBUFFER*				ppErrorMsgs)
	{
		return (*original_D3DXPreprocessShaderFromFileA)(pSrcFile, pDefines, pInclude, ppShaderText, ppErrorMsgs);
	}

	HRESULT WINAPI D3DXPreprocessShaderFromFileW(
		LPCWSTR				   pSrcFile,
		CONST D3DXMACRO*		  pDefines,
		LPD3DXINCLUDE				pInclude,
		LPD3DXBUFFER*				ppShaderText,
		LPD3DXBUFFER*				ppErrorMsgs)
	{
		return (*original_D3DXPreprocessShaderFromFileW)(pSrcFile, pDefines, pInclude, ppShaderText, ppErrorMsgs);
	}

	HRESULT WINAPI D3DXPreprocessShaderFromResourceA(
		HMODULE				   hSrcModule,
		LPCSTR				    pSrcResource,
		CONST D3DXMACRO*		  pDefines,
		LPD3DXINCLUDE				pInclude,
		LPD3DXBUFFER*				ppShaderText,
		LPD3DXBUFFER*				ppErrorMsgs)
	{
		return (*original_D3DXPreprocessShaderFromResourceA)(hSrcModule, pSrcResource, pDefines, pInclude, ppShaderText, ppErrorMsgs);
	}

	HRESULT WINAPI D3DXPreprocessShaderFromResourceW(
		HMODULE				   hSrcModule,
		LPCWSTR				   pSrcResource,
		CONST D3DXMACRO*		  pDefines,
		LPD3DXINCLUDE				pInclude,
		LPD3DXBUFFER*				ppShaderText,
		LPD3DXBUFFER*				ppErrorMsgs)
	{
		return (*original_D3DXPreprocessShaderFromResourceW)(hSrcModule, pSrcResource, pDefines, pInclude, ppShaderText, ppErrorMsgs);
	}

	D3DXQUATERNION* WINAPI D3DXQuaternionBaryCentric(
		D3DXQUATERNION *pOut,
		CONST D3DXQUATERNION *pQ1,
		CONST D3DXQUATERNION *pQ2,
		CONST D3DXQUATERNION *pQ3,
		FLOAT f,
		FLOAT g)
	{
		return (*original_D3DXQuaternionBaryCentric)(pOut, pQ1, pQ2, pQ3, f, g);
	}

	D3DXQUATERNION* WINAPI D3DXQuaternionExp(
		D3DXQUATERNION *pOut,
		CONST D3DXQUATERNION *pQ)
	{
		return (*original_D3DXQuaternionExp)(pOut, pQ);
	}

	D3DXQUATERNION* WINAPI D3DXQuaternionInverse(
		D3DXQUATERNION *pOut,
		CONST D3DXQUATERNION *pQ)
	{
		return (*original_D3DXQuaternionInverse)(pOut, pQ);
	}

	D3DXQUATERNION* WINAPI D3DXQuaternionLn(
		D3DXQUATERNION *pOut,
		CONST D3DXQUATERNION *pQ)
	{
		return (*original_D3DXQuaternionLn)(pOut, pQ);
	}

	D3DXQUATERNION* WINAPI D3DXQuaternionMultiply(
		D3DXQUATERNION *pOut,
		CONST D3DXQUATERNION *pQ1,
		CONST D3DXQUATERNION *pQ2)
	{
		return (*original_D3DXQuaternionMultiply)(pOut, pQ1, pQ2);
	}

	D3DXQUATERNION* WINAPI D3DXQuaternionNormalize(
		D3DXQUATERNION *pOut,
		CONST D3DXQUATERNION *pQ)
	{
		return (*original_D3DXQuaternionNormalize)(pOut, pQ);
	}

	D3DXQUATERNION* WINAPI D3DXQuaternionRotationAxis(
		D3DXQUATERNION *pOut,
		CONST D3DXVECTOR3 *pV,
		FLOAT Angle)
	{
		return (*original_D3DXQuaternionRotationAxis)(pOut, pV, Angle);
	}

	D3DXQUATERNION* WINAPI D3DXQuaternionRotationMatrix(
		D3DXQUATERNION *pOut,
		CONST D3DXMATRIX *pM)
	{
		return (*original_D3DXQuaternionRotationMatrix)(pOut, pM);
	}

	D3DXQUATERNION* WINAPI D3DXQuaternionRotationYawPitchRoll(
		D3DXQUATERNION *pOut,
		FLOAT Yaw,
		FLOAT Pitch,
		FLOAT Roll)
	{
		return (*original_D3DXQuaternionRotationYawPitchRoll)(pOut, Yaw, Pitch, Roll);
	}

	D3DXQUATERNION* WINAPI D3DXQuaternionSlerp(
		D3DXQUATERNION *pOut,
		CONST D3DXQUATERNION *pQ1,
		CONST D3DXQUATERNION *pQ2,
		FLOAT t)
	{
		return (*original_D3DXQuaternionSlerp)(pOut, pQ1, pQ2, t);
	}

	D3DXQUATERNION* WINAPI D3DXQuaternionSquad(
		D3DXQUATERNION *pOut,
		CONST D3DXQUATERNION *pQ1,
		CONST D3DXQUATERNION *pA,
		CONST D3DXQUATERNION *pB,
		CONST D3DXQUATERNION *pC,
		FLOAT t)
	{
		return (*original_D3DXQuaternionSquad)(pOut, pQ1, pA, pB, pC, t);
	}

	void WINAPI D3DXQuaternionSquadSetup(
		D3DXQUATERNION *pAOut,
		D3DXQUATERNION *pBOut,
		D3DXQUATERNION *pCOut,
		CONST D3DXQUATERNION *pQ0,
		CONST D3DXQUATERNION *pQ1,
		CONST D3DXQUATERNION *pQ2,
		CONST D3DXQUATERNION *pQ3)
	{
		return (*original_D3DXQuaternionSquadSetup)(pAOut, pBOut, pCOut, pQ0, pQ1, pQ2, pQ3);
	}

	void WINAPI D3DXQuaternionToAxisAngle(
		CONST D3DXQUATERNION *pQ,
		D3DXVECTOR3 *pAxis,
		FLOAT *pAngle)
	{
		return (*original_D3DXQuaternionToAxisAngle)(pQ, pAxis, pAngle);
	}

	HRESULT WINAPI D3DXRectPatchSize(
		CONST FLOAT *pfNumSegs,
		DWORD *pdwTriangles,
		DWORD *pdwVertices)
	{
		return (*original_D3DXRectPatchSize)(pfNumSegs, pdwTriangles, pdwVertices);
	}

	FLOAT* WINAPI D3DXSHAdd(
		FLOAT *pOut,
		UINT Order,
		CONST FLOAT *pA,
		CONST FLOAT *pB)
	{
		return (*original_D3DXSHAdd)(pOut, Order, pA, pB);
	}

	FLOAT WINAPI D3DXSHDot(
		UINT Order,
		CONST FLOAT *pA,
		CONST FLOAT *pB)
	{
		return (*original_D3DXSHDot)(Order, pA, pB);
	}

	HRESULT WINAPI D3DXSHEvalConeLight(
		UINT Order,
		CONST D3DXVECTOR3 *pDir,
		FLOAT Radius,
		FLOAT RIntensity,
		FLOAT GIntensity,
		FLOAT BIntensity,
		FLOAT *pROut,
		FLOAT *pGOut,
		FLOAT *pBOut)
	{
		return (*original_D3DXSHEvalConeLight)(Order, pDir, Radius, RIntensity, GIntensity, BIntensity, pROut, pGOut, pBOut);
	}

	FLOAT* WINAPI D3DXSHEvalDirection(
		FLOAT *pOut,
		UINT Order,
		CONST D3DXVECTOR3 *pDir)
	{
		return (*original_D3DXSHEvalDirection)(pOut, Order, pDir);
	}

	HRESULT WINAPI D3DXSHEvalDirectionalLight(
		UINT Order,
		CONST D3DXVECTOR3 *pDir,
		FLOAT RIntensity,
		FLOAT GIntensity,
		FLOAT BIntensity,
		FLOAT *pROut,
		FLOAT *pGOut,
		FLOAT *pBOut)
	{
		return (*original_D3DXSHEvalDirectionalLight)(Order, pDir, RIntensity, GIntensity, BIntensity, pROut, pGOut, pBOut);
	}

	HRESULT WINAPI D3DXSHEvalHemisphereLight(
		UINT Order,
		CONST D3DXVECTOR3 *pDir,
		D3DXCOLOR Top,
		D3DXCOLOR Bottom,
		FLOAT *pROut,
		FLOAT *pGOut,
		FLOAT *pBOut)
	{
		return (*original_D3DXSHEvalHemisphereLight)(Order, pDir, Top, Bottom, pROut, pGOut, pBOut);
	}

	HRESULT WINAPI D3DXSHEvalSphericalLight(
		UINT Order,
		CONST D3DXVECTOR3 *pPos,
		FLOAT Radius,
		FLOAT RIntensity,
		FLOAT GIntensity,
		FLOAT BIntensity,
		FLOAT *pROut,
		FLOAT *pGOut,
		FLOAT *pBOut)
	{
		return (*original_D3DXSHEvalSphericalLight)(Order, pPos, Radius, RIntensity, GIntensity, BIntensity, pROut, pGOut, pBOut);
	}

	FLOAT* WINAPI D3DXSHMultiply2(
		FLOAT *pOut,
		CONST FLOAT *pF,
		CONST FLOAT *pG)
	{
		return (*original_D3DXSHMultiply2)(pOut, pF, pG);
	}

	FLOAT* WINAPI D3DXSHMultiply3(
		FLOAT *pOut,
		CONST FLOAT *pF,
		CONST FLOAT *pG)
	{
		return (*original_D3DXSHMultiply3)(pOut, pF, pG);
	}

	FLOAT* WINAPI D3DXSHMultiply4(
		FLOAT *pOut,
		CONST FLOAT *pF,
		CONST FLOAT *pG)
	{
		return (*original_D3DXSHMultiply4)(pOut, pF, pG);
	}

	FLOAT* WINAPI D3DXSHMultiply5(


		FLOAT *pOut,
		CONST FLOAT *pF,
		CONST FLOAT *pG)
	{
		return (*original_D3DXSHMultiply5)(pOut, pF, pG);
	}

	FLOAT* WINAPI D3DXSHMultiply6(


		FLOAT *pOut,
		CONST FLOAT *pF,
		CONST FLOAT *pG)
	{
		return (*original_D3DXSHMultiply6)(pOut, pF, pG);
	}

	HRESULT WINAPI D3DXSHPRTCompSplitMeshSC(
		UINT *pClusterIDs,
		UINT NumVertices,
		UINT NumClusters,
		UINT *pSuperClusterIDs,
		UINT NumSuperClusters,
		LPVOID pInputIB,
		BOOL InputIBIs32Bit,
		UINT NumFaces,
		LPD3DXBUFFER *ppIBData,
		UINT *pIBDataLength,
		BOOL OutputIBIs32Bit,
		LPD3DXBUFFER *ppFaceRemap,
		LPD3DXBUFFER *ppVertData,
		UINT *pVertDataLength,
		UINT *pSCClusterList,
		D3DXSHPRTSPLITMESHCLUSTERDATA *pSCData)
	{
		return (*original_D3DXSHPRTCompSplitMeshSC)(pClusterIDs, NumVertices, NumClusters, pSuperClusterIDs, NumSuperClusters, pInputIB, InputIBIs32Bit, NumFaces, ppIBData, pIBDataLength, OutputIBIs32Bit, ppFaceRemap, ppVertData, pVertDataLength, pSCClusterList, pSCData);
	}

	HRESULT WINAPI D3DXSHPRTCompSuperCluster(
		UINT *pClusterIDs,
		LPD3DXMESH pScene,
		UINT MaxNumClusters,
		UINT NumClusters,
		UINT *pSuperClusterIDs,
		UINT *pNumSuperClusters)
	{
		return (*original_D3DXSHPRTCompSuperCluster)(pClusterIDs, pScene, MaxNumClusters, NumClusters, pSuperClusterIDs, pNumSuperClusters);
	}

	HRESULT WINAPI D3DXSHProjectCubeMap(
		UINT uOrder,
		LPDIRECT3DCUBETEXTURE9 pCubeMap,
		FLOAT *pROut,
		FLOAT *pGOut,
		FLOAT *pBOut)
	{
		return (*original_D3DXSHProjectCubeMap)(uOrder, pCubeMap, pROut, pGOut, pBOut);
	}

	FLOAT* WINAPI D3DXSHRotate(
		FLOAT *pOut,
		UINT Order,
		CONST D3DXMATRIX *pMatrix,
		CONST FLOAT *pIn)
	{
		return (*original_D3DXSHRotate)(pOut, Order, pMatrix, pIn);
	}

	FLOAT* WINAPI D3DXSHRotateZ(
		FLOAT *pOut,
		UINT Order,
		FLOAT Angle,
		CONST FLOAT *pIn)
	{
		return (*original_D3DXSHRotateZ)(pOut, Order, Angle, pIn);
	}

	FLOAT* WINAPI D3DXSHScale(
		FLOAT *pOut,
		UINT Order,
		CONST FLOAT *pIn,
		CONST FLOAT Scale)
	{
		return (*original_D3DXSHScale)(pOut, Order, pIn, Scale);
	}

	HRESULT WINAPI D3DXSaveMeshHierarchyToFileA(
		LPCSTR Filename,
		DWORD XFormat,
		CONST D3DXFRAME *pFrameRoot,
		LPD3DXANIMATIONCONTROLLER pAnimcontroller,
		LPD3DXSAVEUSERDATA pUserDataSaver)
	{
		return (*original_D3DXSaveMeshHierarchyToFileA)(Filename, XFormat, pFrameRoot, pAnimcontroller, pUserDataSaver);
	}

	HRESULT WINAPI D3DXSaveMeshHierarchyToFileW(
		LPCWSTR Filename,
		DWORD XFormat,
		CONST D3DXFRAME *pFrameRoot,
		LPD3DXANIMATIONCONTROLLER pAnimController,
		LPD3DXSAVEUSERDATA pUserDataSaver)
	{
		return (*original_D3DXSaveMeshHierarchyToFileW)(Filename, XFormat, pFrameRoot, pAnimController, pUserDataSaver);
	}

	HRESULT WINAPI D3DXSaveMeshToXA(
		LPCSTR pFilename,
		LPD3DXMESH pMesh,
		CONST DWORD* pAdjacency,
		CONST D3DXMATERIAL* pMaterials,
		CONST D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials,
		DWORD Format)
	{
		return (*original_D3DXSaveMeshToXA)(pFilename, pMesh, pAdjacency, pMaterials, pEffectInstances, NumMaterials, Format);
	}

	HRESULT WINAPI D3DXSaveMeshToXW(
		LPCWSTR pFilename,
		LPD3DXMESH pMesh,
		CONST DWORD* pAdjacency,
		CONST D3DXMATERIAL* pMaterials,
		CONST D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials,
		DWORD Format)
	{
		return (*original_D3DXSaveMeshToXW)(pFilename, pMesh, pAdjacency, pMaterials, pEffectInstances, NumMaterials, Format);
	}

	HRESULT WINAPI D3DXSavePRTBufferToFileA(
		LPCSTR pFileName,
		LPD3DXPRTBUFFER pBuffer)
	{
		return (*original_D3DXSavePRTBufferToFileA)(pFileName, pBuffer);
	}

	HRESULT WINAPI D3DXSavePRTBufferToFileW(
		LPCWSTR pFileName,
		LPD3DXPRTBUFFER pBuffer)
	{
		return (*original_D3DXSavePRTBufferToFileW)(pFileName, pBuffer);
	}

	HRESULT WINAPI D3DXSavePRTCompBufferToFileA
		(
		LPCSTR pFileName,
		LPD3DXPRTCOMPBUFFER pBuffer)
	{
		return (*original_D3DXSavePRTCompBufferToFileA)(pFileName, pBuffer);
	}

	HRESULT WINAPI D3DXSavePRTCompBufferToFileW(
		LPCWSTR pFileName,
		LPD3DXPRTCOMPBUFFER pBuffer)
	{
		return (*original_D3DXSavePRTCompBufferToFileW)(pFileName, pBuffer);
	}

	HRESULT WINAPI D3DXSaveSurfaceToFileA(
		LPCSTR				 pDestFile,
		D3DXIMAGE_FILEFORMAT      DestFormat,
		LPDIRECT3DSURFACE9		pSrcSurface,
		CONST PALETTEENTRY*       pSrcPalette,
		CONST RECT*		    pSrcRect)
	{
		return (*original_D3DXSaveSurfaceToFileA)(pDestFile, DestFormat, pSrcSurface, pSrcPalette, pSrcRect);
	}

	HRESULT WINAPI D3DXSaveSurfaceToFileInMemory(
		LPD3DXBUFFER*		  ppDestBuf,
		D3DXIMAGE_FILEFORMAT      DestFormat,
		LPDIRECT3DSURFACE9		pSrcSurface,
		CONST PALETTEENTRY*       pSrcPalette,
		CONST RECT*		    pSrcRect)
	{
		return (*original_D3DXSaveSurfaceToFileInMemory)(ppDestBuf, DestFormat, pSrcSurface, pSrcPalette, pSrcRect);
	}

	HRESULT WINAPI D3DXSaveSurfaceToFileW(
		LPCWSTR				pDestFile,
		D3DXIMAGE_FILEFORMAT      DestFormat,
		LPDIRECT3DSURFACE9		pSrcSurface,
		CONST PALETTEENTRY*       pSrcPalette,
		CONST RECT*		    pSrcRect)
	{
		return (*original_D3DXSaveSurfaceToFileW)(pDestFile, DestFormat, pSrcSurface, pSrcPalette, pSrcRect);
	}

	HRESULT WINAPI D3DXSaveTextureToFileA(
		LPCSTR				 pDestFile,
		D3DXIMAGE_FILEFORMAT      DestFormat,
		LPDIRECT3DBASETEXTURE9    pSrcTexture,
		CONST PALETTEENTRY*       pSrcPalette)
	{
		return (*original_D3DXSaveTextureToFileA)(pDestFile, DestFormat, pSrcTexture, pSrcPalette);
	}

	HRESULT WINAPI D3DXSaveTextureToFileInMemory(
		LPD3DXBUFFER*		  ppDestBuf,
		D3DXIMAGE_FILEFORMAT      DestFormat,
		LPDIRECT3DBASETEXTURE9    pSrcTexture,
		CONST PALETTEENTRY*       pSrcPalette)
	{
		return (*original_D3DXSaveTextureToFileInMemory)(ppDestBuf, DestFormat, pSrcTexture, pSrcPalette);
	}

	HRESULT WINAPI D3DXSaveTextureToFileW(
		LPCWSTR				pDestFile,
		D3DXIMAGE_FILEFORMAT      DestFormat,
		LPDIRECT3DBASETEXTURE9    pSrcTexture,
		CONST PALETTEENTRY*       pSrcPalette)
	{
		return (*original_D3DXSaveTextureToFileW)(pDestFile, DestFormat, pSrcTexture, pSrcPalette);
	}

	HRESULT WINAPI D3DXSaveVolumeToFileA(
		LPCSTR				 pDestFile,
		D3DXIMAGE_FILEFORMAT      DestFormat,
		LPDIRECT3DVOLUME9		 pSrcVolume,
		CONST PALETTEENTRY*       pSrcPalette,
		CONST D3DBOX*		  pSrcBox)
	{
		return (*original_D3DXSaveVolumeToFileA)(pDestFile, DestFormat, pSrcVolume, pSrcPalette, pSrcBox);
	}

	HRESULT WINAPI D3DXSaveVolumeToFileInMemory(
		LPD3DXBUFFER*		  ppDestBuf,
		D3DXIMAGE_FILEFORMAT      DestFormat,
		LPDIRECT3DVOLUME9		 pSrcVolume,
		CONST PALETTEENTRY*       pSrcPalette,
		CONST D3DBOX*		  pSrcBox)
	{
		return (*original_D3DXSaveVolumeToFileInMemory)(ppDestBuf, DestFormat, pSrcVolume, pSrcPalette, pSrcBox);
	}

	HRESULT WINAPI D3DXSaveVolumeToFileW(
		LPCWSTR				pDestFile,
		D3DXIMAGE_FILEFORMAT      DestFormat,
		LPDIRECT3DVOLUME9		 pSrcVolume,
		CONST PALETTEENTRY*       pSrcPalette,
		CONST D3DBOX*		  pSrcBox)
	{
		return (*original_D3DXSaveVolumeToFileW)(pDestFile, DestFormat, pSrcVolume, pSrcPalette, pSrcBox);
	}

	HRESULT WINAPI D3DXSimplifyMesh(
		LPD3DXMESH pMesh,
		CONST DWORD* pAdjacency,
		CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,
		CONST FLOAT *pVertexWeights,
		DWORD MinValue,
		DWORD Options,
		LPD3DXMESH* ppMesh)
	{
		return (*original_D3DXSimplifyMesh)(pMesh, pAdjacency, pVertexAttributeWeights, pVertexWeights, MinValue, Options, ppMesh);
	}

	BOOL WINAPI D3DXSphereBoundProbe(
		CONST D3DXVECTOR3 *pCenter,
		FLOAT Radius,
		CONST D3DXVECTOR3 *pRayPosition,
		CONST D3DXVECTOR3 *pRayDirection)
	{
		return (*original_D3DXSphereBoundProbe)(pCenter, Radius, pRayPosition, pRayDirection);
	}

	HRESULT WINAPI D3DXSplitMesh(
		LPD3DXMESH pMeshIn,
		CONST DWORD *pAdjacencyIn,
		CONST DWORD MaxSize,
		CONST DWORD Options,
		DWORD *pMeshesOut,
		LPD3DXBUFFER *ppMeshArrayOut,
		LPD3DXBUFFER *ppAdjacencyArrayOut,
		LPD3DXBUFFER *ppFaceRemapArrayOut,
		LPD3DXBUFFER *ppVertRemapArrayOut)
	{
		return (*original_D3DXSplitMesh)(pMeshIn, pAdjacencyIn, MaxSize, Options, pMeshesOut, ppMeshArrayOut, ppAdjacencyArrayOut, ppFaceRemapArrayOut, ppVertRemapArrayOut);
	}

	HRESULT WINAPI D3DXTessellateNPatches(
		LPD3DXMESH pMeshIn,
		CONST DWORD* pAdjacencyIn,
		FLOAT NumSegs,
		BOOL  QuadraticInterpNormals,
		LPD3DXMESH *ppMeshOut,
		LPD3DXBUFFER *ppAdjacencyOut)
	{
		return (*original_D3DXTessellateNPatches)(pMeshIn, pAdjacencyIn, NumSegs, QuadraticInterpNormals, ppMeshOut, ppAdjacencyOut);
	}

	HRESULT WINAPI D3DXTessellateRectPatch(
		LPDIRECT3DVERTEXBUFFER9 pVB,
		CONST FLOAT *pNumSegs,
		CONST D3DVERTEXELEMENT9 *pdwInDecl,
		CONST D3DRECTPATCH_INFO *pRectPatchInfo,
		LPD3DXMESH pMesh)
	{
		return (*original_D3DXTessellateRectPatch)(pVB, pNumSegs, pdwInDecl, pRectPatchInfo, pMesh);
	}

	HRESULT WINAPI D3DXTessellateTriPatch(
		LPDIRECT3DVERTEXBUFFER9 pVB,
		CONST FLOAT *pNumSegs,
		CONST D3DVERTEXELEMENT9 *pInDecl,
		CONST D3DTRIPATCH_INFO *pTriPatchInfo,
		LPD3DXMESH pMesh)
	{
		return (*original_D3DXTessellateTriPatch)(pVB, pNumSegs, pInDecl, pTriPatchInfo, pMesh);
	}

	HRESULT WINAPI D3DXTriPatchSize(
		CONST FLOAT *pfNumSegs,
		DWORD *pdwTriangles,
		DWORD *pdwVertices)
	{
		return (*original_D3DXTriPatchSize)(pfNumSegs, pdwTriangles, pdwVertices);
	}

	/*
	HRESULT WINAPI D3DXUVAtlasCreate(
		LPD3DXMESH pMesh,
		UINT uMaxChartNumber,
		FLOAT fMaxStretch,
		UINT uWidth,
		UINT uHeight,
		FLOAT fGutter,
		DWORD dwTextureIndex,
		CONST DWORD *pdwAdjacency,
		CONST DWORD *pdwFalseEdgeAdjacency,
		CONST FLOAT *pfIMTArray,
		LPD3DXUVATLASCB pStatusCallback,
		FLOAT fCallbackFrequency,
		LPVOID pUserContext,
		LPD3DXMESH *ppMeshOut,
		LPD3DXBUFFER *ppFacePartitioning,
		LPD3DXBUFFER *ppVertexRemapArray,
		FLOAT *pfMaxStretchOut,
		UINT *puNumChartsOut)
	{
		return (*original_D3DXUVAtlasCreate)(pMesh, uMaxChartNumber, fMaxStretch, uWidth, uHeight, fGutter, dwTextureIndex, pdwAdjacency, pdwFalseEdgeAdjacency, pfIMTArray, pStatusCallback, fCallbackFrequency, pUserContext, ppMeshOut, ppFacePartitioning, ppVertexRemapArray, pfMaxStretchOut, puNumChartsOut);
	}
	*/

	/*
	HRESULT WINAPI D3DXUVAtlasPack(
		ID3DXMesh *pMesh,
        UINT uWidth,
        UINT uHeight,
        FLOAT fGutter,
        DWORD dwTextureIndex,
        CONST DWORD *pdwPartitionResultAdjacency,
        LPD3DXUVATLASCB pStatusCallback,
        FLOAT fCallbackFrequency,
        LPVOID pUserContext,
        LPD3DXBUFFER pFacePartitioning)
	{
		return (*original_D3DXUVAtlasPack)
			(pMesh, uWidth, uHeight, fGutter, dwTextureIndex,
			pdwPartitionResultAdjacency, pStatusCallback, fCallbackFrequency, pUserContext, pFacePartitioning);
	}
	*/

	/*
	HRESULT WINAPI D3DXUVAtlasPartition(
		LPD3DXMESH pMesh,
        UINT uMaxChartNumber,
        FLOAT fMaxStretch,
        DWORD dwTextureIndex,
        CONST DWORD *pdwAdjacency,
        CONST DWORD *pdwFalseEdgeAdjacency,
        CONST FLOAT *pfIMTArray,
        LPD3DXUVATLASCB pStatusCallback,
        FLOAT fCallbackFrequency,
        LPVOID pUserContext,
        LPD3DXMESH *ppMeshOut,
        LPD3DXBUFFER *ppFacePartitioning,
        LPD3DXBUFFER *ppVertexRemapArray,
        LPD3DXBUFFER *ppPartitionResultAdjacency,
        FLOAT *pfMaxStretchOut,
        UINT *puNumChartsOut)
	{
		return (*original_D3DXUVAtlasPartition)
			(pMesh, uMaxChartNumber, fMaxStretch, dwTextureIndex, pdwAdjacency,
			pdwFalseEdgeAdjacency, pfIMTArray, pStatusCallback, fCallbackFrequency, pUserContext, ppMeshOut, ppFacePartitioning, ppVertexRemapArray, ppPartitionResultAdjacency, pfMaxStretchOut, puNumChartsOut);
	}
	*/

	HRESULT WINAPI D3DXValidMesh (
		LPD3DXMESH pMeshIn,
		CONST DWORD* pAdjacency,
		LPD3DXBUFFER* ppErrorsAndWarnings)
	{
		return (*original_D3DXValidMesh)(pMeshIn, pAdjacency, ppErrorsAndWarnings);
	}

	HRESULT WINAPI D3DXValidPatchMesh(
		LPD3DXPATCHMESH pMesh,
		DWORD *dwcDegenerateVertices,
		DWORD *dwcDegeneratePatches,
		LPD3DXBUFFER *ppErrorsAndWarnings)
	{
		return (*original_D3DXValidPatchMesh)(pMesh, dwcDegenerateVertices, dwcDegeneratePatches, ppErrorsAndWarnings);
	}

	D3DXVECTOR2* WINAPI D3DXVec2BaryCentric(
		D3DXVECTOR2 *pOut,
		CONST D3DXVECTOR2 *pV1,
		CONST D3DXVECTOR2 *pV2,
		CONST D3DXVECTOR2 *pV3,
		FLOAT f,
		FLOAT g)
	{
		return (*original_D3DXVec2BaryCentric)(pOut, pV1, pV2, pV3, f, g);
	}

	D3DXVECTOR2* WINAPI D3DXVec2CatmullRom(
		D3DXVECTOR2 *pOut,
		CONST D3DXVECTOR2 *pV0,
		CONST D3DXVECTOR2 *pV1,
		CONST D3DXVECTOR2 *pV2,
		CONST D3DXVECTOR2 *pV3,
		FLOAT s)
	{
		return (*original_D3DXVec2CatmullRom)(pOut, pV0, pV1, pV2, pV3, s);
	}

	D3DXVECTOR2* WINAPI D3DXVec2Hermite(
		D3DXVECTOR2 *pOut,
		CONST D3DXVECTOR2 *pV1,
		CONST D3DXVECTOR2 *pT1,
		CONST D3DXVECTOR2 *pV2,
		CONST D3DXVECTOR2 *pT2,
		FLOAT s)
	{
		return (*original_D3DXVec2Hermite)(pOut, pV1, pT1, pV2, pT2, s);
	}

	D3DXVECTOR2* WINAPI D3DXVec2Normalize(
		D3DXVECTOR2 *pOut,
		CONST D3DXVECTOR2 *pV)
	{
		return (*original_D3DXVec2Normalize)(pOut, pV);
	}

	D3DXVECTOR4* WINAPI D3DXVec2Transform(
		D3DXVECTOR4 *pOut,
		CONST D3DXVECTOR2 *pV,
		CONST D3DXMATRIX *pM)
	{
		return (*original_D3DXVec2Transform)(pOut, pV, pM);
	}

	D3DXVECTOR4* WINAPI D3DXVec2TransformArray(
		D3DXVECTOR4 *pOut,
		UINT OutStride,
		CONST D3DXVECTOR2 *pV,
		UINT VStride,
		CONST D3DXMATRIX *pM,
		UINT n)
	{
		return (*original_D3DXVec2TransformArray)(pOut, OutStride, pV, VStride, pM, n);
	}

	D3DXVECTOR2* WINAPI D3DXVec2TransformCoord(
		D3DXVECTOR2 *pOut,
		CONST D3DXVECTOR2 *pV,
		CONST D3DXMATRIX *pM)
	{
		return (*original_D3DXVec2TransformCoord)(pOut, pV, pM);
	}

	D3DXVECTOR2* WINAPI D3DXVec2TransformCoordArray(
		D3DXVECTOR2 *pOut,
		UINT OutStride,
		CONST D3DXVECTOR2 *pV,
		UINT VStride,
		CONST D3DXMATRIX *pM,
		UINT n)
	{
		return (*original_D3DXVec2TransformCoordArray)(pOut, OutStride, pV, VStride, pM, n);
	}

	D3DXVECTOR2* WINAPI D3DXVec2TransformNormal(
		D3DXVECTOR2 *pOut,
		CONST D3DXVECTOR2 *pV,
		CONST D3DXMATRIX *pM)
	{
		return (*original_D3DXVec2TransformNormal)(pOut, pV, pM);
	}

	D3DXVECTOR2* WINAPI D3DXVec2TransformNormalArray(
		D3DXVECTOR2 *pOut,
		UINT OutStride,
		CONST D3DXVECTOR2 *pV,
		UINT VStride,
		CONST D3DXMATRIX *pM,
		UINT n)
	{
		return (*original_D3DXVec2TransformNormalArray)(pOut, OutStride, pV, VStride, pM, n);
	}

	D3DXVECTOR3* WINAPI D3DXVec3BaryCentric(
		D3DXVECTOR3 *pOut,
		CONST D3DXVECTOR3 *pV1,
		CONST D3DXVECTOR3 *pV2,
		CONST D3DXVECTOR3 *pV3,
		FLOAT f,
		FLOAT g)
	{
		return (*original_D3DXVec3BaryCentric)(pOut, pV1, pV2, pV3, f, g);
	}

	D3DXVECTOR3* WINAPI D3DXVec3CatmullRom(
		D3DXVECTOR3 *pOut,
		CONST D3DXVECTOR3 *pV0,
		CONST D3DXVECTOR3 *pV1,
		CONST D3DXVECTOR3 *pV2,
		CONST D3DXVECTOR3 *pV3,
		FLOAT s)
	{
		return (*original_D3DXVec3CatmullRom)(pOut, pV0, pV1, pV2, pV3, s);
	}

	D3DXVECTOR3* WINAPI D3DXVec3Hermite(
		D3DXVECTOR3 *pOut,
		CONST D3DXVECTOR3 *pV1,
		CONST D3DXVECTOR3 *pT1,
		CONST D3DXVECTOR3 *pV2,
		CONST D3DXVECTOR3 *pT2,
		FLOAT s)
	{
		return (*original_D3DXVec3Hermite)(pOut, pV1, pT1, pV2, pT2, s);
	}

	D3DXVECTOR3* WINAPI D3DXVec3Normalize(
		D3DXVECTOR3 *pOut,
		CONST D3DXVECTOR3 *pV)
	{
		return (*original_D3DXVec3Normalize)(pOut, pV);
	}

	D3DXVECTOR3* WINAPI D3DXVec3Project(
		D3DXVECTOR3 *pOut,
		CONST D3DXVECTOR3 *pV,
		CONST D3DVIEWPORT9 *pViewport,
		CONST D3DXMATRIX *pProjection,
		CONST D3DXMATRIX *pView,
		CONST D3DXMATRIX *pWorld)
	{
		return (*original_D3DXVec3Project)(pOut, pV, pViewport, pProjection, pView, pWorld);
	}

	D3DXVECTOR3* WINAPI D3DXVec3ProjectArray(
		D3DXVECTOR3 *pOut,
		UINT OutStride,
		CONST D3DXVECTOR3 *pV,
		UINT VStride,
		CONST D3DVIEWPORT9 *pViewport,
		CONST D3DXMATRIX *pProjection,
		CONST D3DXMATRIX *pView,
		CONST D3DXMATRIX *pWorld,
		UINT n)
	{
		return (*original_D3DXVec3ProjectArray)(pOut, OutStride, pV, VStride, pViewport, pProjection, pView, pWorld, n);
	}

	D3DXVECTOR4* WINAPI D3DXVec3Transform(
		D3DXVECTOR4 *pOut,
		CONST D3DXVECTOR3 *pV,
		CONST D3DXMATRIX *pM)
	{
		return (*original_D3DXVec3Transform)(pOut, pV, pM);
	}

	D3DXVECTOR4* WINAPI D3DXVec3TransformArray(
		D3DXVECTOR4 *pOut,
		UINT OutStride,
		CONST D3DXVECTOR3 *pV,
		UINT VStride,
		CONST D3DXMATRIX *pM,
		UINT n)
	{
		return (*original_D3DXVec3TransformArray)(pOut, OutStride, pV, VStride, pM, n);
	}

	D3DXVECTOR3* WINAPI D3DXVec3TransformCoord(
		D3DXVECTOR3 *pOut,
		CONST D3DXVECTOR3 *pV,
		CONST D3DXMATRIX *pM)
	{
		return (*original_D3DXVec3TransformCoord)(pOut, pV, pM);
	}

	D3DXVECTOR3* WINAPI D3DXVec3TransformCoordArray(
		D3DXVECTOR3 *pOut,
		UINT OutStride,
		CONST D3DXVECTOR3 *pV,
		UINT VStride,
		CONST D3DXMATRIX *pM,
		UINT n)
	{
		return (*original_D3DXVec3TransformCoordArray)(pOut, OutStride, pV, VStride, pM, n);
	}

	D3DXVECTOR3* WINAPI D3DXVec3TransformNormal(
		D3DXVECTOR3 *pOut,
		CONST D3DXVECTOR3 *pV,
		CONST D3DXMATRIX *pM)
	{
		return (*original_D3DXVec3TransformNormal)(pOut, pV, pM);
	}

	D3DXVECTOR3* WINAPI D3DXVec3TransformNormalArray(
		D3DXVECTOR3 *pOut,
		UINT OutStride,
		CONST D3DXVECTOR3 *pV,
		UINT VStride,
		CONST D3DXMATRIX *pM,
		UINT n)
	{
		return (*original_D3DXVec3TransformNormalArray)(pOut, OutStride, pV, VStride, pM, n);
	}

	D3DXVECTOR3* WINAPI D3DXVec3Unproject(
		D3DXVECTOR3 *pOut,
		CONST D3DXVECTOR3 *pV,
		CONST D3DVIEWPORT9 *pViewport,
		CONST D3DXMATRIX *pProjection,
		CONST D3DXMATRIX *pView,
		CONST D3DXMATRIX *pWorld)
	{
		return (*original_D3DXVec3Unproject)(pOut, pV, pViewport, pProjection, pView, pWorld);
	}

	D3DXVECTOR3* WINAPI D3DXVec3UnprojectArray(
		D3DXVECTOR3 *pOut,
		UINT OutStride,
		CONST D3DXVECTOR3 *pV,
		UINT VStride,
		CONST D3DVIEWPORT9 *pViewport,
		CONST D3DXMATRIX *pProjection,
		CONST D3DXMATRIX *pView,
		CONST D3DXMATRIX *pWorld,
		UINT n)
	{
		return (*original_D3DXVec3UnprojectArray)(pOut, OutStride, pV, VStride, pViewport, pProjection, pView, pWorld, n);
	}

	D3DXVECTOR4* WINAPI D3DXVec4BaryCentric(
		D3DXVECTOR4 *pOut,
		CONST D3DXVECTOR4 *pV1,
		CONST D3DXVECTOR4 *pV2,
		CONST  D3DXVECTOR4 *pV3,
		FLOAT f,
		FLOAT g)
	{
		return (*original_D3DXVec4BaryCentric)(pOut, pV1, pV2, pV3, f, g);
	}

	D3DXVECTOR4* WINAPI D3DXVec4CatmullRom(
		D3DXVECTOR4 *pOut,
		CONST D3DXVECTOR4 *pV0,
		CONST D3DXVECTOR4 *pV1,
		CONST D3DXVECTOR4 *pV2,
		CONST D3DXVECTOR4 *pV3,
		FLOAT s)
	{
		return (*original_D3DXVec4CatmullRom)(pOut, pV0, pV1, pV2, pV3, s);
	}

	D3DXVECTOR4* WINAPI D3DXVec4Cross(
		D3DXVECTOR4 *pOut,
		CONST D3DXVECTOR4 *pV1,
		CONST D3DXVECTOR4 *pV2,
		CONST D3DXVECTOR4 *pV3)
	{
		return (*original_D3DXVec4Cross)(pOut, pV1, pV2, pV3);
	}

	D3DXVECTOR4* WINAPI D3DXVec4Hermite(
		D3DXVECTOR4 *pOut,
		CONST D3DXVECTOR4 *pV1,
		CONST D3DXVECTOR4 *pT1,
		CONST D3DXVECTOR4 *pV2,
		CONST D3DXVECTOR4 *pT2,
		FLOAT s)
	{
		return (*original_D3DXVec4Hermite)(pOut, pV1, pT1, pV2, pT2, s);
	}

	D3DXVECTOR4* WINAPI D3DXVec4Normalize(
		D3DXVECTOR4 *pOut,
		CONST D3DXVECTOR4 *pV)
	{
		return (*original_D3DXVec4Normalize)(pOut, pV);
	}

	D3DXVECTOR4* WINAPI D3DXVec4Transform(
		D3DXVECTOR4 *pOut,
		CONST D3DXVECTOR4 *pV,
		CONST D3DXMATRIX *pM)
	{
		return (*original_D3DXVec4Transform)(pOut, pV, pM);
	}

	D3DXVECTOR4* WINAPI D3DXVec4TransformArray(
		D3DXVECTOR4 *pOut,
		UINT OutStride,
		CONST D3DXVECTOR4 *pV,
		UINT VStride,
		CONST D3DXMATRIX *pM,
		UINT n)
	{
		return (*original_D3DXVec4TransformArray)(pOut, OutStride, pV, VStride, pM, n);
	}

	HRESULT WINAPI D3DXWeldVertices(
		LPD3DXMESH pMesh,
		DWORD Flags,
		CONST D3DXWELDEPSILONS *pEpsilons,
		CONST DWORD *pAdjacencyIn,
		DWORD *pAdjacencyOut,
		DWORD *pFaceRemap,
		LPD3DXBUFFER *ppVertexRemap)
	{
		return (*original_D3DXWeldVertices)(pMesh, Flags, pEpsilons, pAdjacencyIn, pAdjacencyOut, pFaceRemap, ppVertexRemap);
	}





} // extern "C"

BOOL init()
{

	TCHAR app_full_path[1024];	// アプリフルパス

	GetModuleFileName(NULL, app_full_path, sizeof(app_full_path) / sizeof(TCHAR));

	//std::wstring pythonScript(app_full_path);
	//pythonScript = pythonScript.substr(0, pythonScript.find_last_of(_T("MikuMikuDance.exe")) -16);
	//pythonScript.append(_T("mmdbridge.py"));

	//std::ifstream ifs(pythonScript.c_str());
	//if (!ifs) return false;
	//char buf[2048];
	//while (ifs.getline( buf, sizeof(buf))) {
	//	mmdbridge_python_script.append(buf);
	//	mmdbridge_python_script.append("\r\n");
	//}

	//::MessageBoxA(NULL, "d3dx9_32.dll読み込みます", "hoge",MB_OK);
	TCHAR system_path_buffer[1024]; // システムパス保存用
	GetSystemDirectory(system_path_buffer, MAX_PATH );
	std::wstring d3d9x_path(system_path_buffer);
	d3d9x_path.append(_T("\\D3DX9_32.DLL"));
	HMODULE d3d9x_module(LoadLibrary(d3d9x_path.c_str())); // オリジナルのD3D9X_32.DLLのモジュール

	if (!d3d9x_module) {
		::MessageBoxA(NULL, "d3dx9_32.dll読み込み失敗", "hoge",MB_OK);
		return FALSE;
	}

	//::MessageBoxA(NULL, "d3dx9_32.dll読み込みました", "hoge",MB_OK);
	//// オリジナルの関数ポインタを取得
	original_D3DXAssembleShader = reinterpret_cast<HRESULT  (WINAPI*)(        LPCSTR                          pSrcData,         UINT                            SrcDataLen,         CONST D3DXMACRO*                pDefines,         LPD3DXINCLUDE                   pInclude,         DWORD                           Flags,         LPD3DXBUFFER*                   ppShader,         LPD3DXBUFFER*                   ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXAssembleShader"));
	original_D3DXAssembleShaderFromFileA = reinterpret_cast<HRESULT  (WINAPI*)(        LPCSTR                          pSrcFile,         CONST D3DXMACRO*                pDefines,         LPD3DXINCLUDE                   pInclude,         DWORD                           Flags,         LPD3DXBUFFER*                   ppShader,         LPD3DXBUFFER*                   ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXAssembleShaderFromFileA"));
	original_D3DXAssembleShaderFromFileW = reinterpret_cast<HRESULT  (WINAPI*)(        LPCWSTR                         pSrcFile,         CONST D3DXMACRO*                pDefines,         LPD3DXINCLUDE                   pInclude,         DWORD                           Flags,         LPD3DXBUFFER*                   ppShader,         LPD3DXBUFFER*                   ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXAssembleShaderFromFileW"));
	original_D3DXAssembleShaderFromResourceA = reinterpret_cast<HRESULT  (WINAPI*)(        HMODULE                         hSrcModule,         LPCSTR                          pSrcResource,         CONST D3DXMACRO*                pDefines,         LPD3DXINCLUDE                   pInclude,         DWORD                           Flags,         LPD3DXBUFFER*                   ppShader,         LPD3DXBUFFER*                   ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXAssembleShaderFromResourceA"));
	original_D3DXAssembleShaderFromResourceW = reinterpret_cast<HRESULT  (WINAPI*)(        HMODULE                         hSrcModule,         LPCWSTR                         pSrcResource,         CONST D3DXMACRO*                pDefines,         LPD3DXINCLUDE                   pInclude,         DWORD                           Flags,         LPD3DXBUFFER*                   ppShader,         LPD3DXBUFFER*                   ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXAssembleShaderFromResourceW"));
	original_D3DXBoxBoundProbe = reinterpret_cast<BOOL  (WINAPI*)(        CONST D3DXVECTOR3 *pMin,         CONST D3DXVECTOR3 *pMax,         CONST D3DXVECTOR3 *pRayPosition,         CONST D3DXVECTOR3 *pRayDirection)>(GetProcAddress(d3d9x_module, "D3DXBoxBoundProbe"));
	original_D3DXCheckCubeTextureRequirements = reinterpret_cast<HRESULT  (WINAPI*)(        LPDIRECT3DDEVICE9         pDevice,         UINT*                     pSize,         UINT*                     pNumMipLevels,         DWORD                     Usage,         D3DFORMAT*                pFormat,         D3DPOOL                   Pool)>(GetProcAddress(d3d9x_module, "D3DXCheckCubeTextureRequirements"));
	original_D3DXCheckTextureRequirements = reinterpret_cast<HRESULT  (WINAPI*)(        LPDIRECT3DDEVICE9         pDevice,         UINT*                     pWidth,         UINT*                     pHeight,         UINT*                     pNumMipLevels,         DWORD                     Usage,         D3DFORMAT*                pFormat,         D3DPOOL                   Pool)>(GetProcAddress(d3d9x_module, "D3DXCheckTextureRequirements"));
	original_D3DXCheckVersion = reinterpret_cast<BOOL  (WINAPI*)(  UINT D3DSdkVersion,   UINT D3DXSdkVersion)>(GetProcAddress(d3d9x_module, "D3DXCheckVersion"));
	original_D3DXCheckVolumeTextureRequirements = reinterpret_cast<HRESULT  (WINAPI*)(        LPDIRECT3DDEVICE9         pDevice,         UINT*                     pWidth,         UINT*                     pHeight,         UINT*                     pDepth,         UINT*                     pNumMipLevels,         DWORD                     Usage,         D3DFORMAT*                pFormat, D3DPOOL                   Pool)>(GetProcAddress(d3d9x_module, "D3DXCheckVolumeTextureRequirements"));
	original_D3DXCleanMesh = reinterpret_cast<HRESULT  (WINAPI*)(  D3DXCLEANTYPE CleanType,   LPD3DXMESH pMeshIn,   CONST DWORD* pAdjacencyIn,   LPD3DXMESH* ppMeshOut,   DWORD* pAdjacencyOut,   LPD3DXBUFFER* ppErrorsAndWarnings)>(GetProcAddress(d3d9x_module, "D3DXCleanMesh"));
	original_D3DXColorAdjustContrast = reinterpret_cast<D3DXCOLOR*  (WINAPI*)(  D3DXCOLOR *pOut,   CONST D3DXCOLOR *pC,   FLOAT c)>(GetProcAddress(d3d9x_module, "D3DXColorAdjustContrast"));
	original_D3DXColorAdjustSaturation = reinterpret_cast<D3DXCOLOR*  (WINAPI*)(  D3DXCOLOR *pOut,   CONST D3DXCOLOR *pC,   FLOAT s)>(GetProcAddress(d3d9x_module, "D3DXColorAdjustSaturation"));
	original_D3DXCompileShader = reinterpret_cast<HRESULT  (WINAPI*)(        LPCSTR                          pSrcData,         UINT                            SrcDataLen,         CONST D3DXMACRO*                pDefines,         LPD3DXINCLUDE                   pInclude,         LPCSTR                          pFunctionName,         LPCSTR                          pProfile,         DWORD                           Flags,         LPD3DXBUFFER*                   ppShader,         LPD3DXBUFFER*                   ppErrorMsgs,         LPD3DXCONSTANTTABLE*            ppConstantTable)>(GetProcAddress(d3d9x_module, "D3DXCompileShader"));
	original_D3DXCompileShaderFromFileA = reinterpret_cast<HRESULT  (WINAPI*)(        LPCSTR                          pSrcFile,         CONST D3DXMACRO*                pDefines,         LPD3DXINCLUDE                   pInclude,         LPCSTR                          pFunctionName,         LPCSTR                          pProfile,         DWORD                           Flags,         LPD3DXBUFFER*                   ppShader,         LPD3DXBUFFER*                   ppErrorMsgs,         LPD3DXCONSTANTTABLE*            ppConstantTable)>(GetProcAddress(d3d9x_module, "D3DXCompileShaderFromFileA"));
	original_D3DXCompileShaderFromFileW = reinterpret_cast<HRESULT  (WINAPI*)(        LPCWSTR                         pSrcFile,         CONST D3DXMACRO*                pDefines,         LPD3DXINCLUDE                   pInclude,         LPCSTR                          pFunctionName,         LPCSTR                          pProfile,         DWORD                           Flags,         LPD3DXBUFFER*                   ppShader,         LPD3DXBUFFER*                   ppErrorMsgs,         LPD3DXCONSTANTTABLE*            ppConstantTable)>(GetProcAddress(d3d9x_module, "D3DXCompileShaderFromFileW"));
	original_D3DXCompileShaderFromResourceA = reinterpret_cast<HRESULT  (WINAPI*)(        HMODULE                         hSrcModule,         LPCSTR                          pSrcResource,         CONST D3DXMACRO*                pDefines,         LPD3DXINCLUDE                   pInclude,         LPCSTR                          pFunctionName,         LPCSTR                          pProfile,         DWORD                           Flags,         LPD3DXBUFFER*                   ppShader,         LPD3DXBUFFER*                   ppErrorMsgs,         LPD3DXCONSTANTTABLE*            ppConstantTable)>(GetProcAddress(d3d9x_module, "D3DXCompileShaderFromResourceA"));
	original_D3DXCompileShaderFromResourceW = reinterpret_cast<HRESULT  (WINAPI*)(        HMODULE                         hSrcModule,         LPCWSTR                         pSrcResource,         CONST D3DXMACRO*                pDefines,         LPD3DXINCLUDE                   pInclude,         LPCSTR                          pFunctionName,         LPCSTR                          pProfile,         DWORD                           Flags,         LPD3DXBUFFER*                   ppShader,         LPD3DXBUFFER*                   ppErrorMsgs,         LPD3DXCONSTANTTABLE*            ppConstantTable)>(GetProcAddress(d3d9x_module, "D3DXCompileShaderFromResourceW"));
	original_D3DXComputeBoundingBox = reinterpret_cast<HRESULT  (WINAPI*)(        CONST D3DXVECTOR3 *pFirstPosition,         DWORD NumVertices,         DWORD dwStride,         D3DXVECTOR3 *pMin,         D3DXVECTOR3 *pMax)>(GetProcAddress(d3d9x_module, "D3DXComputeBoundingBox"));
	original_D3DXComputeBoundingSphere = reinterpret_cast<HRESULT  (WINAPI*)(        CONST D3DXVECTOR3 *pFirstPosition,         DWORD NumVertices,         DWORD dwStride,         D3DXVECTOR3 *pCenter,         FLOAT *pRadius)>(GetProcAddress(d3d9x_module, "D3DXComputeBoundingSphere"));
	original_D3DXComputeIMTFromPerTexelSignal = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXMESH pMesh,   DWORD dwTextureIndex,   FLOAT *pfTexelSignal,   UINT uWidth,   UINT uHeight,   UINT uSignalDimension,   UINT uComponents,   DWORD dwOptions,   LPD3DXUVATLASCB pStatusCallback,   LPVOID pUserContext,   LPD3DXBUFFER *ppIMTData)>(GetProcAddress(d3d9x_module, "D3DXComputeIMTFromPerTexelSignal"));
	original_D3DXComputeIMTFromPerVertexSignal = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXMESH pMesh,   CONST FLOAT *pfVertexSignal,   UINT uSignalDimension,   UINT uSignalStride,   DWORD dwOptions,   LPD3DXUVATLASCB pStatusCallback,   LPVOID pUserContext,   LPD3DXBUFFER *ppIMTData)>(GetProcAddress(d3d9x_module, "D3DXComputeIMTFromPerVertexSignal"));
	original_D3DXComputeIMTFromSignal = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXMESH pMesh,   DWORD dwTextureIndex,   UINT uSignalDimension,   FLOAT fMaxUVDistance,   DWORD dwOptions,   LPD3DXIMTSIGNALCALLBACK pSignalCallback,   VOID *pUserData,   LPD3DXUVATLASCB pStatusCallback,   LPVOID pUserContext,   LPD3DXBUFFER *ppIMTData)>(GetProcAddress(d3d9x_module, "D3DXComputeIMTFromSignal("));
	original_D3DXComputeIMTFromTexture = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXMESH pMesh,   LPDIRECT3DTEXTURE9 pTexture,   DWORD dwTextureIndex,   DWORD dwOptions,   LPD3DXUVATLASCB pStatusCallback,   LPVOID pUserContext,   LPD3DXBUFFER *ppIMTData)>(GetProcAddress(d3d9x_module, "D3DXComputeIMTFromTexture"));
	original_D3DXComputeNormalMap = reinterpret_cast<HRESULT  (WINAPI*)(        LPDIRECT3DTEXTURE9        pTexture,         LPDIRECT3DTEXTURE9        pSrcTexture,         CONST PALETTEENTRY*       pSrcPalette,         DWORD                     Flags,         DWORD                     Channel,         FLOAT                     Amplitude)>(GetProcAddress(d3d9x_module, "D3DXComputeNormalMap"));
	original_D3DXComputeNormals = reinterpret_cast<HRESULT  (WINAPI*)(        LPD3DXBASEMESH pMesh,         CONST DWORD *pAdjacency)>(GetProcAddress(d3d9x_module, "D3DXComputeNormals"));
	original_D3DXComputeTangentFrame = reinterpret_cast<HRESULT  (WINAPI*)(  ID3DXMesh *pMesh,   DWORD dwOptions)>(GetProcAddress(d3d9x_module, "D3DXComputeTangentFrame"));
	original_D3DXComputeTangentFrame = reinterpret_cast<HRESULT  (WINAPI*)(  ID3DXMesh *pMesh,   DWORD dwOptions)>(GetProcAddress(d3d9x_module, "D3DXComputeTangentFrame"));
	original_D3DXComputeTangentFrameEx = reinterpret_cast<HRESULT  (WINAPI*)(  ID3DXMesh *pMesh,   DWORD dwTextureInSemantic,   DWORD dwTextureInIndex,   DWORD dwUPartialOutSemantic,   DWORD dwUPartialOutIndex,   DWORD dwVPartialOutSemantic,   DWORD dwVPartialOutIndex,   DWORD dwNormalOutSemantic,   DWORD dwNormalOutIndex,   DWORD dwOptions,   CONST DWORD *pdwAdjacency,   FLOAT fPartialEdgeThreshold,   FLOAT fSingularPointThreshold,   FLOAT fNormalEdgeThreshold,   ID3DXMesh **ppMeshOut,   ID3DXBuffer **ppVertexMapping)>(GetProcAddress(d3d9x_module, "D3DXComputeTangentFrameEx"));
	original_D3DXConcatenateMeshes = reinterpret_cast<HRESULT  (WINAPI*)(        LPD3DXMESH *ppMeshes,         UINT NumMeshes,         DWORD Options,         CONST D3DXMATRIX *pGeomXForms,         CONST D3DXMATRIX *pTextureXForms,         CONST D3DVERTEXELEMENT9 *pDecl,         LPDIRECT3DDEVICE9 pD3DDevice,         LPD3DXMESH *ppMeshOut)>(GetProcAddress(d3d9x_module, "D3DXConcatenateMeshes"));
	original_D3DXConvertMeshSubsetToSingleStrip = reinterpret_cast<HRESULT  (WINAPI*)(        LPD3DXBASEMESH MeshIn,         DWORD AttribId,         DWORD IBOptions,         LPDIRECT3DINDEXBUFFER9 *ppIndexBuffer,         DWORD *pNumIndices)>(GetProcAddress(d3d9x_module, "D3DXConvertMeshSubsetToSingleStrip"));
	original_D3DXConvertMeshSubsetToStrips = reinterpret_cast<HRESULT  (WINAPI*)(        LPD3DXBASEMESH MeshIn,         DWORD AttribId,         DWORD IBOptions,         LPDIRECT3DINDEXBUFFER9 *ppIndexBuffer,         DWORD *pNumIndices,         LPD3DXBUFFER *ppStripLengths,         DWORD *pNumStrips)>(GetProcAddress(d3d9x_module, "D3DXConvertMeshSubsetToStrips"));
	original_D3DXCreateAnimationController = reinterpret_cast<HRESULT  (WINAPI*)(  UINT MaxNumMatrices,   UINT MaxNumAnimationSets,   UINT MaxNumTracks,   UINT MaxNumEvents,   LPD3DXANIMATIONCONTROLLER *ppAnimController)>(GetProcAddress(d3d9x_module, "D3DXCreateAnimationController"));
	original_D3DXCreateBox = reinterpret_cast<HRESULT  (WINAPI*)(        LPDIRECT3DDEVICE9   pDevice,         FLOAT               Width,         FLOAT               Height,         FLOAT               Depth,         LPD3DXMESH*         ppMesh,         LPD3DXBUFFER*       ppAdjacency)>(GetProcAddress(d3d9x_module, "D3DXCreateBox"));
	original_D3DXCreateBuffer = reinterpret_cast<HRESULT  (WINAPI*)(        DWORD NumBytes,         LPD3DXBUFFER *ppBuffer)>(GetProcAddress(d3d9x_module, "D3DXCreateBuffer"));
	original_D3DXCreateCompressedAnimationSet = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR pName,   DOUBLE TicksPerSecond,   D3DXPLAYBACK_TYPE Playback,   LPD3DXBUFFER pCompressedData,   UINT NumCallbackKeys,   CONST D3DXKEY_CALLBACK *pCallbackKeys,   LPD3DXCOMPRESSEDANIMATIONSET *ppAnimationSet )>(GetProcAddress(d3d9x_module, "D3DXCreateCompressedAnimationSet"));
	original_D3DXCreateCubeTexture = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9         pDevice,   UINT                      Size,   UINT                      MipLevels,   DWORD                     Usage,   D3DFORMAT                 Format,   D3DPOOL                   Pool,   LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateCubeTexture"));
	original_D3DXCreateCubeTextureFromFileA = reinterpret_cast<HRESULT  (WINAPI*)(        LPDIRECT3DDEVICE9         pDevice,         LPCSTR                    pSrcFile,         LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateCubeTextureFromFileA"));
	original_D3DXCreateCubeTextureFromFileExA = reinterpret_cast<HRESULT  (WINAPI*)(        LPDIRECT3DDEVICE9         pDevice,         LPCSTR                    pSrcFile,         UINT                      Size,         UINT                      MipLevels,         DWORD                     Usage,         D3DFORMAT                 Format,         D3DPOOL                   Pool,         DWORD                     Filter,         DWORD                     MipFilter,         D3DCOLOR                  ColorKey,         D3DXIMAGE_INFO*           pSrcInfo,         PALETTEENTRY*             pPalette,         LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateCubeTextureFromFileExA"));
	original_D3DXCreateCubeTextureFromFileExW = reinterpret_cast<HRESULT  (WINAPI*)(        LPDIRECT3DDEVICE9         pDevice,         LPCWSTR                   pSrcFile,         UINT                      Size,         UINT                      MipLevels,         DWORD                     Usage,         D3DFORMAT                 Format,         D3DPOOL                   Pool,         DWORD                     Filter,         DWORD                     MipFilter,         D3DCOLOR                  ColorKey,         D3DXIMAGE_INFO*           pSrcInfo,         PALETTEENTRY*             pPalette,         LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateCubeTextureFromFileExW"));
	original_D3DXCreateCubeTextureFromFileInMemory = reinterpret_cast<HRESULT  (WINAPI*)(        LPDIRECT3DDEVICE9         pDevice,         LPCVOID                   pSrcData,         UINT                      SrcDataSize,         LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateCubeTextureFromFileInMemory"));
	original_D3DXCreateCubeTextureFromFileInMemoryEx = reinterpret_cast<HRESULT  (WINAPI*)(        LPDIRECT3DDEVICE9         pDevice,         LPCVOID                   pSrcData,         UINT                      SrcDataSize,         UINT                      Size,         UINT                      MipLevels,         DWORD                     Usage,         D3DFORMAT                 Format,         D3DPOOL                   Pool,         DWORD                     Filter,         DWORD                     MipFilter,         D3DCOLOR                  ColorKey,         D3DXIMAGE_INFO*           pSrcInfo,         PALETTEENTRY*             pPalette,         LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateCubeTextureFromFileInMemoryEx"));
	original_D3DXCreateCubeTextureFromFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPCWSTR    pSrcFile,   LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateCubeTextureFromFileW"));
	original_D3DXCreateCubeTextureFromResourceA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   HMODULE    hSrcModule,   LPCSTR     pSrcResource,   LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateCubeTextureFromResourceA"));
	original_D3DXCreateCubeTextureFromResourceExA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   HMODULE    hSrcModule,   LPCSTR     pSrcResource,   UINT       Size,   UINT       MipLevels,   DWORD      Usage,   D3DFORMAT     Format,   D3DPOOL    Pool,   DWORD      Filter,   DWORD      MipFilter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo,   PALETTEENTRY*    pPalette,   LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateCubeTextureFromResourceExA"));
	original_D3DXCreateCubeTextureFromResourceExW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   HMODULE    hSrcModule,   LPCWSTR    pSrcResource,   UINT       Size,   UINT       MipLevels,   DWORD      Usage,   D3DFORMAT     Format,   D3DPOOL    Pool,   DWORD      Filter,   DWORD      MipFilter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo,   PALETTEENTRY*    pPalette,   LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateCubeTextureFromResourceExW"));
	original_D3DXCreateCubeTextureFromResourceW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   HMODULE    hSrcModule,   LPCWSTR    pSrcResource,   LPDIRECT3DCUBETEXTURE9*   ppCubeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateCubeTextureFromResourceW"));
	original_D3DXCreateCylinder = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   FLOAT      Radius1,   FLOAT      Radius2,   FLOAT      Length,   UINT    Slices,   UINT    Stacks,   LPD3DXMESH*   ppMesh,   LPD3DXBUFFER*       ppAdjacency)>(GetProcAddress(d3d9x_module, "D3DXCreateCylinder"));
	original_D3DXCreateEffect = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9      pDevice,   LPCVOID       pSrcData,   UINT       SrcDataLen,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXEFFECTPOOL    pPool,   LPD3DXEFFECT*    ppEffect,   LPD3DXBUFFER*    ppCompilationErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffect"));
	original_D3DXCreateEffectCompiler = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR        pSrcData,   UINT       SrcDataLen,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXEFFECTCOMPILER*  ppCompiler,   LPD3DXBUFFER*    ppParseErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectCompiler"));
	original_D3DXCreateEffectCompilerFromFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR        pSrcFile,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXEFFECTCOMPILER*  ppCompiler,   LPD3DXBUFFER*    ppParseErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectCompilerFromFileA"));
	original_D3DXCreateEffectCompilerFromFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR       pSrcFile,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXEFFECTCOMPILER*  ppCompiler,   LPD3DXBUFFER*    ppParseErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectCompilerFromFileW"));
	original_D3DXCreateEffectCompilerFromResourceA = reinterpret_cast<HRESULT  (WINAPI*)(  HMODULE       hSrcModule,   LPCSTR        pSrcResource,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXEFFECTCOMPILER*  ppCompiler,   LPD3DXBUFFER*    ppParseErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectCompilerFromResourceA"));
	original_D3DXCreateEffectCompilerFromResourceW = reinterpret_cast<HRESULT  (WINAPI*)(  HMODULE       hSrcModule,   LPCWSTR       pSrcResource,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXEFFECTCOMPILER*  ppCompiler,   LPD3DXBUFFER*    ppParseErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectCompilerFromResourceW"));
	original_D3DXCreateEffectEx = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9      pDevice,   LPCVOID       pSrcData,   UINT       SrcDataLen,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   LPCSTR        pSkipConstants,   DWORD      Flags,   LPD3DXEFFECTPOOL    pPool,   LPD3DXEFFECT*    ppEffect,   LPD3DXBUFFER*    ppCompilationErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectEx"));
	original_D3DXCreateEffectFromFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9      pDevice,   LPCSTR        pSrcFile,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXEFFECTPOOL    pPool,   LPD3DXEFFECT*    ppEffect,   LPD3DXBUFFER*    ppCompilationErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectFromFileA"));
	original_D3DXCreateEffectFromFileExA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9      pDevice,   LPCSTR        pSrcFile,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   LPCSTR        pSkipConstants,   DWORD      Flags,   LPD3DXEFFECTPOOL    pPool,   LPD3DXEFFECT*    ppEffect,   LPD3DXBUFFER*    ppCompilationErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectFromFileExA"));
	original_D3DXCreateEffectFromFileExW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9      pDevice,   LPCWSTR       pSrcFile,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   LPCSTR        pSkipConstants,   DWORD      Flags,   LPD3DXEFFECTPOOL    pPool,   LPD3DXEFFECT*    ppEffect,   LPD3DXBUFFER*    ppCompilationErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectFromFileExW"));
	original_D3DXCreateEffectFromFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9      pDevice,   LPCWSTR       pSrcFile,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXEFFECTPOOL    pPool,   LPD3DXEFFECT*    ppEffect,   LPD3DXBUFFER*    ppCompilationErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectFromFileW"));
	original_D3DXCreateEffectFromResourceA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9      pDevice,   HMODULE       hSrcModule,   LPCSTR        pSrcResource,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXEFFECTPOOL    pPool,   LPD3DXEFFECT*    ppEffect,   LPD3DXBUFFER*    ppCompilationErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectFromResourceA"));
	original_D3DXCreateEffectFromResourceExA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9      pDevice,   HMODULE       hSrcModule,   LPCSTR        pSrcResource,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   LPCSTR        pSkipConstants,   DWORD      Flags,   LPD3DXEFFECTPOOL    pPool,   LPD3DXEFFECT*    ppEffect,   LPD3DXBUFFER*    ppCompilationErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectFromResourceExA"));
	original_D3DXCreateEffectFromResourceExW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9      pDevice,   HMODULE       hSrcModule,   LPCWSTR       pSrcResource,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   LPCSTR        pSkipConstants,   DWORD      Flags,   LPD3DXEFFECTPOOL    pPool,   LPD3DXEFFECT*    ppEffect,   LPD3DXBUFFER*    ppCompilationErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectFromResourceExW"));
	original_D3DXCreateEffectFromResourceW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9      pDevice,   HMODULE       hSrcModule,   LPCWSTR       pSrcResource,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXEFFECTPOOL    pPool,   LPD3DXEFFECT*    ppEffect,   LPD3DXBUFFER*    ppCompilationErrors)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectFromResourceW"));
	original_D3DXCreateEffectPool = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXEFFECTPOOL*      ppPool)>(GetProcAddress(d3d9x_module, "D3DXCreateEffectPool"));
	original_D3DXCreateFontA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9       pDevice,   INT      Height,   UINT     Width,   UINT     Weight,   UINT     MipLevels,   BOOL     Italic,   DWORD    CharSet,   DWORD    OutputPrecision,   DWORD    Quality,   DWORD    PitchAndFamily,   LPCSTR      pFaceName,   LPD3DXFONT*    ppFont)>(GetProcAddress(d3d9x_module, "D3DXCreateFontA"));
	original_D3DXCreateFontIndirectA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9       pDevice,   CONST D3DXFONT_DESCA*   pDesc,   LPD3DXFONT*    ppFont)>(GetProcAddress(d3d9x_module, "D3DXCreateFontIndirectA"));
	original_D3DXCreateFontIndirectW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9       pDevice,   CONST D3DXFONT_DESCW*   pDesc,   LPD3DXFONT*    ppFont)>(GetProcAddress(d3d9x_module, "D3DXCreateFontIndirectW"));
	original_D3DXCreateFontW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9       pDevice,   INT      Height,   UINT     Width,   UINT     Weight,   UINT     MipLevels,   BOOL     Italic,   DWORD    CharSet,   DWORD    OutputPrecision,   DWORD    Quality,   DWORD    PitchAndFamily,   LPCWSTR     pFaceName,   LPD3DXFONT*    ppFont)>(GetProcAddress(d3d9x_module, "D3DXCreateFontW"));
	//original_D3DXCreateFragmentLinker = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9      pDevice,   UINT       ShaderCacheSize,   LPD3DXFRAGMENTLINKER*  ppFragmentLinker)>(GetProcAddress(d3d9x_module, "D3DXCreateFragmentLinker"));
	original_D3DXCreateKeyframedAnimationSet = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR pName,   DOUBLE TicksPerSecond,   D3DXPLAYBACK_TYPE Playback,   UINT NumAnimations,   UINT NumCallbackKeys,   CONST D3DXKEY_CALLBACK *pCallbackKeys,   LPD3DXKEYFRAMEDANIMATIONSET *ppAnimationSet )>(GetProcAddress(d3d9x_module, "D3DXCreateKeyframedAnimationSet"));
	original_D3DXCreateLine = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPD3DXLINE*   ppLine)>(GetProcAddress(d3d9x_module, "D3DXCreateLine"));
	original_D3DXCreateMatrixStack = reinterpret_cast<HRESULT  (WINAPI*)(  DWORD      Flags,   LPD3DXMATRIXSTACK*  ppStack)>(GetProcAddress(d3d9x_module, "D3DXCreateMatrixStack"));
	original_D3DXCreateMesh = reinterpret_cast<HRESULT  (WINAPI*)(  DWORD NumFaces,   DWORD NumVertices,   DWORD Options,   CONST D3DVERTEXELEMENT9 *pDeclaration,   LPDIRECT3DDEVICE9 pD3DDevice,   LPD3DXMESH* ppMesh)>(GetProcAddress(d3d9x_module, "D3DXCreateMesh"));
	original_D3DXCreateMeshFVF = reinterpret_cast<HRESULT  (WINAPI*)(  DWORD NumFaces,   DWORD NumVertices,   DWORD Options,   DWORD FVF,   LPDIRECT3DDEVICE9 pD3DDevice,   LPD3DXMESH* ppMesh)>(GetProcAddress(d3d9x_module, "D3DXCreateMeshFVF"));
	original_D3DXCreateNPatchMesh = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXMESH pMeshSysMem,   LPD3DXPATCHMESH *pPatchMesh)>(GetProcAddress(d3d9x_module, "D3DXCreateNPatchMesh"));
	original_D3DXCreatePMeshFromStream = reinterpret_cast<HRESULT  (WINAPI*)(  IStream *pStream,   DWORD Options,   LPDIRECT3DDEVICE9 pD3DDevice,   LPD3DXBUFFER *ppMaterials,   LPD3DXBUFFER *ppEffectInstances,   DWORD* pNumMaterials,   LPD3DXPMESH *ppPMesh)>(GetProcAddress(d3d9x_module, "D3DXCreatePMeshFromStream"));
	original_D3DXCreatePRTBuffer = reinterpret_cast<HRESULT  (WINAPI*)(  UINT NumSamples,   UINT NumCoeffs,   UINT NumChannels,   LPD3DXPRTBUFFER* ppBuffer)>(GetProcAddress(d3d9x_module, "D3DXCreatePRTBuffer"));
	original_D3DXCreatePRTBufferTex = reinterpret_cast<HRESULT  (WINAPI*)(  UINT Width,   UINT Height,   UINT NumCoeffs,   UINT NumChannels,   LPD3DXPRTBUFFER* ppBuffer)>(GetProcAddress(d3d9x_module, "D3DXCreatePRTBufferTex"));
	original_D3DXCreatePRTCompBuffer = reinterpret_cast<HRESULT  (WINAPI*)(  D3DXSHCOMPRESSQUALITYTYPE Quality,   UINT NumClusters,   UINT NumPCA,   LPD3DXSHPRTSIMCB pCB,   LPVOID lpUserContext,   LPD3DXPRTBUFFER  pBufferIn,   LPD3DXPRTCOMPBUFFER *ppBufferOut)>(GetProcAddress(d3d9x_module, "D3DXCreatePRTCompBuffer"));
	original_D3DXCreatePRTEngine = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXMESH pMesh,   DWORD *pAdjacency,   BOOL ExtractUVs,   LPD3DXMESH pBlockerMesh,   LPD3DXPRTENGINE* ppEngine)>(GetProcAddress(d3d9x_module, "D3DXCreatePRTEngine"));
	original_D3DXCreatePatchMesh = reinterpret_cast<HRESULT  (WINAPI*)(  CONST D3DXPATCHINFO *pInfo,   DWORD dwNumPatches,   DWORD dwNumVertices,   DWORD dwOptions,   CONST D3DVERTEXELEMENT9 *pDecl,   LPDIRECT3DDEVICE9 pD3DDevice,   LPD3DXPATCHMESH *pPatchMesh)>(GetProcAddress(d3d9x_module, "D3DXCreatePatchMesh"));
	original_D3DXCreatePolygon = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   FLOAT      Length,   UINT    Sides,   LPD3DXMESH*   ppMesh,   LPD3DXBUFFER*       ppAdjacency)>(GetProcAddress(d3d9x_module, "D3DXCreatePolygon"));
	original_D3DXCreateRenderToEnvMap = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9       pDevice,   UINT     Size,   UINT     MipLevels,   D3DFORMAT      Format,   BOOL     DepthStencil,   D3DFORMAT      DepthStencilFormat,   LPD3DXRenderToEnvMap*   ppRenderToEnvMap)>(GetProcAddress(d3d9x_module, "D3DXCreateRenderToEnvMap"));
	original_D3DXCreateRenderToSurface = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9       pDevice,   UINT     Width,   UINT     Height,   D3DFORMAT      Format,   BOOL     DepthStencil,   D3DFORMAT      DepthStencilFormat,   LPD3DXRENDERTOSURFACE*  ppRenderToSurface)>(GetProcAddress(d3d9x_module, "D3DXCreateRenderToSurface"));
	original_D3DXCreateSPMesh = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXMESH pMesh,   CONST DWORD* pAdjacency,   CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,   CONST FLOAT *pVertexWeights,   LPD3DXSPMESH* ppSMesh)>(GetProcAddress(d3d9x_module, "D3DXCreateSPMesh"));
	original_D3DXCreateSkinInfo = reinterpret_cast<HRESULT  (WINAPI*)(  DWORD NumVertices,   CONST D3DVERTEXELEMENT9 *pDeclaration,   DWORD NumBones,   LPD3DXSKININFO* ppSkinInfo)>(GetProcAddress(d3d9x_module, "D3DXCreateSkinInfo"));
	original_D3DXCreateSkinInfoFVF = reinterpret_cast<HRESULT  (WINAPI*)(  DWORD NumVertices,   DWORD FVF,   DWORD NumBones,   LPD3DXSKININFO* ppSkinInfo)>(GetProcAddress(d3d9x_module, "D3DXCreateSkinInfoFVF"));
	original_D3DXCreateSkinInfoFromBlendedMesh = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXBASEMESH pMesh,   DWORD NumBones,   CONST D3DXBONECOMBINATION *pBoneCombinationTable,   LPD3DXSKININFO* ppSkinInfo)>(GetProcAddress(d3d9x_module, "D3DXCreateSkinInfoFromBlendedMesh"));
	original_D3DXCreateSphere = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9  pDevice,   FLOAT     Radius,   UINT      Slices,   UINT      Stacks,   LPD3DXMESH*  ppMesh,   LPD3DXBUFFER*      ppAdjacency)>(GetProcAddress(d3d9x_module, "D3DXCreateSphere"));
	original_D3DXCreateSprite = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPD3DXSPRITE*       ppSprite)>(GetProcAddress(d3d9x_module, "D3DXCreateSprite"));
	original_D3DXCreateTeapot = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPD3DXMESH*   ppMesh,   LPD3DXBUFFER*       ppAdjacency)>(GetProcAddress(d3d9x_module, "D3DXCreateTeapot"));
	original_D3DXCreateTextA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   HDC     hDC,   LPCSTR     pText,   FLOAT      Deviation,   FLOAT      Extrusion,   LPD3DXMESH*   ppMesh,   LPD3DXBUFFER*       ppAdjacency,   LPGLYPHMETRICSFLOAT pGlyphMetrics)>(GetProcAddress(d3d9x_module, "D3DXCreateTextA"));
	original_D3DXCreateTextW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   HDC     hDC,   LPCWSTR    pText,   FLOAT      Deviation,   FLOAT      Extrusion,   LPD3DXMESH*   ppMesh,   LPD3DXBUFFER*       ppAdjacency,   LPGLYPHMETRICSFLOAT pGlyphMetrics)>(GetProcAddress(d3d9x_module, "D3DXCreateTextW"));
	original_D3DXCreateTexture = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   UINT       Width,   UINT       Height,   UINT       MipLevels,   DWORD      Usage,   D3DFORMAT     Format,   D3DPOOL    Pool,   LPDIRECT3DTEXTURE9*       ppTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateTexture"));
	original_D3DXCreateTextureFromFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPCSTR     pSrcFile,   LPDIRECT3DTEXTURE9*       ppTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateTextureFromFileA"));
	original_D3DXCreateTextureFromFileExA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPCSTR     pSrcFile,   UINT       Width,   UINT       Height,   UINT       MipLevels,   DWORD      Usage,   D3DFORMAT     Format,   D3DPOOL    Pool,   DWORD      Filter,   DWORD      MipFilter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo,   PALETTEENTRY*    pPalette,   LPDIRECT3DTEXTURE9*       ppTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateTextureFromFileExA"));
	original_D3DXCreateTextureFromFileExW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPCWSTR    pSrcFile,   UINT       Width,   UINT       Height,   UINT       MipLevels,   DWORD      Usage,   D3DFORMAT     Format,   D3DPOOL    Pool,   DWORD      Filter,   DWORD      MipFilter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo,   PALETTEENTRY*    pPalette,   LPDIRECT3DTEXTURE9*       ppTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateTextureFromFileExW"));
	original_D3DXCreateTextureFromFileInMemory = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPCVOID    pSrcData,   UINT       SrcDataSize,   LPDIRECT3DTEXTURE9*       ppTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateTextureFromFileInMemory"));
	original_D3DXCreateTextureFromFileInMemoryEx = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPCVOID    pSrcData,   UINT       SrcDataSize,   UINT       Width,   UINT       Height,   UINT       MipLevels,   DWORD      Usage,   D3DFORMAT     Format,   D3DPOOL    Pool,   DWORD      Filter,   DWORD      MipFilter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo,   PALETTEENTRY*    pPalette,   LPDIRECT3DTEXTURE9*       ppTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateTextureFromFileInMemoryEx"));
	original_D3DXCreateTextureFromFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPCWSTR    pSrcFile,   LPDIRECT3DTEXTURE9*       ppTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateTextureFromFileW"));
	original_D3DXCreateTextureFromResourceA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   HMODULE    hSrcModule,   LPCSTR     pSrcResource,   LPDIRECT3DTEXTURE9*       ppTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateTextureFromResourceA"));
	original_D3DXCreateTextureFromResourceExA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   HMODULE    hSrcModule,   LPCSTR     pSrcResource,   UINT       Width,   UINT       Height,   UINT       MipLevels,   DWORD      Usage,   D3DFORMAT     Format,   D3DPOOL    Pool,   DWORD      Filter,   DWORD      MipFilter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo,   PALETTEENTRY*    pPalette,   LPDIRECT3DTEXTURE9*       ppTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateTextureFromResourceExA"));
	original_D3DXCreateTextureFromResourceExW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   HMODULE    hSrcModule,   LPCWSTR    pSrcResource,   UINT       Width,   UINT       Height,   UINT       MipLevels,   DWORD      Usage,   D3DFORMAT     Format,   D3DPOOL    Pool,   DWORD      Filter,   DWORD      MipFilter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo,   PALETTEENTRY*    pPalette,   LPDIRECT3DTEXTURE9*       ppTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateTextureFromResourceExW"));
	original_D3DXCreateTextureFromResourceW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   HMODULE    hSrcModule,   LPCWSTR    pSrcResource,   LPDIRECT3DTEXTURE9*       ppTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateTextureFromResourceW"));
	original_D3DXCreateTextureGutterHelper = reinterpret_cast<HRESULT  (WINAPI*)(  UINT Width,   UINT Height,   LPD3DXMESH pMesh,   FLOAT GutterSize,   LPD3DXTEXTUREGUTTERHELPER* ppBuffer)>(GetProcAddress(d3d9x_module, "D3DXCreateTextureGutterHelper"));
	original_D3DXCreateTextureShader = reinterpret_cast<HRESULT  (WINAPI*)(  CONST DWORD*     pFunction,   LPD3DXTEXTURESHADER*   ppTextureShader)>(GetProcAddress(d3d9x_module, "D3DXCreateTextureShader"));
	original_D3DXCreateTorus = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   FLOAT      InnerRadius,   FLOAT      OuterRadius,   UINT    Sides,   UINT    Rings,   LPD3DXMESH*   ppMesh,   LPD3DXBUFFER*       ppAdjacency)>(GetProcAddress(d3d9x_module, "D3DXCreateTorus"));
	original_D3DXCreateVolumeTexture = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   UINT       Width,   UINT       Height,   UINT       Depth,   UINT       MipLevels,   DWORD      Usage,   D3DFORMAT     Format,   D3DPOOL    Pool,   LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateVolumeTexture"));
	original_D3DXCreateVolumeTextureFromFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPCSTR     pSrcFile,   LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateVolumeTextureFromFileA"));
	original_D3DXCreateVolumeTextureFromFileExA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPCSTR     pSrcFile,   UINT       Width,   UINT       Height,   UINT       Depth,   UINT       MipLevels,   DWORD      Usage,   D3DFORMAT     Format,   D3DPOOL    Pool,   DWORD      Filter,   DWORD      MipFilter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo,   PALETTEENTRY*    pPalette,   LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateVolumeTextureFromFileExA"));
	original_D3DXCreateVolumeTextureFromFileExW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPCWSTR    pSrcFile,   UINT       Width,   UINT       Height,   UINT       Depth,   UINT       MipLevels,   DWORD      Usage,   D3DFORMAT     Format,   D3DPOOL    Pool,   DWORD      Filter,   DWORD      MipFilter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo,   PALETTEENTRY*    pPalette,   LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateVolumeTextureFromFileExW"));
	original_D3DXCreateVolumeTextureFromFileInMemory = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPCVOID    pSrcData,   UINT       SrcDataSize,   LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateVolumeTextureFromFileInMemory"));
	original_D3DXCreateVolumeTextureFromFileInMemoryEx = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPCVOID    pSrcData,   UINT       SrcDataSize,   UINT       Width,   UINT       Height,   UINT       Depth,   UINT       MipLevels,   DWORD      Usage,   D3DFORMAT     Format,   D3DPOOL    Pool,   DWORD      Filter,   DWORD      MipFilter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo,   PALETTEENTRY*    pPalette,   LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateVolumeTextureFromFileInMemoryEx"));
	original_D3DXCreateVolumeTextureFromFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   LPCWSTR    pSrcFile,   LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateVolumeTextureFromFileW"));
	original_D3DXCreateVolumeTextureFromResourceA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   HMODULE    hSrcModule,   LPCSTR     pSrcResource,   LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateVolumeTextureFromResourceA"));
	original_D3DXCreateVolumeTextureFromResourceExA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   HMODULE    hSrcModule,   LPCSTR     pSrcResource,   UINT       Width,   UINT       Height,   UINT       Depth,   UINT       MipLevels,   DWORD      Usage,   D3DFORMAT     Format,   D3DPOOL    Pool,   DWORD      Filter,   DWORD      MipFilter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo,   PALETTEENTRY*    pPalette,   LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateVolumeTextureFromResourceExA"));
	original_D3DXCreateVolumeTextureFromResourceExW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   HMODULE    hSrcModule,   LPCWSTR    pSrcResource,   UINT       Width,   UINT       Height,   UINT       Depth,   UINT       MipLevels,   DWORD      Usage,   D3DFORMAT     Format,   D3DPOOL    Pool,   DWORD      Filter,   DWORD      MipFilter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo,   PALETTEENTRY*    pPalette,   LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateVolumeTextureFromResourceExW"));
	original_D3DXCreateVolumeTextureFromResourceW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DDEVICE9   pDevice,   HMODULE    hSrcModule,   LPCWSTR    pSrcResource,   LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)>(GetProcAddress(d3d9x_module, "D3DXCreateVolumeTextureFromResourceW"));
	original_D3DXDebugMute = reinterpret_cast<BOOL  (WINAPI*)(  BOOL Mute)>(GetProcAddress(d3d9x_module, "D3DXDebugMute"));
	original_D3DXDeclaratorFromFVF = reinterpret_cast<HRESULT  (WINAPI*)(  DWORD FVF,   D3DVERTEXELEMENT9 pDeclarator[MAX_FVF_DECL_SIZE])>(GetProcAddress(d3d9x_module, "D3DXDeclaratorFromFVF"));
	original_D3DXDisassembleEffect = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXEFFECT pEffect,   BOOL EnableColorCode,   LPD3DXBUFFER *ppDisassembly)>(GetProcAddress(d3d9x_module, "D3DXDisassembleEffect"));
	original_D3DXDisassembleShader = reinterpret_cast<HRESULT  (WINAPI*)(  CONST DWORD*     pShader,   BOOL       EnableColorCode,   LPCSTR        pComments,   LPD3DXBUFFER*    ppDisassembly)>(GetProcAddress(d3d9x_module, "D3DXDisassembleShader"));
	original_D3DXFVFFromDeclarator = reinterpret_cast<HRESULT  (WINAPI*)(  CONST D3DVERTEXELEMENT9 *pDeclarator,   DWORD *pFVF)>(GetProcAddress(d3d9x_module, "D3DXFVFFromDeclarator"));
	original_D3DXFillCubeTexture = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DCUBETEXTURE9    pCubeTexture,   LPD3DXFILL3D     pFunction,   LPVOID     pData)>(GetProcAddress(d3d9x_module, "D3DXFillCubeTexture"));
	original_D3DXFillCubeTextureTX = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DCUBETEXTURE9    pCubeTexture,   LPD3DXTEXTURESHADER       pTextureShader)>(GetProcAddress(d3d9x_module, "D3DXFillCubeTextureTX"));
	original_D3DXFillTexture = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DTEXTURE9  pTexture,   LPD3DXFILL2D     pFunction,   LPVOID     pData)>(GetProcAddress(d3d9x_module, "D3DXFillTexture"));
	original_D3DXFillTextureTX = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DTEXTURE9  pTexture,   LPD3DXTEXTURESHADER       pTextureShader)>(GetProcAddress(d3d9x_module, "D3DXFillTextureTX"));
	original_D3DXFillVolumeTexture = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DVOLUMETEXTURE9  pVolumeTexture,   LPD3DXFILL3D     pFunction,   LPVOID     pData)>(GetProcAddress(d3d9x_module, "D3DXFillVolumeTexture"));
	original_D3DXFillVolumeTextureTX = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DVOLUMETEXTURE9  pVolumeTexture,   LPD3DXTEXTURESHADER       pTextureShader)>(GetProcAddress(d3d9x_module, "D3DXFillVolumeTextureTX"));
	original_D3DXFilterTexture = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DBASETEXTURE9    pBaseTexture,   CONST PALETTEENTRY*       pPalette,   UINT       SrcLevel,   DWORD      Filter)>(GetProcAddress(d3d9x_module, "D3DXFilterTexture"));
	original_D3DXFindShaderComment = reinterpret_cast<HRESULT  (WINAPI*)(  CONST DWORD*     pFunction,   DWORD      FourCC,   LPCVOID*      ppData,   UINT*      pSizeInBytes)>(GetProcAddress(d3d9x_module, "D3DXFindShaderComment"));
	original_D3DXFloat16To32Array = reinterpret_cast<FLOAT*  (WINAPI*)(  FLOAT *pOut,   CONST D3DXFLOAT16 *pIn,   UINT n )>(GetProcAddress(d3d9x_module, "D3DXFloat16To32Array"));
	original_D3DXFloat32To16Array = reinterpret_cast<D3DXFLOAT16*  (WINAPI*)(  D3DXFLOAT16 *pOut,   CONST FLOAT *pIn,   UINT n )>(GetProcAddress(d3d9x_module, "D3DXFloat32To16Array"));
	original_D3DXFrameAppendChild = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXFRAME pFrameParent,   CONST D3DXFRAME *pFrameChild)>(GetProcAddress(d3d9x_module, "D3DXFrameAppendChild"));
	original_D3DXFrameCalculateBoundingSphere = reinterpret_cast<HRESULT  (WINAPI*)(  CONST D3DXFRAME *pFrameRoot,   LPD3DXVECTOR3 pObjectCenter,   FLOAT *pObjectRadius    )>(GetProcAddress(d3d9x_module, "D3DXFrameCalculateBoundingSphere"));
	original_D3DXFrameDestroy = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXFRAME pFrameRoot,   LPD3DXALLOCATEHIERARCHY pAlloc)>(GetProcAddress(d3d9x_module, "D3DXFrameDestroy"));
	original_D3DXFrameFind = reinterpret_cast<LPD3DXFRAME  (WINAPI*)(  CONST D3DXFRAME *pFrameRoot,   LPCSTR Name)>(GetProcAddress(d3d9x_module, "D3DXFrameFind"));
	original_D3DXFrameNumNamedMatrices = reinterpret_cast<UINT  (WINAPI*)(  CONST D3DXFRAME *pFrameRoot)>(GetProcAddress(d3d9x_module, "D3DXFrameNumNamedMatrices"));
	original_D3DXFrameRegisterNamedMatrices = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXFRAME pFrameRoot,   LPD3DXANIMATIONCONTROLLER pAnimController)>(GetProcAddress(d3d9x_module, "D3DXFrameRegisterNamedMatrices"));
	original_D3DXFresnelTerm = reinterpret_cast<FLOAT  (WINAPI*)(  FLOAT CosTheta,   FLOAT RefractionIndex)>(GetProcAddress(d3d9x_module, "D3DXFresnelTerm"));
	original_D3DXGatherFragments = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR        pSrcData,   UINT       SrcDataLen,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXBUFFER*    ppShader,   LPD3DXBUFFER*    ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXGatherFragments"));
	original_D3DXGatherFragmentsFromFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR        pSrcFile,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXBUFFER*    ppShader,   LPD3DXBUFFER*    ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXGatherFragmentsFromFileA"));
	original_D3DXGatherFragmentsFromFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR       pSrcFile,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXBUFFER*    ppShader,   LPD3DXBUFFER*    ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXGatherFragmentsFromFileW"));
	original_D3DXGatherFragmentsFromResourceA = reinterpret_cast<HRESULT  (WINAPI*)(  HMODULE       hSrcModule,   LPCSTR        pSrcResource,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXBUFFER*    ppShader,   LPD3DXBUFFER*    ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXGatherFragmentsFromResourceA"));
	original_D3DXGatherFragmentsFromResourceW = reinterpret_cast<HRESULT  (WINAPI*)(  HMODULE       hSrcModule,   LPCWSTR       pSrcResource,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   DWORD      Flags,   LPD3DXBUFFER*    ppShader,   LPD3DXBUFFER*    ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXGatherFragmentsFromResourceW"));
	original_D3DXGenerateOutputDecl = reinterpret_cast<HRESULT  (WINAPI*)(  D3DVERTEXELEMENT9 *pOutput,   CONST D3DVERTEXELEMENT9 *pInput)>(GetProcAddress(d3d9x_module, "D3DXGenerateOutputDecl"));
	original_D3DXGeneratePMesh = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXMESH pMesh,   CONST DWORD* pAdjacency,   CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,   CONST FLOAT *pVertexWeights,   DWORD MinValue,   DWORD Options,   LPD3DXPMESH* ppPMesh)>(GetProcAddress(d3d9x_module, "D3DXGeneratePMesh"));
	original_D3DXGetDeclLength = reinterpret_cast<UINT  (WINAPI*)(  CONST D3DVERTEXELEMENT9 *pDecl)>(GetProcAddress(d3d9x_module, "D3DXGetDeclLength"));
	original_D3DXGetDeclVertexSize = reinterpret_cast<UINT  (WINAPI*)(  CONST D3DVERTEXELEMENT9 *pDecl, DWORD Stream)>(GetProcAddress(d3d9x_module, "D3DXGetDeclVertexSize"));
	original_D3DXGetDriverLevel = reinterpret_cast<UINT  (WINAPI*)(  LPDIRECT3DDEVICE9 pDevice)>(GetProcAddress(d3d9x_module, "D3DXGetDriverLevel"));
	original_D3DXGetFVFVertexSize = reinterpret_cast<UINT  (WINAPI*)(  DWORD FVF)>(GetProcAddress(d3d9x_module, "D3DXGetFVFVertexSize"));
	original_D3DXGetImageInfoFromFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR     pSrcFile,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXGetImageInfoFromFileA"));
	original_D3DXGetImageInfoFromFileInMemory = reinterpret_cast<HRESULT  (WINAPI*)(  LPCVOID    pSrcData,   UINT       SrcDataSize,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXGetImageInfoFromFileInMemory"));
	original_D3DXGetImageInfoFromFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR    pSrcFile,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXGetImageInfoFromFileW"));
	original_D3DXGetImageInfoFromResourceA = reinterpret_cast<HRESULT  (WINAPI*)(  HMODULE    hSrcModule,   LPCSTR     pSrcResource,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXGetImageInfoFromResourceA"));
	original_D3DXGetImageInfoFromResourceW = reinterpret_cast<HRESULT  (WINAPI*)(  HMODULE    hSrcModule,   LPCWSTR    pSrcResource,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXGetImageInfoFromResourceW"));
	original_D3DXGetPixelShaderProfile = reinterpret_cast<LPCSTR  (WINAPI*)(  LPDIRECT3DDEVICE9      pDevice)>(GetProcAddress(d3d9x_module, "D3DXGetPixelShaderProfile"));
	original_D3DXGetShaderConstantTable = reinterpret_cast<HRESULT  (WINAPI*)(  CONST DWORD*     pFunction,   LPD3DXCONSTANTTABLE*   ppConstantTable)>(GetProcAddress(d3d9x_module, "D3DXGetShaderConstantTable"));
	original_D3DXGetShaderInputSemantics = reinterpret_cast<HRESULT  (WINAPI*)(  CONST DWORD*     pFunction,   D3DXSEMANTIC*    pSemantics,   UINT*      pCount)>(GetProcAddress(d3d9x_module, "D3DXGetShaderInputSemantics"));
	original_D3DXGetShaderOutputSemantics = reinterpret_cast<HRESULT  (WINAPI*)(  CONST DWORD*     pFunction,   D3DXSEMANTIC*    pSemantics,   UINT*      pCount)>(GetProcAddress(d3d9x_module, "D3DXGetShaderOutputSemantics"));
	original_D3DXGetShaderSamplers = reinterpret_cast<HRESULT  (WINAPI*)(  CONST DWORD*     pFunction,   LPCSTR*       pSamplers,   UINT*      pCount)>(GetProcAddress(d3d9x_module, "D3DXGetShaderSamplers"));
	original_D3DXGetShaderSize = reinterpret_cast<UINT  (WINAPI*)(  CONST DWORD*     pFunction)>(GetProcAddress(d3d9x_module, "D3DXGetShaderSize"));
	original_D3DXGetShaderVersion = reinterpret_cast<DWORD  (WINAPI*)(  CONST DWORD*     pFunction)>(GetProcAddress(d3d9x_module, "D3DXGetShaderVersion"));
	original_D3DXGetVertexShaderProfile = reinterpret_cast<LPCSTR  (WINAPI*)(  LPDIRECT3DDEVICE9      pDevice)>(GetProcAddress(d3d9x_module, "D3DXGetVertexShaderProfile"));
	original_D3DXIntersect = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXBASEMESH pMesh,   CONST D3DXVECTOR3 *pRayPos,   CONST D3DXVECTOR3 *pRayDir,   BOOL    *pHit,   DWORD   *pFaceIndex,   FLOAT   *pU,   FLOAT   *pV,   FLOAT   *pDist,   LPD3DXBUFFER *ppAllHits,   DWORD   *pCountOfHits)>(GetProcAddress(d3d9x_module, "D3DXIntersect"));
	original_D3DXIntersectSubset = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXBASEMESH pMesh,   DWORD AttribId,   CONST D3DXVECTOR3 *pRayPos,   CONST D3DXVECTOR3 *pRayDir,   BOOL    *pHit,   DWORD   *pFaceIndex,   FLOAT   *pU,   FLOAT   *pV,   FLOAT   *pDist,   LPD3DXBUFFER *ppAllHits,   DWORD   *pCountOfHits)>(GetProcAddress(d3d9x_module, "D3DXIntersectSubset"));
	original_D3DXIntersectTri = reinterpret_cast<BOOL  (WINAPI*)(  CONST D3DXVECTOR3 *p0,   CONST D3DXVECTOR3 *p1,   CONST D3DXVECTOR3 *p2,   CONST D3DXVECTOR3 *pRayPos,   CONST D3DXVECTOR3 *pRayDir,   FLOAT *pU,   FLOAT *pV,   FLOAT *pDist)>(GetProcAddress(d3d9x_module, "D3DXIntersectTri "));
	original_D3DXLoadMeshFromXA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR pFilename,   DWORD Options,   LPDIRECT3DDEVICE9 pD3DDevice,   LPD3DXBUFFER *ppAdjacency,   LPD3DXBUFFER *ppMaterials,   LPD3DXBUFFER *ppEffectInstances,   DWORD *pNumMaterials,   LPD3DXMESH *ppMesh)>(GetProcAddress(d3d9x_module, "D3DXLoadMeshFromXA"));
	original_D3DXLoadMeshFromXInMemory = reinterpret_cast<HRESULT  (WINAPI*)(  LPCVOID Memory,   DWORD SizeOfMemory,   DWORD Options,   LPDIRECT3DDEVICE9 pD3DDevice,   LPD3DXBUFFER *ppAdjacency,   LPD3DXBUFFER *ppMaterials,   LPD3DXBUFFER *ppEffectInstances,   DWORD *pNumMaterials,   LPD3DXMESH *ppMesh)>(GetProcAddress(d3d9x_module, "D3DXLoadMeshFromXInMemory"));
	original_D3DXLoadMeshFromXResource = reinterpret_cast<HRESULT  (WINAPI*)(  HMODULE Module,   LPCSTR Name,   LPCSTR Type,   DWORD Options,   LPDIRECT3DDEVICE9 pD3DDevice,   LPD3DXBUFFER *ppAdjacency,   LPD3DXBUFFER *ppMaterials,   LPD3DXBUFFER *ppEffectInstances,   DWORD *pNumMaterials,   LPD3DXMESH *ppMesh)>(GetProcAddress(d3d9x_module, "D3DXLoadMeshFromXResource"));
	original_D3DXLoadMeshFromXW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR pFilename,   DWORD Options,   LPDIRECT3DDEVICE9 pD3DDevice,   LPD3DXBUFFER *ppAdjacency,   LPD3DXBUFFER *ppMaterials,   LPD3DXBUFFER *ppEffectInstances,   DWORD *pNumMaterials,   LPD3DXMESH *ppMesh)>(GetProcAddress(d3d9x_module, "D3DXLoadMeshFromXW"));
	original_D3DXLoadMeshFromXof = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXFILEDATA pxofMesh,   DWORD Options,   LPDIRECT3DDEVICE9 pD3DDevice,   LPD3DXBUFFER *ppAdjacency,   LPD3DXBUFFER *ppMaterials,   LPD3DXBUFFER *ppEffectInstances,   DWORD *pNumMaterials,   LPD3DXMESH *ppMesh)>(GetProcAddress(d3d9x_module, "D3DXLoadMeshFromXof"));
	original_D3DXLoadMeshHierarchyFromXA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR Filename,   DWORD MeshOptions,   LPDIRECT3DDEVICE9 pD3DDevice,   LPD3DXALLOCATEHIERARCHY pAlloc,   LPD3DXLOADUSERDATA pUserDataLoader,   LPD3DXFRAME *ppFrameHierarchy,   LPD3DXANIMATIONCONTROLLER *ppAnimController)>(GetProcAddress(d3d9x_module, "D3DXLoadMeshHierarchyFromXA"));
	original_D3DXLoadMeshHierarchyFromXInMemory = reinterpret_cast<HRESULT  (WINAPI*)(  LPCVOID Memory,   DWORD SizeOfMemory,   DWORD MeshOptions,   LPDIRECT3DDEVICE9 pD3DDevice,   LPD3DXALLOCATEHIERARCHY pAlloc,   LPD3DXLOADUSERDATA pUserDataLoader,   LPD3DXFRAME *ppFrameHierarchy,   LPD3DXANIMATIONCONTROLLER *ppAnimController)>(GetProcAddress(d3d9x_module, "D3DXLoadMeshHierarchyFromXInMemory"));
	original_D3DXLoadMeshHierarchyFromXW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR Filename,   DWORD MeshOptions,   LPDIRECT3DDEVICE9 pD3DDevice,   LPD3DXALLOCATEHIERARCHY pAlloc,   LPD3DXLOADUSERDATA pUserDataLoader,   LPD3DXFRAME *ppFrameHierarchy,   LPD3DXANIMATIONCONTROLLER *ppAnimController)>(GetProcAddress(d3d9x_module, "D3DXLoadMeshHierarchyFromXW"));
	original_D3DXLoadPRTBufferFromFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR pFilename,   LPD3DXPRTBUFFER*       ppBuffer)>(GetProcAddress(d3d9x_module, "D3DXLoadPRTBufferFromFileA"));
	original_D3DXLoadPRTBufferFromFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR pFilename,   LPD3DXPRTBUFFER*       ppBuffer)>(GetProcAddress(d3d9x_module, "D3DXLoadPRTBufferFromFileW"));
	original_D3DXLoadPRTCompBufferFromFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR pFilename, LPD3DXPRTCOMPBUFFER*       ppBuffer)>(GetProcAddress(d3d9x_module, "D3DXLoadPRTCompBufferFromFileA"));
	original_D3DXLoadPRTCompBufferFromFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR pFilename,   LPD3DXPRTCOMPBUFFER*       ppBuffer)>(GetProcAddress(d3d9x_module, "D3DXLoadPRTCompBufferFromFileW"));
	original_D3DXLoadPatchMeshFromXof = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXFILEDATA pXofObjMesh,   DWORD Options,   LPDIRECT3DDEVICE9 pD3DDevice,   LPD3DXBUFFER *ppMaterials,   LPD3DXBUFFER *ppEffectInstances,   PDWORD pNumMaterials,   LPD3DXPATCHMESH *ppMesh)>(GetProcAddress(d3d9x_module, "D3DXLoadPatchMeshFromXof"));
	original_D3DXLoadSkinMeshFromXof = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXFILEDATA pxofMesh,   DWORD Options,   LPDIRECT3DDEVICE9 pD3DDevice,   LPD3DXBUFFER* ppAdjacency,   LPD3DXBUFFER* ppMaterials,   LPD3DXBUFFER *ppEffectInstances,   DWORD *pMatOut,   LPD3DXSKININFO* ppSkinInfo,   LPD3DXMESH* ppMesh)>(GetProcAddress(d3d9x_module, "D3DXLoadSkinMeshFromXof"));
	original_D3DXLoadSurfaceFromFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DSURFACE9  pDestSurface,   CONST PALETTEENTRY*       pDestPalette,   CONST RECT*      pDestRect,   LPCSTR     pSrcFile,   CONST RECT*      pSrcRect,   DWORD      Filter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXLoadSurfaceFromFileA"));
	original_D3DXLoadSurfaceFromFileInMemory = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DSURFACE9  pDestSurface,   CONST PALETTEENTRY*       pDestPalette,   CONST RECT*      pDestRect,   LPCVOID    pSrcData,   UINT       SrcDataSize,   CONST RECT*      pSrcRect,   DWORD      Filter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXLoadSurfaceFromFileInMemory"));
	original_D3DXLoadSurfaceFromFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DSURFACE9  pDestSurface,   CONST PALETTEENTRY*       pDestPalette,   CONST RECT*      pDestRect,   LPCWSTR    pSrcFile,   CONST RECT*      pSrcRect,   DWORD      Filter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXLoadSurfaceFromFileW"));
	original_D3DXLoadSurfaceFromMemory = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DSURFACE9  pDestSurface,   CONST PALETTEENTRY*       pDestPalette,   CONST RECT*      pDestRect,   LPCVOID    pSrcMemory,   D3DFORMAT     SrcFormat,   UINT       SrcPitch,   CONST PALETTEENTRY*       pSrcPalette,   CONST RECT*      pSrcRect,   DWORD      Filter,   D3DCOLOR      ColorKey)>(GetProcAddress(d3d9x_module, "D3DXLoadSurfaceFromMemory"));
	original_D3DXLoadSurfaceFromResourceA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DSURFACE9  pDestSurface,   CONST PALETTEENTRY*       pDestPalette,   CONST RECT*      pDestRect,   HMODULE    hSrcModule,   LPCSTR     pSrcResource,   CONST RECT*      pSrcRect,   DWORD      Filter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXLoadSurfaceFromResourceA"));
	original_D3DXLoadSurfaceFromResourceW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DSURFACE9  pDestSurface,   CONST PALETTEENTRY*       pDestPalette,   CONST RECT*      pDestRect,   HMODULE    hSrcModule,   LPCWSTR    pSrcResource,   CONST RECT*      pSrcRect,   DWORD      Filter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXLoadSurfaceFromResourceW"));
	original_D3DXLoadSurfaceFromSurface = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DSURFACE9  pDestSurface,   CONST PALETTEENTRY*       pDestPalette,   CONST RECT*      pDestRect,   LPDIRECT3DSURFACE9  pSrcSurface,   CONST PALETTEENTRY*       pSrcPalette,   CONST RECT*      pSrcRect,   DWORD      Filter,   D3DCOLOR      ColorKey)>(GetProcAddress(d3d9x_module, "D3DXLoadSurfaceFromSurface"));
	original_D3DXLoadVolumeFromFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DVOLUME9   pDestVolume,   CONST PALETTEENTRY*       pDestPalette,   CONST D3DBOX*    pDestBox,   LPCSTR     pSrcFile,   CONST D3DBOX*    pSrcBox,   DWORD      Filter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXLoadVolumeFromFileA"));
	original_D3DXLoadVolumeFromFileInMemory = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DVOLUME9   pDestVolume,   CONST PALETTEENTRY*       pDestPalette,   CONST D3DBOX*    pDestBox,   LPCVOID    pSrcData,   UINT       SrcDataSize,   CONST D3DBOX*    pSrcBox,   DWORD      Filter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXLoadVolumeFromFileInMemory"));
	original_D3DXLoadVolumeFromFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DVOLUME9   pDestVolume,   CONST PALETTEENTRY*       pDestPalette,   CONST D3DBOX*    pDestBox,   LPCWSTR    pSrcFile,   CONST D3DBOX*    pSrcBox,   DWORD      Filter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXLoadVolumeFromFileW"));
	original_D3DXLoadVolumeFromMemory = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DVOLUME9   pDestVolume,   CONST PALETTEENTRY*       pDestPalette,   CONST D3DBOX*    pDestBox,   LPCVOID    pSrcMemory,   D3DFORMAT     SrcFormat,   UINT       SrcRowPitch,   UINT       SrcSlicePitch,   CONST PALETTEENTRY*       pSrcPalette,   CONST D3DBOX*    pSrcBox,   DWORD      Filter,   D3DCOLOR      ColorKey)>(GetProcAddress(d3d9x_module, "D3DXLoadVolumeFromMemory"));
	original_D3DXLoadVolumeFromResourceA = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DVOLUME9   pDestVolume,   CONST PALETTEENTRY*       pDestPalette,   CONST D3DBOX*    pDestBox,   HMODULE    hSrcModule,   LPCSTR     pSrcResource,   CONST D3DBOX*    pSrcBox,   DWORD      Filter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXLoadVolumeFromResourceA"));
	original_D3DXLoadVolumeFromResourceW = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DVOLUME9   pDestVolume,   CONST PALETTEENTRY*       pDestPalette,   CONST D3DBOX*    pDestBox,   HMODULE    hSrcModule,   LPCWSTR    pSrcResource,   CONST D3DBOX*    pSrcBox,   DWORD      Filter,   D3DCOLOR      ColorKey,   D3DXIMAGE_INFO*  pSrcInfo)>(GetProcAddress(d3d9x_module, "D3DXLoadVolumeFromResourceW"));
	original_D3DXLoadVolumeFromVolume = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DVOLUME9   pDestVolume,   CONST PALETTEENTRY*       pDestPalette,   CONST D3DBOX*    pDestBox,   LPDIRECT3DVOLUME9   pSrcVolume,   CONST PALETTEENTRY*       pSrcPalette,   CONST D3DBOX*    pSrcBox,   DWORD      Filter,   D3DCOLOR      ColorKey)>(GetProcAddress(d3d9x_module, "D3DXLoadVolumeFromVolume"));
	original_D3DXMatrixAffineTransformation = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT Scaling,   CONST D3DXVECTOR3 *pRotationCenter,   CONST D3DXQUATERNION *pRotation,   CONST D3DXVECTOR3 *pTranslation)>(GetProcAddress(d3d9x_module, "D3DXMatrixAffineTransformation"));
	original_D3DXMatrixAffineTransformation2D = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT Scaling,   CONST D3DXVECTOR2* pRotationCenter,   FLOAT Rotation,   CONST D3DXVECTOR2* pTranslation)>(GetProcAddress(d3d9x_module, "D3DXMatrixAffineTransformation2D"));
	original_D3DXMatrixDecompose = reinterpret_cast<HRESULT  (WINAPI*)(  D3DXVECTOR3 *pOutScale,   D3DXQUATERNION *pOutRotation,   D3DXVECTOR3 *pOutTranslation,   CONST D3DXMATRIX *pM )>(GetProcAddress(d3d9x_module, "D3DXMatrixDecompose"));
	original_D3DXMatrixDeterminant = reinterpret_cast<FLOAT  (WINAPI*)(  CONST D3DXMATRIX *pM )>(GetProcAddress(d3d9x_module, "D3DXMatrixDeterminant"));
	original_D3DXMatrixInverse = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT *pDeterminant,   CONST D3DXMATRIX *pM )>(GetProcAddress(d3d9x_module, "D3DXMatrixInverse"));
	original_D3DXMatrixLookAtLH = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   CONST D3DXVECTOR3 *pEye,   CONST D3DXVECTOR3 *pAt,   CONST D3DXVECTOR3 *pUp )>(GetProcAddress(d3d9x_module, "D3DXMatrixLookAtLH"));
	original_D3DXMatrixLookAtRH = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   CONST D3DXVECTOR3 *pEye,   CONST D3DXVECTOR3 *pAt,   CONST D3DXVECTOR3 *pUp )>(GetProcAddress(d3d9x_module, "D3DXMatrixLookAtRH"));
	original_D3DXMatrixMultiply = reinterpret_cast<D3DXMATRIX *  (WINAPI*)(  D3DXMATRIX *pOut, CONST D3DXMATRIX *pM1,   CONST D3DXMATRIX *pM2)>(GetProcAddress(d3d9x_module, "D3DXMatrixMultiply"));
	original_D3DXMatrixMultiplyTranspose = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   CONST D3DXMATRIX *pM1,   CONST D3DXMATRIX *pM2 )>(GetProcAddress(d3d9x_module, "D3DXMatrixMultiplyTranspose"));
	original_D3DXMatrixOrthoLH = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT w,   FLOAT h,   FLOAT zn,   FLOAT zf )>(GetProcAddress(d3d9x_module, "D3DXMatrixOrthoLH"));
	original_D3DXMatrixOrthoOffCenterLH = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT l,   FLOAT r,   FLOAT b,   FLOAT t,   FLOAT zn,   FLOAT zf )>(GetProcAddress(d3d9x_module, "D3DXMatrixOrthoOffCenterLH"));
	original_D3DXMatrixOrthoOffCenterRH = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT l,   FLOAT r,   FLOAT b,   FLOAT t,   FLOAT zn,   FLOAT zf )>(GetProcAddress(d3d9x_module, "D3DXMatrixOrthoOffCenterRH"));
	original_D3DXMatrixOrthoRH = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT w,   FLOAT h,   FLOAT zn,   FLOAT zf )>(GetProcAddress(d3d9x_module, "D3DXMatrixOrthoRH"));
	original_D3DXMatrixPerspectiveFovLH = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT fovy,   FLOAT Aspect,   FLOAT zn,   FLOAT zf )>(GetProcAddress(d3d9x_module, "D3DXMatrixPerspectiveFovLH"));
	original_D3DXMatrixPerspectiveFovRH = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT fovy,   FLOAT Aspect,   FLOAT zn,   FLOAT zf )>(GetProcAddress(d3d9x_module, "D3DXMatrixPerspectiveFovRH"));
	original_D3DXMatrixPerspectiveLH = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT w,   FLOAT h,   FLOAT zn,   FLOAT zf )>(GetProcAddress(d3d9x_module, "D3DXMatrixPerspectiveLH"));
	original_D3DXMatrixPerspectiveOffCenterLH = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT l,   FLOAT r,   FLOAT b,   FLOAT t,   FLOAT zn,   FLOAT zf )>(GetProcAddress(d3d9x_module, "D3DXMatrixPerspectiveOffCenterLH"));
	original_D3DXMatrixPerspectiveOffCenterRH = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT l,   FLOAT r,   FLOAT b,   FLOAT t,   FLOAT zn,   FLOAT zf )>(GetProcAddress(d3d9x_module, "D3DXMatrixPerspectiveOffCenterRH"));
	original_D3DXMatrixPerspectiveRH = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT w,   FLOAT h,   FLOAT zn,   FLOAT zf )>(GetProcAddress(d3d9x_module, "D3DXMatrixPerspectiveRH"));
	original_D3DXMatrixReflect = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   CONST D3DXPLANE *pPlane )>(GetProcAddress(d3d9x_module, "D3DXMatrixReflect"));
	original_D3DXMatrixRotationAxis = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   CONST D3DXVECTOR3 *pV,   FLOAT Angle )>(GetProcAddress(d3d9x_module, "D3DXMatrixRotationAxis"));
	original_D3DXMatrixRotationQuaternion = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   CONST D3DXQUATERNION *pQ)>(GetProcAddress(d3d9x_module, "D3DXMatrixRotationQuaternion"));
	original_D3DXMatrixRotationX = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT Angle )>(GetProcAddress(d3d9x_module, "D3DXMatrixRotationX"));
	original_D3DXMatrixRotationY = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT Angle )>(GetProcAddress(d3d9x_module, "D3DXMatrixRotationY"));
	original_D3DXMatrixRotationYawPitchRoll = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT Yaw,   FLOAT Pitch,   FLOAT Roll )>(GetProcAddress(d3d9x_module, "D3DXMatrixRotationYawPitchRoll"));
	original_D3DXMatrixRotationZ = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT Angle )>(GetProcAddress(d3d9x_module, "D3DXMatrixRotationZ"));
	original_D3DXMatrixScaling = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT sx,   FLOAT sy,   FLOAT sz )>(GetProcAddress(d3d9x_module, "D3DXMatrixScaling"));
	original_D3DXMatrixShadow = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   CONST D3DXVECTOR4 *pLight,   CONST D3DXPLANE *pPlane )>(GetProcAddress(d3d9x_module, "D3DXMatrixShadow"));
	original_D3DXMatrixTransformation = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   CONST D3DXVECTOR3 *pScalingCenter,   CONST D3DXQUATERNION *pScalingRotation,   CONST D3DXVECTOR3 *pScaling,  CONST  D3DXVECTOR3 *pRotationCenter,   CONST D3DXQUATERNION *pRotation,  CONST  D3DXVECTOR3 *pTranslation)>(GetProcAddress(d3d9x_module, "D3DXMatrixTransformation"));
	original_D3DXMatrixTransformation2D = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   CONST D3DXVECTOR2* pScalingCenter,   FLOAT ScalingRotation,   CONST D3DXVECTOR2* pScaling,    CONST D3DXVECTOR2* pRotationCenter,   FLOAT Rotation,    CONST D3DXVECTOR2* pTranslation)>(GetProcAddress(d3d9x_module, "D3DXMatrixTransformation2D"));
	original_D3DXMatrixTranslation = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   FLOAT x,   FLOAT y,   FLOAT z )>(GetProcAddress(d3d9x_module, "D3DXMatrixTranslation"));
	original_D3DXMatrixTranspose = reinterpret_cast<D3DXMATRIX*  (WINAPI*)(  D3DXMATRIX *pOut,   CONST D3DXMATRIX *pM )>(GetProcAddress(d3d9x_module, "D3DXMatrixTranspose"));
	original_D3DXOptimizeFaces = reinterpret_cast<HRESULT  (WINAPI*)(  LPCVOID pbIndices,   UINT cFaces,   UINT cVertices,   BOOL b32BitIndices,   DWORD* pFaceRemap)>(GetProcAddress(d3d9x_module, "D3DXOptimizeFaces"));
	original_D3DXOptimizeVertices = reinterpret_cast<HRESULT  (WINAPI*)(  LPCVOID pbIndices,   UINT cFaces,   UINT cVertices,   BOOL b32BitIndices,   DWORD* pVertexRemap)>(GetProcAddress(d3d9x_module, "D3DXOptimizeVertices"));
	original_D3DXPlaneFromPointNormal = reinterpret_cast<D3DXPLANE*  (WINAPI*)(  D3DXPLANE *pOut,   CONST D3DXVECTOR3 *pPoint,   CONST D3DXVECTOR3 *pNormal)>(GetProcAddress(d3d9x_module, "D3DXPlaneFromPointNormal"));
	original_D3DXPlaneFromPoints = reinterpret_cast<D3DXPLANE*  (WINAPI*)(  D3DXPLANE *pOut,   CONST D3DXVECTOR3 *pV1,   CONST D3DXVECTOR3 *pV2,  CONST  D3DXVECTOR3 *pV3)>(GetProcAddress(d3d9x_module, "D3DXPlaneFromPoints"));
	original_D3DXPlaneIntersectLine = reinterpret_cast<D3DXVECTOR3*  (WINAPI*)(  D3DXVECTOR3 *pOut,   CONST D3DXPLANE *pP,   CONST D3DXVECTOR3 *pV1,  CONST  D3DXVECTOR3 *pV2)>(GetProcAddress(d3d9x_module, "D3DXPlaneIntersectLine"));
	original_D3DXPlaneNormalize = reinterpret_cast<D3DXPLANE*  (WINAPI*)(  D3DXPLANE *pOut,   CONST D3DXPLANE *pP)>(GetProcAddress(d3d9x_module, "D3DXPlaneNormalize"));
	original_D3DXPlaneTransform = reinterpret_cast<D3DXPLANE*  (WINAPI*)(  D3DXPLANE *pOut,   CONST D3DXPLANE *pP,   CONST D3DXMATRIX *pM )>(GetProcAddress(d3d9x_module, "D3DXPlaneTransform"));
	original_D3DXPlaneTransformArray = reinterpret_cast<D3DXPLANE*  (WINAPI*)(  D3DXPLANE *pOut,   UINT OutStride,   CONST D3DXPLANE *pP,   UINT PStride,   CONST D3DXMATRIX *pM,   UINT n )>(GetProcAddress(d3d9x_module, "D3DXPlaneTransformArray"));
	original_D3DXPreprocessShader = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR        pSrcData,   UINT       SrcDataSize,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   LPD3DXBUFFER*    ppShaderText,   LPD3DXBUFFER*    ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXPreprocessShader"));
	original_D3DXPreprocessShaderFromFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR        pSrcFile,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   LPD3DXBUFFER*    ppShaderText,   LPD3DXBUFFER*    ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXPreprocessShaderFromFileA"));
	original_D3DXPreprocessShaderFromFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR       pSrcFile,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   LPD3DXBUFFER*    ppShaderText,   LPD3DXBUFFER*    ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXPreprocessShaderFromFileW"));
	original_D3DXPreprocessShaderFromResourceA = reinterpret_cast<HRESULT  (WINAPI*)(  HMODULE       hSrcModule,   LPCSTR        pSrcResource,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   LPD3DXBUFFER*    ppShaderText,   LPD3DXBUFFER*    ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXPreprocessShaderFromResourceA"));
	original_D3DXPreprocessShaderFromResourceW = reinterpret_cast<HRESULT  (WINAPI*)(  HMODULE       hSrcModule,   LPCWSTR       pSrcResource,   CONST D3DXMACRO*    pDefines,   LPD3DXINCLUDE    pInclude,   LPD3DXBUFFER*    ppShaderText,   LPD3DXBUFFER*    ppErrorMsgs)>(GetProcAddress(d3d9x_module, "D3DXPreprocessShaderFromResourceW"));
	original_D3DXQuaternionBaryCentric = reinterpret_cast<D3DXQUATERNION*  (WINAPI*)(  D3DXQUATERNION *pOut,   CONST D3DXQUATERNION *pQ1,   CONST D3DXQUATERNION *pQ2,   CONST D3DXQUATERNION *pQ3,   FLOAT f,   FLOAT g )>(GetProcAddress(d3d9x_module, "D3DXQuaternionBaryCentric"));
	original_D3DXQuaternionExp = reinterpret_cast<D3DXQUATERNION*  (WINAPI*)(  D3DXQUATERNION *pOut,   CONST D3DXQUATERNION *pQ )>(GetProcAddress(d3d9x_module, "D3DXQuaternionExp"));
	original_D3DXQuaternionInverse = reinterpret_cast<D3DXQUATERNION*  (WINAPI*)(  D3DXQUATERNION *pOut,   CONST D3DXQUATERNION *pQ )>(GetProcAddress(d3d9x_module, "D3DXQuaternionInverse"));
	original_D3DXQuaternionLn = reinterpret_cast<D3DXQUATERNION*  (WINAPI*)(  D3DXQUATERNION *pOut,   CONST D3DXQUATERNION *pQ )>(GetProcAddress(d3d9x_module, "D3DXQuaternionLn"));
	original_D3DXQuaternionMultiply = reinterpret_cast<D3DXQUATERNION*  (WINAPI*)(  D3DXQUATERNION *pOut,   CONST D3DXQUATERNION *pQ1,   CONST D3DXQUATERNION *pQ2 )>(GetProcAddress(d3d9x_module, "D3DXQuaternionMultiply"));
	original_D3DXQuaternionNormalize = reinterpret_cast<D3DXQUATERNION*  (WINAPI*)(  D3DXQUATERNION *pOut,   CONST D3DXQUATERNION *pQ )>(GetProcAddress(d3d9x_module, "D3DXQuaternionNormalize"));
	original_D3DXQuaternionRotationAxis = reinterpret_cast<D3DXQUATERNION*  (WINAPI*)(  D3DXQUATERNION *pOut,   CONST D3DXVECTOR3 *pV,   FLOAT Angle )>(GetProcAddress(d3d9x_module, "D3DXQuaternionRotationAxis"));
	original_D3DXQuaternionRotationMatrix = reinterpret_cast<D3DXQUATERNION*  (WINAPI*)(  D3DXQUATERNION *pOut,   CONST D3DXMATRIX *pM)>(GetProcAddress(d3d9x_module, "D3DXQuaternionRotationMatrix"));
	original_D3DXQuaternionRotationYawPitchRoll = reinterpret_cast<D3DXQUATERNION*  (WINAPI*)(  D3DXQUATERNION *pOut,   FLOAT Yaw,   FLOAT Pitch,   FLOAT Roll )>(GetProcAddress(d3d9x_module, "D3DXQuaternionRotationYawPitchRoll"));
	original_D3DXQuaternionSlerp = reinterpret_cast<D3DXQUATERNION*  (WINAPI*)(  D3DXQUATERNION *pOut,   CONST D3DXQUATERNION *pQ1,   CONST D3DXQUATERNION *pQ2,   FLOAT t )>(GetProcAddress(d3d9x_module, "D3DXQuaternionSlerp"));
	original_D3DXQuaternionSquad = reinterpret_cast<D3DXQUATERNION*  (WINAPI*)(  D3DXQUATERNION *pOut,   CONST D3DXQUATERNION *pQ1,   CONST D3DXQUATERNION *pA,   CONST D3DXQUATERNION *pB,   CONST D3DXQUATERNION *pC,   FLOAT t )>(GetProcAddress(d3d9x_module, "D3DXQuaternionSquad"));
	original_D3DXQuaternionSquadSetup = reinterpret_cast<void  (WINAPI*)(  D3DXQUATERNION *pAOut,   D3DXQUATERNION *pBOut,   D3DXQUATERNION *pCOut,   CONST D3DXQUATERNION *pQ0,   CONST D3DXQUATERNION *pQ1,   CONST D3DXQUATERNION *pQ2,   CONST D3DXQUATERNION *pQ3 )>(GetProcAddress(d3d9x_module, "D3DXQuaternionSquadSetup"));
	original_D3DXQuaternionToAxisAngle = reinterpret_cast<void  (WINAPI*)(  CONST D3DXQUATERNION *pQ,   D3DXVECTOR3 *pAxis,   FLOAT *pAngle )>(GetProcAddress(d3d9x_module, "D3DXQuaternionToAxisAngle"));
	original_D3DXRectPatchSize = reinterpret_cast<HRESULT  (WINAPI*)(  CONST FLOAT *pfNumSegs,   DWORD *pdwTriangles,   DWORD *pdwVertices)>(GetProcAddress(d3d9x_module, "D3DXRectPatchSize"));
	original_D3DXSHAdd = reinterpret_cast<FLOAT*  (WINAPI*)(  FLOAT *pOut,   UINT Order,   CONST FLOAT *pA,   CONST FLOAT *pB )>(GetProcAddress(d3d9x_module, "D3DXSHAdd"));
	original_D3DXSHDot = reinterpret_cast<FLOAT  (WINAPI*)(  UINT Order,   CONST FLOAT *pA,   CONST FLOAT *pB )>(GetProcAddress(d3d9x_module, "D3DXSHDot"));
	original_D3DXSHEvalConeLight = reinterpret_cast<HRESULT  (WINAPI*)(  UINT Order,   CONST D3DXVECTOR3 *pDir,   FLOAT Radius,   FLOAT RIntensity,   FLOAT GIntensity,   FLOAT BIntensity,   FLOAT *pROut,   FLOAT *pGOut,   FLOAT *pBOut )>(GetProcAddress(d3d9x_module, "D3DXSHEvalConeLight"));
	original_D3DXSHEvalDirection = reinterpret_cast<FLOAT*  (WINAPI*)(   FLOAT *pOut,   UINT Order,   CONST D3DXVECTOR3 *pDir )>(GetProcAddress(d3d9x_module, "D3DXSHEvalDirection"));
	original_D3DXSHEvalDirectionalLight = reinterpret_cast<HRESULT  (WINAPI*)(  UINT Order,   CONST D3DXVECTOR3 *pDir,   FLOAT RIntensity,   FLOAT GIntensity,   FLOAT BIntensity,   FLOAT *pROut,   FLOAT *pGOut,   FLOAT *pBOut )>(GetProcAddress(d3d9x_module, "D3DXSHEvalDirectionalLight"));
	original_D3DXSHEvalHemisphereLight = reinterpret_cast<HRESULT  (WINAPI*)(  UINT Order,   CONST D3DXVECTOR3 *pDir,   D3DXCOLOR Top,   D3DXCOLOR Bottom,   FLOAT *pROut,   FLOAT *pGOut,   FLOAT *pBOut )>(GetProcAddress(d3d9x_module, "D3DXSHEvalHemisphereLight"));
	original_D3DXSHEvalSphericalLight = reinterpret_cast<HRESULT  (WINAPI*)(  UINT Order,   CONST D3DXVECTOR3 *pPos,   FLOAT Radius,   FLOAT RIntensity,   FLOAT GIntensity,   FLOAT BIntensity,   FLOAT *pROut,   FLOAT *pGOut,   FLOAT *pBOut )>(GetProcAddress(d3d9x_module, "D3DXSHEvalSphericalLight"));
	original_D3DXSHMultiply2 = reinterpret_cast<FLOAT*  (WINAPI*)(  FLOAT *pOut,   CONST FLOAT *pF,   CONST FLOAT *pG)>(GetProcAddress(d3d9x_module, "D3DXSHMultiply2"));
	original_D3DXSHMultiply3 = reinterpret_cast<FLOAT*  (WINAPI*)(  FLOAT *pOut,   CONST FLOAT *pF,   CONST FLOAT *pG)>(GetProcAddress(d3d9x_module, "D3DXSHMultiply3"));
	original_D3DXSHMultiply4 = reinterpret_cast<FLOAT*  (WINAPI*)(  FLOAT *pOut,   CONST FLOAT *pF,   CONST FLOAT *pG)>(GetProcAddress(d3d9x_module, "D3DXSHMultiply4"));
	original_D3DXSHMultiply5 = reinterpret_cast<FLOAT*  (WINAPI*)(  FLOAT *pOut,   CONST FLOAT *pF,   CONST FLOAT *pG)>(GetProcAddress(d3d9x_module, "D3DXSHMultiply5("));
	original_D3DXSHMultiply6 = reinterpret_cast<FLOAT*  (WINAPI*)(  FLOAT *pOut,   CONST FLOAT *pF,   CONST FLOAT *pG)>(GetProcAddress(d3d9x_module, "D3DXSHMultiply6("));
	original_D3DXSHPRTCompSplitMeshSC = reinterpret_cast<HRESULT  (WINAPI*)(  UINT *pClusterIDs,   UINT NumVertices,   UINT NumClusters,   UINT *pSuperClusterIDs,   UINT NumSuperClusters,   LPVOID pInputIB,   BOOL InputIBIs32Bit,   UINT NumFaces,   LPD3DXBUFFER *ppIBData,   UINT *pIBDataLength,   BOOL OutputIBIs32Bit,   LPD3DXBUFFER *ppFaceRemap,   LPD3DXBUFFER *ppVertData,   UINT *pVertDataLength,   UINT *pSCClusterList,   D3DXSHPRTSPLITMESHCLUSTERDATA *pSCData)>(GetProcAddress(d3d9x_module, "D3DXSHPRTCompSplitMeshSC"));
	original_D3DXSHPRTCompSuperCluster = reinterpret_cast<HRESULT  (WINAPI*)(  UINT *pClusterIDs,   LPD3DXMESH pScene,   UINT MaxNumClusters,   UINT NumClusters,   UINT *pSuperClusterIDs,   UINT *pNumSuperClusters)>(GetProcAddress(d3d9x_module, "D3DXSHPRTCompSuperCluster"));
	original_D3DXSHProjectCubeMap = reinterpret_cast<HRESULT  (WINAPI*)(  UINT uOrder,  LPDIRECT3DCUBETEXTURE9 pCubeMap, FLOAT *pROut,   FLOAT *pGOut,   FLOAT *pBOut )>(GetProcAddress(d3d9x_module, "D3DXSHProjectCubeMap"));
	original_D3DXSHRotate = reinterpret_cast<FLOAT*  (WINAPI*)(  FLOAT *pOut,   UINT Order,   CONST D3DXMATRIX *pMatrix,   CONST FLOAT *pIn )>(GetProcAddress(d3d9x_module, "D3DXSHRotate"));
	original_D3DXSHRotateZ = reinterpret_cast<FLOAT*  (WINAPI*)(  FLOAT *pOut,   UINT Order,   FLOAT Angle,   CONST FLOAT *pIn )>(GetProcAddress(d3d9x_module, "D3DXSHRotateZ"));
	original_D3DXSHScale = reinterpret_cast<FLOAT*  (WINAPI*)(  FLOAT *pOut,   UINT Order,   CONST FLOAT *pIn,   CONST FLOAT Scale )>(GetProcAddress(d3d9x_module, "D3DXSHScale"));
	original_D3DXSaveMeshHierarchyToFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR Filename,   DWORD XFormat,   CONST D3DXFRAME *pFrameRoot,   LPD3DXANIMATIONCONTROLLER pAnimcontroller,   LPD3DXSAVEUSERDATA pUserDataSaver)>(GetProcAddress(d3d9x_module, "D3DXSaveMeshHierarchyToFileA"));
	original_D3DXSaveMeshHierarchyToFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR Filename,   DWORD XFormat,   CONST D3DXFRAME *pFrameRoot,   LPD3DXANIMATIONCONTROLLER pAnimController,   LPD3DXSAVEUSERDATA pUserDataSaver)>(GetProcAddress(d3d9x_module, "D3DXSaveMeshHierarchyToFileW"));
	original_D3DXSaveMeshToXA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR pFilename,   LPD3DXMESH pMesh,   CONST DWORD* pAdjacency,   CONST D3DXMATERIAL* pMaterials,   CONST D3DXEFFECTINSTANCE* pEffectInstances,   DWORD NumMaterials,   DWORD Format)>(GetProcAddress(d3d9x_module, "D3DXSaveMeshToXA"));
	original_D3DXSaveMeshToXW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR pFilename,   LPD3DXMESH pMesh,   CONST DWORD* pAdjacency,   CONST D3DXMATERIAL* pMaterials,   CONST D3DXEFFECTINSTANCE* pEffectInstances,   DWORD NumMaterials,   DWORD Format)>(GetProcAddress(d3d9x_module, "D3DXSaveMeshToXW"));
	original_D3DXSavePRTBufferToFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR pFileName, LPD3DXPRTBUFFER )>(GetProcAddress(d3d9x_module, "D3DXSavePRTBufferToFileA"));
	original_D3DXSavePRTBufferToFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR pFileName,   LPD3DXPRTBUFFER pBuffer)>(GetProcAddress(d3d9x_module, "D3DXSavePRTBufferToFileW"));
	original_D3DXSavePRTCompBufferToFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR pFileName, LPD3DXPRTCOMPBUFFER pBuffer)>(GetProcAddress(d3d9x_module, "D3DXSavePRTCompBufferToFileA"));
	original_D3DXSavePRTCompBufferToFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR pFileName,   LPD3DXPRTCOMPBUFFER pBuffer)>(GetProcAddress(d3d9x_module, "D3DXSavePRTCompBufferToFileW"));
	original_D3DXSaveSurfaceToFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR     pDestFile,   D3DXIMAGE_FILEFORMAT      DestFormat,   LPDIRECT3DSURFACE9  pSrcSurface,   CONST PALETTEENTRY*       pSrcPalette,   CONST RECT*      pSrcRect)>(GetProcAddress(d3d9x_module, "D3DXSaveSurfaceToFileA"));
	original_D3DXSaveSurfaceToFileInMemory = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXBUFFER*    ppDestBuf,   D3DXIMAGE_FILEFORMAT      DestFormat,   LPDIRECT3DSURFACE9  pSrcSurface,   CONST PALETTEENTRY*       pSrcPalette,   CONST RECT*      pSrcRect)>(GetProcAddress(d3d9x_module, "D3DXSaveSurfaceToFileInMemory"));
	original_D3DXSaveSurfaceToFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR    pDestFile,   D3DXIMAGE_FILEFORMAT      DestFormat,   LPDIRECT3DSURFACE9  pSrcSurface,   CONST PALETTEENTRY*       pSrcPalette,   CONST RECT*      pSrcRect)>(GetProcAddress(d3d9x_module, "D3DXSaveSurfaceToFileW"));
	original_D3DXSaveTextureToFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR     pDestFile,   D3DXIMAGE_FILEFORMAT      DestFormat,   LPDIRECT3DBASETEXTURE9    pSrcTexture,   CONST PALETTEENTRY*       pSrcPalette)>(GetProcAddress(d3d9x_module, "D3DXSaveTextureToFileA"));
	original_D3DXSaveTextureToFileInMemory = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXBUFFER*    ppDestBuf,   D3DXIMAGE_FILEFORMAT      DestFormat,   LPDIRECT3DBASETEXTURE9    pSrcTexture,   CONST PALETTEENTRY*       pSrcPalette)>(GetProcAddress(d3d9x_module, "D3DXSaveTextureToFileInMemory"));
	original_D3DXSaveTextureToFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR    pDestFile,   D3DXIMAGE_FILEFORMAT      DestFormat,   LPDIRECT3DBASETEXTURE9    pSrcTexture,   CONST PALETTEENTRY*       pSrcPalette)>(GetProcAddress(d3d9x_module, "D3DXSaveTextureToFileW"));
	original_D3DXSaveVolumeToFileA = reinterpret_cast<HRESULT  (WINAPI*)(  LPCSTR     pDestFile,   D3DXIMAGE_FILEFORMAT      DestFormat,   LPDIRECT3DVOLUME9   pSrcVolume,   CONST PALETTEENTRY*       pSrcPalette,   CONST D3DBOX*    pSrcBox)>(GetProcAddress(d3d9x_module, "D3DXSaveVolumeToFileA"));
	original_D3DXSaveVolumeToFileInMemory = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXBUFFER*    ppDestBuf,   D3DXIMAGE_FILEFORMAT      DestFormat,   LPDIRECT3DVOLUME9   pSrcVolume,   CONST PALETTEENTRY*       pSrcPalette,   CONST D3DBOX*    pSrcBox)>(GetProcAddress(d3d9x_module, "D3DXSaveVolumeToFileInMemory"));
	original_D3DXSaveVolumeToFileW = reinterpret_cast<HRESULT  (WINAPI*)(  LPCWSTR    pDestFile,   D3DXIMAGE_FILEFORMAT      DestFormat,   LPDIRECT3DVOLUME9   pSrcVolume,   CONST PALETTEENTRY*       pSrcPalette,   CONST D3DBOX*    pSrcBox)>(GetProcAddress(d3d9x_module, "D3DXSaveVolumeToFileW"));
	original_D3DXSimplifyMesh = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXMESH pMesh,   CONST DWORD* pAdjacency,   CONST D3DXATTRIBUTEWEIGHTS *pVertexAttributeWeights,   CONST FLOAT *pVertexWeights,   DWORD MinValue,   DWORD Options,   LPD3DXMESH* ppMesh)>(GetProcAddress(d3d9x_module, "D3DXSimplifyMesh"));
	original_D3DXSphereBoundProbe = reinterpret_cast<BOOL  (WINAPI*)(  CONST D3DXVECTOR3 *pCenter,   FLOAT Radius,   CONST D3DXVECTOR3 *pRayPosition,   CONST D3DXVECTOR3 *pRayDirection)>(GetProcAddress(d3d9x_module, "D3DXSphereBoundProbe"));
	original_D3DXSplitMesh = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXMESH pMeshIn,   CONST DWORD *pAdjacencyIn,   CONST DWORD MaxSize,   CONST DWORD Options,   DWORD *pMeshesOut,   LPD3DXBUFFER *ppMeshArrayOut,   LPD3DXBUFFER *ppAdjacencyArrayOut,   LPD3DXBUFFER *ppFaceRemapArrayOut,   LPD3DXBUFFER *ppVertRemapArrayOut)>(GetProcAddress(d3d9x_module, "D3DXSplitMesh"));
	original_D3DXTessellateNPatches = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXMESH pMeshIn,   CONST DWORD* pAdjacencyIn,   FLOAT NumSegs,   BOOL  QuadraticInterpNormals,   LPD3DXMESH *ppMeshOut,   LPD3DXBUFFER *ppAdjacencyOut)>(GetProcAddress(d3d9x_module, "D3DXTessellateNPatches"));
	original_D3DXTessellateRectPatch = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DVERTEXBUFFER9 pVB,   CONST FLOAT *pNumSegs,   CONST D3DVERTEXELEMENT9 *pdwInDecl,   CONST D3DRECTPATCH_INFO *pRectPatchInfo,   LPD3DXMESH pMesh)>(GetProcAddress(d3d9x_module, "D3DXTessellateRectPatch"));
	original_D3DXTessellateTriPatch = reinterpret_cast<HRESULT  (WINAPI*)(  LPDIRECT3DVERTEXBUFFER9 pVB,   CONST FLOAT *pNumSegs,   CONST D3DVERTEXELEMENT9 *pInDecl,   CONST D3DTRIPATCH_INFO *pTriPatchInfo,   LPD3DXMESH pMesh)>(GetProcAddress(d3d9x_module, "D3DXTessellateTriPatch"));
	original_D3DXTriPatchSize = reinterpret_cast<HRESULT  (WINAPI*)(  CONST FLOAT *pfNumSegs,   DWORD *pdwTriangles,   DWORD *pdwVertices)>(GetProcAddress(d3d9x_module, "D3DXTriPatchSize"));
	//original_D3DXUVAtlasCreate = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXMESH pMesh,   UINT uMaxChartNumber,   FLOAT fMaxStretch,   UINT uWidth,   UINT uHeight,   FLOAT fGutter,   DWORD dwTextureIndex,   CONST DWORD *pdwAdjacency,   CONST DWORD *pdwFalseEdgeAdjacency,   CONST FLOAT *pfIMTArray,   LPD3DXUVATLASCB pStatusCallback,   FLOAT fCallbackFrequency,   LPVOID pUserContext,   LPD3DXMESH *ppMeshOut,   LPD3DXBUFFER *ppFacePartitioning,   LPD3DXBUFFER *ppVertexRemapArray,   FLOAT *pfMaxStretchOut,   UINT *puNumChartsOut)>(GetProcAddress(d3d9x_module, "D3DXUVAtlasCreate"));
	//original_D3DXUVAtlasPack = reinterpret_cast<HRESULT  (WINAPI*)(  ID3DXMesh *pMesh, UINT uWidth,   UINT uHeight,   FLOAT fGutter,   DWORD dwTextureIndex,   CONST DWORD *pdwPartitionResultAdjacency,   LPD3DXUVATLASCB pStatusCallback,   FLOAT fCallbackFrequency,   LPVOID pUserContext,    LPD3DXBUFFER pFacePartitioning)>(GetProcAddress(d3d9x_module, "D3DXUVAtlasPack"));
	//original_D3DXUVAtlasPartition = reinterpret_cast<HRESULT  (WINAPI*)( LPD3DXMESH pMesh, UINT uMaxChartNumber, FLOAT fMaxStretch,  DWORD dwTextureIndex, 	CONST DWORD *pdwAdjacency, 	CONST DWORD *pdwFalseEdgeAdjacency, 	CONST FLOAT *pfIMTArray, 	LPD3DXUVATLASCB pStatusCallback, 	FLOAT fCallbackFrequency,	LPVOID pUserContext, LPD3DXMESH *ppMeshOut,  LPD3DXBUFFER *ppFacePartitioning,LPD3DXBUFFER *ppVertexRemapArray, LPD3DXBUFFER *ppPartitionResultAdjacency,  FLOAT *pfMaxStretchOut, UINT *puNumChartsOut)>(GetProcAddress(d3d9x_module, "D3DXUVAtlasPartition"));
	original_D3DXValidMesh = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXMESH pMeshIn,   CONST DWORD* pAdjacency,   LPD3DXBUFFER* ppErrorsAndWarnings)>(GetProcAddress(d3d9x_module, "D3DXValidMesh"));
	original_D3DXValidPatchMesh = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXPATCHMESH pMesh,   DWORD *dwcDegenerateVertices,   DWORD *dwcDegeneratePatches,   LPD3DXBUFFER *ppErrorsAndWarnings)>(GetProcAddress(d3d9x_module, "D3DXValidPatchMesh("));
	original_D3DXVec2BaryCentric = reinterpret_cast<D3DXVECTOR2*  (WINAPI*)(  D3DXVECTOR2 *pOut,   CONST D3DXVECTOR2 *pV1,   CONST D3DXVECTOR2 *pV2,   CONST D3DXVECTOR2 *pV3,   FLOAT f,   FLOAT g)>(GetProcAddress(d3d9x_module, "D3DXVec2BaryCentric"));
	original_D3DXVec2CatmullRom = reinterpret_cast<D3DXVECTOR2*  (WINAPI*)(  D3DXVECTOR2 *pOut,   CONST D3DXVECTOR2 *pV0,   CONST D3DXVECTOR2 *pV1,   CONST D3DXVECTOR2 *pV2,   CONST D3DXVECTOR2 *pV3,   FLOAT s )>(GetProcAddress(d3d9x_module, "D3DXVec2CatmullRom"));
	original_D3DXVec2Hermite = reinterpret_cast<D3DXVECTOR2*  (WINAPI*)(  D3DXVECTOR2 *pOut,   CONST D3DXVECTOR2 *pV1,   CONST D3DXVECTOR2 *pT1,   CONST D3DXVECTOR2 *pV2,   CONST D3DXVECTOR2 *pT2,   FLOAT s )>(GetProcAddress(d3d9x_module, "D3DXVec2Hermite"));
	original_D3DXVec2Normalize = reinterpret_cast<D3DXVECTOR2*  (WINAPI*)(  D3DXVECTOR2 *pOut,   CONST D3DXVECTOR2 *pV )>(GetProcAddress(d3d9x_module, "D3DXVec2Normalize"));
	original_D3DXVec2Transform = reinterpret_cast<D3DXVECTOR4*  (WINAPI*)(  D3DXVECTOR4 *pOut,   CONST D3DXVECTOR2 *pV,   CONST D3DXMATRIX *pM )>(GetProcAddress(d3d9x_module, "D3DXVec2Transform"));
	original_D3DXVec2TransformArray = reinterpret_cast<D3DXVECTOR4*  (WINAPI*)(  D3DXVECTOR4 *pOut,   UINT OutStride,   CONST D3DXVECTOR2 *pV,   UINT VStride,   CONST D3DXMATRIX *pM,   UINT n)>(GetProcAddress(d3d9x_module, "D3DXVec2TransformArray"));
	original_D3DXVec2TransformCoord = reinterpret_cast<D3DXVECTOR2*  (WINAPI*)(  D3DXVECTOR2 *pOut,   CONST D3DXVECTOR2 *pV,   CONST D3DXMATRIX *pM )>(GetProcAddress(d3d9x_module, "D3DXVec2TransformCoord"));
	original_D3DXVec2TransformCoordArray = reinterpret_cast<D3DXVECTOR2*  (WINAPI*)(  D3DXVECTOR2 *pOut,   UINT OutStride,   CONST D3DXVECTOR2 *pV,   UINT VStride,   CONST D3DXMATRIX *pM,   UINT n )>(GetProcAddress(d3d9x_module, "D3DXVec2TransformCoordArray"));
	original_D3DXVec2TransformNormal = reinterpret_cast<D3DXVECTOR2*  (WINAPI*)(  D3DXVECTOR2 *pOut,   CONST D3DXVECTOR2 *pV,   CONST D3DXMATRIX *pM )>(GetProcAddress(d3d9x_module, "D3DXVec2TransformNormal"));
	original_D3DXVec2TransformNormalArray = reinterpret_cast<D3DXVECTOR2*  (WINAPI*)(  D3DXVECTOR2 *pOut,   UINT OutStride,   CONST D3DXVECTOR2 *pV,   UINT VStride,   CONST D3DXMATRIX *pM,   UINT n )>(GetProcAddress(d3d9x_module, "D3DXVec2TransformNormalArray"));
	original_D3DXVec3BaryCentric = reinterpret_cast<D3DXVECTOR3*  (WINAPI*)(  D3DXVECTOR3 *pOut,   CONST D3DXVECTOR3 *pV1,   CONST D3DXVECTOR3 *pV2,   CONST D3DXVECTOR3 *pV3,   FLOAT f,   FLOAT g)>(GetProcAddress(d3d9x_module, "D3DXVec3BaryCentric"));
	original_D3DXVec3CatmullRom = reinterpret_cast<D3DXVECTOR3*  (WINAPI*)(  D3DXVECTOR3 *pOut,   CONST D3DXVECTOR3 *pV0,   CONST D3DXVECTOR3 *pV1,   CONST D3DXVECTOR3 *pV2,   CONST D3DXVECTOR3 *pV3,   FLOAT s )>(GetProcAddress(d3d9x_module, "D3DXVec3CatmullRom"));
	original_D3DXVec3Hermite = reinterpret_cast<D3DXVECTOR3*  (WINAPI*)(  D3DXVECTOR3 *pOut,   CONST D3DXVECTOR3 *pV1,   CONST D3DXVECTOR3 *pT1,   CONST D3DXVECTOR3 *pV2,   CONST D3DXVECTOR3 *pT2,   FLOAT s )>(GetProcAddress(d3d9x_module, "D3DXVec3Hermite"));
	original_D3DXVec3Normalize = reinterpret_cast<D3DXVECTOR3*  (WINAPI*)(  D3DXVECTOR3 *pOut,   CONST D3DXVECTOR3 *pV )>(GetProcAddress(d3d9x_module, "D3DXVec3Normalize"));
	original_D3DXVec3Project = reinterpret_cast<D3DXVECTOR3*  (WINAPI*)(  D3DXVECTOR3 *pOut,   CONST D3DXVECTOR3 *pV,   CONST D3DVIEWPORT9 *pViewport,   CONST D3DXMATRIX *pProjection,   CONST D3DXMATRIX *pView,   CONST D3DXMATRIX *pWorld)>(GetProcAddress(d3d9x_module, "D3DXVec3Project"));
	original_D3DXVec3ProjectArray = reinterpret_cast<D3DXVECTOR3*  (WINAPI*)(  D3DXVECTOR3 *pOut,   UINT OutStride,   CONST D3DXVECTOR3 *pV, UINT VStride,   CONST D3DVIEWPORT9 *pViewport,   CONST D3DXMATRIX *pProjection,   CONST D3DXMATRIX *pView,   CONST D3DXMATRIX *pWorld,   UINT n)>(GetProcAddress(d3d9x_module, "D3DXVec3ProjectArray"));
	original_D3DXVec3Transform = reinterpret_cast<D3DXVECTOR4*  (WINAPI*)(  D3DXVECTOR4 *pOut,   CONST D3DXVECTOR3 *pV,   CONST D3DXMATRIX *pM )>(GetProcAddress(d3d9x_module, "D3DXVec3Transform"));
	original_D3DXVec3TransformArray = reinterpret_cast<D3DXVECTOR4*  (WINAPI*)(  D3DXVECTOR4 *pOut,   UINT OutStride,   CONST D3DXVECTOR3 *pV,   UINT VStride,   CONST D3DXMATRIX *pM,   UINT n )>(GetProcAddress(d3d9x_module, "D3DXVec3TransformArray"));
	original_D3DXVec3TransformCoord = reinterpret_cast<D3DXVECTOR3*  (WINAPI*)(  D3DXVECTOR3 *pOut,   CONST D3DXVECTOR3 *pV,   CONST D3DXMATRIX *pM )>(GetProcAddress(d3d9x_module, "D3DXVec3TransformCoord"));
	original_D3DXVec3TransformCoordArray = reinterpret_cast<D3DXVECTOR3*  (WINAPI*)(  D3DXVECTOR3 *pOut,   UINT OutStride,   CONST D3DXVECTOR3 *pV,   UINT VStride,   CONST D3DXMATRIX *pM,   UINT n )>(GetProcAddress(d3d9x_module, "D3DXVec3TransformCoordArray"));
	original_D3DXVec3TransformNormal = reinterpret_cast<D3DXVECTOR3*  (WINAPI*)(  D3DXVECTOR3 *pOut,   CONST D3DXVECTOR3 *pV,   CONST D3DXMATRIX *pM )>(GetProcAddress(d3d9x_module, "D3DXVec3TransformNormal"));
	original_D3DXVec3TransformNormalArray = reinterpret_cast<D3DXVECTOR3*  (WINAPI*)(  D3DXVECTOR3 *pOut,   UINT OutStride,   CONST D3DXVECTOR3 *pV,   UINT VStride,   CONST D3DXMATRIX *pM,   UINT n )>(GetProcAddress(d3d9x_module, "D3DXVec3TransformNormalArray"));
	original_D3DXVec3Unproject = reinterpret_cast<D3DXVECTOR3*  (WINAPI*)(  D3DXVECTOR3 *pOut,   CONST D3DXVECTOR3 *pV,   CONST D3DVIEWPORT9 *pViewport,   CONST D3DXMATRIX *pProjection,   CONST D3DXMATRIX *pView,   CONST D3DXMATRIX *pWorld)>(GetProcAddress(d3d9x_module, "D3DXVec3Unproject"));
	original_D3DXVec3UnprojectArray = reinterpret_cast<D3DXVECTOR3*  (WINAPI*)(  D3DXVECTOR3 *pOut,   UINT OutStride,   CONST D3DXVECTOR3 *pV,   UINT VStride,   CONST D3DVIEWPORT9 *pViewport,   CONST D3DXMATRIX *pProjection,   CONST D3DXMATRIX *pView,   CONST D3DXMATRIX *pWorld,   UINT n)>(GetProcAddress(d3d9x_module, "D3DXVec3UnprojectArray"));
	original_D3DXVec4BaryCentric = reinterpret_cast<D3DXVECTOR4*  (WINAPI*)(  D3DXVECTOR4 *pOut,   CONST D3DXVECTOR4 *pV1,   CONST D3DXVECTOR4 *pV2,   CONST  D3DXVECTOR4 *pV3,   FLOAT f,   FLOAT g)>(GetProcAddress(d3d9x_module, "D3DXVec4BaryCentric"));
	original_D3DXVec4CatmullRom = reinterpret_cast<D3DXVECTOR4*  (WINAPI*)(  D3DXVECTOR4 *pOut,   CONST D3DXVECTOR4 *pV0,   CONST D3DXVECTOR4 *pV1,   CONST D3DXVECTOR4 *pV2,   CONST D3DXVECTOR4 *pV3,   FLOAT s )>(GetProcAddress(d3d9x_module, "D3DXVec4CatmullRom"));
	original_D3DXVec4Cross = reinterpret_cast<D3DXVECTOR4*  (WINAPI*)(  D3DXVECTOR4 *pOut,   CONST D3DXVECTOR4 *pV1,   CONST D3DXVECTOR4 *pV2,   CONST D3DXVECTOR4 *pV3)>(GetProcAddress(d3d9x_module, "D3DXVec4Cross"));
	original_D3DXVec4Hermite = reinterpret_cast<D3DXVECTOR4*  (WINAPI*)(  D3DXVECTOR4 *pOut,   CONST D3DXVECTOR4 *pV1,   CONST D3DXVECTOR4 *pT1,   CONST D3DXVECTOR4 *pV2,   CONST D3DXVECTOR4 *pT2,   FLOAT s )>(GetProcAddress(d3d9x_module, "D3DXVec4Hermite"));
	original_D3DXVec4Normalize = reinterpret_cast<D3DXVECTOR4*  (WINAPI*)(  D3DXVECTOR4 *pOut,   CONST D3DXVECTOR4 *pV)>(GetProcAddress(d3d9x_module, "D3DXVec4Normalize"));
	original_D3DXVec4Transform = reinterpret_cast<D3DXVECTOR4*  (WINAPI*)(  D3DXVECTOR4 *pOut,   CONST D3DXVECTOR4 *pV,   CONST D3DXMATRIX *pM )>(GetProcAddress(d3d9x_module, "D3DXVec4Transform"));
	original_D3DXVec4TransformArray = reinterpret_cast<D3DXVECTOR4*  (WINAPI*)(  D3DXVECTOR4 *pOut,   UINT OutStride,   CONST D3DXVECTOR4 *pV,   UINT VStride,   CONST D3DXMATRIX *pM,   UINT n )>(GetProcAddress(d3d9x_module, "D3DXVec4TransformArray"));
	original_D3DXWeldVertices = reinterpret_cast<HRESULT  (WINAPI*)(  LPD3DXMESH pMesh,   DWORD Flags,   CONST D3DXWELDEPSILONS *pEpsilons,   CONST DWORD *pAdjacencyIn,   DWORD *pAdjacencyOut,   DWORD *pFaceRemap,   LPD3DXBUFFER *ppVertexRemap)>(GetProcAddress(d3d9x_module, "D3DXWeldVertices"));

	return TRUE;
}

void dispose()
{
}

// DLLエントリポイント
BOOL APIENTRY DllMain(HINSTANCE, DWORD reason, LPVOID)
{
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		init();
	case DLL_PROCESS_DETACH:
		dispose();
	}
	return TRUE;
}
