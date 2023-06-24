﻿
// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "마지막과제.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	drawSort = 0;
	sel_Rect.first = CPoint(0, 0);
	sel_Rect.second = CPoint(0, 0);
	catchPos = CPoint(0, 0);
	is_moving = false;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_DRAW_RECTANGLE, &CChildView::OnDrawRectangle)
	ON_COMMAND(ID_DRAW_CIRCLE, &CChildView::OnDrawCircle)
	ON_COMMAND(ID_DRAW_CURVE, &CChildView::OnDrawCurve)
	ON_COMMAND(ID_DRAW_STAR, &CChildView::OnDrawStar)
	ON_COMMAND(ID_ACTION_SELECT, &CChildView::OnActionSelect)
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	CRect rect;
	GetClientRect(&rect);

	// double buffering
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	memDC.SelectObject(&bitmap);

	//배경
	memDC.Rectangle(rect);

	//개수 출력
	CString n_shape, n_sel;
	n_shape.Format(_T("Number of Shapes: %d"), shapes.size());
	n_sel.Format(_T("Number of Selected: %d"), getSelCnt());

	memDC.TextOutW(0, 0, n_shape);
	memDC.TextOutW(0, 20, n_sel);

	//배경 텍스트
	CFont font; font.CreatePointFont(400, _T("Arial")); memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(240, 200, 200));
	memDC.DrawText(_T("C++ Homework Final"), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//선택용 사각형 출력
	if (drawSort == 4)
	{
		CPen pen(PS_DOT, 1, RGB(0, 0, 0));
		memDC.SelectObject(&pen);
		memDC.SelectStockObject(NULL_BRUSH);
		memDC.Rectangle(sel_Rect.first.x, sel_Rect.first.y, sel_Rect.second.x, sel_Rect.second.y);
	}

	//도형 출력
	for (auto e : shapes)
	{
		if (e->is_selected)
		{
			//boundary
			CPen pen(PS_DOT, 1, RGB(255, 0, 0));
			memDC.SelectObject(&pen);
			memDC.SelectStockObject(NULL_BRUSH);
			CRect out = e->RetBoundary();
			memDC.Rectangle(out);
		}
		e->drawShape(memDC);
	}

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	//return CWnd::OnEraseBkgnd(pDC);
	return true;
}
void CChildView::FinishSelObj()
{
	for (auto& e : shapes)
		e->is_selected = false;	
}
bool CChildView::SelectObjByClick(CPoint point)
{
	bool is_sel = false;
	//뒤부터 탐색 (화면상 위에 있는 것을 찾기)
	for (int i = shapes.size() - 1; i >= 0; i--)
	{
		if (!shapes[i]->is_selected && shapes[i]->ShapeClick(point))
		{
			shapes[i]->is_selected = true; 
			is_sel = true; break;
		}
	}
	Invalidate();
	return is_sel;
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//쉬프트 선택 상황 확인
	bool is_shift_sel = false;
	if (nFlags & MK_SHIFT) 
		is_shift_sel = true;

	switch (drawSort)
	{
	case 0: {shapes.push_back(new CMyRect(point)); Invalidate(); break; }
	case 1: {shapes.push_back(new CMyCircle(point)); Invalidate(); break; }
	case 2: {shapes.push_back(new CMyCurve(point)); Invalidate(); break; }
	case 3: {shapes.push_back(new CMyStar(point)); Invalidate(); break; }
	case 4: 
	{
		//도형 이동을 위한 초기화
		catchPos = point;

		//범위 지정 선택을 위한 초기화
		sel_Rect.first = point; sel_Rect.second = point;

		//단일 클릭 선택 vs 쉬프트 클릭 선택
		if (!is_shift_sel)
			FinishSelObj();
		is_moving = SelectObjByClick(point);
		Invalidate(); break; 
	}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nFlags & VK_LBUTTON)
	{
		if(drawSort <= 3)
		{
			shapes.back()->editSize(point);
			Invalidate();
		}
		else if (drawSort == 4)
		{
			if (is_moving) //마우스가 도형 내부를 클릭 & 움직임 => 이동시킴
			{
				int dx = point.x - catchPos.x;
				int dy = point.y - catchPos.y;
				for (auto e : shapes)
				{
					if (e->is_selected)
					{
						e->ShapeMove(dx, dy);
					}
				}
				catchPos = point;
			}
			else //마우스가 도형 외부를 클릭 & 움직임 => 직사각형 범위 그림
			{
				sel_Rect.second = point;
			}
			Invalidate();
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (drawSort == 4)
	{
		if (is_moving)
		{

		}
		else
		{
			CPoint left = sel_Rect.first;
			CPoint right = sel_Rect.second;
			CMyRect::checkLeftRight(left, right);  //왼쪽 오른쪽 보정

			for (int i = 0; i < shapes.size(); i++)
			{
				if (shapes[i]->is_selected == true)continue;
				CRect bound = shapes[i]->RetBoundary();
				if (bound.TopLeft().x < left.x || bound.TopLeft().y < left.y ||
					bound.BottomRight().x > right.x || bound.BottomRight().y > right.y)
					continue;

				shapes[i]->is_selected = true;
			}
		}
	}

	sel_Rect.first = CPoint(0, 0);
	sel_Rect.second = CPoint(0, 0);
	Invalidate();

	CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnDrawRectangle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	drawSort = 0;
	FinishSelObj();
}
void CChildView::OnDrawCircle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	drawSort = 1;
	FinishSelObj();
}
void CChildView::OnDrawCurve()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	drawSort = 2;
	FinishSelObj();
}
void CChildView::OnDrawStar()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	drawSort = 3;
	FinishSelObj();
}

void CChildView::OnActionSelect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	drawSort = 4;
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nChar == VK_DELETE && drawSort == 4)
	{
		if(!shapes.empty())
		{
			auto iter = shapes.begin();
			while (iter != shapes.end())
			{
				if ((*iter)->is_selected)
					iter = shapes.erase(iter);
				else
					iter++;
			}
			FinishSelObj();
			Invalidate();
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CChildView::getSelCnt()
{
	int cnt = 0;
	for (auto e : shapes)
	{
		if (e->is_selected)cnt++;
	}
	return cnt;
}