#include <ncurses.h>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>

int rows = 0;
int columns = 0;

class Ball
{
	int horizontalShift = 1;
	int verticalShift = 1;
	int xPosition;
	int yPosition;
	int direction;
	int velocity;
	int steps;

public:

	Ball(int xPosition, int yPosition, int velocityChoice)
	{
		this->xPosition = xPosition;
		this->yPosition = yPosition;
		InitializeDirection(rand() % 8 + 1);
		InitializeSpeed(velocityChoice);
		steps = 0;
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
			case 1: this->velocity = 80;
			// wolniej
			case 2: this->velocity = 140;
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
		while(steps <= 100)
		{
			steps++;
			if(GetXPosition() == 0)
			{
				direction = rand() % 3 + 1;
				switch(direction)
				{
					case 1: MoveRight(); break;
					case 2: MoveUpperRight(); break;
					case 3: MoveLowerRight(); break;
				}
			}
			else if(GetXPosition() == rows - 1)
			{
				direction = rand() % 3 + 1;
				switch(direction)
				{
					case 1: MoveLeft(); break;
					case 2: MoveUpperLeft(); break;
					case 3: MoveLowerLeft(); break;
				}
			}
			else if(GetYPosition() == 0)
			{
				direction = rand() % 3 + 1;
				switch(direction)
				{
					case 1: MoveLowerLeft(); break;
					case 2: MoveDown(); break;
					case 3: MoveLowerRight(); break;
				}
			}
			else if(GetYPosition() == columns - 1)
			{
				direction = rand() % 3 + 1;
				switch(direction)
				{
					case 1: MoveUpperLeft(); break;
					case 2: MoveUp(); break;
					case 3: MoveUpperRight(); break;
				}
			}
			DisplaceBall();
			std::this_thread::sleep_for(std::chrono::milliseconds(velocity));
		}
	}

	std::thread MotionThread()
	{
		return std::thread(&Ball::ChangeDirection, this);
	}
};

std::vector<Ball> balls;
std::vector<std::thread> threadsOfBalls;
bool runningLoop = true;

void CreateBall()
{
	while(runningLoop)
	{
		getmaxyx(stdscr, rows, columns);
		balls.push_back(*new Ball(rows / 2, columns / 2, rand() % 3 + 1));
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));	
	}
}

void TerminateAllThreads()
{
	for (int i = 0; i < threadsOfBalls.size(); ++i)
	{
		threadsOfBalls[i].join();
	}
}

void RenderScene()
{
	while(runningLoop)
	{
		clear();
		for (int i = 0; i < balls.size(); ++i)
		{
			mvprintw(balls[i].GetXPosition(), balls[i].GetYPosition(), "o");
		}

		refresh();
		// odświeżanie
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		for (int i = 0; i < balls.size(); ++i)
		{
			balls[i].ChangeDirection();
		}
	}
}

int main(int argc, char const *argv[])
{
	int numOfBalls = 0;
	srand(time(NULL));
	initscr();
	curs_set(0);
	std::thread scene(RenderScene);
	std::thread createBalls(CreateBall);
	scene.join();
	createBalls.join();
	TerminateAllThreads();
	getch();
    endwin();
	return 0;
}