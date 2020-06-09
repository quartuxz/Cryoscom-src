#pragma once
#define MAKING_LEVELS false
#define CRYOSCOM_DEBUG false
#define ADD_PLAYER_VELOCITY_TO_BULLET false
#define MULTITHREADED_SCRIPTING_AND_MESSAGING false
#include <limits>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <random>

enum initType {
	allValuesToZero
};

inline int generateRandomNumber(int rangeStart, int rangeEnd) {
	static bool firstTime = true;
	static std::default_random_engine randEngine;
	if (firstTime) {
		randEngine.seed(time(NULL));
		firstTime = false;
	}
	std::uniform_int_distribution<int> randDist(rangeStart, rangeEnd);
	

	return randDist(randEngine);
}

inline float vectorDistance(const sf::Vector2f &v, const sf::Vector2f &w)
{
	return std::sqrt(pow(v.x - w.x, 2) + pow(v.y - w.y, 2));
}



inline float lengthSquared(const sf::Vector2f& point) {
	return (point.x * point.x + point.y * point.y);

}



inline float distanceSquared(const sf::Vector2f &v, const sf::Vector2f &w)
{
	return pow(v.x - w.x, 2) + pow(v.y - w.y, 2);
}

//float dot(sf::Vector2f v, sf::Vector2f w)
//{
//	return v.x * w.x + v.y * w.y;
//}

inline float dotProduct(const sf::Vector2f& first, const sf::Vector2f& second) {

	return first.x * second.x + first.y * second.y;
}

// Return minimum distance between line segment vw and point p
inline float minimum_distance(const sf::Vector2f& v, const sf::Vector2f& w, const sf::Vector2f& p, sf::Vector2f* proj = nullptr)
{
	const float l2 = distanceSquared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
	if (l2 == 0.0) return vectorDistance(p, v);   // v == w case
											// Consider the line extending the segment, parameterized as v + t (w - v).
											// We find projection of point p onto the line. 
											// It falls where t = [(p-v) . (w-v)] / |w-v|^2  
											// We clamp t from [0,1] to handle points outside the segment vw.
	const float t = std::max(static_cast<float>(0), std::min(static_cast<float>(1), dotProduct((p - v), (w - v)) / l2));
	const sf::Vector2f projection = v + t * (w - v);  // Projection falls on the segment
	if (proj != nullptr) {
		*proj = projection;
	}
	return vectorDistance(p, projection);
}


inline sf::Vector2f rotateByAngle(sf::Vector2f o, sf::Vector2f p, float angle)
{
	float s = sin(angle * (M_PI / 180));
	float c = cos(angle * (M_PI / 180));;

	// translate point back to origin:
	p.x -= o.x;
	p.y -= o.y;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + o.x;
	p.y = ynew + o.y;
	return p;
}

//first param is the origin of rotation and the second param is the point to be rotated
inline sf::Vector2f rotate90(sf::Vector2f o, sf::Vector2f p)
{
	return rotateByAngle(o, p, 90);
}

inline sf::Vector2f reflect(sf::Vector2f v, sf::Vector2f w, sf::Vector2f p)
{
	float s = 0;
	float c = -1;

	sf::Vector2f proj;
	minimum_distance(v, w, p, &proj);

	// translate point back to origin:
	p.x -= proj.x;
	p.y -= proj.y;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + proj.x;
	p.y = ynew + proj.y;
	return p;
}


inline float getAngleInDegrees(const sf::Vector2f &unitVec)
{
	return std::atan2(unitVec.x, -unitVec.y) * 180 / M_PI;
}

inline sf::Vector2f getUnitVec(sf::Vector2f pos1, sf::Vector2f pos2)
{
	sf::Vector2f dist = sf::Vector2f(pos2 - pos1);
	float mag = sqrt(pow(dist.x, 2) + pow(dist.y, 2));
	//std::cout << newUnit.getBody()->first.x << ", " << newUnit.getBody()->first.y << std::endl;
	return sf::Vector2f(sf::Vector2f(dist.x / mag, dist.y / mag));
}



inline sf::Vector2f multiplyVectors(const sf::Vector2f &first, const sf::Vector2f &second) {
	return sf::Vector2f(first.x * second.x, first.y * second.y);
}

inline float crossMultiply(const sf::Vector2f &first, const sf::Vector2f &second) {
	return first.x * second.y - first.y * second.x;
}


class MockMutex {
public:
	void lock(){}
	void unlock(){}
};

inline bool isZero(float val) {
	return (abs(val) < std::numeric_limits<float>::epsilon());
}


inline bool LineSegementsIntersect(const sf::Vector2f &p, const sf::Vector2f &p2, const sf::Vector2f &q, const sf::Vector2f &q2,
	sf::Vector2f* intersection, bool considerCollinearOverlapAsIntersect = false)
{

	sf::Vector2f r = p2 - p;
	sf::Vector2f s = q2 - q;
	float rxs = crossMultiply(r, s);
	float qpxr = crossMultiply((q-p), r);

	// If r x s = 0 and (q - p) x r = 0, then the two lines are collinear.
	if (isZero(rxs) && isZero(qpxr))
	{
		// 1. If either  0 <= (q - p) * r <= r * r or 0 <= (p - q) * s <= * s
		// then the two lines are overlapping,
		if (considerCollinearOverlapAsIntersect)
			if (((0 <= multiplyVectors((q - p), r).x ||0 <= multiplyVectors((q - p), r).y) 
				&& (multiplyVectors((q - p), r).x <= multiplyVectors(r, r).x || multiplyVectors((q - p), r).y <= multiplyVectors(r, r).y))
				|| ((0 <= multiplyVectors((p - q), s).x || (0 <= multiplyVectors((p - q), s).y)) && (multiplyVectors((p - q), s).x <= multiplyVectors(s, s).x || multiplyVectors((p - q), s).y <= multiplyVectors(s, s).y)))
				return true;

		// 2. If neither 0 <= (q - p) * r = r * r nor 0 <= (p - q) * s <= s * s
		// then the two lines are collinear but disjoint.
		// No need to implement this expression, as it follows from the expression above.
		return false;
	}

	// 3. If r x s = 0 and (q - p) x r != 0, then the two lines are parallel and non-intersecting.
	if (isZero(rxs) && !isZero(qpxr))
		return false;

	// t = (q - p) x s / (r x s)
	float t = crossMultiply((q-p),s) / rxs;

	// u = (q - p) x r / (r x s)

	float u = crossMultiply((q-p),r) / rxs;

	// 4. If r x s != 0 and 0 <= t <= 1 and 0 <= u <= 1
	// the two line segments meet at the point p + t r = q + u s.
	if (!isZero(rxs) && (0 <= t && t <= 1) && (0 <= u && u <= 1))
	{
		// We can calculate the intersection point using either t or u.
		*intersection = p + multiplyVectors(sf::Vector2f(t,t), r);

		// An intersection was found.
		return true;
	}

	// 5. Otherwise, the two line segments are not parallel but do not intersect.
	return false;
}


inline bool isCircleInPolygon(const std::vector<sf::Vector2f> &points, const sf::Vector2f &circlePosition, float circleRadius)
{
	float radiusSquared = circleRadius * circleRadius;

	sf::Vector2f vertex = points[points.size() - 1];

	sf::Vector2f circleCenter = circlePosition;

	float nearestDistance = std::numeric_limits<float>::max();
	bool nearestIsInside = false;
	int nearestVertex = -1;
	bool lastIsInside = false;

	for (int i = 0; i < points.size(); i++)
	{
		sf::Vector2f nextVertex = points[i];

		sf::Vector2f axis = circleCenter - vertex;

		float distance = lengthSquared(axis) - radiusSquared;

		if (distance <= 0)
		{
			return true;
		}

		bool isInside = false;

		sf::Vector2f edge = nextVertex - vertex;

		float edgeLengthSquared = lengthSquared(edge);

		if (edgeLengthSquared != 0)
		{
			float dot = dotProduct(edge, axis);

			if (dot >= 0 && dot <= edgeLengthSquared)
			{
				sf::Vector2f projection = vertex + (dot / edgeLengthSquared) * edge;

				axis = projection - circleCenter;

				if (lengthSquared(axis) <= radiusSquared)
				{
					return true;
				}
				else
				{
					if (edge.x > 0)
					{
						if (axis.y > 0)
						{
							return false;
						}
					}
					else if (edge.x < 0)
					{
						if (axis.y < 0)
						{
							return false;
						}
					}
					else if (edge.y > 0)
					{
						if (axis.x < 0)
						{
							return false;
						}
					}
					else
					{
						if (axis.x > 0)
						{
							return false;
						}
					}

					isInside = true;
				}
			}
		}

		if (distance < nearestDistance)
		{
			nearestDistance = distance;
			nearestIsInside = isInside || lastIsInside;
			nearestVertex = i;
		}

		vertex = nextVertex;
		lastIsInside = isInside;
	}

	if (nearestVertex == 0)
	{
		return nearestIsInside || lastIsInside;
	}
	else
	{
		return nearestIsInside;
	}
}


inline int fast_atou(const char* str)
{
	int val = 0;
	while (*str) {
		val = val * 10 + (*str++ - '0');
	}
	return val;
}