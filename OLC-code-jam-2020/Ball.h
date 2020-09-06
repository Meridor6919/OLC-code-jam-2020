#pragma once
#include "DrawingClass.h"
#include <vector>

class Ball
{
public:
	const float radius = 15.0f;
	DirectX::XMVECTORF32 color1 = DirectX::Colors::White;
	DirectX::XMVECTORF32 color2 = DirectX::Colors::Black;
	DirectX::XMFLOAT2 velocity;
	DirectX::XMFLOAT2 position;

	Ball(DirectX::XMFLOAT2 position) {
		this->position = position;
		velocity.y = -200.0f;
		
	}

	void Render(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitive_batch) {
		DrawHallowCircle<50>(primitive_batch, radius, position, color1);
	};
	bool Update(float delta_time)
	{
		DirectX::XMFLOAT2 pos_before = position;
		position.x -= velocity.x * delta_time;
		position.y -= velocity.y * delta_time;
		if (velocity.y < 0)
		{
			velocity.y -= 10.0f * delta_time;
		}
		else
		{
			velocity.y += 10.0f * delta_time;
		}

		if (position.y > 800)
			return false;
		return true;

		//TO DO 
		//collisions
	}
	
};

