#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx12.h>
#include "ImGuiManager.h"
#include <cassert>

/**
 * @file ImGuiManager.cpp
 * @brief 'ImGui'の機能を使いやすいようにまとめたクラス
 */

ImGuiManager* ImGuiManager::Getinstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Init()
{
	HRESULT result;

	winApi_ = WinAPI::GetInstance();
	directX_ = RDirectX::GetInstance();
	//ImGuiのコンテキストを生成
	ImGui::CreateContext();
	//ImGuiのスタイルを設定
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(winApi_->hwnd_);

	//デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//デスクリプタヒープ生成
	result = directX_->GetDevice()->
		CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	ImGui_ImplDX12_Init(
		directX_->GetDevice(),
		static_cast<uint32_t>(directX_->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	);

	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig config;
	config.MergeMode = true;
	//標準フォント
	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese());
	//examples/example_win32_directx12/main.cppに呼び出し例がある
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void ImGuiManager::Finalize()
{
	//後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	//デスクリプタヒープを開放
	srvHeap_.Reset();
}

void ImGuiManager::Begin()
{
	//ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	ImGui::Render();
}

void ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* commandList = directX_->GetCommandList();

	//デスクリプタヒープの配列をセットするコマンド
	std::vector<ID3D12DescriptorHeap*> ppHeaps = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps((UINT)ppHeaps.size(), &ppHeaps.at(0));
	//描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}
