#ifndef C_MY_SHAPE_H__
#define C_MY_SHAPE_H__

#include<vector>
class CMyShape
{
public: 
	//�׷�ȭ
	std::vector<CMyShape*>childShapes;

	//������
	CMyShape();

	//�׸���
	virtual void drawShape(CDC&memDC);
	virtual void editSize(CPoint p);

	//����
	bool is_selected;
	virtual bool ShapeClick(CPoint p); //�� ��ġ�� Ŭ���� ��ġ�ΰ�.
	virtual CRect RetBoundary();

	//������
	virtual void ShapeMove(int dx, int dy);
};

#endif