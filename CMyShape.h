#ifndef C_MY_SHAPE_H__
#define C_MY_SHAPE_H__
class CMyShape
{
public: 
	bool is_selected;
	CMyShape();

	//�׸���
	virtual void drawShape(CDC&memDC) = 0;
	virtual void editSize(CPoint p) = 0;

	//����
	virtual bool ShapeClick(CPoint p) = 0; //�� ��ġ�� Ŭ���� ��ġ�ΰ�.
	virtual CRect RetBoundary() = 0;

	//������
	virtual void ShapeMove(int dx, int dy) = 0;
};

#endif