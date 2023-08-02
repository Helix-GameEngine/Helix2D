#pragma once

namespace helix2d
{
	namespace Inbuilt
	{
		//≥ı ºªØ¿‡
		class Initialize
		{
		public:

			static void function(void);
		private:

			Initialize();

			~Initialize();
		private:

			static bool bGameDone;

			static Initialize init;
		};
	}
}