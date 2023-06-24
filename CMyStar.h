#ifndef C_MY_STAR_H__
#define C_MY_STAR_H__
#include "CMyShape.h"
class CMyStar : public CMyShape
{
	CPoint m_start;
	int m_radius;
	int m_color[3]; //RGB
	CPoint m_outline[10];//������ ���� �� ���
public:
	CMyStar(CPoint p);

	//�׸���
	virtual void drawShape(CDC& memDC);
	virtual void editSize(CPoint p);

	//����
	virtual bool ShapeClick(CPoint p); //�� ��ġ�� Ŭ���� ��ġ�ΰ�.
	virtual CRect RetBoundary();

	//������
	virtual void ShapeMove(int dx, int dy);
};

#endif