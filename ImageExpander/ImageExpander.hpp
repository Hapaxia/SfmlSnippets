#ifndef HAPAXIA_SFMLSNIPPETS_IMAGEEXPANDER
#define HAPAXIA_SFMLSNIPPETS_IMAGEEXPANDER

#include <SFML/Graphics.hpp>

void imageExpander(sf::Image& image, sf::IntRect rect, const std::size_t expansionAmount = 1u)
{
	if (expansionAmount == 0u)
		return;
	const sf::Vector2i imageSize(image.getSize());
	if (rect.size.x == 0)
		rect.size.x = imageSize.x;
	if (rect.size.y == 0)
		rect.size.y = imageSize.y;
	if (rect.position.x < 0)
	{
		rect.size.x += rect.position.x;
		rect.position.x = 0;
	}
	if (rect.position.y < 0)
	{
		rect.size.y += rect.position.y;
		rect.position.y = 0;
	}
	rect.size.x = std::min(rect.size.x, imageSize.x - rect.position.x);
	rect.size.y = std::min(rect.size.y, imageSize.y - rect.position.y);
	auto setPixel = [&](const sf::Vector2i position, const sf::Vector2i offset)
	{
		if ((position.x < 0) || (position.y < 0) || (position.x >= imageSize.x) || (position.y >= imageSize.y))
			return;
		image.setPixel(sf::Vector2u(position), image.getPixel(sf::Vector2u(position + offset)));
	};
	for (int i{ 0 }; i < std::max(rect.size.x, rect.size.y); ++i)
	{
		if (i < rect.size.x)
		{
			setPixel({ rect.position.x + i, rect.position.y - 1 }, { 0, 1 });
			setPixel({ rect.position.x + i, rect.position.y + rect.size.y }, { 0, -1 });
		}
		if (i < rect.size.y)
		{
			setPixel({ rect.position.x - 1, rect.position.y + i }, { 1, 0 });
			setPixel({ rect.position.x + rect.size.x, rect.position.y + i }, { -1, 0 });
		}
	}
	const sf::Vector2i topLeftCorner{ rect.position - sf::Vector2i{ 1, 1 } };
	const sf::Vector2i bottomRightCorner{ rect.position + rect.size };
	setPixel(topLeftCorner, { 1, 1 });
	setPixel(bottomRightCorner, { -1, -1 });
	setPixel({ topLeftCorner.x, bottomRightCorner.y }, { 1, -1 });
	setPixel({ bottomRightCorner.x, topLeftCorner.y }, { -1, 1 });
	if (expansionAmount > 1u)
	{
		rect.position -= { 1, 1 };
		rect.size += { 2, 2 };
		imageExpander(image, rect, expansionAmount - 1u);
	}
}

#endif // HAPAXIA_SFMLSNIPPETS_IMAGEEXPANDER
