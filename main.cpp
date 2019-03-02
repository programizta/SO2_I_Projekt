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
	bool moving;
	float color;

public:

	Ball(int xPosition, int yPosition, int velocity, float color)
	{
		this->xPosition = xPosition;
		this->yPosition = yPosition;
		this->velocity = velocity;
		this->color = color;
		direction = rand() % 9;
		moving = false;
	}

	
	~Ball() { }

	int GetxPosition()
	{
		return xPosition;
	}

	int GetYPosition()
	{
		return yPosition;
	}

	void StartMovement()
	{
		moving = true;
	}

	void MotionMechanism()
	{
		if(GetxPosition() == 0 || GetxPosition() == rows - 1) horizontalShift = -horizontalShift;
		if(GetYPosition() == 0 || GetYPosition() == columns - 1) verticalShift = -verticalShift;
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
		//getmaxyx(stdscr, rows, columns);

		for (int i = 0; i < balls.size(); ++i)
		{
			mvprintw(balls[i].GetxPosition(), balls[i].GetYPosition(), "o");
		}

		refresh();
		// odświeżanie
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		for (int i = 0; i < balls.size(); ++i)
		{
			balls[i].MotionMechanism();
		}
	}
}

void CreateBall()
{
	/*while(runningLoop)
	{*/
		getmaxyx(stdscr, rows, columns);
		balls.push_back(*new Ball(rows / 2, columns / 2, 10, 0));
		//std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		//balls.back().StartMovement();
	//}
}

int main(int argc, char const *argv[])
{
	initscr();
	curs_set(0);
	CreateBall();
	RenderScene();
	getch();
    endwin();
	return 0;
}