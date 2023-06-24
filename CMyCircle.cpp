#include "pch.h"
#include "CMyCircle.h"

CMyCircle::CMyCircle(CPoint p)
{
	m_start = p;
	m_radius = 0;
	for (int i = 0; i < 3; i++)
		m_color[i] = rand() % 156 + 100;
}
void CMyCircle::drawShape(CDC& memDC)
{
	CPen pen(PS_SOLID, 4, RGB(m_color[0] - 40, m_color[1] - 40, m_color[2] - 40));
	memDC.SelectObject(&pen);
	CBrush brush(RGB(m_color[0], m_color[1], m_color[2]));
	memDC.SelectObject(&brush);

	memDC.Ellipse(m_start.x - m_radius, m_start.y - m_radius,
		m_start.x + m_radius, m_start.y + m_radius);
}
void CMyCircle::editSize(CPoint p)
{
	m_radius = sqrt(pow(p.x - m_start.x, 2) + pow(p.y - m_start.y, 2));
}
bool CMyCircle::ShapeClick(CPoint p)
{
	int dist = sqrt(pow(p.x - m_start.x, 2) + pow(p.y - m_start.y, 2));
	if (dist <= m_radius)
		return true;
	return false;
}

CRect CMyCircle::RetBoundary()
{
	return CRect(m_start.x - m_radius - 8, m_start.y - m_radius - 8,
		m_start.x + m_radius + 8, m_start.y + m_radius + 8);
}

void CMyCircle::ShapeMove(int dx, int dy)
{
	m_start.x += dx;
	m_start.y += dy;
}