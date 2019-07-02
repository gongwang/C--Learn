
// paintDemoDlg.h : header file
//

#pragma once


// CpaintDemoDlg dialog
class CpaintDemoDlg : public CDialogEx
{
// Construction
public:
	CpaintDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PAINTDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    CImage m_MyImage;
    HDC hDCScreen;
    int nBitPerPixel;
    int nWidthScreen;
    int nHeightScreen;
    bool m_bInit;
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
