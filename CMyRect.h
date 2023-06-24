#ifndef C_MY_RECT_H__
#define C_MY_RECT_H__
#include "CMyShape.h"
class CMyRect : public CMyShape
{
	CPoint m_start, m_end;
	int m_color[3]; //RGB
public:
	CMyRect(CPoint p);

	//���� ������ üũ (�簢�� �׷��� ���� ���� �� �ʿ��ϹǷ� static)
	static void checkLeftRight(CPoint& left, CPoint& right);

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