#include <ncurses.h>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>

int rows = 0;
int columns = 0;
bool runningLoop = true;

class Ball
{
	int horizontalShift = 1;
	int verticalShift = 1;
	int xPosition;
	int yPosition;
	int direction;
	int velocity;
	int choiceSpeed;

public:

	Ball(int xPosition, int yPosition)
	{
		this->xPosition = xPosition;
		this->yPosition = yPosition;
		InitializeDirection(rand() % 8 + 1);
		// tu jest coś nie tak, napraw prędkość!
		this->choiceSpeed = rand() % 3 + 1;
		InitializeSpeed(choiceSpeed);
		
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
		switch(choice)
		{
			// bardzo szybko
			case 1: this->velocity = 50;
			// wolniej
			case 2: this->velocity = 100;
			// najwolniej
			case 3: this->velocity = 200;
		}
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

	void ChangeDirection()
	{
		while(runningLoop)
		{
			direction = rand() % 3 + 1;
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
			DisplaceBall();
			std::this_thread::sleep_for(std::chrono::milliseconds(this->velocity));
		}
	}

	std::thread MotionThread()
	{
		return std::thread(&Ball::ChangeDirection, this);
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
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));	
	}
}

void TerminateBallThreads()
{
	for (int i = 0; i < threadsOfBalls.size(); ++i)
	{
		threadsOfBalls[i].join();
	}
}

void PressKeyToEnd()
{
	// zakończ program na wciśnięcie klawisza
	while(getch() == 13) { std::this_thread::sleep_for(std::chrono::milliseconds(20)); }
	runningLoop = false;
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
	srand(time(NULL));
	initscr();
	curs_set(0);
	std::thread scene(RenderScene);
	std::thread createBalls(CreateBall);
	std::thread exitProgram(PressKeyToEnd);


	// zawieszanie wątku głównego dopóki nie wciśnięto klawisza
	while(runningLoop) { std::this_thread::sleep_for(std::chrono::milliseconds(500)); }
	TerminateBallThreads();
	scene.join();
	createBalls.join();
	exitProgram.join();
	endwin();
	return 0;
}