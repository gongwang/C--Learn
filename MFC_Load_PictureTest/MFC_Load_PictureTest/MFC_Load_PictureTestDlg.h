
// MFC_Load_PictureTestDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CMFC_Load_PictureTestDlg dialog
class CMFC_Load_PictureTestDlg : public CDialogEx
{
// Construction
public:
	CMFC_Load_PictureTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFC_LOAD_PICTURETEST_DIALOG };

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
public:
    afx_msg void OnBnClickedButton1();
    CStatic m_Picture;
};
