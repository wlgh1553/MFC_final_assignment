
// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once
#include"CMyShape.h"
#include"CMyRect.h" //여기다 넣는게 맞나??
#include"CMyCircle.h"
#include"CMyCurve.h"
#include"CMyStar.h"
#include<vector>

// CChildView 창

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

	//도형
	std::vector<CMyShape*>shapes;

	//메뉴
	int drawSort; //rect : 0 / circle : 1 / curve : 2 / star : 3 / 선택 : 4

	//선택
	std::pair<CPoint, CPoint>sel_Rect; //선택용 직사각형 좌표

	//움직임
	bool is_moving;
	CPoint catchPos; 


// 특성입니다.
public:

// 작업입니다.
public:
	bool SelectObjByClick(CPoint point);
	void FinishSelObj();
	int getSelCnt();
// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDrawRectangle();
	afx_msg void OnDrawCircle();
	afx_msg void OnDrawCurve();
	afx_msg void OnDrawStar();
	afx_msg void OnActionSelect();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

