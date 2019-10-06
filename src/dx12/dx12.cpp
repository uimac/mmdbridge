// DirectX12
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_5.h>
#include <Windows.h>

#include <vector>

#include "d3dx12.h"
#include "DXSampleHelper.h"
#include "dx12.h"

#include "UMMathTypes.h"
#include "UMMath.h"

namespace dx12
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	const uint32_t BackBufferCount = 2;
	const uint32_t CBVSRVUAVCount = 32768;

	struct ConstantBuffer
	{

	};

	void GetHardwareAdapter(IDXGIFactory2* factory, IDXGIAdapter1** dst_adapter, std::wstring& dst_adapter_desc)
	{
		ComPtr<IDXGIAdapter1> adapter;
		*dst_adapter = nullptr;

		for (uint32_t adapter_index = 0;
			DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapter_index, &adapter);
			++adapter_index)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				// If you want a software adapter, pass in "/warp" on the command line.
				continue;
			}

			// Check to see if the adapter supports Direct3D 12, but don't create the
			// actual device yet.
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				dst_adapter_desc = desc.Description;
				break;
			}
		}
		*dst_adapter = adapter.Detach();
	}

	ComPtr<ID3D12Resource> CreateTextureResource(
		ComPtr<ID3D12Device5> device,
		uint32_t width, uint32_t height,
		DXGI_FORMAT format,
		D3D12_RESOURCE_FLAGS flag,
		D3D12_RESOURCE_STATES resource_state,
		D3D12_CLEAR_VALUE* clear_value)
	{
		D3D12_HEAP_PROPERTIES  heap_prop;
		heap_prop.Type = D3D12_HEAP_TYPE_DEFAULT;
		heap_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heap_prop.CreationNodeMask = 1;
		heap_prop.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC  resource_desc;
		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resource_desc.Alignment = 0;
		resource_desc.Width = width;
		resource_desc.Height = height;
		resource_desc.DepthOrArraySize = 1;
		resource_desc.MipLevels = 0;
		resource_desc.Format = format;
		resource_desc.SampleDesc.Count = 1;
		resource_desc.SampleDesc.Quality = 0;
		resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resource_desc.Flags = flag;

		ComPtr<ID3D12Resource> tex;
		device->CreateCommittedResource(
			&heap_prop, D3D12_HEAP_FLAG_NONE,
			&resource_desc, resource_state,
			clear_value, IID_PPV_ARGS(&tex));

		return tex;
	}

	ComPtr<ID3D12Resource> CreateConstantResource(
		ComPtr<ID3D12Device5> device,
		uint32_t byte_size)
	{
		D3D12_HEAP_PROPERTIES  heap_prop;
		heap_prop.Type = D3D12_HEAP_TYPE_DEFAULT;
		heap_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heap_prop.CreationNodeMask = 1;
		heap_prop.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC  resource_desc;
		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resource_desc.Alignment = 0;
		resource_desc.Width = byte_size;
		resource_desc.Height = 1;
		resource_desc.DepthOrArraySize = 1;
		resource_desc.MipLevels = 0;
		resource_desc.Format = DXGI_FORMAT_UNKNOWN;
		resource_desc.SampleDesc.Count = 1;
		resource_desc.SampleDesc.Quality = 0;
		resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		ComPtr<ID3D12Resource> tex;
		device->CreateCommittedResource(
			&heap_prop, D3D12_HEAP_FLAG_NONE,
			&resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&tex));

		return tex;
	}


	class DX12::Impl
	{
	public:
		Impl() {}
		~Impl() {}

		void Init();

	private:
		uint32_t width_ = 800;
		uint32_t height_ = 600;
		umbase::UMVec4f clear_color_ = { 0.3, 0.3, 0.3, 1.0 };

		ComPtr<ID3D12Device5> device_;
		ComPtr<IDXGIFactory5> factory_;
		ComPtr<ID3D12GraphicsCommandList4> command_list_;
		ComPtr<ID3D12CommandQueue> command_queue_;
		ComPtr<ID3D12CommandAllocator> command_allocator_;
		ComPtr<ID3D12CommandSignature> command_signature_;
		ComPtr<ID3D12Fence> fence_;
		// ComPtr<IDXGISwapChain3> swap_chain_;
		ComPtr<ID3D12Debug> debug_controller_;

		// backbuffer resources
		std::vector< ComPtr<ID3D12Resource> > render_target_resources_;
		std::vector< ComPtr<ID3D12Resource> > depth_stencil_resources_;

		// backbuffer heaps
		ComPtr<ID3D12DescriptorHeap> render_target_heap_;
		std::vector< D3D12_CPU_DESCRIPTOR_HANDLE > render_target_cpu_handles_;
		std::vector< D3D12_GPU_DESCRIPTOR_HANDLE > render_target_gpu_handles_;

		ComPtr<ID3D12DescriptorHeap> depth_stencil_heap_;
		std::vector< D3D12_CPU_DESCRIPTOR_HANDLE > depth_stencil_cpu_handles_;
		std::vector< D3D12_GPU_DESCRIPTOR_HANDLE > depth_stencil_gpu_handles_;

		// constant resource
		ComPtr < ID3D12Resource > constant_resource_;

		// cbv srv uav heaps
		ComPtr<ID3D12DescriptorHeap> cbv_srv_uav_heap_;
		std::vector< D3D12_CPU_DESCRIPTOR_HANDLE > cbv_srv_uav_cpu_handles_;
		std::vector< D3D12_GPU_DESCRIPTOR_HANDLE > cbv_srv_uav_gpu_handles_;


		std::vector<uint64_t> fence_value_list_ = { 0, 0 };
		HANDLE fence_event_ = nullptr;
		uint32_t current_back_buffer_ = 0;
		std::vector<D3D12_RESOURCE_BARRIER> begin_barrier_list;
		std::vector<D3D12_RESOURCE_BARRIER> end_barrier_list;

		void InitDescriptorHeaps();
		void InitBackBuffers();
		void WaitForGPU();
		void ResetCommands();
		void ExecuteCommands();
		
	};

	void DX12::Impl::InitDescriptorHeaps()
	{
		// Descriptor Heaps for Back Buffers 
		render_target_cpu_handles_.resize(BackBufferCount);
		render_target_gpu_handles_.resize(BackBufferCount);
		{
			D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
			heap_desc.NumDescriptors = BackBufferCount;
			heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			ThrowIfFailed(device_->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&render_target_heap_)));
			const uint32_t increment_size = device_->GetDescriptorHandleIncrementSize(heap_desc.Type);
			for (uint32_t i = 0; i < BackBufferCount; ++i)
			{
				CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(render_target_heap_->GetCPUDescriptorHandleForHeapStart());
				CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle(render_target_heap_->GetGPUDescriptorHandleForHeapStart());
				render_target_cpu_handles_[i] = cpu_handle.Offset(i, increment_size);
				render_target_gpu_handles_[i] = gpu_handle.Offset(i, increment_size);
			}
		}

		depth_stencil_cpu_handles_.resize(BackBufferCount);
		depth_stencil_gpu_handles_.resize(BackBufferCount);
		{
			D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
			heap_desc.NumDescriptors = BackBufferCount;
			heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			ThrowIfFailed(device_->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&depth_stencil_heap_)));
			const uint32_t increment_size = device_->GetDescriptorHandleIncrementSize(heap_desc.Type);
			for (uint32_t i = 0; i < BackBufferCount; ++i)
			{
				CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(depth_stencil_heap_->GetCPUDescriptorHandleForHeapStart());
				CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle(depth_stencil_heap_->GetGPUDescriptorHandleForHeapStart());
				depth_stencil_cpu_handles_[i] = cpu_handle.Offset(i, increment_size);
				depth_stencil_gpu_handles_[i] = gpu_handle.Offset(i, increment_size);
			}
		}

		// Descriptor Heaps for cbv srv uav
		cbv_srv_uav_cpu_handles_.resize(CBVSRVUAVCount);
		cbv_srv_uav_gpu_handles_.resize(CBVSRVUAVCount);
		{
			D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
			heap_desc.NumDescriptors = CBVSRVUAVCount;
			heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			ThrowIfFailed(device_->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&cbv_srv_uav_heap_)));
			const uint32_t increment_size = device_->GetDescriptorHandleIncrementSize(heap_desc.Type);
			for (uint32_t i = 0; i < CBVSRVUAVCount; ++i)
			{
				CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(cbv_srv_uav_heap_->GetCPUDescriptorHandleForHeapStart());
				CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle(cbv_srv_uav_heap_->GetGPUDescriptorHandleForHeapStart());
				cbv_srv_uav_cpu_handles_[i] = cpu_handle.Offset(i, increment_size);
				cbv_srv_uav_gpu_handles_[i] = gpu_handle.Offset(i, increment_size);
			}
		}
	}

	void DX12::Impl::InitBackBuffers()
	{
		D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
		D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
		rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtv_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
		for (uint32_t i = 0; i < BackBufferCount; ++i)
		{
			D3D12_CLEAR_VALUE rt_clear_value;
			rt_clear_value.Format = rtv_desc.Format;
			rt_clear_value.Color[0] = clear_color_[0];
			rt_clear_value.Color[1] = clear_color_[1];
			rt_clear_value.Color[2] = clear_color_[2];
			rt_clear_value.Color[3] = clear_color_[3];
			render_target_resources_[i] = CreateTextureResource(
					device_, width_, height_, rtv_desc.Format,
					D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
					D3D12_RESOURCE_STATE_COMMON, &rt_clear_value);

			D3D12_CLEAR_VALUE ds_clear_value;
			ds_clear_value.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
			ds_clear_value.DepthStencil.Depth = 1.0f;
			ds_clear_value.DepthStencil.Stencil = 0;

			depth_stencil_resources_[i] = CreateTextureResource(
					device_, width_, height_, dsv_desc.Format,
					D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL,
					D3D12_RESOURCE_STATE_DEPTH_WRITE, &ds_clear_value);

			// Back Buffer Views
			device_->CreateRenderTargetView(
				render_target_resources_.back().Get(), &rtv_desc, render_target_cpu_handles_[i]);
			device_->CreateDepthStencilView(
				depth_stencil_resources_.back().Get(), &dsv_desc, depth_stencil_cpu_handles_[i]);

		}
	}

	void DX12::Impl::Init()
	{
		begin_barrier_list.resize(BackBufferCount);
		end_barrier_list.resize(BackBufferCount);
		render_target_resources_.resize(BackBufferCount);
		depth_stencil_resources_.resize(BackBufferCount);

		// Debug Layer
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller_)))) {
			debug_controller_->EnableDebugLayer();
		}

		// Factory
		ComPtr<IDXGIFactory4> old_factory;
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&old_factory)));
		ThrowIfFailed(old_factory.As(&factory_));

		// Adapter
		ComPtr<IDXGIAdapter1> hardware_adapter;
		std::wstring adapter_description;
		GetHardwareAdapter(factory_.Get(), &hardware_adapter, adapter_description);

		// Device
		ThrowIfFailed(D3D12CreateDevice(hardware_adapter.Get(), 
			D3D_FEATURE_LEVEL_11_0,IID_PPV_ARGS(&device_)));

		// Command Allocator
		ThrowIfFailed(device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator_)));

		// Command Queue
		D3D12_COMMAND_QUEUE_DESC queue_desc = {};
		ThrowIfFailed(device_->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&command_queue_)));

		// Command List
		ThrowIfFailed(device_->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			command_allocator_.Get(),
			nullptr,
			IID_PPV_ARGS(&command_list_)));

		// Descriptor Heaps
		InitDescriptorHeaps();

		// Back Buffer (rtv, dsv) resources, views
		InitBackBuffers();

		// CBV resource, view
		uint32_t cbv_srv_uav_index = 0;
		{
			const uint32_t constant_buffer_size_ = sizeof(ConstantBuffer);
			Align(constant_buffer_size_, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
			constant_resource_ = CreateConstantResource(device_, constant_buffer_size_);

			D3D12_CONSTANT_BUFFER_VIEW_DESC view = {};
			view.SizeInBytes = constant_buffer_size_;
			view.BufferLocation = constant_resource_->GetGPUVirtualAddress();
			device_->CreateConstantBufferView(&view, cbv_srv_uav_cpu_handles_[cbv_srv_uav_index++]);
		}
		// 

		// Fence
		ThrowIfFailed(device_->CreateFence(
			fence_value_list_[current_back_buffer_], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)));

		fence_value_list_[current_back_buffer_]++;
		fence_event_ = CreateEvent(nullptr, false, false, nullptr);

		for (uint32_t i = 0; i < BackBufferCount; ++i)
		{
			fence_value_list_[i] = fence_value_list_[current_back_buffer_];
		}

		ExecuteCommands();
		WaitForGPU();
	}

	void DX12::Impl::WaitForGPU()
	{
		uint64_t fence_value = fence_value_list_[current_back_buffer_];

		if (SUCCEEDED(command_queue_->Signal(fence_.Get(), fence_value)))
		{
			if (SUCCEEDED(fence_->SetEventOnCompletion(fence_value, fence_event_)))
			{
				WaitForSingleObjectEx(fence_event_, INFINITE, FALSE);
				fence_value_list_[current_back_buffer_]++;
			}
		}

	}

	void DX12::Impl::ResetCommands()
	{
		command_allocator_->Reset();
		command_list_->Reset(command_allocator_.Get(), 0);
	}

	void DX12::Impl::ExecuteCommands()
	{
		command_list_->Close();
		ID3D12CommandList *ppLists[] = { command_list_.Get() };
		command_queue_->ExecuteCommandLists(1, ppLists);
	}

	//----------------------------------------------------------------

	DX12::DX12() : impl_(std::make_unique<DX12::Impl>()) {}

	DX12::~DX12() {}

	void DX12::Init() { impl_->Init(); }

} // namespace dx12
