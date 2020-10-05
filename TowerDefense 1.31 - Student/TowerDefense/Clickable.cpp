#include "Clickable.h"
#include <SFML/Graphics.hpp>
#include <iostream>

/*
 Clickable is an interface to be extended by objects
 This will be the core behind the Tower, Buttons, and Tiles.
 It is important to note that all clic  kable objects
 must be associated with a drawable & transformable object,
 as well as a texture to be mapped to it
 */
using sf::Vector2f;
using std::cout;
using std::endl;

Clickable::Clickable(Vector2f _position, Vector2f _size) {
	position = _position;
	size = _size;
}

Clickable::~Clickable() {
	// TODO Auto-generated destructor stub
}

void Clickable::setPosition(const sf::Vector2f& _position) {
	position = _position;
	update();
}

const sf::Vector2f& Clickable::getPosition() const {
	return position;
}

void Clickable::setSize(const sf::Vector2f& _size) {
	size = _size;
	update();
}

const sf::Vector2f& Clickable::getSize() const {
	return size;
}

void Clickable::setDebug(bool mode) {
	debug = mode;
}

bool Clickable::insideShape(const sf::Vector2i& mousePos) {
	if (((mousePos.x >= position.x) && (mousePos.x <= (position.x + size.x)))) {
		if ((mousePos.y >= position.y)
				&& (mousePos.y <= (position.y + size.y))) {
			return true;
		}
	}
	return false;
}

// This references the current event and mouse position, to
// determine whether the mouse is currently within the clickable object
void Clickable::process(const sf::Event& event, const sf::Vector2i& mousePos) {
	// Determine if the mouse is inside the bound
	if (insideShape(mousePos)) {
		if (isHovering == 0) {
			isHovering = 1;
			onMouseEnter();
			update();
		}
		if ((event.type == sf::Event::MouseButtonPressed)
				&& (event.mouseButton.button == sf::Mouse::Left)) {
			isPressed = 1;
			onMousePressed();
			update();
		} else if ((event.type == sf::Event::MouseButtonReleased)
				&& (event.mouseButton.button == sf::Mouse::Left) && isPressed) {
			onClick();
			update();
		}
		if (debug && (sf::Mouse::isButtonPressed(sf::Mouse::Left))) {
			sf::Vector2f temp = getSize();
			setPosition(
					sf::Vector2f(mousePos.x - (temp.x / 2),
							mousePos.y - (temp.y / 2)));
			update();
		}
	} else if (isHovering == 1) {
		isHovering = 0;
		isPressed = 0;
		onMouseLeave();
		update();
	}
}

void Clickable::onMouseEnter() {
}
void Clickable::onMousePressed() {
}
void Clickable::onMouseLeave() {
}
