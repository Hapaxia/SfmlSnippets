#ifndef GET_CHARACTER_AT_COORD_HPP
#define GET_CHARACTER_AT_COORD_HPP

#include <cstddef>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

std::size_t getCharacterIndexAtCoordSingleLine(const sf::Text& text, const sf::Vector2f coord)
{
	std::size_t indexOfCharacterUnderCoord{ text.getString().getSize() };
	if (text.getGlobalBounds().contains(coord))
	{
		const std::size_t size{ text.getString().getSize() };
		for (std::size_t i{ 0u }; i < size; ++i)
		{
			if (text.findCharacterPos(i).x > coord.x)
				break;
			indexOfCharacterUnderCoord = i;
		}
	}
	return indexOfCharacterUnderCoord;
}

std::size_t getCharacterIndexAtCoord(const sf::Text& text, const sf::Vector2f coord)
{
	std::size_t indexOfCharacterUnderCoord{ text.getString().getSize() };
	if (text.getGlobalBounds().contains(coord))
	{
		const std::size_t size{ text.getString().getSize() };
		std::size_t startI{ 0u };
		std::size_t nextStartI{ 0u };
		float y{ text.findCharacterPos(0u).y };
		for (std::size_t i{ 0u }; i <= size; ++i)
		{
			const float characterPosY{ text.findCharacterPos(i).y };
			if (characterPosY > coord.y)
			{
				startI = nextStartI;
				break;
			}
			if ((characterPosY > y) || ((i + 1u) == text.getString().getSize()))
			{
				y = characterPosY;
				startI = nextStartI;
				nextStartI = i;
			}
		}
		y = text.findCharacterPos(startI).y;
		for (std::size_t i{ startI }; i <= size; ++i)
		{
			const sf::Vector2f characterPos{ text.findCharacterPos(i) };
			if ((characterPos.y > y) || (characterPos.x > coord.x))
				break;
			indexOfCharacterUnderCoord = i;
		}
	}
	return indexOfCharacterUnderCoord;
}

#endif // GET_CHARACTER_AT_COORD_HPP
