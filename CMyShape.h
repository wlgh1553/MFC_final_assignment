#ifndef C_MY_SHAPE_H__
#define C_MY_SHAPE_H__
class CMyShape
{
public: 
	bool is_selected;
	CMyShape();

	//그리기
	virtual void drawShape(CDC&memDC) = 0;
	virtual void editSize(CPoint p) = 0;

	//선택
	virtual bool ShapeClick(CPoint p) = 0; //이 위치는 클릭한 위치인가.
	virtual CRect RetBoundary() = 0;

	//움직임
	virtual void ShapeMove(int dx, int dy) = 0;
};

#endif