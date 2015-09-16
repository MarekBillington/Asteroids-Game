#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

#include <cmath>

class Vector2D
{
public:
	Vector2D();
	Vector2D(float x, float y);
	~Vector2D();
	float getLength();
	void normalise();
	void add(Vector2D v);
	void mult(float n);
	void div(float n);
	static float dot(Vector2D v, Vector2D u);
	static Vector2D proj(Vector2D s, Vector2D t);
	float GetX();
	float GetY();
	void SetX(float x);
	void SetY(float y);


private:
	float m_x;
	float m_y;

};


#endif // __VECTOR2D_H__