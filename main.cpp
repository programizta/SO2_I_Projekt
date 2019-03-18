#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <ncurses.h>

thread_local std::mt19937 gen{ std::random_device{}() };

// szablonik funkcji do generowania liczb
// zgodnie z rozkładem jednostajnym
template<typename var>
var random(var min, var max) {
	using dist = std::conditional_t<
		std::is_integral<var>::value,
		std::uniform_int_distribution<var>,
		std::uniform_real_distribution<var>
	>;
	return dist{ min, max }(gen);
}

int rows = 0;
int columns = 0;
bool runningLoop = true;

class Ball
{
	int horizontalShift;
	int verticalShift;
	int xPosition;
	int yPosition;
	int direction;
	int velocity;

public:

	Ball(int xPosition, int yPosition)
	{
		horizontalShift = 1;
		verticalShift = 1;
		this->xPosition = xPosition;
		this->yPosition = yPosition;
		InitializeDirection(random(1, 8));
		InitializeSpeed(random(100, 300));
	}

	~Ball() { }

	void InitializeDirection(int choice)
	{
		switch(choice)
		{
			case 1: MoveLeft(); break;
			case 2: MoveUpperLeft(); break;
			case 3: MoveLowerLeft(); break;
			case 4: MoveRight(); break;
			case 5: MoveUpperRight(); break;
			case 6: MoveLowerRight(); break;
			case 7: MoveUp(); break;
			case 8: MoveDown(); break;
		}
	}

	void InitializeSpeed(int choice)
	{
		if(choice % 3 == 0) velocity = 50;
		else if(choice % 3 == 1) velocity = 150;
		else velocity = 200;
	}

	int GetXPosition()
	{
		return xPosition;
	}

	int GetYPosition()
	{
		return yPosition;
	}

	void MoveRight()
	{
		horizontalShift = 1;
		verticalShift = 0;
	}

	void MoveUpperRight()
	{
		horizontalShift = 1;
		verticalShift = -1;
	}

	void MoveLowerRight()
	{
		horizontalShift = 1;
		verticalShift = 1;
	}

	void MoveLeft()
	{
		horizontalShift = -1;
		verticalShift = 0;
	}

	void MoveUpperLeft()
	{
		horizontalShift = -1;
		verticalShift = -1;		
	}

	void MoveLowerLeft()
	{
		horizontalShift = -1;
		verticalShift = 1;
	}

	void MoveUp()
	{
		horizontalShift = 0;
		verticalShift = -1;
	}

	void MoveDown()
	{
		horizontalShift = 0;
		verticalShift = 1;
	}

	void DisplaceBall()
	{
		xPosition += horizontalShift;
		yPosition += verticalShift;
	}

	void BallReflection()
	{
		while(runningLoop)
		{
			direction = random(1, 3);
			if(GetXPosition() == 0)
			{
				switch(direction)
				{
					case 1: MoveRight(); break;
					case 2: MoveUpperRight(); break;
					case 3: MoveLowerRight(); break;
				}
			}
			else if(GetXPosition() == rows - 1)
			{
				switch(direction)
				{
					case 1: MoveLeft(); break;
					case 2: MoveUpperLeft(); break;
					case 3: MoveLowerLeft(); break;
				}
			}
			else if(GetYPosition() == 0)
			{
				switch(direction)
				{
					case 1: MoveLowerLeft(); break;
					case 2: MoveDown(); break;
					case 3: MoveLowerRight(); break;
				}
			}
			else if(GetYPosition() == columns - 1)
			{
				switch(direction)
				{
					case 1: MoveUpperLeft(); break;
					case 2: MoveUp(); break;
					case 3: MoveUpperRight(); break;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(velocity));
			DisplaceBall();
		}
	}

	std::thread MotionThread()
	{
		return std::thread(&Ball::BallReflection, this);
	}
};

std::vector<Ball*> balls;
std::vector<std::thread> threadsOfBalls;

void CreateBall()
{
	while(runningLoop)
	{
		getmaxyx(stdscr, rows, columns);
		balls.push_back(new Ball(rows / 2, columns / 2));
		threadsOfBalls.push_back(balls.back()->MotionThread());
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
}

void TerminateThreadsOfBalls()
{
	for (int i = 0; i < threadsOfBalls.size(); ++i)
	{
		threadsOfBalls[i].join();
	}
}

void PressKeyToEnd()
{
	// zakończ program gdy wciśnięto klawisz [ENTER]
	while(runningLoop)
	{
		char key = getchar();
		if (key == 13) runningLoop = false;
		else std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void RenderScene()
{
	while(runningLoop)
	{
		clear();
		for (int i = 0; i < balls.size(); ++i)
		{
			mvprintw(balls[i]->GetXPosition(), balls[i]->GetYPosition(), "o");
		}

		refresh();
		// odświeżanie
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

int main(int argc, char const *argv[])
{
	initscr();
	curs_set(0);
	std::thread scene(RenderScene);
	std::thread createBalls(CreateBall);
	std::thread exitProgram(PressKeyToEnd);

	// zawieszanie wątku głównego dopóki animacja trwa
	while(runningLoop) { std::this_thread::sleep_for(std::chrono::milliseconds(500)); }
	TerminateThreadsOfBalls();
	scene.join();
	createBalls.join();
	exitProgram.join();
	endwin();
	return 0;
}