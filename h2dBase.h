#pragma once
#include "h2dMacros.h"
#include "h2dRender.h"
#include "h2dTool.h"
#include "h2dInbuilt.h"
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>

namespace helix2d
{
	class Window;
	class Camera;
	
	class Window
	{
		friend class Renderer;
		friend class Camera;
		friend class Input;
		friend class Inbuilt::Initialize;
	public:

		Window(
			std::wstring title = L"Helix2D",
			unsigned int width = 600, 
			unsigned int height = 600,
			Window* parent = nullptr, 
			unsigned int fps = 60
		);

		~Window();
	public:

		void addPainter(Painter* pPainter);	
	public:

		void setBackgroundColor(const Color& c);

		void setFPS(unsigned int fps);

		void setCamera(Camera* camera);

		void setFullScreen(bool fullScreen);
	public:

		unsigned int getWidth()const;
		unsigned int getHeight()const;

		HWND getHWND()const;

		Renderer* getRenderer()const;

		std::vector<Painter*> getAllPainter()const;

		Camera* getCamera()const;
	public:

		static void uninit();
	private:

		void create();

		void update(float delta);

		void clearInput();
	private:

		static Window* findWindow(HWND hWnd);
	private:

		WNDCLASSEXW wc;

		HWND hWnd;

		std::wstring title;

		int width;
		int height;

		Window* parent;

		bool bCreated;

		bool bFullScreen;

		unsigned int fps;

		bool bWindowDone;

		bool bIsDragging;

		float lastUpdateTime;
	private:

		Renderer* renderer;
	private:

		std::vector<Painter*> painterList;

		Camera* camera;
	private:

		Color backgroundColor = Color::White;
	private:

		std::vector<KeyCode> downKey;

		std::vector<KeyCode> downPressKey;

		std::vector<KeyCode> upPressKey;

		std::map<KeyCode, float> downKeyTime;
	private:

		std::vector<MouseCode> downMouse;

		std::vector<MouseCode> downPressMouse;

		std::vector<MouseCode> upPressMouse;

		std::map<MouseCode, float> downMouseTime;
	private:

		static size_t ordinal;

		static bool bAllWindowDone;

		static std::vector<Window*> allWindows;

		static std::mutex mtx_allwin;
	private:

		static void winControl(Window* window);

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}