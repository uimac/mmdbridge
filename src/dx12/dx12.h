#pragma once

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include <memory>

namespace dx12
{
	class DLL_API DX12
	{
	public:
		DX12();
		~DX12();

		void Init();

	private:
		class Impl;
		std::unique_ptr<Impl> impl_;
	};
} // namespace dx12
