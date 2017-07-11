////////////////////////////////////////////////////////////////
//
// The MIT License (MIT)
//
// Copyright (c) 2017 M.J.Silk
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//
//
//       ------------
//       INTRODUCTION
//       ------------
//
//   Move/rotate/scale the rectangular object around the window. If it collides with the other rectangular object, it is coloured red, otherwise it is coloured green.
//
//
//       --------
//       CONTROLS
//       --------
//
//   ARROWS			        move
//   < >                    rotate (anti-clockwise/clockwise)
//   [ ]                    scale (shrink/enlarge)
//   SHIFT                  boost speed of move/rotate/scale
//
//
//       -------
//       DEFINES
//       -------
//
//   USE_SPRITES            use sprites (requires USE_TEXTURE)
//   USE_RECTANGLESHAPES    use rectangle shapes
//   USE_TEXTURE            uses texture if defined, uses color only if not defined (must be defined for USE_SPRITES)
//
//   One of either USE_SPRITES or USE_RECTANGLES must be defined but never both
//
//
//       -------
//       TEXTURE
//       -------
//
//   A texture of your choice can be used although one is provided. The path used here is from the root of SfmlSnippets. (SfmlSnippets/resources/images/sw ring rainbow.png)
//
//
////////////////////////////////////////////////////////////////

#include <SFML/Graphics.hpp>

#include "RectangularBoundaryCollision.hpp"

#define USE_TEXTURE
#define USE_SPRITES
//#define USE_RECTANGLESHAPES

int main()
{
	constexpr float movementSpeed{ 150.f };
	constexpr float scaleSpeed{ 1.f };
	constexpr float rotationSpeed{ 75.f };
	constexpr float boostMultiplier{ 3.f };

#ifdef USE_TEXTURE
	sf::Texture texture;
	if (!texture.loadFromFile("resources/images/sw ring rainbow.png"))
		return EXIT_FAILURE;
#endif // USE_TEXTURE

#ifdef USE_RECTANGLESHAPES
	std::vector<sf::RectangleShape> objects(2);
	for (auto& object : objects)
	{
		object.setSize({ 150.f, 100.f });
		object.setOrigin(object.getSize() / 2.f);
#ifdef USE_TEXTURE
		object.setTexture(&texture);
#endif // USE_TEXTURE
	}
	objects[0].setFillColor(sf::Color::Green);
	objects[1].setFillColor(sf::Color(255, 255, 255, 128));
	objects[0].setRotation(10.f);
	objects[1].setRotation(25.f);
#endif // USE_RECTANGLESHAPES
#ifdef USE_SPRITES
	std::vector<sf::Sprite> objects(2, sf::Sprite(texture));
	for (auto& object : objects)
	{
		object.setOrigin(sf::Vector2f(texture.getSize()) / 2.f);
		object.setScale({ 0.5f, 0.5f });
	}
	objects[0].setColor(sf::Color::Green);
	objects[1].setColor(sf::Color(255, 255, 255, 128));
	objects[0].setRotation(10.f);
	objects[1].setRotation(25.f);
#endif // USE_SPRITES

	sf::RenderWindow window(sf::VideoMode(800, 600), "Sprite/Rectangle Collision", sf::Style::Default);
	window.setFramerateLimit(60);

	objects[1].setPosition(sf::Vector2f(window.getSize() / 2u));

	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::Time frameTime{ clock.restart() };
		float dt{ frameTime.asSeconds() };

		const float controlMultiplier{ sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? boostMultiplier : 1.f };

		sf::Vector2i movementControl;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			--movementControl.y;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			++movementControl.y;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			--movementControl.x;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			++movementControl.x;
		sf::Vector2f movement(movementControl);
		if (movementControl.x != 0 && movementControl.y != 0)
			movement *= 0.707f;
		objects[0].move(movement * movementSpeed * controlMultiplier * dt);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket))
			objects[0].scale({ 1.f + (scaleSpeed * controlMultiplier * dt), 1.f + (scaleSpeed * controlMultiplier * dt) });
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket))
			objects[0].scale({ 1.f / (1.f + (scaleSpeed * controlMultiplier * dt)), 1.f / (1.f + (scaleSpeed * controlMultiplier * dt)) });

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
			objects[0].rotate(rotationSpeed * controlMultiplier * dt);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma))
			objects[0].rotate(-rotationSpeed * controlMultiplier * dt);

		const sf::Time collisionStartTime{ clock.getElapsedTime() };
		const bool areColliding{ collision::areColliding(objects[0], objects[1]) }; // this is the collision detection section
		const sf::Time collisionEndTime{ clock.getElapsedTime() };
		const sf::Time collisionDuration{ collisionEndTime - collisionStartTime };

#ifdef USE_RECTANGLESHAPES
		if (areColliding)
			objects[0].setFillColor(sf::Color::Red);
		else
			objects[0].setFillColor(sf::Color::Green);
#endif // USE_RECTANGLESHAPES
#ifdef USE_SPRITES
		if (areColliding)
			objects[0].setColor(sf::Color::Red);
		else
			objects[0].setColor(sf::Color::Green);
#endif // USE_SPRITES

		window.setTitle("collision duration: " + std::to_string(collisionDuration.asMicroseconds()) + "\n");

		window.clear();
		for (auto& object : objects)
			window.draw(object);
		window.display();
	}
}
