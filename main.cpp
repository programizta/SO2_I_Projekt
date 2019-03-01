#include <ncurses.h>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <thread>

class Ball
{
	char figure = 'o';
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
		moving = false;
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

	void StartMovement()
	{
		moving = true;
	}

	char GetFigure()
	{
		return figure;
	}
};

int rows = 0;
int columns = 0;
bool running = true;
std::vector<Ball> balls;
std::vector<std::thread> ballsThreads;

void DrawScene()
{
	while(running)
	{
		clear();
		getmaxyx(stdscr, rows, columns);
		for (int i = 0; i < balls.size(); ++i)
		{
			mvprintw(balls[i].GetXPosition(), ((balls[i].GetYPosition() / 2) - (sizeof('o' / 2))), "o");
		}
		refresh();
		// odświeżanie co 0.01s
		usleep(10000);
	}
}

int main(int argc, char const *argv[])
{
	Ball *b = new Ball(30, 50, 0, 0);
	initscr();
	getch();
	endwin();

	return 0;
}