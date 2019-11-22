// DirectX12
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_5.h>
#include <Windows.h>

#include <vector>
#include <map>
#include <codecvt>
#include <string>
#include <cassert>
#include <locale>
#include <iostream>
#include <D3Dcompiler.h>

#include "d3dx12.h"
#include "DXSampleHelper.h"
#include "dx12.h"

#include "UMMathTypes.h"
#include "UMMath.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

namespace dx12
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	const uint32_t BackBufferCount = 2;
	const uint32_t RenderTargetCount = 0;
	const uint32_t CBVSRVUAVCount = 32768;
	const DXGI_FORMAT RenderTargetFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
	const DXGI_FORMAT BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	struct Constants
	{
		UMMat44f wvp;
	};

	struct VertexElement
	{
		umbase::UMVec3f v;
		umbase::UMVec3f n;
		umbase::UMVec2f uv;
	};

	struct Mesh
	{
		std::vector<uint32_t> indices;
		std::vector<VertexElement> vertices;

		D3D12_CPU_DESCRIPTOR_HANDLE index_cpu_handle;
		D3D12_GPU_DESCRIPTOR_HANDLE index_gpu_handle;
		D3D12_CPU_DESCRIPTOR_HANDLE vertex_cpu_handle;
		D3D12_GPU_DESCRIPTOR_HANDLE vertex_gpu_handle;
		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
		D3D12_INDEX_BUFFER_VIEW index_buffer_view;
		ComPtr<ID3D12Resource> index_buffer_resource;
		ComPtr<ID3D12Resource> vertex_buffer_resource;
	};

	struct Image
	{
		std::vector<uint8_t> data;
		uint32_t width;
		uint32_t height;
		uint32_t bpp;
		uint32_t channels;
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

	ComPtr<ID3D12Resource> CreateReadBackResource(
		ComPtr<ID3D12Device5> device,
		uint32_t byte_size,
		D3D12_RESOURCE_STATES resource_state)
	{
		D3D12_HEAP_PROPERTIES  heap_prop;
		heap_prop.Type = D3D12_HEAP_TYPE_READBACK;
		heap_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heap_prop.CreationNodeMask = 1;
		heap_prop.VisibleNodeMask = 1;

		ComPtr<ID3D12Resource> resource;
		device->CreateCommittedResource(
			&heap_prop, D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(byte_size), resource_state,
			nullptr, IID_PPV_ARGS(&resource));

		return resource;
	}

	ComPtr<ID3D12Resource> CreateUploadResource(
		ComPtr<ID3D12Device5> device,
		uint32_t byte_size)
	{
		D3D12_HEAP_PROPERTIES  heap_prop;
		heap_prop.Type = D3D12_HEAP_TYPE_UPLOAD;
		heap_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heap_prop.CreationNodeMask = 1;
		heap_prop.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC  resource_desc;
		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resource_desc.Alignment = 0;
		resource_desc.Width = byte_size;
		resource_desc.Height = 1;
		resource_desc.DepthOrArraySize = 1;
		resource_desc.MipLevels = 1;
		resource_desc.SampleDesc.Count = 1;
		resource_desc.SampleDesc.Quality = 0;
		resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
		resource_desc.Format = DXGI_FORMAT_UNKNOWN;

		ComPtr<ID3D12Resource> resource;
		device->CreateCommittedResource(
			&heap_prop, D3D12_HEAP_FLAG_NONE,
			&resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&resource));

		return resource;
	}


	class DX12::Impl
	{
	public:
		Impl() {}
		~Impl() {}

		void Init();

		void ShareVertexBuffer(IDirect3DVertexBuffer9* v);

		IUnknown* GetDevice() { return device_.Get(); }
		IUnknown* GetCommandQueue() { return command_queue_.Get(); }
		
		void OutputImage();
	private:
		uint32_t width_ = 800;
		uint32_t height_ = 600;
		umbase::UMVec4f clear_color_ = { 0.3, 0.3, 0.3, 1.0 };
		D3D12_CLEAR_VALUE clear_value_;

		ComPtr<IDXGIAdapter1> hardware_adapter_;
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
		Constants constants_;
		uint32_t constant_handle_index_ = 0;
		ComPtr< ID3D12Resource > constant_resource_;

		// uav resources
		std::vector< ComPtr<ID3D12Resource> > uav_resources_;

		// cbv srv uav heaps
		ComPtr<ID3D12DescriptorHeap> cbv_srv_uav_heap_;
		uint32_t cbv_srv_uav_index_ = 0;
		std::vector< D3D12_CPU_DESCRIPTOR_HANDLE > cbv_srv_uav_cpu_handles_;
		std::vector< D3D12_GPU_DESCRIPTOR_HANDLE > cbv_srv_uav_gpu_handles_;


		std::vector<uint64_t> fence_value_list_ = { 0, 0 };
		HANDLE fence_event_ = nullptr;
		uint32_t current_back_buffer_ = 0;
		std::vector<D3D12_RESOURCE_BARRIER> begin_barrier_list;
		std::vector<D3D12_RESOURCE_BARRIER> end_barrier_list;

		// root signature
		ComPtr<ID3D12RootSignature> rasterize_root_signature_;
		std::map<uint32_t, ComPtr<ID3D12PipelineState> >  pipeline_state_;

		// shader
		using ShaderBlobEntry = std::tuple<std::string, std::string, ComPtr<ID3DBlob> >;
		std::vector<ShaderBlobEntry> shader_blobs_ = {
			{ "vsmain", "vs_5_1", ComPtr<ID3DBlob>() },
			{ "psmain", "ps_5_1", ComPtr<ID3DBlob>() },
		};

		// plane
		Mesh plane_;

		void InitDescriptorHeaps();
		void InitBackBuffers();
		void InitConstants();
		void WaitForGPU();
		void ResetCommands();
		void ExecuteCommands();
		void InitPlane();
		void LoadVertexBuffer();
		void LoadMesh(Mesh& mesh);
		uint32_t LoadShader(const std::string& file_path);
		void CreatePipeline(uint32_t);
		void Resize(uint32_t width, uint32_t height);
		void Update();
		void Render();
		void GoToNextFrame();
		void GetRenderImage(Image& image);
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
			heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
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

	void DX12::Impl::InitConstants()
	{
		constants_.wvp = umbase::UMMat44f();
	}

	void DX12::Impl::InitBackBuffers()
	{
		D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
		D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
		rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;

		clear_value_.Format = rtv_desc.Format;
		clear_value_.Color[0] = clear_color_[0];
		clear_value_.Color[1] = clear_color_[1];
		clear_value_.Color[2] = clear_color_[2];
		clear_value_.Color[3] = clear_color_[3];
		for (uint32_t i = 0; i < BackBufferCount; ++i)
		{
			render_target_resources_[i] = CreateTextureResource(
				device_, width_, height_, rtv_desc.Format,
				D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
				D3D12_RESOURCE_STATE_COMMON, &clear_value_);

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
				render_target_resources_[i].Get(), &rtv_desc, render_target_cpu_handles_[i]);
			device_->CreateDepthStencilView(
				depth_stencil_resources_[i].Get(), &dsv_desc, depth_stencil_cpu_handles_[i]);

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
		std::wstring adapter_description;
		GetHardwareAdapter(factory_.Get(), &hardware_adapter_, adapter_description);

		// Device
		ThrowIfFailed(D3D12CreateDevice(hardware_adapter_.Get(),
			D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_)));

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
		{
			uint32_t constant_buffer_size_ = sizeof(Constants);
			constant_buffer_size_ = Align(constant_buffer_size_, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
			constant_resource_ = CreateUploadResource(device_, constant_buffer_size_);

			D3D12_CONSTANT_BUFFER_VIEW_DESC view = {};
			view.SizeInBytes = constant_buffer_size_;
			view.BufferLocation = constant_resource_->GetGPUVirtualAddress();
			constant_handle_index_ = cbv_srv_uav_index_++;
			device_->CreateConstantBufferView(&view, cbv_srv_uav_cpu_handles_[constant_handle_index_]);
		}
		// UAV resouce, view
		{
			uav_resources_.resize(3);

			// readback buffer
			{
				ComPtr<ID3D12Resource> resource = render_target_resources_[0];
				D3D12_RESOURCE_DESC desc = resource->GetDesc();
				D3D12_PLACED_SUBRESOURCE_FOOTPRINT foot_print;
				uint32_t nrow;
				uint64_t rowsize;
				uint64_t size;
				device_->GetCopyableFootprints(&desc, 0, 1, 0, &foot_print, &nrow, &rowsize, &size);


				const uint32_t index = 0;
				uav_resources_[index] = CreateReadBackResource(
					device_, size, 
					D3D12_RESOURCE_STATE_COPY_DEST);

				/*
				D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
				uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
				device_->CreateUnorderedAccessView(
					uav_resources_[index].Get(), nullptr, &uav_desc, cbv_srv_uav_cpu_handles_[cbv_srv_uav_index_++]);
					*/
			}

			// raytracing result buffer
			{
				const uint32_t index = 1;
				uav_resources_[index] = CreateTextureResource(
					device_, width_, height_, DXGI_FORMAT_R8G8B8A8_UNORM,
					D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
					D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
					nullptr);

				D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
				uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
				device_->CreateUnorderedAccessView(
					uav_resources_[index].Get(), nullptr, &uav_desc, cbv_srv_uav_cpu_handles_[cbv_srv_uav_index_++]);
			}

			// raytracing accumulation buffer
			{
				const uint32_t index = 2;
				uav_resources_[index] = CreateTextureResource(
					device_, width_, height_, DXGI_FORMAT_R32G32B32A32_FLOAT,
					D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
					D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
					nullptr);

				D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
				uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
				device_->CreateUnorderedAccessView(
					uav_resources_[index].Get(), nullptr, &uav_desc, cbv_srv_uav_cpu_handles_[cbv_srv_uav_index_++]);
			}
		}

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

		InitConstants();
		InitPlane();

		LoadShader("shader/rasterize.hlsl");

		/*
		Update();
		Render();

		GoToNextFrame();
		Update();
		Render();
		*/
	}

	void DX12::Impl::OutputImage()
	{
		Image image;
		image.width = width_;
		image.height = height_;
		image.channels = 4;
		image.bpp = image.channels * 8;
		const size_t image_data_size = image.width * image.height * image.channels;
		image.data.resize(image_data_size);

		GetRenderImage(image);
		stbi_write_png("out.png",
			image.width,
			image.height, STBI_rgb_alpha, &image.data[0], image.width * 4);
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

	void DX12::Impl::GoToNextFrame()
	{
		const uint64_t fence_value = fence_value_list_[current_back_buffer_];
		ThrowIfFailed(command_queue_->Signal(fence_.Get(), fence_value));

		// wait for renderable
		if (fence_->GetCompletedValue() < fence_value)
		{
			ThrowIfFailed(fence_->SetEventOnCompletion(fence_value, fence_event_));
			WaitForSingleObjectEx(fence_event_, INFINITE, FALSE);
		}


		current_back_buffer_ = (current_back_buffer_ + 1) % 2;

		// next frame fence value
		fence_value_list_[current_back_buffer_] = fence_value + 1;
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

	void DX12::Impl::InitPlane()
	{
		plane_.indices = {
			0, 1, 2, 3, 4, 5
		};
		float verts[] = {
			-0.5f, -0.5f, 0.5f,
			-0.5f,  0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f,  0.5f, 0.5f,
			0.5f,  0.5f, 0.5f,
		};
		float normals[] = {
			0.0, 0.0, -1.0,
			0.0, 0.0, -1.0,
			0.0, 0.0, -1.0,
			0.0, 0.0, -1.0,
			0.0, 0.0, -1.0,
			0.0, 0.0, -1.0,
		};
		float uvs[] = {
			0.0, 0.0,
			1.0, 0.0,
			0.0, 1.0,
			0.0, 1.0,
			1.0, 0.0,
			1.0, 1.0
		};

		plane_.vertices.resize(6);
		for (uint32_t i = 0; i < 6; ++i)
		{
			VertexElement& ve = plane_.vertices[i];
			ve.v = umbase::UMVec3f(
				verts[i * 3 + 0],
				verts[i * 3 + 1],
				verts[i * 3 + 2]
			);
			ve.n = umbase::UMVec3f(
				normals[i * 3 + 0],
				normals[i * 3 + 1],
				normals[i * 3 + 2]
			);
			ve.uv = umbase::UMVec2f(
				uvs[i * 2 + 0],
				uvs[i * 2 + 1]
			);
		}

		LoadMesh(plane_);
	}

	void DX12::Impl::LoadMesh(Mesh& mesh)
	{

		// create shader resource view for mesh
		{
			const uint32_t index_size = sizeof(uint32_t);
			const uint32_t full_size = index_size * mesh.indices.size();
			mesh.index_buffer_resource = CreateUploadResource(device_, full_size);

			// upload
			void *mapped_data;
			mesh.index_buffer_resource->Map(0, nullptr, &mapped_data);
			memcpy(mapped_data, &mesh.indices[0], full_size);
			mesh.index_buffer_resource->Unmap(0, nullptr);

			// srv
			D3D12_SHADER_RESOURCE_VIEW_DESC srv_dsc = {};
			srv_dsc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			srv_dsc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srv_dsc.Buffer.NumElements = mesh.indices.size();
			srv_dsc.Format = DXGI_FORMAT_R32_TYPELESS;
			srv_dsc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
			srv_dsc.Buffer.StructureByteStride = 0;

			const uint32_t index = cbv_srv_uav_index_++;
			mesh.index_cpu_handle = cbv_srv_uav_cpu_handles_[index];
			device_->CreateShaderResourceView(
				mesh.index_buffer_resource.Get(), &srv_dsc, mesh.index_cpu_handle);
			mesh.index_gpu_handle = cbv_srv_uav_gpu_handles_[index];

			// index buffer view
			mesh.index_buffer_view = {};
			mesh.index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
			mesh.index_buffer_view.SizeInBytes = full_size;
			mesh.index_buffer_view.BufferLocation = mesh.index_buffer_resource->GetGPUVirtualAddress();
		}

		// create shader resource view for mesh
		{
			const uint32_t vertex_element_size = sizeof(VertexElement);
			const uint32_t full_size = vertex_element_size * mesh.vertices.size();
			mesh.vertex_buffer_resource = CreateUploadResource(device_, full_size);

			// upload
			void *mapped_data;
			mesh.vertex_buffer_resource->Map(0, nullptr, &mapped_data);
			memcpy(mapped_data, &mesh.vertices[0], full_size);
			mesh.vertex_buffer_resource->Unmap(0, nullptr);

			// srv
			D3D12_SHADER_RESOURCE_VIEW_DESC srv_dsc = {};
			srv_dsc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			srv_dsc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srv_dsc.Buffer.NumElements = mesh.vertices.size();
			srv_dsc.Format = DXGI_FORMAT_UNKNOWN;
			srv_dsc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
			srv_dsc.Buffer.StructureByteStride = vertex_element_size;

			const uint32_t index = cbv_srv_uav_index_++;
			mesh.vertex_cpu_handle = cbv_srv_uav_cpu_handles_[index];
			device_->CreateShaderResourceView(
				mesh.vertex_buffer_resource.Get(), &srv_dsc, mesh.vertex_cpu_handle);
			mesh.vertex_gpu_handle = cbv_srv_uav_gpu_handles_[index];

			// vertex buffer view
			mesh.vertex_buffer_view = {};
			mesh.vertex_buffer_view.StrideInBytes = vertex_element_size;
			mesh.vertex_buffer_view.SizeInBytes = full_size;
			mesh.vertex_buffer_view.BufferLocation = mesh.vertex_buffer_resource->GetGPUVirtualAddress();
		}
	}


	void DX12::Impl::LoadVertexBuffer()
	{
	}

	uint32_t DX12::Impl::LoadShader(const std::string& file_path)
	{
		static uint32_t shader_id = 0;
		++shader_id;

		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		const std::wstring shader_filename = converter.from_bytes(file_path);
		const uint32_t flag = D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;

		for (auto& entry : shader_blobs_)
		{
			ID3DBlob *blob_signature = nullptr;
			ID3DBlob *blob_error = nullptr;
			ComPtr<ID3DBlob>& blob = std::get<2>(entry);
			D3DCompileFromFile(
				shader_filename.c_str(),
				NULL,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				std::get<0>(entry).c_str(),
				std::get<1>(entry).c_str(),
				flag,
				0,
				&blob,
				&blob_error);
			
			if (blob_error)
			{
				OutputDebugStringA(reinterpret_cast<const char*>(blob_error->GetBufferPointer()));
			}

			if (blob)
			{
				if (!rasterize_root_signature_)
				{
					if (S_OK == D3DGetBlobPart(
						blob->GetBufferPointer(),
						blob->GetBufferSize(),
						D3D_BLOB_ROOT_SIGNATURE, 0, &blob_signature))
					{
						device_->CreateRootSignature(
							0,
							blob_signature->GetBufferPointer(),
							blob_signature->GetBufferSize(), IID_PPV_ARGS(&rasterize_root_signature_));
					}
				}
			}
		}

		if (rasterize_root_signature_)
		{
			CreatePipeline(shader_id);
		}
		return shader_id;
	}

	void DX12::Impl::CreatePipeline(uint32_t shader_id)
	{
		D3D12_INPUT_ELEMENT_DESC element_desc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		D3D12_INPUT_LAYOUT_DESC input_layout = {};
		input_layout.pInputElementDescs = element_desc;
		input_layout.NumElements = static_cast<UINT>(std::size(element_desc));

		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
		desc.InputLayout = input_layout;
		desc.pRootSignature = rasterize_root_signature_.Get();
		desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		desc.SampleMask = UINT_MAX;
		desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		desc.NumRenderTargets = BackBufferCount + RenderTargetCount;
		desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		for (auto& entry : shader_blobs_)
		{
			std::string entry_point = std::get<0>(entry);
			std::string profile = std::get<1>(entry);
			ComPtr<ID3DBlob> blob = std::get<2>(entry);
			if (profile.find("vs") != std::string::npos)
			{
				desc.VS = CD3DX12_SHADER_BYTECODE(blob.Get());
			}
			else if (profile.find("ps") != std::string::npos)
			{
				desc.PS = CD3DX12_SHADER_BYTECODE(blob.Get());
			}
			else if (profile.find("gs") != std::string::npos)
			{
				desc.GS = CD3DX12_SHADER_BYTECODE(blob.Get());
			}
		}

		for (uint32_t k = 0; k < BackBufferCount; ++k)
		{
			desc.RTVFormats[k] = BackBufferFormat;
		}
		for (uint32_t k = BackBufferCount; k < desc.NumRenderTargets; ++k)
		{
			desc.RTVFormats[k] = RenderTargetFormat;
		}

		ThrowIfFailed(device_->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipeline_state_[shader_id])));
	}

	void DX12::Impl::Resize(uint32_t width, uint32_t height)
	{

	}

	void DX12::Impl::Update()
	{
		const void *data = &constants_;
		const uint32_t size = sizeof(Constants);
		uint8_t *mapped_data = nullptr;
		D3D12_RANGE range;
		range.Begin = 0;
		range.End = 0;
		constant_resource_->Map(0, &range, reinterpret_cast<void**>(&mapped_data));
		if (mapped_data)
		{
			mapped_data = reinterpret_cast<uint8_t*>(
				Align(reinterpret_cast<SIZE_T>(mapped_data),
					D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT));
			memcpy(mapped_data, data, size);
		}
		constant_resource_->Unmap(0, nullptr);
	}

	void DX12::Impl::Render()
	{
		ResetCommands();

		using Handle = D3D12_CPU_DESCRIPTOR_HANDLE;

		ID3D12DescriptorHeap* heaps[] = {
			cbv_srv_uav_heap_.Get()
		};
		command_list_->SetDescriptorHeaps(std::size(heaps), heaps);

		// Shader
		command_list_->SetPipelineState(pipeline_state_[1].Get());
		command_list_->SetGraphicsRootSignature(rasterize_root_signature_.Get());

		ComPtr<ID3D12Resource> back_buffer = render_target_resources_[current_back_buffer_];

		{
			std::vector<D3D12_RESOURCE_BARRIER> barrier_list = {
				CD3DX12_RESOURCE_BARRIER::Transition(
					back_buffer.Get(),
					D3D12_RESOURCE_STATE_PRESENT,
					D3D12_RESOURCE_STATE_RENDER_TARGET)
			};

			command_list_->ResourceBarrier(barrier_list.size(), barrier_list.data());
		}

		const Handle rt_handle = render_target_cpu_handles_[current_back_buffer_];
		const Handle ds_handle = depth_stencil_cpu_handles_[current_back_buffer_];

		command_list_->OMSetRenderTargets(1, &rt_handle, false, &ds_handle);

		command_list_->ClearRenderTargetView(rt_handle, clear_value_.Color, 0, nullptr);

		command_list_->ClearDepthStencilView(ds_handle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0, 0, 0, nullptr);

		D3D12_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(width_), static_cast<float>(height_), 0.0f, 1.0f };
		D3D12_RECT rect = { 0, 0, static_cast<uint16_t>(width_), static_cast<uint16_t>(height_) };
		command_list_->RSSetViewports(1, &viewport);
		command_list_->RSSetScissorRects(1, &rect);

		// D3D12_DISPATCH_RAYS_DESC desc;
		// command_list_->DispatchRays(&desc);


		command_list_->SetGraphicsRootDescriptorTable(
			0, cbv_srv_uav_gpu_handles_[constant_handle_index_]);

		// Draw
		command_list_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		command_list_->IASetIndexBuffer(&plane_.index_buffer_view);
		command_list_->IASetVertexBuffers(0, 1, &plane_.vertex_buffer_view);
		command_list_->DrawIndexedInstanced(plane_.indices.size(), 1, 0, 0, 0);

		{
			std::vector<D3D12_RESOURCE_BARRIER> barrier_list = {
				// backbuffer -> copy source
				CD3DX12_RESOURCE_BARRIER::Transition(
					back_buffer.Get(),
					D3D12_RESOURCE_STATE_RENDER_TARGET,
					D3D12_RESOURCE_STATE_PRESENT)
			};
			command_list_->ResourceBarrier(barrier_list.size(), barrier_list.data());

		}

		ExecuteCommands();
		WaitForGPU();


		// readback raytracing result
		/*
		ResetCommands();
		{
			std::vector<D3D12_RESOURCE_BARRIER> barrier_list = {
				// backbuffer -> copy dest
				CD3DX12_RESOURCE_BARRIER::Transition(
					back_buffer.Get(),
					D3D12_RESOURCE_STATE_RENDER_TARGET,
					D3D12_RESOURCE_STATE_COPY_DEST),
				// uav for render target in raytracing -> copy source
				CD3DX12_RESOURCE_BARRIER::Transition(
					uav_resources_[1].Get(),
					D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
					D3D12_RESOURCE_STATE_COPY_SOURCE),
			};
			command_list_->ResourceBarrier(barrier_list.size(), barrier_list.data());

			command_list_->CopyResource(back_buffer.Get(), uav_resources_[1].Get());

		}

		{
			std::vector<D3D12_RESOURCE_BARRIER> barrier_list = {
				// backbuffer -> present
				CD3DX12_RESOURCE_BARRIER::Transition(
					back_buffer.Get(),
					D3D12_RESOURCE_STATE_COPY_DEST,
					D3D12_RESOURCE_STATE_PRESENT),
				CD3DX12_RESOURCE_BARRIER::Transition(
					uav_resources_[1].Get(),
					D3D12_RESOURCE_STATE_COPY_SOURCE,
					D3D12_RESOURCE_STATE_UNORDERED_ACCESS),
			};
			command_list_->ResourceBarrier(barrier_list.size(), barrier_list.data());
		}

		ExecuteCommands();
		WaitForGPU();
		*/
	}


	void DX12::Impl::GetRenderImage(Image& image)
	{
		if (current_back_buffer_ <= 0) return;

		ResetCommands();

		ComPtr<ID3D12Resource> back_buffer = render_target_resources_[current_back_buffer_ - 1];

		D3D12_RESOURCE_DESC desc = back_buffer->GetDesc();
		D3D12_PLACED_SUBRESOURCE_FOOTPRINT foot_print;
		uint32_t nrow;
		uint64_t rowsize;
		uint64_t size;
		device_->GetCopyableFootprints(&desc, 0, 1, 0, &foot_print, &nrow, &rowsize, &size);

		
		{
			std::vector<D3D12_RESOURCE_BARRIER> barrier_list = {
				// backbuffer -> copy source
				CD3DX12_RESOURCE_BARRIER::Transition(
					back_buffer.Get(),
					D3D12_RESOURCE_STATE_PRESENT,
					D3D12_RESOURCE_STATE_COPY_SOURCE)
			};
			command_list_->ResourceBarrier(barrier_list.size(), barrier_list.data());


			D3D12_TEXTURE_COPY_LOCATION dst_loction;
			D3D12_TEXTURE_COPY_LOCATION src_location;
			dst_loction.pResource = uav_resources_[0].Get();
			dst_loction.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			dst_loction.PlacedFootprint = foot_print;
			src_location.pResource = back_buffer.Get();
			src_location.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			src_location.SubresourceIndex = 0;

			command_list_->CopyTextureRegion(&dst_loction, 0, 0, 0, &src_location, nullptr);
		}

		{
			std::vector<D3D12_RESOURCE_BARRIER> barrier_list = {
				// backbuffer -> present
				CD3DX12_RESOURCE_BARRIER::Transition(
					back_buffer.Get(),
					D3D12_RESOURCE_STATE_COPY_SOURCE,
					D3D12_RESOURCE_STATE_PRESENT)
			};
			command_list_->ResourceBarrier(barrier_list.size(), barrier_list.data());
		}

		ExecuteCommands();
		WaitForGPU();


		ComPtr< ID3D12Resource > readback_resouce = uav_resources_[0];
		if (readback_resouce)
		{
			uint8_t * data = nullptr;
			D3D12_RANGE range;
			range.Begin = 0;
			range.End = 0;
			readback_resouce->Map(0, &range, reinterpret_cast<void**>(&data));

			if (data)
			{
				for (uint32_t y = 0; y < image.height; ++y)
				{
					memcpy(
						&image.data[ y * image.width * image.channels ], 
						&data[foot_print.Footprint.RowPitch * y],
						rowsize);

				}
				readback_resouce->Unmap(0, NULL);
			}
		}
	}

	void DX12::Impl::ShareVertexBuffer(IDirect3DVertexBuffer9* v)
	{

	}

	//----------------------------------------------------------------

	DX12::DX12() : impl_(std::make_unique<DX12::Impl>()) {}

	DX12::~DX12() {}

	void DX12::Init() { impl_->Init(); }

	void DX12::ShareVertexBuffer(IDirect3DVertexBuffer9* v) { impl_->ShareVertexBuffer(v); }

	IUnknown* DX12::GetDevice() 
	{
		return impl_->GetDevice();
	}

	IUnknown* DX12::GetCommandQueue()
	{
		return impl_->GetCommandQueue();
	}
	
	void DX12::OutputImage()
	{
		impl_->OutputImage();
	}
} // namespace dx12
