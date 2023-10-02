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

		//设置实际宽度
		void setRealWidth(float width);
		//设置实际高度
		void setRealHeight(float height);
	public:

		//获取宽度
		float getWidth()const;
		//获取高度
		float getHeight()const;
	public:

		void Render()override;
	};

	//直线
	class Line :
		public Painter
	{
	public:

		Line();

		Line(Vector2 ep1, Vector2 ep2, float width = 2.0f);
	public:

		//设置线条端点
		void setEndpoint(Vector2 ep1, Vector2 ep2);

		//设置线条宽度
		void setLineWidth(float width);
	public:

		//获取线条端点
		void getEndpoint(Vector2& ep1, Vector2& ep2)const;

		//获取线条宽度
		float getLineWidth()const;

		//获取实际包围宽度
		float getRealBoundingWidth()const;
		//获取实际包围高度
		float getRealBoundingHeight()const;

		//获取实际包围宽度
		float getBoundingWidth()const;
		//获取实际包围高度
		float getBoundingHeight()const;
	public:

		void Render() override;
	private:

		Vector2 endpoint1;
		Vector2 endpoint2;

		float lineWidth;
	};
}