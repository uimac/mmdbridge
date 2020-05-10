#define CINTERFACE

#include "dx12_hook.h"
#include <d3d12.h>
#include <dxgi1_5.h>

HRESULT(WINAPI *original_create_commad_queue)(
	ID3D12Device5 * This,
	_In_  const D3D12_COMMAND_QUEUE_DESC *pDesc,
	REFIID riid,
	_COM_Outptr_  void **ppCommandQueue
	)(NULL);

HRESULT(WINAPI *original_create_commited_resource)(
	ID3D12Device5 * This,
	_In_  const D3D12_HEAP_PROPERTIES *pHeapProperties,
	D3D12_HEAP_FLAGS HeapFlags,
	_In_  const D3D12_RESOURCE_DESC *pDesc,
	D3D12_RESOURCE_STATES InitialResourceState,
	_In_opt_  const D3D12_CLEAR_VALUE *pOptimizedClearValue,
	REFIID riidResource,
	_COM_Outptr_opt_  void **ppvResource)(NULL);

HRESULT(WINAPI *original_create_graphics_pipeline_state)(
	ID3D12Device5 * This,
	_In_  const D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc,
	REFIID riid,
	_COM_Outptr_  void **ppPipelineState)(NULL);

void(WINAPI *original_draw_indexed_instanced)(
	ID3D12GraphicsCommandList *dCommandList, 
	UINT IndexCount, 
	UINT InstanceCount, 
	UINT StartIndex
	)(NULL);

static HRESULT WINAPI hkCreateCommandQueue(
	ID3D12Device5 * This,
	_In_  const D3D12_COMMAND_QUEUE_DESC *pDesc,
	REFIID riid,
	_COM_Outptr_  void **ppCommandQueue)
{

	return original_create_commad_queue(This, pDesc, riid, ppCommandQueue);
}

static
HRESULT WINAPI hkCreateGraphicsPipelineState(
	ID3D12Device5 * This,
	_In_  const D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc,
	REFIID riid,
	_COM_Outptr_  void **ppPipelineState)
{
	return original_create_graphics_pipeline_state(This, pDesc, riid, ppPipelineState);
}

static HRESULT WINAPI hkCreateCommitedResource(
	ID3D12Device5 * This,
	_In_  const D3D12_HEAP_PROPERTIES *pHeapProperties,
	D3D12_HEAP_FLAGS HeapFlags,
	_In_  const D3D12_RESOURCE_DESC *pDesc,
	D3D12_RESOURCE_STATES InitialResourceState,
	_In_opt_  const D3D12_CLEAR_VALUE *pOptimizedClearValue,
	REFIID riidResource,
	_COM_Outptr_opt_  void **ppvResource)
{

	return original_create_commited_resource(
		This, pHeapProperties, HeapFlags, pDesc, 
		InitialResourceState, pOptimizedClearValue, riidResource, ppvResource);
}
void D3D12_Hook_Inject(ID3D12Device5* device)
{
	//original_create_commad_queue = device->lpVtbl->CreateCommandQueue;
	original_create_commited_resource = device->lpVtbl->CreateCommittedResource;
	original_create_graphics_pipeline_state = device->lpVtbl->CreateGraphicsPipelineState;

	// 書き込み属性付与
	DWORD old_protect;
	VirtualProtect(reinterpret_cast<void *>(device->lpVtbl), sizeof(device->lpVtbl), PAGE_EXECUTE_READWRITE, &old_protect);

	device->lpVtbl->CreateCommittedResource = hkCreateCommitedResource;
	device->lpVtbl->CreateGraphicsPipelineState = hkCreateGraphicsPipelineState;

	// 書き込み属性元に戻す
	VirtualProtect(reinterpret_cast<void *>(device->lpVtbl), sizeof(device->lpVtbl), old_protect, &old_protect);
}