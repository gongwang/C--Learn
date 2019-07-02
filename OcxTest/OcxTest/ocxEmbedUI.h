#pragma once


// ocxEmbedUI dialog

class ocxEmbedUI : public CDialogEx
{
	DECLARE_DYNAMIC(ocxEmbedUI)

public:
	ocxEmbedUI(CWnd* pParent = NULL);   // standard constructor
	virtual ~ocxEmbedUI();

// Dialog Data
	enum { IDD = IDD_MAIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
