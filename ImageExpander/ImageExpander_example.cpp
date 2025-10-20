////////////////////////////////////////////////////////////////
//
// The MIT License (MIT)
//
// Copyright (c) 2025 M.J.Silk
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
//   Loads a small (5x5 tiles) tileset into an sf::Image
//   Duplicates the image for comparison
//   Expands each tile in the set individually
//   Shows both original tileset and the processed side-by-side
//
//
//       --------
//       CONTROLS
//       --------
//
//   SPACE                  toggle texture for processed image (right); toggles between processed texture (default) and the original texture
//   ESC                    quit
// 
// 
//        ----
//        NOTE
//        ----
//
//    The texture is available in the resources folder, which is in the root folder. You may need to adjust the path.
//    You may also need to adjust the path of the included header ("ImageExpander.hpp") depending on your approach.
// 
//    This example is for use with SFML 3.
//
//
////////////////////////////////////////////////////////////////



#include <SFML/Graphics.hpp>

#include "../ImageExpander/ImageExpander.hpp"



int main()
{
	constexpr unsigned int gapBetweenImages{ 10u };


	// images
	sf::Image origImage{};
	if (!origImage.loadFromFile("resources/images/spaced_tiles.png"))
		return EXIT_FAILURE;
	sf::Image processedImage{ origImage };



	const sf::Vector2u windowSize{ (origImage.getSize().x * 2u) + (gapBetweenImages * 3u), origImage.getSize().y + (gapBetweenImages * 2u) };



	// expand tiles
	sf::IntRect rect{};
	rect.size = { 32, 32 };
	for (int y{ 0 }; y < 5; ++y)
	{
		rect.position.y = 34 * y;
		for (int x{ 0 }; x < 5; ++x)
		{
			rect.position.x = 34u * x;

			imageExpander(processedImage, rect);
		}
	}



	// textures
	sf::Texture origTexture{};
	sf::Texture processedTexture{};
	if ((!origTexture.loadFromImage(origImage)) || (!processedTexture.loadFromImage(processedImage)))
		return EXIT_FAILURE;



	// sprites
	sf::Sprite origSprite(origTexture);
	origSprite.setPosition({ static_cast<float>(gapBetweenImages), static_cast<float>(gapBetweenImages) });
	sf::Sprite processedSprite(processedTexture);
	processedSprite.setPosition({ origImage.getSize().x + static_cast<float>(gapBetweenImages * 2u), static_cast<float>(gapBetweenImages) });



	// flag to determine whether to show original image instead of the processed image (for in-place comparison). this can be toggled by pressing SPACE
	bool showOrig{ false };



	sf::RenderWindow window(sf::VideoMode(windowSize), "");
	while (window.isOpen())
	{
		// render
		window.clear();
		window.draw(origSprite);
		window.draw(processedSprite);
		window.display();

		// events
		while (const auto event{ window.pollEvent() })
		{
			if (event->is<sf::Event::Closed>())
				window.close();
			else if (const auto keyPressed{ event->getIf<sf::Event::KeyPressed>() })
			{
				switch (keyPressed->code)
				{
				case sf::Keyboard::Key::Escape:
					window.close();
					break;
				case sf::Keyboard::Key::Space:
					processedSprite.setTexture((showOrig = !showOrig) ? origTexture : processedTexture);
					break;
				}
			}
		}
	}
}
