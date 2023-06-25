#include "pch.h"
#include "CMyShape.h"

CMyShape::CMyShape()
{
	is_selected = false;
}

//그룹의 경우를 생각
void CMyShape::drawShape(CDC& memDC)
{
	for (auto e : childShapes)
		e->drawShape(memDC);
}

bool CMyShape::ShapeClick(CPoint p)
{
	for (auto e : childShapes)
		if (e->ShapeClick(p))
			return true;
	return false;
}

CRect CMyShape::RetBoundary()
{
	CRect bound = childShapes.front()->RetBoundary();
	for (auto e : childShapes)
	{
		CRect boundrect = e->RetBoundary();
		int min_x = boundrect.TopLeft().x;
		int min_y = boundrect.TopLeft().y;
		int max_x = boundrect.BottomRight().x;
		int max_y = boundrect.BottomRight().y;
		if (bound.TopLeft().x > min_x)
			bound.TopLeft().x = min_x;
		if (bound.TopLeft().y > min_y)
			bound.TopLeft().y = min_y;
		if (bound.BottomRight().x < max_x)
			bound.BottomRight().x = max_x;
		if (bound.BottomRight().y < max_y)
			bound.BottomRight().y = max_y;
	}
	return bound;
}

void CMyShape::ShapeMove(int dx, int dy)
{
	for (auto e : childShapes)
	{
		e->ShapeMove(dx, dy);
	}
}

//그룹의 경우 크기 조정을 할 일이 없으므로
void CMyShape::editSize(CPoint p)
{
	;
}