#pragma once

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include <memory>

#pragma warning(disable : 4819)
#include <d3d9.h>
#include <d3d9types.h>
#pragma warning(default : 4819)

namespace dx12
{
	class DLL_API DX12
	{
	public:
		DX12();
		~DX12();

		void Init();

		void ShareVertexBuffer(IDirect3DVertexBuffer9* v);

		IUnknown* GetDevice();

		IUnknown* GetCommandQueue();

		void OutputImage();

	private:
		class Impl;
		std::unique_ptr<Impl> impl_;
	};
} // namespace dx12
