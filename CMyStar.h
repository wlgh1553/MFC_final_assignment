#ifndef C_MY_STAR_H__
#define C_MY_STAR_H__
#include "CMyShape.h"
class CMyStar : public CMyShape
{
	CPoint m_start;
	int m_radius;
	int m_color[3]; //RGB
	CPoint m_outline[10];//윤곽선 구성 점 기억
public:
	CMyStar(CPoint p);

	//그리기
	virtual void drawShape(CDC& memDC);
	virtual void editSize(CPoint p);

	//선택
	virtual bool ShapeClick(CPoint p); //이 위치는 클릭한 위치인가.
	virtual CRect RetBoundary();

	//움직임
	virtual void ShapeMove(int dx, int dy);
};

#endif