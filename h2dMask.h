#pragma once
#include "h2dRender.h"

namespace helix2d
{
	class Mask :
		public Painter
	{
	public:

		Mask();

		~Mask();
	public:

		void setWidth(float width);
		void setHeight(float height);

		void setInverse(bool inverse);
	private:

		void Render() override;

		void AfterChildRender() override;
	private:

		ID2D1Layer* pLayer;
		ID2D1GeometrySink* pGeoSink;
		ID2D1PathGeometry* pPathGeo;

		ID2D1Geometry* lastGeometry;

		bool isInverse;
	private:

		static ID2D1Geometry* currGeometry;

		static bool isPushLayer;
	};
}