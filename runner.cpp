#include <SFML/Graphics.hpp>

struct Position {
	int x;
	int y;

	bool operator!=(const Position& other) {
		return this->x != other.x || this->y != other.y;
	}

	bool operator==(const Position& other) {
		return !(*this != other);
	}

};

enum CellType { VISITED, WALL, EMPTY };

int main(int argc, char** argv)
{

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

	if (!image.loadFromFile(argv[1])) {
		return -1;
	}

	Position imageSize = { image.getSize().x, image.getSize().y };
	sf::RenderWindow window(sf::VideoMode(500 * ((float)imageSize.x / (float)imageSize.y), 500), "Maze runner");

	int pixelsSize = imageSize.x * imageSize.y * 4;

	const sf::Uint8 * pixels = image.getPixelsPtr();

	texture.loadFromImage(image);
	sprite.setTexture(texture);

	Position initialPosition, finalPosition;
	CellType ** cells = new CellType*[imageSize.x]; 
	for (int i = 0; i < imageSize.x; i++) { cells[i] = new CellType[imageSize.y]; }

	for (int i = 0; i < imageSize.x * imageSize.y * 4; i += 4) {
		int fila = i / (imageSize.x * 4);
		int columna =  (i / 4 ) % imageSize.x;

		sf::Color curColor(	pixels[i],
							pixels[i + 1],
							pixels[i + 2],
							pixels[i + 3]
		);

		if (curColor == sf::Color::White)
			cells[columna][fila] = CellType::EMPTY;
		else
			cells[columna][fila] = CellType::WALL;

		if (fila == (imageSize.y - 1) && curColor == sf::Color::White)
			initialPosition = { columna, fila};
		else if (fila == 0 && curColor == sf::Color::White)
			finalPosition = { columna, fila };
	}

	sf::Uint8 redPixel[4] = { 242, 38, 19, 255 };
	sf::Uint8 whitePixel[4] = { 255, 255, 255, 255 };

	texture.update(redPixel, 1, 1, initialPosition.x, initialPosition.y);

	sf::View view; view.reset(sf::FloatRect(0, 0, image.getSize().x, image.getSize().y));
	window.setView(view);

	Position curPosition = initialPosition, prevPosition;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed) {
				bool noCollision = false;

				switch (event.key.code)
				{
					case sf::Keyboard::Up:
						if (curPosition.y != 0 && cells[curPosition.x][curPosition.y - 1] != CellType::WALL) {
							prevPosition = curPosition;
							curPosition.y -= 1;

							noCollision = true;
						}
						break;

					case sf::Keyboard::Right:
						if (cells[curPosition.x + 1][curPosition.y] != CellType::WALL) {
							prevPosition = curPosition;
							curPosition.x += 1;

							noCollision = true;
						}
						break;

					case sf::Keyboard::Down:
						if (curPosition != initialPosition && cells[curPosition.x][curPosition.y + 1] != CellType::WALL) {
							prevPosition = curPosition;
							curPosition.y += 1;

							noCollision = true;
						}
						break;

					case sf::Keyboard::Left:
						if (cells[curPosition.x - 1][curPosition.y] != CellType::WALL) {
							prevPosition = curPosition;
							curPosition.x -= 1;

							noCollision = true;
						}
						break;
				}

				if (noCollision) {
					if (cells[curPosition.x][curPosition.y] == CellType::VISITED) {
						texture.update(whitePixel, 1, 1, prevPosition.x, prevPosition.y);
						cells[prevPosition.x][prevPosition.y] = CellType::EMPTY;
					}
					else {
						texture.update(redPixel, 1, 1, curPosition.x, curPosition.y);
						cells[curPosition.x][curPosition.y] = CellType::VISITED;
					}
				}

				if (curPosition == finalPosition) {
					image = texture.copyToImage();
					image.saveToFile("maze-solved.png");
					window.close();
				}
			}
		}

		window.clear();
		window.draw(sprite);
		window.display();
	}

	return 0;
}