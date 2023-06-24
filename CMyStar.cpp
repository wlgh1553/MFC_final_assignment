#include "pch.h"
#include "CMyStar.h"

CMyStar::CMyStar(CPoint p)
{
	m_start = p;
	m_radius = 0;
	for (int i = 0; i < 3; i++)
		m_color[i] = rand() % 156 + 100;
}
void CMyStar::drawShape(CDC& memDC)
{
	CBrush brush(RGB(m_color[0], m_color[1], m_color[2]));
	memDC.SelectObject(&brush);

	//점 설정
	CPoint temp[5];
	double theta = -90;
	for (int i = 0; i < 5; i++)
	{
		int x = m_start.x + (double)m_radius * cos(theta / 180 * 3.14);
		int y = m_start.y + (double)m_radius * sin(theta / 180 * 3.14);
		temp[i] = CPoint(x, y);

		theta += 72;
	}

	//별 내부 칠하기
	CPoint p[5] = { temp[0], temp[2], temp[4], temp[1], temp[3] };
	CRgn rgn;
	rgn.CreatePolygonRgn(p, 5, WINDING); 
	memDC.PaintRgn(&rgn);

	//윤곽선 점 잡기
	theta = -90 + 36; int line_idx = 0;
	for (int i = 0; i < 5; i++)
	{
		m_outline[line_idx++] = temp[i];
		int x = m_start.x + (double)m_radius/2.6 * cos(theta / 180 * 3.14);
		int y = m_start.y + (double)m_radius/2.6 * sin(theta / 180 * 3.14);
		CPoint mid = CPoint(x, y);
		m_outline[line_idx++] = mid;
		theta += 72;
	}

	//윤곽선 그리기
	CPen pen(PS_SOLID, 4, RGB(m_color[0] - 40, m_color[1] - 40, m_color[2] - 40));
	memDC.SelectObject(&pen);
	for (int i = 0; i < 10; i++)
	{
		int j = (i + 1) % 10;
		memDC.MoveTo(m_outline[i]);
		memDC.LineTo(m_outline[j]);
	}
}
void CMyStar::editSize(CPoint p)
{
	m_radius = sqrt(pow(p.x - m_start.x, 2) + pow(p.y - m_start.y, 2));
}
bool CMyStar::ShapeClick(CPoint p)
{ 
	//내부 : 홀수, 외부 : 짝수
	int cross_cnt = 0;

	for (int i = 0; i < 10; i++)
	{
		int j = (i + 1) % 10; //i번째 vertex, j번째 vetex

		//Vi와 Vj를 잇는 edge
		//조건 1 y값이 두 점 사이에 있는가?
		int minY = min(m_outline[i].y, m_outline[j].y);
		int maxY = max(m_outline[i].y, m_outline[j].y);

		if (p.y < minY || p.y > maxY)
			continue;

		//조건 2 x값이 edge 오른쪽에 있는가?
		double edgeX = (double)(p.y - m_outline[i].y) * (m_outline[i].x - m_outline[j].x) /
			(m_outline[i].y - m_outline[j].y) + m_outline[i].x;
		if (edgeX < p.x)
			cross_cnt++;
	}
	return (cross_cnt % 2 == 1);
}
CRect CMyStar::RetBoundary()
{
	CPoint minPoint = m_outline[0];
	CPoint maxPoint = m_outline[0];
	for (int i = 0; i < 10; i++)
	{
		if (minPoint.x > m_outline[i].x)
			minPoint.x = m_outline[i].x;
		if (minPoint.y > m_outline[i].y)
			minPoint.y = m_outline[i].y;
		if (maxPoint.x < m_outline[i].x)
			maxPoint.x = m_outline[i].x;
		if (maxPoint.y < m_outline[i].y)
			maxPoint.y = m_outline[i].y;
	}
	return CRect(minPoint.x - 8, minPoint.y - 8, maxPoint.x +8, maxPoint.y + 8);
}

void CMyStar::ShapeMove(int dx, int dy)
{
	m_start.x += dx;
	m_start.y += dy;
}