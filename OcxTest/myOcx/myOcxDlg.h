
// myOcxDlg.h : header file
//

#pragma once



// CmyOcxDlg dialog
class CmyOcxDlg : public CDialogEx
{
// Construction
public:
	CmyOcxDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MYOCX_DIALOG };

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
	
	DECLARE_EVENTSINK_MAP()
	void OnSendOcxtestctrl1(LPCTSTR lpStrTime);
	void HelloCoco();
};
