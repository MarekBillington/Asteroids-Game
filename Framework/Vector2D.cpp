#include "Vector2D.h"

#include <cmath>

Vector2D::Vector2D()
: m_x(0), m_y(0)
{
}

Vector2D::Vector2D(float x, float y)
{
	m_x = x;
	m_y = y;
}

Vector2D::~Vector2D()
{
}

void
Vector2D::add(Vector2D v)
{
	m_x += v.m_x;
	m_y += v.m_y;
}

void
Vector2D::mult(float n)
{
	m_x *= n;
	m_y *= n;
}

void
Vector2D::div(float n)
{
	m_x /= n;
	m_y /= n;
}

float
Vector2D::getLength()
{
	return sqrt((m_x*m_x) + (m_y*m_y));
}

void
Vector2D::normalise()
{
	float length = getLength();
	m_x = m_x / length;
	m_y = m_y / length;
}

float
Vector2D::dot(Vector2D v, Vector2D u)
{
	return (v.GetX()*u.GetX()) + (v.GetY()*u.GetY());
}


Vector2D
Vector2D::proj(Vector2D s, Vector2D t)
{

	t.normalise();
	Vector2D proj = t;
	float multiple = proj.dot(s, t) / proj.dot(t, t);
	proj.mult(multiple);
	return proj;

}

float
Vector2D::GetX()
{
	return (m_x);
}

float
Vector2D::GetY()
{
	return (m_y);
}

void
Vector2D::SetX(float x)
{
	m_x = x;
}

void
Vector2D::SetY(float y)
{
	m_y = y;
}