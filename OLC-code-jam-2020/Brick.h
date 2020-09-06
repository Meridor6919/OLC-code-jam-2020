#pragma once
#include "DrawingClass.h"

struct Brick {

	int durability = 1;
	std::array<DirectX::VertexPositionColor, 4> position;

	Brick(std::array<DirectX::VertexPositionColor, 4> position)
	{
		this->position = position;
	}
	void Render(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitive_batch) 
	{
			DrawPlatform<50>(primitive_batch, position, durability != 2);
	};
	bool OnHit()
	{
		--durability;
		return durability > 0;
	}



};