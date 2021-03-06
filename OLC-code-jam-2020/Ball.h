#pragma once
#include "DrawingClass.h"
#include "Paddle.h"
#include "Brick.h"
#include <vector>


class Ball
{
public:
	const float radius = 15.0f;
	int score = 80;
	DirectX::XMVECTORF32 color = DirectX::Colors::White;
	DirectX::XMFLOAT2 velocity;
	DirectX::XMFLOAT2 position;
	Paddle* paddle;
	std::array<Brick*, 80> *bricks;
	int last_bounce = -1;

	Ball(DirectX::XMFLOAT2 position, Paddle*paddle, std::array<Brick*, 80>* bricks) {
		this->position = position;
		velocity = { -500.0f, 300.0f };
		this->paddle = paddle;
		this->bricks = bricks;
	}

	void Render(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitive_batch) {
		DrawHallowCircle<50>(primitive_batch, radius, position, color);
	};
	bool HorizontalLineCollision(DirectX::XMFLOAT2 point1, DirectX::XMFLOAT2 point2, bool up)
	{
		if (point1.x > point2.x)
		{
			float temp_x = point2.x;
			point2.x = point1.x;
			point1.x = temp_x;
		}
		if (position.x < point2.x && position.x > point1.x && position.y + radius > point1.y && position.y - radius < point1.y)
		{
			if ((up && velocity.y > 0) || (!up && velocity.y < 0))
			{
				velocity.y *= -1;
			}
			OutputDebugString(L"x");
			return true;
		}
		return false;
	};
	bool CircleCollision(DirectX::XMFLOAT2 point, float radius)
	{
		if (sqrtf((position.x - point.x) * (position.x - point.x) + (position.y - point.y) * (position.y - point.y)) <= radius + this->radius)
		{
			
			velocity.x *= -1;
			velocity.y *= -1;
			return true;
		}
		return false;
	}
	bool Update(float delta_time)
	{
		position.x -= velocity.x * delta_time;
		position.y -= velocity.y * delta_time;

		//wall collision
		if (position.x < radius)
		{
			last_bounce = 81;
			if (velocity.x > 0)
			{
				velocity.x *= -1;
			}
		}
		else if (position.x > 600 - radius)
		{
			last_bounce = 82;
			if (velocity.x < 0)
			{
				velocity.x *= -1;
			}
		}
		if (position.y < radius)
		{
			last_bounce = 83;
			if (velocity.y > 0)
			{
				velocity.y *= -1;
			}
		}
		else if (position.y > 800.0f)
		{
			return false;
		}
		if (paddle != nullptr)
		{
			if (last_bounce != 80)
			{
				bool hit = false;

				float circle_radius = (paddle->position[2].position.y - paddle->position[0].position.y) / 2;
				float circle_center = paddle->position[0].position.y + circle_radius;
				if (!(hit = HorizontalLineCollision(DirectX::XMFLOAT2(paddle->position[0].position.x, paddle->position[0].position.y), DirectX::XMFLOAT2(paddle->position[1].position.x, paddle->position[1].position.y), false)))
					if (!(hit = HorizontalLineCollision(DirectX::XMFLOAT2(paddle->position[2].position.x, paddle->position[2].position.y), DirectX::XMFLOAT2(paddle->position[3].position.x, paddle->position[3].position.y), true)))
						if (!( hit = CircleCollision(DirectX::XMFLOAT2(paddle->position[0].position.x, circle_center), circle_radius)))
							hit = CircleCollision(DirectX::XMFLOAT2(paddle->position[1].position.x, circle_center), circle_radius);

				if (hit)
				{
					velocity.x *= 1.01f;
					velocity.y *= 1.015f;
					last_bounce = 80;
				}
			}
		}


		//bricks collisions
		
		for (int i = 0; i < 80; ++i)
		{
			if ((*bricks)[i] != nullptr && last_bounce != i)
			{
				float circle_radius = ((*bricks)[i]->position[2].position.y - (*bricks)[i]->position[0].position.y) / 2;
				float circle_center = (*bricks)[i]->position[0].position.y + circle_radius;
				bool hit = false;

				if (!(hit = HorizontalLineCollision(DirectX::XMFLOAT2((*bricks)[i]->position[0].position.x, (*bricks)[i]->position[0].position.y),
					DirectX::XMFLOAT2((*bricks)[i]->position[1].position.x, (*bricks)[i]->position[1].position.y), false)))
					if (!(hit = HorizontalLineCollision(DirectX::XMFLOAT2((*bricks)[i]->position[2].position.x, (*bricks)[i]->position[2].position.y),
						DirectX::XMFLOAT2((*bricks)[i]->position[3].position.x, (*bricks)[i]->position[3].position.y), true)))
						if (!(hit = CircleCollision(DirectX::XMFLOAT2((*bricks)[i]->position[0].position.x, circle_center), circle_radius)))
							hit = CircleCollision(DirectX::XMFLOAT2((*bricks)[i]->position[1].position.x, circle_center), circle_radius);
				if (hit)
				{
					last_bounce = i;
					velocity.x *= 1.01f;
					velocity.y *= 1.015f;
					if (!(*bricks)[i]->OnHit())
					{
						delete (*bricks)[i];
						(*bricks)[i] = nullptr;
						score--;
					}
					break;
				}
			}
		}
		
		
		return true;
	}
	
};

