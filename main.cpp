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
	float color;

public:

	Ball(int xPosition, int yPosition, int velocity, float color)
	{
		this->xPosition = xPosition;
		this->yPosition = yPosition;
		this->velocity = velocity;
		this->color = color;
		direction = rand() % 9;
	}

	
	~Ball() { }

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

	void ChangeDirection()
	{
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
		xPosition += horizontalShift;
		yPosition += verticalShift;
	}
};

bool runningLoop = true;
std::vector<Ball> balls;

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

void CreateBall()
{
	getmaxyx(stdscr, rows, columns);
	balls.push_back(*new Ball(rows / 2, columns / 2, 10, 0));
}

int main(int argc, char const *argv[])
{
	srand(time(NULL));
	initscr();
	curs_set(0);
	CreateBall();
	RenderScene();
	getch();
    endwin();
	return 0;
}