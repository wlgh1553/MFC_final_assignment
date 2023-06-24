#include "pch.h"
#include "CMyRect.h"

CMyRect::CMyRect(CPoint p)
{
	m_start = p;
	m_end = p;
	for (int i = 0; i < 3; i++)
		m_color[i] = rand() % 156 + 100;
}
void CMyRect::drawShape(CDC& memDC)
{
	CPen pen(PS_SOLID, 4, RGB(m_color[0] - 40, m_color[1] - 40, m_color[2] - 40));
	memDC.SelectObject(&pen);
	CBrush brush(RGB(m_color[0], m_color[1], m_color[2]));
	memDC.SelectObject(&brush);

	//m_start가 m_end보다 오른쪽, 아래에 있는 경우
	CPoint left = m_start, right = m_end;
	checkLeftRight(left, right);
	
	memDC.Rectangle(left.x, left.y, right.x, right.y);
}
void CMyRect::editSize(CPoint p)
{
	m_end = p;
}
bool CMyRect::ShapeClick(CPoint p)
{
	CPoint left = m_start, right = m_end;
	checkLeftRight(left, right);
	if (left.x < p.x && p.x < right.x
		&& left.y < p.y && p.y < right.y)
		return true;
	return false;
}

//사각형을 그리는 4가지 방식 -> 어떤 방식으로 그려도 생성되도록...!
void CMyRect::checkLeftRight(CPoint& left, CPoint& right)
{
	int minX = left.x < right.x ? left.x : right.x;
	int maxX = left.x > right.x ? left.x : right.x;
	int minY = left.y < right.y ? left.y : right.y;
	int maxY = left.y > right.y ? left.y : right.y;
	left = CPoint(minX, minY);
	right = CPoint(maxX, maxY);
}

CRect CMyRect::RetBoundary()
{
	CPoint left = m_start, right = m_end;
	checkLeftRight(left, right);
	return CRect(left.x - 8, left.y - 8, right.x + 8, right.y + 8);
}

void CMyRect::ShapeMove(int dx, int dy)
{
	m_start.x += dx;
	m_start.y += dy;
	m_end.x += dx;
	m_end.y += dy;
}