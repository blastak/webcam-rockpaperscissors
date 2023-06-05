
// RockScissorsPaperDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "RockScissorsPaper.h"
#include "RockScissorsPaperDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.141592

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRockScissorsPaperDlg 대화 상자



CRockScissorsPaperDlg::CRockScissorsPaperDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ROCKSCISSORSPAPER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	startflag = 0;
	timercounter = 0;
	num = 3;
}

void CRockScissorsPaperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRockScissorsPaperDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
END_MESSAGE_MAP()


// CRockScissorsPaperDlg 메시지 처리기

CRockScissorsPaperDlg* obj;
void FrameCallbackProc(HWND hWnd, VIDEOHDR* hdr)
{
	if (hWnd && hdr && hdr->lpData && obj)
	{
		obj->Processing((unsigned char*)hdr->lpData);
	}
}

BOOL CRockScissorsPaperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	SetTimer(1, 100, NULL);

	HWND PARENT = m_hWnd;
	m_hWndCam = capCreateCaptureWindow(L"kkk", WS_CHILD | WS_VISIBLE, 0, 100, 320, 240, PARENT, NULL);

	if (capDriverConnect(m_hWndCam, 0) == FALSE) return FALSE;

	capPreviewRate(m_hWndCam, 33);
	capOverlay(m_hWndCam, TRUE);
	capPreview(m_hWndCam, TRUE);

	BITMAPINFO bmi;

	memset(&bmi.bmiHeader, 0, sizeof(bmi.bmiHeader));

	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = 320;
	bmi.bmiHeader.biHeight = 240;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;

	if (capSetVideoFormat(m_hWndCam, &bmi, sizeof(bmi)))
	{
		capSetCallbackOnFrame(m_hWndCam, FrameCallbackProc);
		obj = this;
		return TRUE;
	}
	else
		capDriverDisconnect(m_hWndCam);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CRockScissorsPaperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CRockScissorsPaperDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CRockScissorsPaperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRockScissorsPaperDlg::Processing(unsigned char* data)
{
	int i, j;
	int R, G, B;
	int max_RGB = -999, min_RGB = 999;
	int A;
	int Xt, Yt;
	int r;
	int I[240][320];
	int alpha = 0;
	int p = 0;
	double Y, Cb, Cr;
	finger = 0;

	while (p <= 320)
	{
		A = 0;
		Xt = 0;
		Yt = 0;
		for (i = 0; i < 240; i++)
		{
			for (j = 0; j < p; j++)
			{
				data2[240 - i - 1][j][0] = 0;
				data2[240 - i - 1][j][1] = 0;
				data2[240 - i - 1][j][2] = 0;
				I[240 - i - 1][j] = 0;
			}
			for (j = p; j < 320; j++)
			{
				R = data2[240 - i - 1][j][0] = data[(320 * i + j) * 3 + 2]; //R
				G = data2[240 - i - 1][j][1] = data[(320 * i + j) * 3 + 1]; //G
				B = data2[240 - i - 1][j][2] = data[(320 * i + j) * 3 + 0]; //B
				/*if(R < G && R < B)
				{
					if(G<B) max_RGB=B;
					else max_RGB=G;
					min_RGB=R;
				}
				else if(G<R && G<B)
				{
					if(R<B) max_RGB=B;
					else max_RGB=R;
					min_RGB=G;
				}
				else
				{
					if(R<G) max_RGB=G;
					else max_RGB=R;
					min_RGB=B;
				}*/
				Y = (0.299 * R + (1 - 0.114 - 0.299) * G + 0.114 * B);
				Cb = (0.5 / (1 - 0.114) * (B - Y));
				Cr = (0.5 / (1 - 0.299) * (R - Y));
				//				if(R>95 && G>40 && B>20 && abs(R-G)>15 && R>G && R>B && max_RGB-min_RGB>15)
				if (Cr >= 5 && Cr <= 80)
					//if(R>55 && G>30 && B>20 && abs(R-G)>10 && R>G && R>B && max_RGB-min_RGB>15)
				{
					data2[240 - i - 1][j][0] = 255;
					data2[240 - i - 1][j][1] = 255;
					data2[240 - i - 1][j][2] = 255;
					A++;
					I[240 - i - 1][j] = 1;
				}
				else
				{
					data2[240 - i - 1][j][0] = 0;
					data2[240 - i - 1][j][1] = 0;
					data2[240 - i - 1][j][2] = 0;
					I[240 - i - 1][j] = 0;
				}
			}
		}
		//무게 중심 찾기
		for (i = 0; i < 320 * 240; i++)
		{
			Xt = (int)(Xt + (i % 320) * I[i / 320][i % 320]);
			Yt = (int)(Yt + (i / 320) * I[i / 320][i % 320]);
		}
		x_bar = (int)(Xt / (A + 1));
		y_bar = (int)(Yt / (A + 1));

		Sq_F(); // 사각형중심 찾기

		p = p + 5;

		if (x_max - x_min <= (y_max - y_min) * 1.4)
			break;
		if (abs(x_bar - (int)x2) >= 2) //거리가 멀 때
			continue;
		else
			break;
	}
	if (x_min + 10 < 320)
	{
		for (i = y_min; i <= y_max; i++) //왼쪽 잘라버리기
		{
			for (j = x_min; j <= x_min + 10; j++)
			{
				data2[i][j][0] = 0;
				data2[i][j][1] = 0;
				data2[i][j][2] = 0;
			}
		}
		x_min += 10;
	}
	block_F(); //잡음제거 블록

	x_bar += 18;

	Mugejungsim(); //무게 중심 그리기
	Mugejungsim2(); //사각형중심 그리기

	r = (int)sqrt((A - alpha) / PI); //반지름
	r = (int)(r + r * 0.18);
	finger = Circle_F(r); //원 그리기
	Rect_F(); //사각형 그리기
	Invalidate(false);
	Reload();
}

void CRockScissorsPaperDlg::Reload()
{
	int i, j;
	CClientDC dc(this);
	for (i = 0; i < 240; i++)
		for (j = 0; j < 320; j++)
			dc.SetPixel(j + 641, i + 100, RGB(data2[i][j][0], data2[i][j][1], data2[i][j][2]));
}

void CRockScissorsPaperDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (startflag == 1)
	{
		int temp;

		CClientDC dc(this);
		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc);
		CBitmap bitmap_rand, bitmap_count;
		CBitmap* pOldBitmap;
		BITMAP bmpINFO;

		timercounter++;

		temp = GetTickCount();
		srand(temp);
		rand_var = rand() % 3;
		switch (rand_var)
		{
		case 0:
			bitmap_rand.LoadBitmap(IDB_BITMAP_MOOK);
			break;
		case 1:
			bitmap_rand.LoadBitmap(IDB_BITMAP_JJI);
			break;
		case 2:
			bitmap_rand.LoadBitmap(IDB_BITMAP_BBA);
			break;
		}
		bitmap_rand.GetBitmap(&bmpINFO);
		SetWindowPos(this, 0, 0, bmpINFO.bmWidth, bmpINFO.bmHeight, SWP_NOSIZE);
		pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap_rand);
		dc.BitBlt(641, 341, bmpINFO.bmWidth, bmpINFO.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);
		if (timercounter >= 10)
		{
			switch (num--)
			{
			case 3:
				bitmap_count.LoadBitmap(IDB_BITMAP_THREE);
				break;
			case 2:
				bitmap_count.LoadBitmap(IDB_BITMAP_TWO);
				break;
			case 1:
				bitmap_count.LoadBitmap(IDB_BITMAP_ONE);
				break;
			case 0:
				bitmap_count.LoadBitmap(IDB_BITMAP_ZERO);
				break;
			}
			timercounter = 0;
			bitmap_count.GetBitmap(&bmpINFO);
			SetWindowPos(this, 0, 0, bmpINFO.bmWidth, bmpINFO.bmHeight, SWP_NOSIZE);
			pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap_count);
			dc.BitBlt(321, 100, bmpINFO.bmWidth, bmpINFO.bmHeight, &MemDC, 0, 0, SRCCOPY);
			MemDC.SelectObject(pOldBitmap);
			if (num < 0)
			{
				startflag = 2;
				Drawing();
			}
		}
	}
	else if (startflag == 0)
	{
		CClientDC dc(this);
		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc);
		CBitmap bitmap_user, bitmap_result, bitmap_rand, bitmap_count;
		CBitmap* pOldBitmap;
		BITMAP bmpINFO;

		bitmap_user.LoadBitmap(IDB_BITMAP_REV_MOOK);
		bitmap_result.LoadBitmap(IDB_BITMAP_VS);
		bitmap_rand.LoadBitmap(IDB_BITMAP_MOOK);
		bitmap_count.LoadBitmap(IDB_BITMAP_ZERO);

		bitmap_user.GetBitmap(&bmpINFO);
		SetWindowPos(this, 0, 0, bmpINFO.bmWidth, bmpINFO.bmHeight, SWP_NOSIZE);
		pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap_user);
		dc.BitBlt(1, 341, bmpINFO.bmWidth, bmpINFO.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);

		bitmap_result.GetBitmap(&bmpINFO);
		SetWindowPos(this, 0, 0, bmpINFO.bmWidth, bmpINFO.bmHeight, SWP_NOSIZE);
		pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap_result);
		dc.BitBlt(321, 341, bmpINFO.bmWidth, bmpINFO.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);

		bitmap_rand.GetBitmap(&bmpINFO);
		SetWindowPos(this, 0, 0, bmpINFO.bmWidth, bmpINFO.bmHeight, SWP_NOSIZE);
		pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap_rand);
		dc.BitBlt(641, 341, bmpINFO.bmWidth, bmpINFO.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);

		bitmap_count.GetBitmap(&bmpINFO);
		SetWindowPos(this, 0, 0, bmpINFO.bmWidth, bmpINFO.bmHeight, SWP_NOSIZE);
		pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap_count);
		dc.BitBlt(321, 101, bmpINFO.bmWidth, bmpINFO.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);
	}
	CDialog::OnTimer(nIDEvent);
}

void CRockScissorsPaperDlg::Mugejungsim()//무게중심표시함수
{
	if (y_bar > 0 && y_bar < 240 - 1 && x_bar>0 && x_bar < 320 - 1)
	{
		data2[y_bar][x_bar][0] = 255;
		data2[y_bar][x_bar][1] = 0;
		data2[y_bar][x_bar][2] = 0;

		data2[y_bar - 1][x_bar][0] = 255;
		data2[y_bar - 1][x_bar][1] = 0;
		data2[y_bar - 1][x_bar][2] = 0;

		data2[y_bar + 1][x_bar][0] = 255;
		data2[y_bar + 1][x_bar][1] = 0;
		data2[y_bar + 1][x_bar][2] = 0;

		data2[y_bar][x_bar - 1][0] = 255;
		data2[y_bar][x_bar - 1][1] = 0;
		data2[y_bar][x_bar - 1][2] = 0;

		data2[y_bar][x_bar + 1][0] = 255;
		data2[y_bar][x_bar + 1][1] = 0;
		data2[y_bar][x_bar + 1][2] = 0;
	}
}

int CRockScissorsPaperDlg::Circle_F(int r)//원 표시 함수
{
	int i, x, y;
	int flag = 0;
	int flag2 = 0;
	int cnt = 0;
	for (i = 0; i < 360; i++)
	{
		x = (int)(r * cos((PI / 180) * i));
		y = (int)(r * sin((PI / 180) * i));
		if (y_bar + y > 0 && y_bar + y < 240 - 1 && x_bar + x>0 && x_bar + x < 320 - 1)
		{
			if (data2[y_bar + y][x_bar + x][0] == 255 && data2[y_bar + y][x_bar + x][1] == 255 && data2[y_bar + y][x_bar + x][2] == 255) //현 픽셀이 흰색이라면
			{
				if (flag < 7)
				{
					flag++;
					if (flag >= 7)
						cnt++;
				}
			}
			else
			{
				if (data2[y_bar + y][x_bar + x][0] == 0 && data2[y_bar + y][x_bar + x][1] == 0 && data2[y_bar + y][x_bar + x][2] == 0)
				{
					if (flag2 < 3)
					{
						flag2++;
						if (flag2 >= 3)
						{
							flag2 = 0;
							if (flag > 0)
							{
								flag = 0;
							}
						}
					}
				}
			}
			data2[y_bar + y][x_bar + x][0] = 255;
			data2[y_bar + y][x_bar + x][1] = 255;
			data2[y_bar + y][x_bar + x][2] = 0;
		}
	}
	return cnt;
}

void CRockScissorsPaperDlg::Sq_F() //사각형 그리기
{
	int i, j;
	int x_cnt[320];
	int y_cnt[240];
	int IMGYE = 5;
	x_min = 0;
	x_max = 0;
	y_min = 0;
	y_max = 0;
	for (i = 0; i < 240; i++) //Y축의 히스토그램
	{
		y_cnt[i] = 0;
		for (j = 0; j < 320; j++)
		{
			if (data2[i][j][0] == 255 && data2[i][j][1] == 255 && data2[i][j][2] == 255)
			{
				y_cnt[i]++;
			}
		}
	}
	for (j = 0; j < 320; j++) //X축의 히스토그램
	{
		x_cnt[j] = 0;
		for (i = 0; i < 240; i++)
		{
			if (data2[i][j][0] == 255 && data2[i][j][1] == 255 && data2[i][j][2] == 255)
			{
				x_cnt[j]++;
			}
		}
	}
	for (i = 0; i < 320; i++)
	{
		if (x_cnt[i] >= IMGYE)
		{
			x_min = i;
			break;
		}
	}
	for (i = 0; i < 320; i++)
	{
		if (x_cnt[320 - i - 1] >= IMGYE)
		{
			x_max = 320 - i - 1;
			break;
		}
	}
	for (i = 0; i < 240; i++)
	{
		if (y_cnt[i] >= IMGYE)
		{
			y_min = i;
			break;
		}
	}
	for (i = 0; i < 240; i++)
	{
		if (y_cnt[240 - i - 1] >= IMGYE)
		{
			y_max = 240 - i - 1;
			break;
		}
	}
	x2 = (int)((x_max + x_min) / 2);
	y2 = (int)((y_max + y_min) / 2);
}

void CRockScissorsPaperDlg::Mugejungsim2()//사각형중심표시함수
{
	if (y2 > 0 && y2 < 240 - 1 && x2>0 && x2 < 320 - 1)
	{
		data2[y2][x2][0] = 0;
		data2[y2][x2][1] = 170;
		data2[y2][x2][2] = 255;

		data2[y2 - 1][x2][0] = 0;
		data2[y2 - 1][x2][1] = 170;
		data2[y2 - 1][x2][2] = 255;

		data2[y2 + 1][x2][0] = 0;
		data2[y2 + 1][x2][1] = 170;
		data2[y2 + 1][x2][2] = 255;

		data2[y2][x2 - 1][0] = 0;
		data2[y2][x2 - 1][1] = 170;
		data2[y2][x2 - 1][2] = 255;

		data2[y2][x2 + 1][0] = 0;
		data2[y2][x2 + 1][1] = 170;
		data2[y2][x2 + 1][2] = 255;
	}
}

void CRockScissorsPaperDlg::Rect_F() //사각형 그리기 함수
{
	int i;
	for (i = y_min; i < y_max; i++) //그리기(x_min,x_max)
	{
		data2[i][x_min][0] = 0;
		data2[i][x_min][1] = 255;
		data2[i][x_min][2] = 0;

		data2[i][x_max][0] = 0;
		data2[i][x_max][1] = 255;
		data2[i][x_max][2] = 0;
	}
	for (i = x_min; i < x_max; i++) //그리기(y_min,y_max)
	{
		data2[y_min][i][0] = 0;
		data2[y_min][i][1] = 255;
		data2[y_min][i][2] = 0;

		data2[y_max][i][0] = 0;
		data2[y_max][i][1] = 255;
		data2[y_max][i][2] = 0;
	}
}

void CRockScissorsPaperDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	KillTimer(1);
}

void CRockScissorsPaperDlg::block_F()
{
	int i, j;
	int k, l;
	int cnt;
	int var1 = 5;

	for (j = x_min; j <= x_max - var1; j += 5)
	{
		for (i = y_min; i <= y_max - var1; i += 5)
		{
			cnt = 0;
			for (k = i; k <= i + var1; k++)
			{
				for (l = j; l <= j + var1; l++)
				{
					if (k == i || k == i + var1 || l == j || l == j + var1)
					{
						if (data2[k][l][0] == 255 && data2[k][l][1] == 255 && data2[k][l][2] == 255)
						{
							cnt++;
						}
					}
				}
			}
			if (cnt >= 14)
			{
				for (k = i; k <= i + var1; k++)
				{
					for (l = j; l <= j + var1; l++)
					{
						data2[k][l][0] = 255;
						data2[k][l][1] = 255;
						data2[k][l][2] = 255;
					}
				}
			}
		}
	}
}

void CRockScissorsPaperDlg::OnButtonStart()
{
	// TODO: Add your control notification handler code here
	startflag = 1;
	timercounter = 0;
	num = 3;

	CClientDC dc(this);
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap bitmap_result;
	CBitmap* pOldBitmap;
	BITMAP bmpINFO;

	bitmap_result.LoadBitmap(IDB_BITMAP_VS);

	bitmap_result.GetBitmap(&bmpINFO);
	SetWindowPos(this, 0, 0, bmpINFO.bmWidth, bmpINFO.bmHeight, SWP_NOSIZE);
	pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap_result);
	dc.BitBlt(321, 341, bmpINFO.bmWidth, bmpINFO.bmHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBitmap);
}

void CRockScissorsPaperDlg::Drawing()
{
	CClientDC dc(this);
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap bitmap_user, bitmap_result;
	CBitmap* pOldBitmap;
	BITMAP bmpINFO;

	int finger_temp;

	if (finger && finger < 3) //주먹
	{
		bitmap_user.LoadBitmap(IDB_BITMAP_REV_MOOK);

		bitmap_user.GetBitmap(&bmpINFO);
		SetWindowPos(this, 0, 0, bmpINFO.bmWidth, bmpINFO.bmHeight, SWP_NOSIZE);
		pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap_user);
		dc.BitBlt(1, 341, bmpINFO.bmWidth, bmpINFO.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);
		finger_temp = 0;
	}
	else if (finger < 5) //가위
	{
		bitmap_user.LoadBitmap(IDB_BITMAP_REV_JJI);

		bitmap_user.GetBitmap(&bmpINFO);
		SetWindowPos(this, 0, 0, bmpINFO.bmWidth, bmpINFO.bmHeight, SWP_NOSIZE);
		pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap_user);
		dc.BitBlt(1, 341, bmpINFO.bmWidth, bmpINFO.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);
		finger_temp = 1;
	}
	else if (finger < 7) //보자기
	{
		bitmap_user.LoadBitmap(IDB_BITMAP_REV_BBA);

		bitmap_user.GetBitmap(&bmpINFO);
		SetWindowPos(this, 0, 0, bmpINFO.bmWidth, bmpINFO.bmHeight, SWP_NOSIZE);
		pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap_user);
		dc.BitBlt(1, 341, bmpINFO.bmWidth, bmpINFO.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);
		finger_temp = 2;
	}
	else
	{
		AfxMessageBox(L"내지 않았습니다.");
		return;
	}
	if (finger_temp == rand_var)//비김
	{
		bitmap_result.LoadBitmap(IDB_BITMAP_DRAW);

		bitmap_result.GetBitmap(&bmpINFO);
		SetWindowPos(this, 0, 0, bmpINFO.bmWidth, bmpINFO.bmHeight, SWP_NOSIZE);
		pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap_result);
		dc.BitBlt(321, 341, bmpINFO.bmWidth, bmpINFO.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);
	}
	else if ((finger_temp == 0 && rand_var == 1) || (finger_temp == 1 && rand_var == 2) || (finger_temp == 2 && rand_var == 0))//이김
	{
		bitmap_result.LoadBitmap(IDB_BITMAP_WIN);

		bitmap_result.GetBitmap(&bmpINFO);
		SetWindowPos(this, 0, 0, bmpINFO.bmWidth, bmpINFO.bmHeight, SWP_NOSIZE);
		pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap_result);
		dc.BitBlt(321, 341, bmpINFO.bmWidth, bmpINFO.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);
	}
	else //졌다
	{
		bitmap_result.LoadBitmap(IDB_BITMAP_LOSE);

		bitmap_result.GetBitmap(&bmpINFO);
		SetWindowPos(this, 0, 0, bmpINFO.bmWidth, bmpINFO.bmHeight, SWP_NOSIZE);
		pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap_result);
		dc.BitBlt(321, 341, bmpINFO.bmWidth, bmpINFO.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);
	}
}
