#include "Physics.h"
#include "EntityManager.h"

//DEPRECATED
Vec2 Physics::getOverlap(EntityManager & entityManager, Entity entity1, Entity entity2)
{
	Vec2 vec();
	if (entityManager.hasComponent<Component::BoundingBox>(entity1) && entityManager.hasComponent<Component::BoundingBox>(entity2))
	{

		float dx = abs(entityManager.getComponent<Component::Transform>(entity1).position.x - entityManager.getComponent<Component::Transform>(entity2).position.x);
		float dy = abs(entityManager.getComponent<Component::Transform>(entity1).position.y - entityManager.getComponent<Component::Transform>(entity2).position.y);

		auto& bb1 = entityManager.getComponent<Component::BoundingBox>(entity1);
		auto& bb2 = entityManager.getComponent<Component::BoundingBox>(entity2);

		return Vec2(bb1.halfSize.x + bb2.halfSize.x - dx, bb1.halfSize.y + bb2.halfSize.y - dy);
	}
	return Vec2(0, 0);
}

bool Physics::isColliding(EntityManager& entityManager, Entity entity1, Entity entity2) 
{

    if (!entityManager.hasComponent<Component::BoundingBox>(entity1) || !entityManager.hasComponent<Component::BoundingBox>(entity2)) 
    {
        return false;
    }

	auto& bb1 = entityManager.getComponent<Component::BoundingBox>(entity1);
	auto& bb2 = entityManager.getComponent<Component::BoundingBox>(entity2);

	auto& t1 = entityManager.getComponent<Component::Transform>(entity1);
	auto& t2 = entityManager.getComponent<Component::Transform>(entity2);

	sf::RectangleShape rect1;
	rect1.setSize({ bb1.size.x, bb1.size.y });
	rect1.setOrigin({ bb1.halfSize.x, bb1.halfSize.y });
	rect1.setPosition({ t1.position.x, t1.position.y });
	if (t1.rotates) {
		rect1.setRotation(Vec2(0, -1).angle(t1.direction) * 180.0f / PI);
	}

	sf::RectangleShape rect2;
	rect2.setSize({ bb2.size.x, bb2.size.y });
	rect2.setOrigin({ bb2.halfSize.x, bb2.halfSize.y });
	rect2.setPosition({ t2.position.x, t2.position.y });
	if (t2.rotates) {
		rect2.setRotation(Vec2(0, -1).angle(t2.direction) * 180.0f / PI);
	}
    /*
	OrientedBoundingBox OBB1(rect1);
	OrientedBoundingBox OBB2(rect2);

	sf::Vector2f Axes[4] = {
			sf::Vector2f(OBB1.Points[1].x - OBB1.Points[0].x,
			OBB1.Points[1].y - OBB1.Points[0].y),
			sf::Vector2f(OBB1.Points[1].x - OBB1.Points[2].x,
			OBB1.Points[1].y - OBB1.Points[2].y),
			sf::Vector2f(OBB2.Points[0].x - OBB2.Points[3].x,
			OBB2.Points[0].y - OBB2.Points[3].y),
			sf::Vector2f(OBB2.Points[0].x - OBB2.Points[1].x,
			OBB2.Points[0].y - OBB2.Points[1].y)
	};

	for (int i = 0; i < 4; i++) // For each axis...
	{
		float MinOBB1, MaxOBB1, MinOBB2, MaxOBB2;

		// ... project the points of both OBBs onto the axis ...
		OBB1.ProjectOntoAxis(Axes[i], MinOBB1, MaxOBB1);
		OBB2.ProjectOntoAxis(Axes[i], MinOBB2, MaxOBB2);

		// ... and check whether the outermost projected points of both OBBs overlap.
		// If this is not the case, the Separating Axis Theorem states that there can be no collision between the rectangles
		if (!((MinOBB2 <= MaxOBB1) && (MaxOBB2 >= MinOBB1)))
			return false;
	}
	return true;
    */

    return BoundingBoxTest(rect1, rect2);
}

//From Rotated Rectangles Collision Detection, Oren Becker, 2001

bool Physics::BoundingBoxTest(const sf::RectangleShape& Object1, const sf::RectangleShape& Object2) {

    sf::Vector2f A, B, C, BL, TR;
    sf::Vector2f HalfSize1 = Object1.getSize();
    sf::Vector2f HalfSize2 = Object2.getSize();

    //For somereason the Vector2d divide by operator
    //was misbehaving
    //Doing it manually
    HalfSize1.x /= 2;
    HalfSize1.y /= 2;
    HalfSize2.x /= 2;
    HalfSize2.y /= 2;
    //Get the Angle we're working on
    float Angle = Object1.getRotation() - Object2.getRotation();
    float CosA = cos(Angle * PI / 180.0f);
    float SinA = sin(Angle * PI / 180.0f);

    float t, x, a, dx, ext1, ext2;

    //Normalise the Center of Object2 so its axis aligned an represented in
    //relation to Object 1
    C = Object2.getPosition();

    C -= Object1.getPosition();

    C = RotatePoint(C, Object2.getRotation());

    //Get the Corners
    BL = TR = C;
    BL -= HalfSize2;
    TR += HalfSize2;

    //Calculate the vertices of the rotate Rect
    A.x = -HalfSize1.y * SinA;
    B.x = A.x;
    t = HalfSize1.x * CosA;
    A.x += t;
    B.x -= t;

    A.y = HalfSize1.y * CosA;
    B.y = A.y;
    t = HalfSize1.x * SinA;
    A.y += t;
    B.y -= t;

    t = SinA * CosA;

    // verify that A is vertical min/max, B is horizontal min/max
    if (t < 0) {
        t = A.x;
        A.x = B.x;
        B.x = t;
        t = A.y;
        A.y = B.y;
        B.y = t;
    }

    // verify that B is horizontal minimum (leftest-vertex)
    if (SinA < 0) {
        B.x = -B.x;
        B.y = -B.y;
    }

    // if rr2(ma) isn't in the horizontal range of
    // colliding with rr1(r), collision is impossible
    if (B.x > TR.x || B.x > -BL.x) return false;

    // if rr1(r) is axis-aligned, vertical min/max are easy to get
    if (t == 0) {
        ext1 = A.y;
        ext2 = -ext1;
    }// else, find vertical min/max in the range [BL.x, TR.x]
    else {
        x = BL.x - A.x;
        a = TR.x - A.x;
        ext1 = A.y;
        // if the first vertical min/max isn't in (BL.x, TR.x), then
        // find the vertical min/max on BL.x or on TR.x
        if (a * x > 0) {
            dx = A.x;
            if (x < 0) {
                dx -= B.x;
                ext1 -= B.y;
                x = a;
            }
            else {
                dx += B.x;
                ext1 += B.y;
            }
            ext1 *= x;
            ext1 /= dx;
            ext1 += A.y;
        }

        x = BL.x + A.x;
        a = TR.x + A.x;
        ext2 = -A.y;
        // if the second vertical min/max isn't in (BL.x, TR.x), then
        // find the local vertical min/max on BL.x or on TR.x
        if (a * x > 0) {
            dx = -A.x;
            if (x < 0) {
                dx -= B.x;
                ext2 -= B.y;
                x = a;
            }
            else {
                dx += B.x;
                ext2 += B.y;
            }
            ext2 *= x;
            ext2 /= dx;
            ext2 -= A.y;
        }
    }

    // check whether rr2(ma) is in the vertical range of colliding with rr1(r)
    // (for the horizontal range of rr2)
    return !((ext1 < BL.y && ext2 < BL.y) ||
        (ext1 > TR.y&& ext2 > TR.y));

}

sf::Vector2f Physics::RotatePoint(const sf::Vector2f& Point, float Angle) {
    Angle = Angle * PI / 180.0f;
    sf::Vector2f RotatedPoint;
    RotatedPoint.x = Point.x * cos(Angle) + Point.y * sin(Angle);
    RotatedPoint.y = -Point.x * sin(Angle) + Point.y * cos(Angle);
    return RotatedPoint;
}
