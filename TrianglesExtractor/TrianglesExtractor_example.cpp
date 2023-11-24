////////////////////////////////////////////////////////////////
//
// The MIT License (MIT)
//
// Copyright (c) 2023 M.J.Silk
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
//   Creates a base vertex array with 9 vertices to show a triangle fan.
//   That base vertex array then has its triangles extracted into another vertex array.
//   The two vertex arrays are shown next to each other (and should look identical).
//   You can switch the primitive of the base vertex array and the extracted triangles vertex array is automatically updated (with extracted triangles)
//   You can change the primitive type of each of the base vertex array to a line strip to see its underlying structure (it never changes).
//   You can also change the primitive type of the extracted triangles vertex array to either a line strip, lines or points to see its underlying structure.
//   Note that this does affect the other vertex array and the primitive is (obviously) lost when a new primitive type is chosen.
//
//
//       --------
//       CONTROLS
//       --------
//
//   1			            change base vertex array primitive to triangle fan (also re-extracts triangles)
//   2			            change base vertex array primitive to triangle fan (also re-extracts triangles)
//   3			            change base vertex array primitive to triangle fan (also re-extracts triangles)
//   4			            change base vertex array primitive to triangle fan (also re-extracts triangles)
// 
//   Q			            change base vertex array primitive to line strip (does not affect extracted triangles vertex array)
//   W                      change extracted triangles vertex array primitive to line strip (does not affect base vertex array)
//   E                      change extracted triangles vertex array primitive to lines (does not affect base vertex array)
//   R                      change extracted triangles vertex array primitive to points (does not affect base vertex array)
// 
//   ESC                    quit
// 
// 
//        ----
//        NOTE
//        ----
//
//    You may also need to adjust the path of the included header ("TrianglesExtractor.hpp") depending on your approach.
//
//
////////////////////////////////////////////////////////////////



#include <SFML/Graphics.hpp>

#include "../TrianglesExtractor/TrianglesExtractor.hpp"



int main()
{
	// create a base vertex array
	sf::VertexArray vertexArray;
	vertexArray.setPrimitiveType(sf::PrimitiveType::TriangleFan);
	vertexArray.resize(9u);

	// fill base vertex array with vertices to show off a triangle fan (but works with other primitives)
	const sf::Vector2f vertexArrayCenter{ 150.f, 100.f };
	const sf::Vector2f vertexArrayRadius{ 100.f, 100.f };
	vertexArray[0u].position = vertexArrayCenter;
	for (std::size_t i{ 1u }; i < vertexArray.getVertexCount(); ++i)
		vertexArray[i].position = vertexArrayCenter + sf::Vector2f(vertexArrayRadius.x * std::cos((i - 1u) * 0.5f), vertexArrayRadius.y * std::sin((i - 1u) * 0.5f));

	// create a vertex array with extracted triangles
	sf::VertexArray vertexArrayTriangles;
	vertexArrayTriangles = trianglesExtractor::vertexArrayFrom(vertexArray);

	// move the extracted triangles vertex array
	sf::Transform vertexArrayTrianglesTransform;
	vertexArrayTrianglesTransform.translate({ 250.f, 0.f });

	sf::RenderWindow window(sf::VideoMode(550u, 250u), "Triangles Extractor - example");
	while (window.isOpen())
	{
		// render
		window.clear();
		window.draw(vertexArray); // show base vertex array
		window.draw(vertexArrayTriangles, vertexArrayTrianglesTransform); // show extracted triangles (should look the same as base vertex array)
		window.display();

		// events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
				window.close();
			switch (event.type)
			{
			case sf::Event::KeyPressed:
				switch (event.key.scancode)
				{
				case sf::Keyboard::Scancode::Q: // set primitive of base vertex array to line strip
					vertexArray.setPrimitiveType(sf::LineStrip);
					break;
				case sf::Keyboard::Scancode::W: // set primitive of extracted triangles vertex array to line strip
					vertexArrayTriangles.setPrimitiveType(sf::LineStrip);
					break;
				case sf::Keyboard::Scancode::E: // set primitive of extracted triangles vertex array to lines
					vertexArrayTriangles.setPrimitiveType(sf::Lines);
					break;
				case sf::Keyboard::Scancode::R: // set primitive of extracted triangles vertex array to points
					vertexArrayTriangles.setPrimitiveType(sf::Points);
					break;
				case sf::Keyboard::Scancode::Num1: // switch base vertex to a triangle fan and re-extract triangles
					vertexArray.setPrimitiveType(sf::PrimitiveType::TriangleFan);
					vertexArrayTriangles = trianglesExtractor::vertexArrayFrom(vertexArray);
					break;
				case sf::Keyboard::Scancode::Num2: // switch base vertex to a triangle strip and re-extract triangles
					vertexArray.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
					vertexArrayTriangles = trianglesExtractor::vertexArrayFrom(vertexArray);
					break;
				case sf::Keyboard::Scancode::Num3: // switch base vertex to independent quads and re-extract triangles
					vertexArray.setPrimitiveType(sf::PrimitiveType::Quads);
					vertexArrayTriangles = trianglesExtractor::vertexArrayFrom(vertexArray);
					break;
				case sf::Keyboard::Scancode::Num4: // switch base vertex to independent triangle and re-extract triangles
					vertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
					vertexArrayTriangles = trianglesExtractor::vertexArrayFrom(vertexArray);
					break;
				}
				break;
			}
		}
	}
}
