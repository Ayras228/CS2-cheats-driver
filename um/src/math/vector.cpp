#include "vector.hpp"

int screenWidth = GetSystemMetrics(SM_CXSCREEN); // 0
int screenHeight = GetSystemMetrics(SM_CYSCREEN); // 1

constexpr const my_Vector& my_Vector::operator-(const my_Vector& other) const noexcept
{
    return my_Vector{ x - other.x, y - other.y, z - other.z };
}

constexpr const my_Vector& my_Vector::operator+(const my_Vector& other) const noexcept
{
    return my_Vector{ x + other.x, y + other.y, z + other.z };
}

constexpr const my_Vector& my_Vector::operator/(const float factor) const noexcept
{
    return my_Vector{ x / factor, y / factor, z / factor };
}

constexpr const my_Vector& my_Vector::operator*(const float factor) const noexcept
{
    return my_Vector{ x * factor, y * factor, z * factor };
}

// w2s, alot of explanations already exist
const bool my_Vector::world_to_screen(view_matrix_t vm, my_Vector& in, my_Vector& out)
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
    
    return true;
}

const bool my_Vector::IsZero() {
    return x == 0.0f && y == 0.0f && z == 0.0f;
}
