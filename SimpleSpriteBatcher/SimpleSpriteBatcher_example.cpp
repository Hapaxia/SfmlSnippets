////////////////////////////////////////////////////////////////
//
// The MIT License (MIT)
//
// Copyright (c) 2023-2025 M.J.Silk
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
//   Creates a large vector of sprites.
//   They are (trivially) randomly placed, scaled and rotated.
//   Each frame, all of them are rotated by a set amount. The frame-rate will affect their speed.
//   If the batcher is not used, all of the sprites are drawn separately.
//   If the batcher is used, all of the sprites are batches every frame and then drawn together as one.
//   This is a simple batcher that should always be no slower than drawing them separately but likely to have improvements, usually significant.
//   The batcher can more than double the frame-rate at lower frame-rates (when drawing is the biggest issue)
//
//
//       --------
//       CONTROLS
//       --------
//
//   SPACE			        toggle batcher (starts off)
//   ESC                    quit
// 
// 
//       -------------
//       CUSTOMISATION
//       -------------
//
//   You can (should) change the number of sprites ('n') created (but not too many!) so that they lower your frame-rate using this line: sprites.resize('n', sf::Sprite(texture));
//
//
//        ----
//        NOTE
//        ----
//
//    If the window is too large (1920u, 1080u) for your resolution, you can uncomment the following define line to halve the window size (to 960x540): #define HALVE_WINDOW_SIZE
//    If you'd like to push the window size a little larger (2880, 1620)!, you can uncomment: #define LARGER_WINDOW_SIZE. Note that halving the size affects this value as well.
//    The texture is available in the resources folder, which is in the root folder. You may need to adjust the path.
//    You may also need to adjust the path of the included header ("SimpleSpriteBatcher.hpp") depending on your approach.
//    Remember to test in both debug and release modes for comparisons. The batcher may be less effective in debug mode.
// 
//    This example is for use with SFML 3.
//
//
////////////////////////////////////////////////////////////////



#include <SFML/Graphics.hpp>

#include "../SimpleSpriteBatcher/SimpleSpriteBatcher.hpp"



//#define HALVE_WINDOW_SIZE
//#define LARGER_WINDOW_SIZE



int main()
{
	sf::Vector2u windowSize{ 1920u, 1080u };
#ifdef HALVE_WINDOW_SIZE
	windowSize /= 2u;
#endif // HALVE_WINDOW_SIZE
#ifdef LARGER_WINDOW_SIZE
	windowSize *= 3u;
	windowSize /= 2u;
#endif // LARGER_WINDOW_SIZE



	// texture
	sf::Texture texture;
	if (!texture.loadFromFile("resources/images/16colours(16x16_4x4each)-tex.png"))
		return EXIT_FAILURE;



	// sprites
	std::vector<sf::Sprite> sprites;
	// personal values for showcase: 5k, 50k, 100k, 250k, 500k, 1000k.
	sprites.resize(5000u, sf::Sprite(texture)); // change this to a value that is affects your frame-rate. this will be different on every system. first, try a few thousand. for fast computers/graphics cards, try tens (or even hundreds!) of thousands.
	for (auto& sprite : sprites)
	{
		sprite.setTexture(texture);
		const std::size_t randomTileIndex{ rand() % 16u };
		sprite.setTextureRect({ { (static_cast<int>(randomTileIndex) % 4) * 4, (static_cast<int>(randomTileIndex) / 4) * 4 }, { 4, 4 } });
		const float scale{ ((rand() % 950) + 50) / 100.f };
		sprite.setScale({ scale , scale });
		sprite.setPosition({ static_cast<float>(rand() % windowSize.x), static_cast<float>(rand() % windowSize.y) });
		sprite.setRotation(sf::degrees((rand() % 1000) * 0.36f));
	}



	// batcher (Simple Sprite Batcher)
	SimpleSpriteBatcher batcher;
	batcher.texture = &texture;



	// flag to determine whether to use batcher or not. this can be toggled by pressing SPACE
	bool useBatcher{ false };



	sf::Clock clock; // clock for measuring FPS
	sf::RenderWindow window(sf::VideoMode(windowSize), "");
	while (window.isOpen())
	{
		// update sprites
		for (std::size_t i{ 0u }; i < sprites.size(); ++i)
			sprites[i].rotate(sf::degrees(2.f));

		// batch sprites
		if (useBatcher)
			batcher.batchSprites(sprites);

		// show trivial FPS in window title
		window.setTitle(std::string("BATCHER ") + (useBatcher ? "ON:             " : "OFF:            ") + std::to_string(static_cast<int>(1.f / clock.restart().asSeconds())) + "\tFPS");

		// render
		window.clear();
		if (useBatcher)
			window.draw(batcher);
		else
		{
			for (auto& sprite : sprites)
				window.draw(sprite);
		}
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
					useBatcher = !useBatcher;
					break;
				}
			}
		}
	}
}
