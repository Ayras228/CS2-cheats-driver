#pragma once
#include <Windows.h>

#define _USE_MATH_DEFINES
#include <cmath>



struct view_matrix_t
{
	float* operator[](int index)
	{
		return matrix[index];
	}

	float matrix[4][4];
};

class Vector2
{
private:
	float x, y;

public:
	// Constructors
	Vector2() : x(0.f), y(0.f) {}
	Vector2(float x_, float y_) : x(x_), y(y_) {}

	// Operator overloads
	Vector2 operator+(Vector2 Vector2_);
	Vector2 operator-(Vector2 Vector2_);
	Vector2 operator*(Vector2 Vector2_);
	Vector2 operator/(Vector2 Vector2_);
	Vector2 operator*(float n);
	Vector2 operator/(float n);
	bool operator==(Vector2 Vector2_) const;
	bool operator!=(Vector2 Vector2_) const;

	// Getters
	float get_x() const;
	float get_y() const;

	// Utility functions
	const bool IsZero();
	float Length();
	float DistanceTo(const Vector2& Pos);
};


class Vector3
{
private:
	
	float x, y, z;
public:
	Vector3() :x(0.f), y(0.f), z(0.f) {}
	Vector3(float x_, float y_, float z_) :x(x_), y(y_), z(z_) {}
	Vector3 operator+(Vector3 Vector3_);
	Vector3 operator-(Vector3 Vector3_);
	Vector3 operator*(Vector3 Vector3_);
	Vector3 operator/(Vector3 Vector3_);
	Vector3 operator*(float n);
	Vector3 operator/(float n);
	bool operator==(Vector3 Vector3_)const;
	bool operator!=(Vector3 Vector3_)const;
	//float operator[](int n)const;
	//Vector3 get_cords();

	float get_x()const;
	float get_y()const;
	float get_z()const;
	const bool IsZero();
	float Length();
	float DistanceTo(const Vector3& Pos);
	Vector3 RelativeAngle();				//unused
	static const bool world_to_screen(view_matrix_t vm, Vector3& in, Vector3& out);
};