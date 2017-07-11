#ifndef RECTANGULAR_BOUNDARY_COLLISION_HPP
#define RECTANGULAR_BOUNDARY_COLLISION_HPP

#include <SFML/Graphics.hpp>
#include <array>

namespace collision
{
	namespace impl
	{

bool satRectangleAndPoints(const sf::Vector2f rectangleSize, const std::array<sf::Vector2f, 4>& points);

	} // namespace impl

// returns a boolean representing if the two objects' rectangular boundaries are colliding
// allows the collision level to be specified:
//     0: AABB (axis-aligned boundaries boxes)
//     1: tests if any corners are inside opposite rectangle (performs level 0 first)
//     2: SAT (separating axis theorem) (performs levels 0 and 1 first)
//    -1: maximum level (default) (performs levels 0, 1 and 2)
// "short-circuits" higher level tests if a lower level is certain of a result
// allows a negative value for collision level to represent the "maximum" collision level (default value)
// can test any two (different) objects as long as they have these functions:
//     getTransform()        (must return sf::Transform)
//     getInverseTransform() (must return sf::Transform)
//     getLocalBounds()      (must return sf::FloatRect)
template <class T1, class T2>
bool areColliding(const T1& object1, const T2& object2, const int collisionLevel = -1)
{
	// LEVEL 0 (axis-aligned bounding box)
	const sf::Transform transform1{ object1.getTransform() };
	const sf::Transform transform2{ object2.getTransform() };
	const bool level0{ transform1.transformRect(object1.getLocalBounds()).intersects(transform2.transformRect(object2.getLocalBounds())) };
	if (!level0 || collisionLevel == 0)
		return level0;

	// LEVEL 1 (any corners inside opposite rectangle)
	const sf::Transform inverseTransform1{ object1.getInverseTransform() };
	const sf::Transform inverseTransform2{ object2.getInverseTransform() };
	const sf::FloatRect rect1Bounds{ object1.getLocalBounds() };
	const sf::FloatRect rect2Bounds{ object2.getLocalBounds() };
	const sf::Vector2f rect1Size{ rect1Bounds.width, rect1Bounds.height };
	const sf::Vector2f rect2Size{ rect2Bounds.width, rect2Bounds.height };
	const sf::Vector2f rect1TopLeft{ inverseTransform2.transformPoint(transform1.transformPoint({ 0.f, 0.f })) };
	const sf::Vector2f rect1TopRight{ inverseTransform2.transformPoint(transform1.transformPoint({ rect1Size.x, 0.f })) };
	const sf::Vector2f rect1BottomRight{ inverseTransform2.transformPoint(transform1.transformPoint(rect1Size)) };
	const sf::Vector2f rect1BottomLeft{ inverseTransform2.transformPoint(transform1.transformPoint({ 0.f, rect1Size.y })) };
	const sf::Vector2f rect2TopLeft{ inverseTransform1.transformPoint(transform2.transformPoint({ 0.f, 0.f })) };
	const sf::Vector2f rect2TopRight{ inverseTransform1.transformPoint(transform2.transformPoint({ rect2Size.x, 0.f })) };
	const sf::Vector2f rect2BottomRight{ inverseTransform1.transformPoint(transform2.transformPoint(rect2Size)) };
	const sf::Vector2f rect2BottomLeft{ inverseTransform1.transformPoint(transform2.transformPoint({ 0.f, rect2Size.y })) };
	const bool level1{ (
		(rect1Bounds.contains(rect2TopLeft)) ||
		(rect1Bounds.contains(rect2TopRight)) ||
		(rect1Bounds.contains(rect2BottomLeft)) ||
		(rect1Bounds.contains(rect2BottomRight)) ||
		(rect2Bounds.contains(rect1TopLeft)) ||
		(rect2Bounds.contains(rect1TopRight)) ||
		(rect2Bounds.contains(rect1BottomLeft)) ||
		(rect2Bounds.contains(rect1BottomRight))) };
	if (level1 || collisionLevel == 1)
		return level1;

	// LEVEL 2 (SAT)
	std::array<sf::Vector2f, 4> rect1Points
	{
		rect1BottomLeft,
		rect1BottomRight,
		rect1TopRight,
		rect1TopLeft,
	};
	if (!impl::satRectangleAndPoints(rect2Size, rect1Points))
		return false;
	std::array<sf::Vector2f, 4> rect2Points
	{
		rect2BottomLeft,
		rect2BottomRight,
		rect2TopRight,
		rect2TopLeft,
	};
	return impl::satRectangleAndPoints(rect1Size, rect2Points);
}



	namespace impl
	{

bool satRectangleAndPoints(const sf::Vector2f rectangleSize, const std::array<sf::Vector2f, 4>& points)
{
	bool allPointsLeftOfRectangle{ true };
	bool allPointsRightOfRectangle{ true };
	bool allPointsAboveRectangle{ true };
	bool allPointsBelowRectangle{ true };
	for (auto& point : points)
	{
		if (point.x >= 0.f)
			allPointsLeftOfRectangle = false;
		if (point.x <= rectangleSize.x)
			allPointsRightOfRectangle = false;
		if (point.y >= 0.f)
			allPointsAboveRectangle = false;
		if (point.y <= rectangleSize.y)
			allPointsBelowRectangle = false;
	}
	return !(allPointsLeftOfRectangle || allPointsRightOfRectangle || allPointsAboveRectangle || allPointsBelowRectangle);
}

	} // namespace impl
} // namespace collision
#endif // RECTANGULAR_BOUNDARY_COLLISION_HPP
