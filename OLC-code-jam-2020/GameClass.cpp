#include "GameClass.h"

bool GameClass::InitDX(HWND hwnd)
{
	constexpr int sample_count = 4;

	//Creating dummy device to access CheckMultisampleQualityLevels method
	ComPtr<ID3D11Device> dummy_device;
	D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0, D3D11_SDK_VERSION, dummy_device.ReleaseAndGetAddressOf(), 0, 0);
	UINT max_quality_level;
	dummy_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sample_count, &max_quality_level);
	--max_quality_level; 

	//Creating swap chain and assigning it to device, device context and feature_level
	DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Width = 600;
	swap_chain_desc.BufferDesc.Height = 800;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = hwnd;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Windowed = true;
	swap_chain_desc.SampleDesc.Count = sample_count;
	swap_chain_desc.SampleDesc.Quality = max_quality_level;
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0, D3D11_SDK_VERSION, &swap_chain_desc,
		swap_chain.ReleaseAndGetAddressOf(), device.ReleaseAndGetAddressOf(), &feature_level, device_context.ReleaseAndGetAddressOf());

	//Creating render_target_view
	swap_chain->GetBuffer(0, IID_PPV_ARGS(render_target.ReleaseAndGetAddressOf()));
	device->CreateRenderTargetView(render_target.Get(), nullptr, &render_target_view);

	//Creating Depth stencil view
	CD3D11_TEXTURE2D_DESC depth_stencil_desc(DXGI_FORMAT_D24_UNORM_S8_UINT, 600, 800,
		1, 1, D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT, 0, 4, 0);

	ComPtr<ID3D11Texture2D> depth_stencil;
	device->CreateTexture2D(&depth_stencil_desc, NULL, depth_stencil.ReleaseAndGetAddressOf());
	CD3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc(D3D11_DSV_DIMENSION_TEXTURE2DMS);
	device->CreateDepthStencilView(depth_stencil.Get(), &depth_stencil_view_desc, depth_stencil_view.ReleaseAndGetAddressOf());

	//Creating rasterizer
	CD3D11_RASTERIZER_DESC rasterizer_desc(D3D11_FILL_SOLID, D3D11_CULL_NONE, FALSE,
		D3D11_DEFAULT_DEPTH_BIAS, D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
		D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, TRUE, FALSE, TRUE, FALSE);
	device->CreateRasterizerState(&rasterizer_desc, rasterizer_state.ReleaseAndGetAddressOf());
	
	//Setting remaining variables
	viewport = CD3D11_VIEWPORT(0.0f, 0.0f, 600.0f, 800.0f, 0.0f, 1.0f);
	common_states = std::make_unique<DirectX::CommonStates>(device.Get());
	sprite_batch = std::make_unique<DirectX::SpriteBatch>(device_context.Get());
	primitive_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(device_context.Get());
	basic_effect = std::make_unique<DirectX::BasicEffect>(device.Get());
	basic_effect->SetVertexColorEnabled(true);

	//Creating Input layout
	void const* shaderByteCode;
	size_t byteCodeLength;
	basic_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		input_layout.ReleaseAndGetAddressOf());

	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, float(600), float(800), 0.f, 0.f, 1.f);
	basic_effect->SetProjection(proj);

	return true;
}

void GameClass::Update(double delta_time)
{
}

void GameClass::Render(double delta_time)
{
	device_context->ClearRenderTargetView(render_target_view.Get(), DirectX::Colors::CornflowerBlue);
	device_context->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(), depth_stencil_view.Get());
	device_context->RSSetViewports(1, &viewport);
	device_context->OMSetBlendState(common_states->Opaque(), nullptr, 0xFFFFFFFF);
	device_context->OMSetDepthStencilState(common_states->DepthNone(), 0);
	device_context->RSSetState(rasterizer_state.Get());
	basic_effect->SetWorld(world);
	basic_effect->Apply(device_context.Get());
	device_context->IASetInputLayout(input_layout.Get());
	primitive_batch->Begin();


	
	primitive_batch->End();
	swap_chain->Present(0, 0);
}
