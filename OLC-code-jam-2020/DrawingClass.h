#pragma once
#include "PrimitiveBatch.h"
#include "VertexTypes.h"
#include <DirectXColors.h>
#include <numbers>
#include <array>

template<int T>
void DrawHallowCircle(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitive_batch, const float radius, DirectX::XMFLOAT2 orgin, DirectX::XMVECTORF32 color = DirectX::Colors::Black, int start = 0, int stop = T)
{
	const float angle = 1 / static_cast<float>(T-1) * std::numbers::pi * 2;
	DirectX::VertexPositionColor arr[T];
	for (int i = start; i < stop; i++)
	{
		arr[i - start] = DirectX::VertexPositionColor({ orgin.x + sin(angle * i) * radius, orgin.y + cos(angle * i) * radius, 0 }, color);
	}
	primitive_batch->Draw(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, arr, stop-start);
}
template<int T>
void DrawFullCircle(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitive_batch, const float radius, DirectX::XMFLOAT2 orgin, DirectX::XMVECTORF32 color = DirectX::Colors::Black, DirectX::XMVECTORF32 color2 = DirectX::Colors::Black, int start = 0, int stop = T)
{
	const float angle = 1 / static_cast<float>(T - 1) * std::numbers::pi * 2;
	DirectX::VertexPositionColor arr[T];
	for (int i = start; i < stop; i++)
	{
		arr[i - start] = DirectX::VertexPositionColor({ orgin.x + sin(angle * i) * radius, orgin.y + cos(angle * i) * radius, 0 }, color);
	}
	for (int i = 0; i < stop - start - 1; ++i)
	{
		DirectX::VertexPositionColor center({ orgin.x, orgin.y, 0 }, color2);
		primitive_batch->DrawTriangle(arr[i], arr[i + 1], center);
	}
}
template<int T>
void DrawPlatform(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitive_batch, const std::array<DirectX::VertexPositionColor, 4> v, bool hallow = true)
{
	primitive_batch->DrawLine(v[0], v[1]);
	primitive_batch->DrawLine(v[2], v[3]);
	const DirectX::XMFLOAT2 orgin1 = { (v[0].position.x + v[2].position.x)/2, (v[0].position.y + v[2].position.y)/2 };
	const DirectX::XMFLOAT2 orgin2 = { (v[1].position.x + v[3].position.x)/2, (v[1].position.y + v[3].position.y)/2 };
	const float radius1 = sqrtf((v[0].position.x - v[2].position.x) * (v[0].position.x - v[2].position.x) + (v[0].position.y - v[2].position.y) * (v[0].position.y - v[2].position.y))/2;
	const float radius2 = sqrtf((v[1].position.x - v[3].position.x) * (v[1].position.x - v[3].position.x) + (v[1].position.y - v[3].position.y) * (v[1].position.y - v[3].position.y))/2;

	DirectX::XMVECTORF32 color1;
	color1.v = DirectX::XMLoadFloat4(&v[0].color);
	DirectX::XMVECTORF32 color2;
	color2.v = DirectX::XMLoadFloat4(&v[3].color);
	if (hallow)
	{
		DrawHallowCircle<T>(primitive_batch, radius1, orgin1, color1);
		DrawHallowCircle<T>(primitive_batch, radius2, orgin2, color1);
	}
	else
	{
		primitive_batch->DrawQuad(v[0], v[1], v[3], v[2]);
		DrawFullCircle<T>(primitive_batch, radius1, orgin1, color1, color2);
		DrawFullCircle<T>(primitive_batch, radius2, orgin2, color1, color2);
	}
}

