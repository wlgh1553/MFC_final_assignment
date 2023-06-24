#ifndef C_MY_RECT_H__
#define C_MY_RECT_H__
#include "CMyShape.h"
class CMyRect : public CMyShape
{
	CPoint m_start, m_end;
	int m_color[3]; //RGB
public:
	CMyRect(CPoint p);

	//왼쪽 오른쪽 체크 (사각형 그려서 영역 선택 시 필요하므로 static)
	static void checkLeftRight(CPoint& left, CPoint& right);

	//그리기
	virtual void drawShape(CDC& memDC);
	virtual void editSize(CPoint p);

	//선택
	virtual bool ShapeClick(CPoint p);
	virtual CRect RetBoundary();

	//움직임
	virtual void ShapeMove(int dx, int dy);
};

#endif