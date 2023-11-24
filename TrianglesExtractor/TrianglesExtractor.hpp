#ifndef HAPAXIA_SFML_SNIPPETS_TRIANGLES_EXTRACTOR_HPP
#define HAPAXIA_SFML_SNIPPETS_TRIANGLES_EXTRACTOR_HPP

#include <SFML/Graphics.hpp>

namespace trianglesExtractor
{
	namespace convert
	{

sf::VertexArray vertexArrayFromVertices(const std::vector<sf::Vertex>& vertices, const sf::PrimitiveType primitiveType = sf::PrimitiveType::Triangles)
{
	const std::size_t size{ vertices.size() };
	sf::VertexArray vertexArray(primitiveType, size);
	for (std::size_t i{ 0u }; i < size; ++i)
		vertexArray[i] = vertices[i];
	return vertexArray;
}

std::vector<sf::Vertex> verticesFromVertexArray(const sf::VertexArray& vertexArray)
{
	const std::size_t size{ vertexArray.getVertexCount() };
	std::vector<sf::Vertex> vertices(size);
	for (std::size_t i{ 0u }; i < size; ++i)
		vertices[i] = vertexArray[i];
	return vertices;
}

sf::VertexBuffer vertexBufferFromVertices(const std::vector<sf::Vertex>& vertices, const sf::PrimitiveType primitiveType = sf::PrimitiveType::Triangles)
{
	sf::VertexBuffer v(primitiveType);
	v.create(vertices.size());
	v.update(vertices.data());
	return v;
}

sf::VertexBuffer vertexBufferFromVertexArray(const sf::VertexArray& vertexArray)
{
	return vertexBufferFromVertices(verticesFromVertexArray(vertexArray), vertexArray.getPrimitiveType());
}

	} // namespace convert



std::vector<sf::Vertex> verticesFrom(const std::vector<sf::Vertex>& vertices, sf::PrimitiveType primitiveType)
{
	if (primitiveType == sf::PrimitiveType::Triangles)
		return vertices;

	std::vector<sf::Vertex> result;

	if (vertices.size() < 3u)
		return result;

	std::size_t numberOfVerticesPerPrimitiveOrig{ 1u };
	std::size_t numberOfVerticesPerPrimitiveNew{ 3u };
	std::size_t baseSizeRemoval{ 2u };
	if (primitiveType == sf::PrimitiveType::Quads)
	{
		numberOfVerticesPerPrimitiveOrig = 4u;
		numberOfVerticesPerPrimitiveNew = 6u;
		baseSizeRemoval = 0u;
	}
	const std::size_t numberOfPrimitives{ (vertices.size() - baseSizeRemoval) / numberOfVerticesPerPrimitiveOrig };
	result.resize(numberOfPrimitives * numberOfVerticesPerPrimitiveNew);
	for (std::size_t p{ 0u }; p < numberOfPrimitives; ++p)
	{
		const std::size_t startVertexOrig{ p * numberOfVerticesPerPrimitiveOrig };
		const std::size_t startVertexNew{ p * numberOfVerticesPerPrimitiveNew };
		result[startVertexNew + 0u] = (primitiveType == sf::TriangleFan) ? vertices[0u] : vertices[startVertexOrig + 0u];
		result[startVertexNew + 1u] = vertices[startVertexOrig + 1u];
		result[startVertexNew + 2u] = vertices[startVertexOrig + 2u];
		if (primitiveType == sf::PrimitiveType::Quads)
		{
			result[startVertexNew + 3u] = vertices[startVertexOrig + 0u];
			result[startVertexNew + 4u] = vertices[startVertexOrig + 2u];
			result[startVertexNew + 5u] = vertices[startVertexOrig + 3u];
		}
	}
	return result;
}

std::vector<sf::Vertex> verticesFrom(const sf::VertexArray& vertexArray)
{
	return verticesFrom(convert::verticesFromVertexArray(vertexArray), vertexArray.getPrimitiveType());
}

sf::VertexArray vertexArrayFrom(const std::vector<sf::Vertex>& vertices, sf::PrimitiveType primitiveType)
{
	return convert::vertexArrayFromVertices(verticesFrom(vertices, primitiveType), sf::PrimitiveType::Triangles);
}

sf::VertexArray vertexArrayFrom(const sf::VertexArray& vertexArray)
{
	return vertexArrayFrom(convert::verticesFromVertexArray(vertexArray), vertexArray.getPrimitiveType());
}

} // namespace trianglesExtractor
#endif // HAPAXIA_SFML_SNIPPETS_TRIANGLES_EXTRACTOR_HPP
