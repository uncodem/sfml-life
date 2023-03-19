#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>

#include <cstdlib>
#include <ctime>

#include "type.hpp"
#include "icon.c"

#define B_SIZE 100
#define W_SIZE 800

#define get_indx(x,y) y * B_SIZE + x
const int c_size = W_SIZE / B_SIZE;

void init_map(std::vector<Cell>& map) {
	
	for (int y = 0; y < B_SIZE; y++) {
		for (int x = 0; x < B_SIZE; x++) {
			map[get_indx(x, y)].shape.setPosition(sf::Vector2f(x*c_size, y*c_size));
			map[get_indx(x, y)].shape.setSize(sf::Vector2f(c_size, c_size));
			map[get_indx(x, y)].live = (rand() % 50) % 2 == 0;
		}
	}
}

void r_drawmap(sf::RenderWindow& window, std::vector<Cell>& map) {
	for (Cell c : map) {
		c.shape.setFillColor((c.live) ? sf::Color::White : sf::Color::Black);
		window.draw(c.shape);
	}
}

int wrap(int n, int max) {
	if (n == -1) return max - 1;
	if (n == max) return 0;
	return n;
}

int count_neighbors(int x, int y, std::vector<Cell>& map) {
	int neighbors = 0;
	neighbors -= (map[get_indx(x, y)].live) ? 1 : 0;
	for (int cy = -1; cy < 2; cy++) {
		for (int cx = -1; cx < 2; cx++) {
			int ox = wrap(cx + x, B_SIZE);
			int oy = wrap(cy + y, B_SIZE);
			neighbors += (map[get_indx(ox, oy)].live) ? 1 : 0;
		}
	}
	return neighbors;
}

void s_drawmap(std::vector<Cell>& map, sf::RenderWindow& window, std::stack<CellChange>& stack, bool skip = false) {
	if (!skip) r_drawmap(window, map);
	while (!stack.empty()) {
		CellChange change = stack.top();
		stack.pop();
		Cell& cell = map[get_indx(change.first.x, change.first.y)];
		cell.live = change.second;
		cell.shape.setFillColor((cell.live) ? sf::Color::White : sf::Color::Black);
		window.draw(cell.shape);
	}
}

void updatemap(std::stack<CellChange>& stack, std::vector<Cell>& map, bool changes_only = false) {
	for (int y = 0; y < B_SIZE; y++) {
		for (int x = 0; x < B_SIZE; x++) {
			int n = count_neighbors(x, y, map);
			Cell c = map[get_indx(x, y)];
			int state = c.live;
			if (c.live && (n < 2 || n > 3)) state = false;
			else if (n == 3) state = true;
			if (state != c.live || (state && c.live && !changes_only)) stack.push(std::make_pair(sf::Vector2f(x, y), state));
		}
	}
}

int main() {
	sf::RenderWindow window(sf::VideoMode(W_SIZE, W_SIZE), "Game of Life");
	window.setIcon(icon_image.width, icon_image.height, icon_image.pixel_data);
	std::vector<Cell> map(B_SIZE*B_SIZE);
	std::stack<CellChange> changes;
	
	int fps_limit = 30;
	bool updating = true;
	bool skip = true;
	bool onlychanges = false;

	srand(time(NULL));

	init_map(map);

	window.setFramerateLimit(fps_limit);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyReleased:
					if (event.key.code == sf::Keyboard::Space) {
						updating = !updating;
					}
					else if (event.key.code == sf::Keyboard::E && !updating) {
						for (Cell& c : map) {
							c.live = (rand() % 50) % 2 == 0;
						}
					}
					else if (event.key.code == sf::Keyboard::R && !updating) {
						for (Cell& c : map) {
							c.live = false;
						}
					}
					else if (event.key.code == sf::Keyboard::Up) {
						if (fps_limit == 120) break;
						fps_limit += 5;
						window.setFramerateLimit(fps_limit);
					}
					else if (event.key.code == sf::Keyboard::Down) {
						if (fps_limit == 5) break;
						fps_limit -= 5;
						window.setFramerateLimit(fps_limit);
					}
					else if (event.key.code == sf::Keyboard::A) {
						skip = !skip;
						if (skip && onlychanges) onlychanges = false;
					}
					else if (event.key.code == sf::Keyboard::F) {
						fps_limit = 30;
						window.setFramerateLimit(fps_limit);
					} else if (event.key.code == sf::Keyboard::C) {
						onlychanges = !onlychanges;
						if (onlychanges && !skip) skip = true;
					}
					break;
				case sf::Event::MouseButtonPressed:
					if (!updating && event.mouseButton.button == sf::Mouse::Left) {
						int x = (double)event.mouseButton.x / c_size;
						int y = (double)event.mouseButton.y / c_size;
						if (x >= 0 && x <= B_SIZE && y >= 0 && y <= B_SIZE) {
							map[get_indx(x, y)].live = !map[get_indx(x, y)].live;
						}
					}
					break;
				default:
					break;
			}
		}
		window.clear();
		if (!updating) {
			r_drawmap(window, map);
		}
		else {
			updatemap(changes, map, onlychanges);
			s_drawmap(map, window, changes, skip);
		}
		window.display();
	}

	return 0;
}
