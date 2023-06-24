#ifndef C_MY_CIRCLE_H__
#define C_MY_CIRCLE_H__
#include "CMyShape.h"
class CMyCircle : public CMyShape
{
	CPoint m_start;
	int m_radius;
	int m_color[3]; //RGB
public:
	CMyCircle(CPoint p);

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