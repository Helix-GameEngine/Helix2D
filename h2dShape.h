#pragma once
#include "h2dRender.h"


namespace helix2d
{
	//圆形
	class Circle :
		public Painter
	{
	public:

		Circle();

		Circle(float radiusX, float radiusY);

		Circle(float radius);
	public:

		//设置半径
		void setRadius(float radiusX, float radiusY);
		//设置X轴半径
		void setRadiusX(float radiusX);
		//设置Y轴半径
		void setRadiusY(float radiusY);
	public:

		//获取X轴半径
		float getRadiusX()const;
		//获取Y轴半径
		float getRadiusY()const;
	public:

		void Render() override;
	};

	//矩形
	class Rect :
		public Painter
	{
	public:

		Rect();

		Rect(float width, float height);

		Rect(float side);
	public:

		//设置宽度
		void setWidth(float width);
		//设置高度
		void setHeight(float height);
	public:

		//获取宽度
		float getWidth()const;
		//获取高度
		float getHeight()const;
	public:

		void Render()override;
	};
}