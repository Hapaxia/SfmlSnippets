#ifndef HAPAXIA_SFMLSNIPPETS_SIMPLE_SPRITE_BATCHER
#define HAPAXIA_SFMLSNIPPETS_SIMPLE_SPRITE_BATCHER

#include <SFML/Graphics.hpp>

class SimpleSpriteBatcher : public sf::Drawable
{
public:
	sf::Texture* texture{ nullptr };
	SimpleSpriteBatcher() = default;
	void batchSprites(const std::vector<sf::Sprite>& sprites)
	{
		m_vertices.resize(sprites.size() * 6u);
		for (std::size_t i{ 0u }; i < sprites.size(); ++i)
			setQuad(&(sprites[i]), i * 6u);
	}
	void batchSprites(const std::vector<sf::Sprite*>& sprites)
	{
		m_vertices.resize(sprites.size() * 6u);
		for (std::size_t i{ 0u }; i < sprites.size(); ++i)
			setQuad(sprites[i], i * 6u);
	}

private:
	std::vector<sf::Vertex> m_vertices{};
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.texture = texture;
		target.draw(m_vertices.data(), m_vertices.size(), sf::PrimitiveType::Triangles, states);
	}

	void setQuad(const sf::Sprite* sprite, std::size_t startVertex)
	{
		const sf::Transform transform{ sprite->getTransform() };
		const sf::Color color{ sprite->getColor() };
		const sf::IntRect rect{ sprite->getTextureRect() };

		sf::Vector2f shapeTopLeft{ 0.f, 0.f };
		sf::Vector2f shapeBottomRight(rect.getSize());
		sf::Vector2f shapeTopRight{ shapeBottomRight.x, shapeTopLeft.y };
		sf::Vector2f shapeBottomLeft{ shapeTopLeft.x, shapeBottomRight.y };
		const sf::Vector2f textureTopLeft(rect.getPosition());
		const sf::Vector2f textureBottomRight{ textureTopLeft + shapeBottomRight };
		const sf::Vector2f textureTopRight{ textureBottomRight.x, textureTopLeft.y };
		const sf::Vector2f textureBottomLeft{ textureTopLeft.x, textureBottomRight.y };
		shapeTopLeft = transform.transformPoint(shapeTopLeft);
		shapeBottomRight = transform.transformPoint(shapeBottomRight);
		shapeTopRight = transform.transformPoint(shapeTopRight);
		shapeBottomLeft = transform.transformPoint(shapeBottomLeft);

		m_vertices[startVertex + 0u].position = shapeTopLeft;
		m_vertices[startVertex + 0u].texCoords = textureTopLeft;
		m_vertices[startVertex + 0u].color = color;
		m_vertices[startVertex + 1u].position = shapeBottomLeft;
		m_vertices[startVertex + 1u].texCoords = textureBottomLeft;
		m_vertices[startVertex + 1u].color = color;
		m_vertices[startVertex + 2u].position = shapeBottomRight;
		m_vertices[startVertex + 2u].texCoords = textureBottomRight;
		m_vertices[startVertex + 2u].color = color;
		m_vertices[startVertex + 5u].position = shapeTopRight;
		m_vertices[startVertex + 5u].texCoords = textureTopRight;
		m_vertices[startVertex + 5u].color = color;

		m_vertices[startVertex + 3u] = m_vertices[startVertex + 0u];
		m_vertices[startVertex + 4u] = m_vertices[startVertex + 2u];
	}
};

#endif // HAPAXIA_SFMLSNIPPETS_SIMPLE_SPRITE_BATCHER
