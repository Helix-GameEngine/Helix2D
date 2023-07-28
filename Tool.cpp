#include "h2dTool.h"
#include "h2dBase.h"
#include <vector>
#include <map>

helix2d::Vector2::Vector2()
{
	x = 0.0f;
	y = 0.0f;
}

helix2d::Vector2::Vector2(const D2D1_POINT_2F& point)
{
	x = point.x;
	y = point.y;
}

helix2d::Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

helix2d::Vector2 helix2d::Vector2::operator+(const Vector2& point)
const
{
	return Vector2(x + point.x, y + point.y);
}

helix2d::Vector2 helix2d::Vector2::operator-(const Vector2& point)
const
{
	return Vector2(x - point.x, y - point.y);
}

helix2d::Vector2 helix2d::Vector2::operator=(const Vector2& point)
{
	x = point.x;
	y = point.y;
	return *this;
}

helix2d::Vector2 helix2d::Vector2::operator+=(const Vector2& point)
{
	x += point.x;
	y += point.y;
	return *this;
}

helix2d::Vector2 helix2d::Vector2::operator-=(const Vector2& point)
{
	x -= point.x;
	y -= point.y;
	return *this;
}

helix2d::Vector2 helix2d::Vector2::operator*(float num)
const
{
	return Vector2(x * num, y * num);
}

helix2d::Vector2 helix2d::operator*(float num, const helix2d::Vector2& vec)
{
	return vec * num;
}

helix2d::Vector2 helix2d::Vector2::operator/(float num) const
{
	return Vector2(x / num, y / num);
}

helix2d::Vector2 helix2d::Vector2::operator*=(float num)
{
	x *= num;
	y *= num;
	return *this;
}

helix2d::Vector2 helix2d::Vector2::operator/=(float num)
{
	x /= num;
	y /= num;
	return *this;
}

helix2d::Vector2 helix2d::Vector2::operator-()
{
	return Vector2(-x, -y);
}

helix2d::Vector2::operator D2D1_POINT_2F()
{
	return D2D1::Point2F(x, y);
}

float helix2d::Vector2::dot(const Vector2& vec)const
{
	return x * vec.x + y * vec.y;
}

helix2d::Vector2 helix2d::Vector2::cross(const Vector2& vec)const
{
	return Vector2(
		x * vec.y - y * vec.x, 
		y * vec.x - x * vec.y
	);
}

helix2d::Vector2 helix2d::Vector2::getLeftNormalLine() const
{
	return Vector2(-y, x);
}

helix2d::Vector2 helix2d::Vector2::getRightNormalLine() const
{
	return Vector2(y, -x);
}

float helix2d::Vector2::getDistance(const Vector2& vec) const
{
	return sqrtf(
		powf((x - vec.x), 2) +
		powf((y - vec.y), 2)
	);
}

float helix2d::Vector2::getModulus() const
{
	return sqrtf(
		x * x +
		y * y
	);
}

helix2d::Vector2 helix2d::Vector2::getUnit() const
{
	return (*this) / getModulus();
}

float helix2d::Vector2::getDistance(const Vector2& vec1, const Vector2& vec2)
{
	return vec1.getDistance(vec2);
}

float helix2d::Vector2::dot(const Vector2& vec1, const Vector2& vec2)
{
	return vec1.dot(vec2);
}

helix2d::Vector2 helix2d::Vector2::cross(const Vector2& vec1, const Vector2& vec2)
{
	return vec1.cross(vec2);
}

bool helix2d::Input::isDown(Window* window, const KeyCode& key, float downTime)
{
	auto it = std::find(
		window->downKey.begin(),
		window->downKey.end(),
		key
	);

	auto ct = clock() / 1000.0f;
	
	if (it != window->downKey.end() && ct - window->downKeyTime[key] >= downTime)
	{
		return true;
	}

	return false;
}

bool helix2d::Input::isDown(Window* window, const MouseCode& key, float downTime)
{
	auto it = std::find(
		window->downMouse.begin(),
		window->downMouse.end(),
		key
	);

	auto ct = clock() / 1000.0f;

	if (it != window->downMouse.end() && ct - window->downMouseTime[key] >= downTime)
	{
		return true;
	}

	return false;
}

bool helix2d::Input::isRelease(Window* window, const KeyCode& key)
{
	auto it = std::find(
		window->downKey.begin(),
		window->downKey.end(),
		key
	);

	if (it == window->downKey.end())
	{
		return true;
	}

	return false;
}

bool helix2d::Input::isRelease(Window* window, const MouseCode& key)
{
	auto it = std::find(
		window->downMouse.begin(),
		window->downMouse.end(),
		key
	);

	if (it == window->downMouse.end())
	{
		return true;
	}

	return false;
}

bool helix2d::Input::isUpPress(Window* window, const KeyCode& key, float downTime)
{
	auto it = std::find(
		window->upPressKey.begin(),
		window->upPressKey.end(),
		key
	);

	auto ct = clock() / 1000.0f;

	if (it != window->upPressKey.end() && ct - window->downKeyTime[key] >= downTime)
	{
		return true;
	}
	return false;
}

bool helix2d::Input::isUpPress(Window* window, const MouseCode& key, float downTime)
{
	auto it = std::find(
		window->upPressMouse.begin(),
		window->upPressMouse.end(),
		key
	);

	auto ct = clock() / 1000.0f;

	if (it != window->upPressMouse.end() && ct - window->downMouseTime[key] >= downTime)
	{
		return true;
	}
	return false;
}

bool helix2d::Input::isDownPress(Window* window, const KeyCode& key)
{
	auto it = std::find(
		window->downPressKey.begin(),
		window->downPressKey.end(),
		key
	);

	if (it != window->downPressKey.end())
	{
		return true;
	}
	return false;
}

bool helix2d::Input::isDownPress(Window* window, const MouseCode& key)
{
	auto it = std::find(
		window->downPressMouse.begin(),
		window->downPressMouse.end(),
		key
	);

	if (it != window->downPressMouse.end())
	{
		return true;
	}
	return false;
}