
// RockScissorsPaperDlg.h: 헤더 파일
//

#pragma once

#include "vfw.h"
#include <math.h>


// CRockScissorsPaperDlg 대화 상자
class CRockScissorsPaperDlg : public CDialogEx
{
// 생성입니다.
public:
	void Drawing();
	int num;
	char startflag;
	int finger;
	void block_F();
	void Rect_F();
	int y_max;
	int y_min;
	int x_max;
	int x_min;
	int y_bar;
	int x_bar;
	unsigned int y2;
	unsigned int x2;
	void Sq_F();
	int Circle_F(int r);
	int timercounter;
	void Reload();
	unsigned char data2[240][320][3];
	void Processing(unsigned char* data);
	void Mugejungsim2();
	void Mugejungsim();
	int rand_var;
	HWND m_hWndCam;

	CRockScissorsPaperDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROCKSCISSORSPAPER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnButtonStart();
	DECLARE_MESSAGE_MAP()
};
