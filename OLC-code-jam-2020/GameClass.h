#pragma once
#pragma warning(disable : 4996)
#pragma comment(lib, "d3d11.lib")

#include<wrl/module.h>
#include <directxmath.h>
#include <d3d11_1.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <PrimitiveBatch.h>
#include <CommonStates.h>
#include <Effects.h>
#include <VertexTypes.h>
#include <SpriteFont.h>
#include <SimpleMath.h>

using Microsoft::WRL::ComPtr;

class GameClass
{
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> device_context;
	ComPtr<IDXGISwapChain> swap_chain;
	ComPtr<ID3D11RasterizerState> rasterizer_state;
	ComPtr<ID3D11Texture2D> render_target;
	ComPtr<ID3D11DepthStencilView> depth_stencil_view;
	ComPtr<ID3D11RenderTargetView> render_target_view;
	ComPtr<ID3D11InputLayout> input_layout;
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;

	std::unique_ptr<DirectX::Mouse> mouse;
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> mouse_tracker;
	std::unique_ptr<DirectX::Keyboard> keyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> keyboard_tracker;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> primitive_batch;
	std::unique_ptr<DirectX::SpriteBatch> sprite_batch;
	std::unique_ptr<DirectX::CommonStates> common_states;
	std::unique_ptr<DirectX::BasicEffect> basic_effect;
	std::unique_ptr<DirectX::SpriteFont> sprite_font;
	DirectX::XMVECTORF32 main_color = { 0.f, 0.7, 0.f };

	D3D_DRIVER_TYPE driver_type;
	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_9_1;
	D3D11_VIEWPORT viewport;
	
public:
	GameClass() {};
	bool InitDX(HWND hwnd);
	void Update(double delta_time);
	void Render(double delta_time);
};

