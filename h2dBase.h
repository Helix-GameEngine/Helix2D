#pragma once
#include "h2dMacros.h"
#include "h2dRender.h"
#include "h2dTool.h"
#include "h2dInbuilt.h"
#include <string>
#include <vector>
#include <map>

namespace helix2d
{
	class Window;
	class Camera;
	
	//窗口
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

		//添加画家
		void addPainter(Painter* pPainter);	
		void addPainter(Painter* pPainter, std::wstring name);

		//移除画家
		bool removePainter(Painter* pPainter);

		//从子画家中查找画家
		Painter* findPainter(std::wstring name);
	public:

		//设置窗口背景颜色
		void setBackgroundColor(const Color& c);

		//设置期望帧率
		void setFPS(unsigned int fps);

		//设置摄像机（已添加的摄像机）
		void setCamera(Camera* camera);

		//设置是否全屏
		void setFullScreen(bool fullScreen);
	public:

		//获取宽度
		unsigned int getWidth()const;
		//获取高度
		unsigned int getHeight()const;

		//获取窗口句柄
		HWND getHWND()const;

		//获取渲染器
		Renderer* getRenderer()const;

		//获取所有已添加画家
		std::vector<Painter*> getAllPainter()const;

		//获取摄像机
		Camera* getCamera()const;
	public:

		static void uninit();
	protected:

		virtual void Tick(float deltaTime) {};
	private:

		//创建窗口
		void create();

		//设置透明模式
		void setAlphaWindow();

		//更新窗口
		void Update(float delta);

		//清除输入
		void clearInput();
	private:

		//查找窗口
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
	private:

		static void winControl(Window* window);

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}