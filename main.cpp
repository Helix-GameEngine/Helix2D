#include "helix2d.h"
#include <iostream>
#include <easy2d/easy2d.h>
using namespace helix2d;
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
//#include <mferror.h>

void FPS(float s)
{
	static unsigned int num;
	num++;
	static float time = 1000.0f;

	time -= s * 1000.0f;
	if (time <= 0.0f)
	{
		time = 1000.0f;
		printf("%d\n", num);
		num = 0;
	}
}

class A :
	public helix2d::Rect
{
public:

	A() :Rect(50.0f, 50.0f)
	{
		setPos(50.0f, 50.0f);
		speed = 3.5f;
	}
public:

	float speed;
public:

	void Tick(float delta) override
	{
		if (helix2d::Input::isDown(window, helix2d::KeyCode::W))
		{
			movePosY(-speed);
		}
		else if (helix2d::Input::isDown(window, helix2d::KeyCode::S))
		{
			movePosY(speed);
		}
		if (helix2d::Input::isDown(window, helix2d::KeyCode::A))
		{
			movePosX(-speed);
		}
		else if (helix2d::Input::isDown(window, helix2d::KeyCode::D))
		{
			movePosX(speed);
		}

		if (helix2d::Input::isUpPress(window, helix2d::MouseCode::Left))
		{
			setPosX(50.0f);
		}
	}
};

class B :
	public helix2d::Rect
{
public:

	B()
	{
		setPos(100.0f, 100.0f);
		setColor(Color::Orange);
		setAnchor(0.0f, 0.0f);
	}

	void Tick(float delta)override
	{
		//rotate(0.3f);
		movePosX(1.0f);
		//printf("x: %f | y: %f\n", getUpperleftPosX(), getUpperleftPosY());
	}
};

class C :
	public helix2d::Text
{
public:

	C(std::wstring text) :Text(text, Font(L"宋体"))
	{

	}

	void Render()override
	{
		/*auto p = pos;

		pos.x += 300.0f;
		Text::Render();

		pos.y += 300.0f;
		Text::Render();

		pos = p;
		Text::Render();*/
	}

	void Tick(float delta)override
	{
		if (helix2d::Input::isDown(window, helix2d::KeyCode::A))
		{
			movePosX(-2.0f);
		}
		else if (helix2d::Input::isDown(window, helix2d::KeyCode::D))
		{
			movePosX(2.0f);
		}

		if (helix2d::Input::isDown(window, helix2d::KeyCode::W))
		{
			movePosY(-2.0f);
		}
		else if (helix2d::Input::isDown(window, helix2d::KeyCode::S))
		{
			movePosY(2.0f);
		}

		/*if (Input::isUpPress(window, KeyCode::K))
		{
			setText(getText() + L".");
			std::wcout << getText() << L"\n";
		}*/
	}
};

class D :
	public Sprite
{
public:

	D(std::wstring path) : Sprite(path)
	{

	}
public:
	
	void Tick(float delta) override
	{
		setPos(0.0f, 0.0f);
		for (size_t i = 0; i < 100; i++)
		{
			Paint(getPos() + Vector2{i * 10.0f,  i * 10.0f}, angle + 50.0f + i * 10);
		}
	}
};

class E :
	public Rect
{
public:

	E() :Rect(100.0f, 100.0f)
	{
		setPos(100.0f, 0.0f);
	}

	void Tick(float)
	{
		if (helix2d::Input::isDown(window, helix2d::KeyCode::A))
		{
			movePosX(-2.0f);
		}
		else if (helix2d::Input::isDown(window, helix2d::KeyCode::D))
		{
			movePosX(2.0f);
		}

		if (helix2d::Input::isDown(window, helix2d::KeyCode::W))
		{
			movePosY(-2.0f);
		}
		else if (helix2d::Input::isDown(window, helix2d::KeyCode::S))
		{
			movePosY(2.0f);
		}
	}
};

class F :
	public Camera
{
public:

	void Tick(float)
	{
		if (helix2d::Input::isDown(window, helix2d::KeyCode::E))
		{
			rotate(2.0f);
		}
		else if (helix2d::Input::isDown(window, helix2d::KeyCode::Q))
		{
			rotate(-2.0f);
		}
		else if (helix2d::Input::isDown(window,helix2d::KeyCode::X))
		{
			angle = 0.0f;
		}
	}
};

class G :
	public Sprite
{
public:

	G()
	{
		open(L"./src/Alchemist.png");
		setScaleMode(ScaleMode::Neighbor);
		setScale(2.0f, 2.0f);
	}

	void Tick(float)
	{
		if (Input::isDownPress(window, KeyCode::X))
		{
			flipX();
		}

		if (Input::isDownPress(window, KeyCode::Y))
		{
			flipY();
		}
	}
};

class H :
	public Rect
{
public:

	H()
	{
		this->up = KeyCode::W;
		this->down = KeyCode::S;
		this->left = KeyCode::A;
		this->right = KeyCode::D;
		setColor(Color::Orange);
		//box.enableCollision(true);
	}

	H(Color c, KeyCode up, KeyCode down, KeyCode left, KeyCode right) :Rect(50.0f, 50.0f)
	{
		this->up = up;
		this->down = down;
		this->left = left;
		this->right = right;
		setColor(c);
		//box.enableCollision(true);
	}

	void Tick(float)
	{
		if (helix2d::Input::isDown(window, left))
		{
			//addVelocityX(-2.0f);
			movePosX(-2.0f);
		}
		else if (helix2d::Input::isDown(window, right))
		{
			//addVelocityX(2.0f);
			movePosX(2.0f);
		}

		if (helix2d::Input::isDownPress(window, up))
		{
			//movePosY(-5.0f);
			//setVelocity(getVelocity() + Vector2{0.0f, -10.0f * 50.0f});
			addVelocityY(-20.0f * 50.0f);
		}

		if (helix2d::Input::isDown(window, up))
		{
			//addVelocityY(-0.5f * 50.0f);
		}
		
		if (helix2d::Input::isDown(window, down))
		{
			//movePosY(2.0f);
		}
	}

	KeyCode up;
	KeyCode down;
	KeyCode left;
	KeyCode right;
};

class I :
	public Circle
{
public:

	I() : Circle(25.0f, 25.0f)
	{
		//box.enableCollision(true);
		allObj.push_back(this);
		setColor(Color::DarkRed);
	}

	~I()
	{
		allObj.erase(std::find(std::begin(allObj), std::end(allObj), this));
	}

	void Tick(float)
	{
		for (auto& obj : allObj)
		{
			if (isOverlap(obj))
			{
				auto v_i = velocity;
				velocity = velocity.getUnit() *
					(
						mass * velocity.getModulus() +
						obj->mass * obj->velocity.getModulus() +
						0.1f * (obj->velocity.getModulus() - velocity.getModulus())
					) /
					(
						mass + obj->mass
					);

				obj->velocity = obj->velocity.getUnit() *
					(
						mass * v_i.getModulus() +
						obj->mass * obj->velocity.getModulus() +
						0.1f * (v_i.getModulus() - obj->velocity.getModulus())
					) /
					(
						mass + obj->mass
					);
			}
		}
	}

	static std::vector<I*> allObj;
};

std::vector<I*> I::allObj;

int main16512()
{
	Window win{ L"Window",1000,1000 };
	//win.setFullScreen(true);

	H h1{ Color::Yellow,KeyCode::Up,KeyCode::Down,KeyCode::Left,KeyCode::Right };
	h1.setPos(500.0f, 100.0f);
	//h1.setMass(3);

	H h2{ Color::Orange,KeyCode::W,KeyCode::S,KeyCode::A,KeyCode::D };
	h2.setPos(500.0f, 700.0f);
	//h2.setMass(3.0f);

	H h3{ Color::DeepSkyBlue,KeyCode::U,KeyCode::J,KeyCode::H,KeyCode::K };
	//h3.setMass(3.0f);
	h3.setPos(500.0f, 0.0f);

	for (size_t i = 0; i < 15; i++)
	{
		auto* h = new H(Color::Orange, KeyCode::W, KeyCode::S, KeyCode::A, KeyCode::D);
		h->setPos(i * 100.0f, 700.0f);
		//h->getDefaultCollisionBox().setBoxType(Box::Type::Dynamic);
		//h->setMass(3);
		win.addPainter(h);
	}

	/*for (size_t i = 15; i < ((size_t)15 + 15); i++)
	{
		auto* h = new H();
		h->setPos(i * 100.0f, 300.0f);
		h->setMass(3);
		win.addPainter(h);
	}*/

	Camera c;
	

	Gravity g1{ 9.8f * 2 };
	Gravity g2{ 9.8f * 2 };

	h1.addModule(&g1);
	h1.addPainter(&c);

	h3.addModule(&g2);

	win.addPainter(&h1);
	win.addPainter(&h3);
	//win.addPainter(&h2);

	win.setCamera(&c);

	size_t num{};
	DWORD ct = clock();
	while (true)
	{
		std::cout << "Yellow: Velocity: x:" << h1.getVelocity().x << " y: " << h1.getVelocity().y << "\t";
		std::cout << "Blue: Velocity: x:" << h3.getVelocity().x << " y: " << h3.getVelocity().y << "\n";
		/*if (clock() - ct >= 1000)
		{
			std::cout << "Message: h2's RealPos:\nx:" << h2.getWorldPos().x << "\ny:" << h2.getWorldPos().y << std::endl;
			ct += 1000;
		}*/
		Sleep(50);
	}
	return 0;
}

int main999()
{
	Window win{ L"Window",1000,1000 };

	Rect r1;
	Rect r2;
	Rect r3;

	r1.setPos(100.0f, 100.0f);
	r2.setPos(200.0f, 100.0f);
	r3.setPos(300.0f, 100.0f);

	r1.setColor(100);
	r2.setColor(200);
	r3.setColor(300);

	win.addPainter(&r1);
	r1.addPainter(&r2);
	r2.addPainter(&r3);

	while (true)
	{
		Sleep(16);
	}
	return 0;
}

int mainHOMO()
{
	Window win{ L"Physics",1000,1000 };
	I i1;
	I i2;

	i1.setPos(500.0f, 0.0f);
	i2.setPos(500.0f, 500);

	Gravity g;
	i1.addModule(&g);

	win.addPainter(&i1);
	win.addPainter(&i2);
	while (true)
	{

	}
	return 0;
}

int main7898756()
{
	Window win{ L"Input" };

	class input :
		public Painter
	{
	public:

		void Tick(float delta)
		{
			if (Input::isDownPress(window, KeyCode::F))
			{
				std::cout << "F**k\n";
			}

			if (Input::isUpPress(window, KeyCode::L))
			{
				std::cout << "Good Luck\n";
			}
		}
	};

	input i;

	win.addPainter(&i);

	while (true)
	{
		Sleep(16);
	}
	return 0;
}

int main8541365(void)
{
	Window win{ L"Image-Sprite",1000,1000 };

	Image::load(L"./src/Alchemist.png");

	Sprite s{ L"./src/Alchemist.png" };

	s.setPos(100, 100);

	win.addPainter(&s);

	Sprite s2{ L"D:/File/Photos/1.png" };
	s2.setPos(500, 500);

	win.addPainter(&s2);

	while (true)
	{
		Sleep(16);
	}
	return 0;
}

int main96451()
{
	std::vector<std::string> ints = { "Yellow","SkyBlue","Orange" };

	for (size_t i = 0; i < ints.size(); i++)
	{
		auto& int1 = ints[i];

		for (size_t j = i + 1; j < ints.size(); j++)
		{
			auto& int2 = ints[j];

			std::cout << int1 << "\t-  " << int2 << std::endl;
		}
	}
	
	return 0;
}

int main()
{
	class Bullet :
		public Rect
	{
	public:

		Bullet() : Rect(5.0f,20.0f)
		{
			setColor(Color::Yellow);
		}

		void Tick(float dt /*间隔时间*/)
		{
			moveAhead(speed * dt);

			duration += dt;
		}

		float speed = 500.0f;
		float duration = 0.0f;
	};

	class Player;

	class Enemy :
		public Rect
	{
	public:

		Enemy() :Rect(50.0f)
		{
			setPos(500.0f, 500.0f);
			setColor(Color::DarkRed);
		}

		void Tick(float d)
		{
			moveAhead(d * speed);
		}
		float speed = 100.0f;
	};

	class Player :
		public Rect
	{
	public:

		Player() :Rect(50.0f)
		{
			setPos(500.0f, 500.0f);
			setColor(Color::Yellow);
		}

		~Player()
		{
			for (auto& bullet : allBullet)
			{
				window->removePainter(bullet);
				delete bullet;
			}
			allBullet.clear();

			for (auto& enemy : allEnemy)
			{
				window->removePainter(enemy);
				delete enemy;
			}
			allEnemy.clear();
		}

		void Tick(float delta) override
		{
			move();

			generateEnemy(2, delta);

			if (Input::isDownPress(window, KeyCode::Space))
			{
				sendBullet();
			}

			updateEnemy();

			removeBullet();
		}

		void move()
		{
			if (Input::isDown(window, KeyCode::LShift))
			{
				speed = 6.0f;
			}

			if (Input::isRelease(window, KeyCode::LShift))
			{
				speed = 3.0f;
			}

			if (Input::isDown(window, KeyCode::A))
			{
				rotate(-3.0f);
			}
			else if (Input::isDown(window, KeyCode::D))
			{
				rotate(3.0f);
			}

			if (Input::isDown(window, KeyCode::W))
			{
				moveAhead(speed);
			}
		}

		//发射子弹
		void sendBullet()
		{
			auto* b = new Bullet;
			b->setAngle(angle);
			b->setPos(getRealPos());

			allBullet.push_back(b);
			window->addPainter(b);
		}

		void removeBullet()
		{
			for (auto it = allBullet.begin(); it != allBullet.end();)
			{
				if ((*it)->duration >= 2.0f)
				{
					auto b = (*it);
					it = allBullet.erase(it);
					window->removePainter(b);
					delete b;
					//std::cout << "Remove Bullet!\n";
				}
				else
				{
					it++;
				}
			}
			
		}

		void generateEnemy(size_t num, float delta)
		{
			static float t{};
			t += delta;
			if (t > 3.0f)
			{
				for (size_t i = 0; i < num; i++)
				{
					auto* e = new Enemy;
					e->setPosY(i * 10.0f);

					allEnemy.push_back(e);
					window->addPainter(e);
				}
				t -= 3.0f;
			}
		}

		void updateEnemy()
		{
			for (size_t i = 0; i < allEnemy.size();)
			{
				auto* e = allEnemy[i];
				auto d = (getWorldPos() - e->getWorldPos()).getUnit();
				float a = Math::getAngle(acosf(d.x)) + 90.0f;
				e->setAngle(a);

				for (size_t j = 0; j < allBullet.size(); j++)
				{
					if (e->isOverlap(allBullet[j]))
					{
						allEnemy.erase(allEnemy.begin() + i);
						window->removePainter(e);
						delete e;
						e = nullptr;

						auto* b = allBullet[j];
						allBullet.erase((allBullet.begin() + j));
						window->removePainter(b);
						delete b;
						b = nullptr;

						break;
					}
				}

				if (e)
				{
					i++;
				}
			}
		}

		std::vector<Enemy*> allEnemy;
		std::vector<Bullet*> allBullet;
		float speed = 3.0f;
	};

	Window win{ L"Player",1000,1000 };
	win.setBackgroundColor(Color::SkyBlue);

	Player p;
	win.addPainter(&p);

	while (true)
	{
		Sleep(16);
	}
	return 0;
}

int main4865419651()
{
	class FUCK :
		public Painter
	{
	public:

		void Tick(float)
		{
			std::cout << "FUCK\n";
		}
	};

	class R :
		public Painter
	{
	public:

		R()
		{
			p = new FUCK;
			addPainter(p);
		}

		void Tick(float)
		{
			std::cout << "R1\n";

			removePainter(p);
			delete p;
			p = nullptr;
		}

		FUCK* p = nullptr;
	};

	Window win;

	R* r = new R;
	win.addPainter(r);

	while (true)
	{
		Sleep(16);
	}
	return 0;
}

int main48665451()
{
	class W :
		public easy2d::Node
	{
	public:

		W()
		{
			n = easy2d::gcnew Node;
			addChild(n);
		}

		void onUpdate()
		{
			removeChild(n);
		}

		Node* n = nullptr;
	};

	if (easy2d::Game::init())
	{
		auto scene = easy2d::gcnew easy2d::Scene;

		scene->addChild(easy2d::gcnew W);

		easy2d::SceneManager::enter(scene);
		easy2d::Game::start(60);
	}
	easy2d::Game::destroy();

	return 0;
}