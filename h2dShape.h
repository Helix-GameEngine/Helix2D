#pragma once
#include "h2dRender.h"


namespace helix2d
{
	class Circle :
		public Painter
	{
	public:

		Circle();

		Circle(float radiusX, float radiusY);

		Circle(float radius);

	public:

		void Render() override;
	};

	class Rect :
		public Painter
	{
	public:

		Rect();

		Rect(float width, float height);

		Rect(float side);
	public:

		void setWidth(float width);
		void setHeight(float height);
	public:

		void Render()override;
	};
}