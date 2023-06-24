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

	//�׸���
	virtual void drawShape(CDC& memDC);
	virtual void editSize(CPoint p);

	//����
	virtual bool ShapeClick(CPoint p);
	virtual CRect RetBoundary();

	//������
	virtual void ShapeMove(int dx, int dy);
};

#endif