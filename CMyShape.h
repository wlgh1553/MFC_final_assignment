#ifndef C_MY_SHAPE_H__
#define C_MY_SHAPE_H__

#include<vector>
class CMyShape
{
public: 
	//그룹화
	std::vector<CMyShape*>childShapes;

	//생성자
	CMyShape();

	//그리기
	virtual void drawShape(CDC&memDC);
	virtual void editSize(CPoint p);

	//선택
	bool is_selected;
	virtual bool ShapeClick(CPoint p); //이 위치는 클릭한 위치인가.
	virtual CRect RetBoundary();

	//움직임
	virtual void ShapeMove(int dx, int dy);
};

#endif