#pragma once
#include <numbers>
#include <cmath>
#include <Windows.h>

struct view_matrix_t
{
	float* operator[](int index)
	{
		return matrix[index];
	}

	float matrix[4][4];
};

class my_Vector
{
public:
	constexpr my_Vector(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) { }
	
	constexpr const my_Vector& operator-(const my_Vector& other) const noexcept;
	constexpr const my_Vector& operator+(const my_Vector& other) const noexcept;
	constexpr const my_Vector& operator/(const float factor) const noexcept;
	constexpr const my_Vector& operator*(const float factor) const noexcept;

	// 3d -> 2d, explanations already exist.
	const static bool world_to_screen(view_matrix_t view_matrix, my_Vector& in, my_Vector& out);

	const bool IsZero();

	float x, y, z;
};

