#include "pch.h"
#include "CMyCurve.h"

CMyCurve::CMyCurve(CPoint p)
{
	points.push_back(p);
	m_color = RGB(rand() % 156 + 80, rand() % 156 + 80, rand() % 156 + 80);
}
void CMyCurve::drawShape(CDC& memDC)
{
	CPen pen(PS_SOLID, 3, m_color);
	memDC.SelectObject(&pen);

	memDC.MoveTo(points[0]);
	for (int i = 1; i < points.size() - 1; i++)
	{
		memDC.LineTo(points[i]);
		memDC.MoveTo(points[i]);
	}memDC.LineTo(points[points.size() - 1]);
}
void CMyCurve::editSize(CPoint p)
{
	points.push_back(p);
}
bool CMyCurve::ShapeClick(CPoint p)
{
	for (auto e : points)
	{
		if (abs(e.x - p.x) < 5 && abs(e.y - p.y) < 5)
			return true;
	}
	return false;
}
CRect CMyCurve::RetBoundary()
{
	CPoint minPoint = points.front();
	CPoint maxPoint = points.front();
	for (auto e : points)
	{
		if (minPoint.x > e.x)
			minPoint.x = e.x;
		if (minPoint.y > e.y)
			minPoint.y = e.y;
		if (maxPoint.x < e.x)
			maxPoint.x = e.x;
		if (maxPoint.y < e.y)
			maxPoint.y = e.y;
	}
	return CRect(minPoint.x - 8 , minPoint.y - 8, maxPoint.x + 8, maxPoint.y + 8);
}

void CMyCurve::ShapeMove(int dx, int dy)
{
	for (auto& e : points)
	{
		e.x += dx;
		e.y += dy;
	}
}