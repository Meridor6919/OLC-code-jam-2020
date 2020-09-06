#pragma once
#include "DrawingClass.h"

class Paddle {

public:
	
	float width;
	float additional_width;
	std::array<DirectX::VertexPositionColor, 4> position;

	Paddle(std::array<DirectX::VertexPositionColor, 4> position)
	{
		this->position = position;
		additional_width = (position[2].position.y - position[0].position.y) / 2;
		width = position[1].position.x - position[0].position.x;
	}

	void Render(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitive_batch) 
	{
		DrawPlatform<50>(primitive_batch, position, false);
	};
	void Move(bool left, float delta_time)
	{
		for (int i = 0; i < 4; ++i)
		{
			position[i].position.x += 500.0f * delta_time * (1 - 2 * left);
		}
		if (position[0].position.x < additional_width)
		{
			position[0].position.x = additional_width;
			position[2].position.x = additional_width;
			position[1].position.x = width + additional_width;
			position[3].position.x = width + additional_width;
		}
		if (position[1].position.x > 600 - additional_width)
		{
			position[0].position.x = 600 - width - additional_width;
			position[2].position.x = 600 - width - additional_width;
			position[1].position.x = 600 - additional_width;
			position[3].position.x = 600 - additional_width;
		}
	}
};