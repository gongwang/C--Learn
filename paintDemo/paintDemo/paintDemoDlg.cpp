
// paintDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "paintDemo.h"
#include "paintDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CpaintDemoDlg dialog




CpaintDemoDlg::CpaintDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CpaintDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_bInit = FALSE;
}

void CpaintDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CpaintDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CpaintDemoDlg message handlers

BOOL CpaintDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

    hDCScreen = ::GetDC(GetDesktopWindow()->GetSafeHwnd());//首先获取到屏幕的句柄    
    nBitPerPixel = GetDeviceCaps(hDCScreen, BITSPIXEL);//获取到每个像素的bit数目
    nWidthScreen = GetDeviceCaps(hDCScreen, HORZRES);
    nHeightScreen = GetDeviceCaps(hDCScreen, VERTRES);
    //创建一个CImage的对象
    //Create实例化CImage，使得其内部的画布大小与屏幕一致
    m_MyImage.Create(400, 300, 32);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CpaintDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CpaintDemoDlg::OnPaint()
{
// 	if (IsIconic())
// 	{
// 		CPaintDC dc(this); // device context for painting
// 
// 		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
// 
// 		// Center icon in client rectangle
// 		int cxIcon = GetSystemMetrics(SM_CXICON);
// 		int cyIcon = GetSystemMetrics(SM_CYICON);
// 		CRect rect;
// 		GetClientRect(&rect);
// 		int x = (rect.Width() - cxIcon + 1) / 2;
// 		int y = (rect.Height() - cyIcon + 1) / 2;
// 
// 		// Draw the icon
// 		dc.DrawIcon(x, y, m_hIcon);
// 	}
// 	else
// 	{
// 		CDialogEx::OnPaint();
// 	}
    if(m_bInit == FALSE)
    {
        m_bInit = TRUE;
        HDC hDCImg = m_MyImage.GetDC();
        BitBlt(hDCImg, 0, 0, 400, 300, hDCScreen, 960, 540, SRCCOPY);
    }
    CRect cRect;
    GetClientRect(&cRect);

    CPaintDC dc(this);
    m_MyImage.Save("D:\\test_paintdemo.png",Gdiplus::ImageFormatPNG);
    m_MyImage.Draw(dc.m_hDC, cRect);
    

}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CpaintDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CpaintDemoDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if(m_bInit)
    {
        RedrawWindow();
    }
}
