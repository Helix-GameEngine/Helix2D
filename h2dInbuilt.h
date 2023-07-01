#pragma once
#include <functional>
#include <vector>
#include <mutex>

namespace helix2d
{
	namespace Inbuilt
	{
		class Initialize
		{
		private:

			Initialize();

			~Initialize();
		private:

			static void RenderResourcesCreater();
		private:

			static bool bGameDone;

			static Initialize init;
		};

		class QueueMessage
		{
		public:
			static std::vector<std::function<void()>> msgPeek;

			static std::mutex mtx_msgPeek;
		};
	}
}