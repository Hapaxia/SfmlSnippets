////////////////////////////////////////////////////////////////
//
// The MIT License (MIT)
//
// Copyright (c) 2022-2025 M.J.Silk
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
//   Displays a single line of text as well as a multi-line text object.
//   Whichever character the mouse hovering over, is shown (as well as its index in the text string) and the character is also visually highlighted.
//
//
//       --------
//       CONTROLS
//       --------
//
//   MOUSE			        hover cursor over a text character
//   ESC                    quit
//
//
//        ----
//        NOTE
//        ----
//
//    If the window is too large (1600x900) for your resolution, you can uncomment the following define line to halve the window size (to 800x450) and scale the texts to match: #define HALVE_WINDOW_SIZE
// 
//    This example is for use with SFML 3.
//
//
////////////////////////////////////////////////////////////////



#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>

#include "GetCharacterAtCoord.hpp"



//#define HALVE_WINDOW_SIZE



int main()
{
	sf::Vector2u windowSize{ 1600u, 900u };
#ifdef HALVE_WINDOW_SIZE
	windowSize /= 2u;
#endif // HALVE_WINDOW_SIZE



	sf::Font font;
	if (!font.openFromFile("resources/fonts/arial.ttf"))
	{
		std::cerr << "ERROR LOADING FONT" << std::endl;
		return EXIT_FAILURE;
	}



	auto setTextOriginToCenter = [](sf::Text& text)
	{
		const sf::FloatRect textLocalBounds{ text.getLocalBounds() };
		text.setOrigin(textLocalBounds.position + (textLocalBounds.size / 2.f));
	};

	sf::Text text(font);
	text.setCharacterSize(64u);
	text.setFillColor(sf::Color::White);
	text.setString("ABCDEF\nGHIJKLM\nNOPQRST\n\tUVWXYZ\n12345\n67890");
	setTextOriginToCenter(text);
	text.setPosition(sf::Vector2f(windowSize.x * 0.5f, windowSize.y * 0.6f));

	sf::Text textSingleLine = text;
	textSingleLine.setString("ABCDEFGHIJKLMNOPQRST\tUVWXYZ1234567890");
	setTextOriginToCenter(textSingleLine);
	textSingleLine.setPosition(sf::Vector2f(windowSize.x * 0.5f, windowSize.y * 0.25f));

	sf::Text textCharacterHighlight = text;
	textCharacterHighlight.setString("");
	textCharacterHighlight.setOrigin({ 0.f, 0.f });
	textCharacterHighlight.setFillColor(sf::Color::Red);
	textCharacterHighlight.setOutlineColor(sf::Color::White);
	textCharacterHighlight.setOutlineThickness(2.f);

	sf::Text feedbackText(font);
	feedbackText.setCharacterSize(36u);
	feedbackText.setFillColor(sf::Color::Green);
	feedbackText.setPosition({ 5.f, 5.f });

#ifdef HALVE_WINDOW_SIZE
	text.setScale({ 0.5f, 0.5f });
	textSingleLine.setScale({ 0.5f, 0.5f });
	textCharacterHighlight.setScale({ 0.5f, 0.5f });
	feedbackText.setScale({ 0.5f, 0.5f });
#endif // HALVE_WINDOW_SIZE



	sf::RenderWindow window(sf::VideoMode(windowSize), "");

	while (window.isOpen())
	{
		// EVENTS

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
				}
			}
		}





		// UPDATE

		const sf::Vector2f mouseCoord{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) };

		bool isMouseInsideText{ false };
		bool isMouseInsideSingleLineText{ false };
		char characterUnderMouse{ '\n' };
		char singleLineCharacterUnderMouse{ '\n' };
		std::size_t indexOfCharacterUnderMouse{ 0u };
		std::size_t indexOfSingleLineCharacterUnderMouse{ 0u };

		auto processTextUnderMouse = [mouseCoord, &textCharacterHighlight](bool& isMouseInside, sf::Text& text, char& c, std::size_t& i)
		{
			isMouseInside = text.getGlobalBounds().contains(mouseCoord);
			i = getCharacterIndexAtCoord(text, mouseCoord);
			if (i < text.getString().getSize())
			{
				c = text.getString()[i];
				textCharacterHighlight.setString(c);
				textCharacterHighlight.setPosition(text.findCharacterPos(i));
			}
		};
		textCharacterHighlight.setString("");
		processTextUnderMouse(isMouseInsideText, text, characterUnderMouse, indexOfCharacterUnderMouse);
		processTextUnderMouse(isMouseInsideSingleLineText, textSingleLine, singleLineCharacterUnderMouse, indexOfSingleLineCharacterUnderMouse);

		std::string feedbackString;
		auto composeFeedbackString = [](char c, std::size_t i) { return ((c == '\n') ? "" : "Character Under Mouse: " + ((c == '\t') ? "[TAB]" : "\'" + std::string(1u, c) + "\'") + " (index: " + std::to_string(i) + ")"); };
		if (isMouseInsideText)
			feedbackString = composeFeedbackString(characterUnderMouse, indexOfCharacterUnderMouse);
		else if (isMouseInsideSingleLineText)
			feedbackString = composeFeedbackString(singleLineCharacterUnderMouse, indexOfSingleLineCharacterUnderMouse);
		feedbackText.setString(feedbackString);





		// RENDER

		window.clear();
		window.draw(textSingleLine);
		window.draw(text);
		if (isMouseInsideText || isMouseInsideSingleLineText)
			window.draw(textCharacterHighlight);
		window.draw(feedbackText);
		window.display();
	}
}
