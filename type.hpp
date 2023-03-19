#ifndef TYPE_HPP_
#define TYPE_HPP_

#include <utility>
#include <SFML/Graphics.hpp>

typedef std::pair<sf::Vector2f, int> CellChange;

class Cell {
public:
	int live;
	sf::RectangleShape shape;
	Cell(int x, int y, int w, int h);
	Cell();
};

#endif