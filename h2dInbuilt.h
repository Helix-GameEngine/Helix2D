#pragma once
#include <functional>
#include <vector>
#include <mutex>

namespace helix2d
{
	namespace Inbuilt
	{
		//≥ı ºªØ¿‡
		class Initialize
		{
		private:

			Initialize();

			~Initialize();
		private:

			static bool bGameDone;

			static Initialize init;
		};
	}
}