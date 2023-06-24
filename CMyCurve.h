#ifndef C_MY_CURVE_H__
#define C_MY_CURVE_H__
#include "CMyShape.h"
#include<vector>
class CMyCurve : public CMyShape
{
	std::vector<CPoint>points;
	COLORREF m_color;
public:
	CMyCurve(CPoint p);

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