#include "h2dTool.h"
#include "h2dBase.h"
#include <vector>
#include <map>
#include <time.h>
#include <iostream>

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

void helix2d::Input::addKeyCode(Window* window, const KeyCode& key)
{
	auto it = std::find(
		window->downKey.begin(),
		window->downKey.end(),
		key
	);

	//判断之前是否是松开的
	if (it == window->downKey.end())
	{
		window->downKey.push_back(key);
		window->downPressKey.push_back(key);
		window->downKeyTime[key] =
			clock() / 1000.f;
	}
}

void helix2d::Input::removeKeyCode(Window* window, const KeyCode& key)
{
	auto it = std::find(
		window->downKey.begin(),
		window->downKey.end(),
		key
	);

	//判断之前是否是按下的
	if (it != window->downKey.end())
	{
		window->downKey.erase(it);
		window->upPressKey.push_back(key);

		//不修改时间，以备使用
	}
	else
	{
		window->downKeyTime[key] = -1.f;
	}
}

void helix2d::Input::addMouseCode(Window* window, const MouseCode& key)
{
	auto it = std::find(
		window->downMouse.begin(),
		window->downMouse.end(),
		key
	);

	if (it == window->downMouse.end())
	{
		window->downMouse.push_back(key);
		window->downPressMouse.push_back(key);
		window->downMouseTime[key] = clock() / 1000.f;
	}
}

void helix2d::Input::removeMouseCode(Window* window, const MouseCode& key)
{
	auto it = std::find(
		window->downMouse.begin(),
		window->downMouse.end(),
		key
	);

	if (it != window->downMouse.end())
	{
		window->downMouse.erase(it);
		window->upPressMouse.push_back(key);
	}
	else
	{
		window->downMouseTime[key] = -1.f;
	}
}

helix2d::KeyCode helix2d::Input::getLRKeyCode(const KeyCode& key, LPARAM lparam)
{
	if ((lparam & 0x01000000) == 0)
	{
		//左键
		switch (key)
		{
		case KeyCode::Ctrl:
			return KeyCode::LCtrl;
		case KeyCode::Alt:
			return KeyCode::LAlt;
		case KeyCode::Shift:
			return KeyCode::LShift;
		default:
			break;
		}
	}
	else
	{
		//右键
		switch (key)
		{
		case KeyCode::Ctrl:
			return KeyCode::RCtrl;
		case KeyCode::Alt:
			return KeyCode::RAlt;
		case KeyCode::Shift:
			return KeyCode::RShift;
		default:
			break;
		}
	}

	return key;
}

const float helix2d::Math::Pi = 3.1415926f;
const float helix2d::Math::e = 2.718281828f;

float helix2d::Math::getRadian(float angle)
{
	return (angle * Math::Pi / 180.0f);
}

float helix2d::Math::getAngle(float radian)
{
	return (radian * 180.0f / Math::Pi);
}

bool helix2d::Logger::isEnableError = true;
bool helix2d::Logger::isEnableMessage = true;
bool helix2d::Logger::isEnableWarning = true;

void helix2d::Logger::warning(std::wstring detail)
{
	if (isEnableWarning)
	{
		std::wcout << L"Helix2D Warning: " << detail << "\n";
	}
}

void helix2d::Logger::error(std::wstring detail)
{
	if (isEnableWarning)
	{
		std::wcout << L"Helix2D Error: " << detail << "\n";
	}
}

void helix2d::Logger::message(std::wstring detail)
{
	if (isEnableWarning)
	{
		std::wcout << L"Helix2D Message: " << detail << "\n";
	}
}

void helix2d::Logger::enableWarning(bool b)
{
	isEnableWarning = b;
}

void helix2d::Logger::enableError(bool b)
{
	isEnableError = b;
}

void helix2d::Logger::enableMessage(bool b)
{
	isEnableMessage = b;
}

std::random_device helix2d::Random::seedGenerator;
std::default_random_engine helix2d::Random::engine(helix2d::Random::seedGenerator());

double helix2d::Random::range(double min, double max)
{
	std::uniform_real_distribution<double> maker(min, max);
	return maker(seedGenerator);
}

float helix2d::Random::range(float min, float max)
{
	std::uniform_real_distribution<float> maker(min, max);
	return maker(engine);
}

int helix2d::Random::range(int min, int max)
{
	std::uniform_int_distribution<int> maker(min, max);
	return maker(engine);
}

bool helix2d::Random::probability(float probability)
{
	std::bernoulli_distribution maker(probability);
	return maker(engine);
}
