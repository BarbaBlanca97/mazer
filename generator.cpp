#include <SFML/Graphics.hpp>
#include <stack>

enum CellType
{
	WALL,
	WALL_REMOVED,
	UNVISITED,
	VISITED
};

struct Cell
{
	CellType type;
	int x;
	int y;
};

int main(int argc, char** args)
{
	if (argc < 3)
		return -1;

	std::srand(std::time(0));

	int
		mazeWidth = std::stoi(args[1]),
		mazeHeight = std::stoi(args[2]);

	if (mazeWidth % 2 == 0) mazeWidth++;
	if (mazeHeight % 2 == 0) mazeHeight++;

	std::vector<std::vector<Cell>> maze(mazeWidth, std::vector<Cell>(mazeHeight));

	for (int i = 0; i < mazeWidth; i++) {
		for (int j = 0; j < mazeHeight; j++) {
			if (i % 2 == 0 || j % 2 == 0)
				maze[i][j] = { CellType::WALL, i, j };
			else
				maze[i][j] = { CellType::UNVISITED, i, j };
		}
	}

	int randomInt = 3;

	int initX = 1 + (2 * (int)((float)std::rand() / (float)RAND_MAX * ((mazeWidth / 2) - 1)));
	int initY = 1 + (2 * (int)((float)std::rand() / (float)RAND_MAX * ((mazeHeight / 2) - 1)));

	std::stack<Cell> cellStack;
	maze[initX][initY].type = CellType::VISITED;
	cellStack.push(maze[initX][initY]);

	while (!cellStack.empty()) {

		Cell currentCell = cellStack.top();
		cellStack.pop();

		int rndOffset = rndOffset = std::round((float)std::rand() / (float)RAND_MAX * 4);
		for (int i = 0; i < 4; i++) {
			int xDir, yDir;

			switch ((i + rndOffset) % 4) {
				case 0: xDir = 0; yDir = 2; break;
				case 1: xDir = 2; yDir = 0; break;
				case 2: xDir = 0; yDir = -2; break;
				case 3: xDir = -2; yDir = 0; break;
			}
			int nPosX = currentCell.x + xDir, nPosY = currentCell.y + yDir;

			if (nPosX < 0) nPosX = 0;
			else if (nPosX >= mazeWidth) nPosX = mazeWidth - 1;

			if (nPosY < 0) nPosY = 0;
			else if (nPosY >= mazeHeight) nPosY = mazeHeight - 1;

			Cell curNeighbour = maze[nPosX][nPosY];

			if (curNeighbour.type == UNVISITED) {

				cellStack.push(curNeighbour);

				maze[currentCell.x + (xDir / 2)][currentCell.y + (yDir / 2)].type = CellType::WALL_REMOVED;

				curNeighbour.type = CellType::VISITED;
				maze[nPosX][nPosY] = curNeighbour;
				cellStack.push(curNeighbour);

				break;	
			}
		}
	}

	for (int i = 1; i < mazeWidth - 1; i++) 
		if (maze[i][mazeHeight - 2].type != CellType::WALL) {
			maze[i][mazeHeight - 1].type = CellType::WALL_REMOVED; 
			break;
		}

	for (int i = mazeWidth - 1; i > 1; i--)
		if (maze[i][1].type != CellType::WALL) {
			maze[i][0].type = CellType::WALL_REMOVED;
			break;
		}

	sf::Image mazeImage;
	mazeImage.create(mazeWidth, mazeHeight, sf::Color::Black);

	for (int i = 0; i < mazeWidth; i++) {
		for (int j = 0; j < mazeHeight; j++) {
			if (maze[i][j].type != CellType::WALL)
				mazeImage.setPixel(i, j, sf::Color::White);
		}
	}

	if (mazeImage.saveToFile("maze.png"))
		return 0;
	else
		return -1;

}