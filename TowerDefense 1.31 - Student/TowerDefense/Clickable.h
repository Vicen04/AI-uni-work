#ifndef CLICKABLE_H_
#define CLICKABLE_H_

#include <SFML/Graphics.hpp>

/*
 Clickable is an interface to be extended by objects
 This will be the core behind the Tower, Buttons, and Tiles.
 It is important to note that all clic  kable objects
 must be associated with a drawable & transformable object,
 as well as a texture to be mapped to it
 */
class Clickable {
public:
	Clickable(sf::Vector2f _position = sf::Vector2f(0, 0), sf::Vector2f _size =
			sf::Vector2f(100, 100));
	virtual ~Clickable();

	const sf::Vector2f& getPosition() const;
	void setPosition(const sf::Vector2f& _position);

	const sf::Vector2f& getSize() const;
	void setSize(const sf::Vector2f& _size);

	// This references the current event and mouse position, to
	// determine whether the mouse is currently within the clickable object
	void process(const sf::Event& event, const sf::Vector2i& mousePos);

	// For efficiency, this should only be called when a change
	// is detected, ie by the process function
	virtual void update() = 0;

	// Draw the button, relying on textures/shapes defined
	// by the class that implements this interface.
	virtual void render(sf::RenderWindow& _window) = 0;

	void setDebug(bool mode);
private:
	bool isHovering = 0, isPressed = 0, debug = 0;
	sf::Vector2f position, size;
	virtual bool insideShape(const sf::Vector2i& mousePos);

	// Default action simply changes the opacity of a button
	virtual void onMouseEnter();
	virtual void onMousePressed();
	virtual void onMouseLeave();
	// This will be called when a button is confirmed to be clicked
	virtual void onClick() = 0;
};

#endif /* CLICKABLE_H_ */
