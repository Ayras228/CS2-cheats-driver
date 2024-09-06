#include"Vectors.h"
const int screenWidth = GetSystemMetrics(SM_CXSCREEN); // 0
const int screenHeight = GetSystemMetrics(SM_CYSCREEN); // 1


// Operator implementations
Vector2 Vector2::operator+(Vector2 Vector2_)
{
	return { x + Vector2_.x, y + Vector2_.y };
}

Vector2 Vector2::operator-(Vector2 Vector2_)
{
	return { x - Vector2_.x, y - Vector2_.y };
}

Vector2 Vector2::operator*(Vector2 Vector2_)
{
	return { x * Vector2_.x, y * Vector2_.y };
}

Vector2 Vector2::operator/(Vector2 Vector2_)
{
	return { x / Vector2_.x, y / Vector2_.y };
}

Vector2 Vector2::operator*(float n)
{
	return { x * n, y * n };
}

Vector2 Vector2::operator/(float n)
{
	return { x / n, y / n };
}

bool Vector2::operator==(Vector2 Vector2_) const
{
	return x == Vector2_.x && y == Vector2_.y;
}

bool Vector2::operator!=(Vector2 Vector2_) const
{
	return x != Vector2_.x || y != Vector2_.y;
}

// Getters
float Vector2::get_x() const
{
	return x;
}

float Vector2::get_y() const
{
	return y;
}

// Utility function implementations
const bool Vector2::IsZero()
{
	return x == 0.0f && y == 0.0f;
}

float Vector2::Length()
{
	return sqrtf(powf(x, 2) + powf(y, 2));
}

float Vector2::DistanceTo(const Vector2& Pos)
{
	return sqrtf(powf(Pos.x - x, 2) + powf(Pos.y - y, 2));
}

Vector3 Vector3::operator+(Vector3 Vector3_)
{
	return { x + Vector3_.x,y + Vector3_.y,z + Vector3_.z };
}

Vector3 Vector3::operator-(Vector3 Vector3_)
{
	return { x - Vector3_.x,y - Vector3_.y,z - Vector3_.z };
}

Vector3 Vector3::operator*(Vector3 Vector3_)
{
	return { x * Vector3_.x,y * Vector3_.y,z * Vector3_.z };
}

Vector3 Vector3::operator/(Vector3 Vector3_)
{
	return { x / Vector3_.x,y / Vector3_.y,z / Vector3_.z };
}

Vector3 Vector3::operator*(float n)
{
	return { x * n,y * n,z * n };
}

Vector3 Vector3::operator/(float n)
{
	return { x / n,y / n,z / n };
}

bool Vector3::operator==(Vector3 Vector3_)const
{
	return x == Vector3_.x && y == Vector3_.y && z == Vector3_.z;
}

bool Vector3::operator!=(Vector3 Vector3_)const
{
	return x != Vector3_.x || y != Vector3_.y || z != Vector3_.z;
}

//float Vector3::operator[](int n) const
//{
//	switch (n) 
//	{
//		case 0: return x;
//		case 1: return y;
//		case 2: return z;
//		//default: throw std::out_of_range("Index out of range for Vector3");
//	}
//}

float Vector3::get_x()const
{
	return x;
}

float Vector3::get_y()const
{
	return y;
}

float Vector3::get_z()const
{
	return z;
}

const bool Vector3::IsZero()
{
	return x == 0.0f && y == 0.0f && z == 0.0f;
}


float Vector3::Length()
{
	return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
}

float Vector3::DistanceTo(const Vector3& Pos)
{
	return sqrtf(powf(Pos.x - x, 2) + powf(Pos.y - y, 2) + powf(Pos.z - z, 2));
}

Vector3 Vector3::RelativeAngle()
{
	return {
			static_cast<float>(std::atan2(-z, std::hypot(x, y)) * (180.0f / M_PI)),
			static_cast<float>(std::atan2(y, x) * (180.0f / M_PI)),
			0.0f
	};
}

const bool Vector3::world_to_screen(view_matrix_t vm, Vector3& in, Vector3& out)
{
	out.x = vm[0][0] * in.x + vm[0][1] * in.y + vm[0][2] * in.z + vm[0][3];
	out.y = vm[1][0] * in.x + vm[1][1] * in.y + vm[1][2] * in.z + vm[1][3];

	float width = vm[3][0] * in.x + vm[3][1] * in.y + vm[3][2] * in.z + vm[3][3];

	if (width < 0.01f) {
		return false;
	}

	float inverseWidth = 1.f / width;

	out.x *= inverseWidth;
	out.y *= inverseWidth;

	float x = screenWidth / 2;
	float y = screenHeight / 2;

	x += 0.5f * out.x * screenWidth + 0.5f;
	y -= 0.5f * out.y * screenHeight + 0.5f;

	out.x = x;
	out.y = y;

	return true;;
}

