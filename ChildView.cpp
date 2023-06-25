
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
	ON_COMMAND(ID_ACTION_GROUP, &CChildView::OnActionGroup)
	ON_COMMAND(ID_ACTION_UNGROUP, &CChildView::OnActionUngroup)
	ON_COMMAND(ID_ALIGN_BRINGFRONT, &CChildView::OnAlignBringfront)
	ON_COMMAND(ID_ALIGN_BRINTBACK, &CChildView::OnAlignBrintback)
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECTANGLE, &CChildView::OnUpdateRectMenu)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CIRCLE, &CChildView::OnUpdateCircleMenu)
	ON_UPDATE_COMMAND_UI(ID_DRAW_CURVE, &CChildView::OnUpdateCurveMenu)
	ON_UPDATE_COMMAND_UI(ID_DRAW_STAR, &CChildView::OnUpdateStarMenu)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SELECT, &CChildView::OnUpdateSelectMenu)
	ON_UPDATE_COMMAND_UI(ID_ACTION_GROUP, &CChildView::OnUpdateGroupMenu)
	ON_UPDATE_COMMAND_UI(ID_ACTION_UNGROUP, &CChildView::OnUpdateUngroupMenu)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_BRINGFRONT, &CChildView::OnUpdateBFMenu)
	ON_UPDATE_COMMAND_UI(ID_ALIGN_BRINTBACK, &CChildView::OnUpdateBBMenu)
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
	memDC.DrawText(_T("22011824 이지호"), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

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

	//선택용 사각형 출력
	if (drawSort == 4)
	{
		CPen pen(PS_DOT, 1, RGB(0, 0, 0));
		memDC.SelectObject(&pen);
		memDC.SelectStockObject(NULL_BRUSH);
		memDC.Rectangle(sel_Rect.first.x, sel_Rect.first.y, sel_Rect.second.x, sel_Rect.second.y);
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

	//마우스 신호 독점
	SetCapture();

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
		if (!is_moving)
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
	//마우스 신호 독점 해방
	ReleaseCapture();

	CWnd::OnLButtonUp(nFlags, point);
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

void CChildView::OnActionGroup()
{
	if (getSelCnt() > 1)
	{
		DoGroup();
	}
}


void CChildView::OnActionUngroup()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (getSelCnt() >= 1)
	{
		DoUngroup();
	}
}

void  CChildView::DoGroup()
{
	CMyShape* makeGroup = new CMyShape();
	makeGroup->is_selected = true;

	//해당 도형들을 벡터에서 없앰, 그룹의 child로 만들기
	auto iter = shapes.begin();
	while (iter != shapes.end())
	{
		if ((*iter)->is_selected)
		{
			makeGroup->childShapes.push_back(*iter);
			iter = shapes.erase(iter);
		}
		else
			iter++;
	}
	//그룹을 다시 벡터에 꿰어주기
	shapes.push_back(makeGroup);

	Invalidate();
}

void  CChildView::DoUngroup()
{
	//그룹 속 도형 기록
	std::vector<CMyShape*>del;

	//그룹 해제
	auto iter = shapes.begin();
	while (iter != shapes.end())
	{	
		//그룹화 되어있는가&&선택되었는가
		if ((*iter)->is_selected && !(*iter)->childShapes.empty())
		{
			for (auto e : (*iter)->childShapes)
				del.push_back(e);
			iter = shapes.erase(iter);
		}
		else
			iter++;
	}

	//도형 다시 shapes벡터에 꿰기
	for (auto e : del)
		shapes.push_back(e);

	Invalidate();
}

void CChildView::OnAlignBringfront()
{
	//위치 변경할 도형들 기록 + 삭제
	std::vector<CMyShape*>aligns;
	auto iter = shapes.begin();
	while (iter != shapes.end())
	{
		if ((*iter)->is_selected)
		{
			aligns.push_back(*iter);
			iter = shapes.erase(iter);
		}
		else
			iter++;
	}

	//shapes벡터의 맨뒤에 다시 꿰기
	for (auto e : aligns)
	{
		shapes.push_back(e);
	}

	Invalidate();
}


void CChildView::OnAlignBrintback()
{
	//위치 변경할 도형들 기록 + 삭제
	std::vector<CMyShape*>aligns;
	auto iter = shapes.begin();
	while (iter != shapes.end())
	{
		if ((*iter)->is_selected)
		{
			aligns.push_back(*iter);
			iter = shapes.erase(iter);
		}
		else
			iter++;
	}

	//shapes벡터의 맨앞에 다시 꿰기
	for (auto iter = aligns.rbegin(); iter != aligns.rend(); iter++)
		shapes.insert(shapes.begin(), *iter);

	Invalidate();
}

//메뉴 관련
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


void CChildView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenuW(IDR_MAINFRAME);

	CMenu* pMenu = menu.GetSubMenu(4);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		point.x, point.y, AfxGetMainWnd());
}

void CChildView::OnUpdateRectMenu(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(drawSort == 0);
}
void CChildView::OnUpdateCircleMenu(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(drawSort == 1);
}
void CChildView::OnUpdateCurveMenu(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(drawSort == 2);
}
void CChildView::OnUpdateStarMenu(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(drawSort == 3);
}
void CChildView::OnUpdateSelectMenu(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(drawSort == 4);
}
void CChildView::OnUpdateGroupMenu(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(getSelCnt() >= 2);
}
void CChildView::OnUpdateUngroupMenu(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(getSelCnt() >= 1);
}
void CChildView::OnUpdateBFMenu(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(getSelCnt() > 0);
}
void CChildView::OnUpdateBBMenu(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(getSelCnt() > 0);
}