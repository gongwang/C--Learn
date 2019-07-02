// ocxEmbedUI.cpp : implementation file
//

#include "stdafx.h"
#include "OcxTest.h"
#include "ocxEmbedUI.h"
#include "afxdialogex.h"


// ocxEmbedUI dialog

IMPLEMENT_DYNAMIC(ocxEmbedUI, CDialogEx)

ocxEmbedUI::ocxEmbedUI(CWnd* pParent /*=NULL*/)
	: CDialogEx(ocxEmbedUI::IDD, pParent)
{

}

ocxEmbedUI::~ocxEmbedUI()
{
}

void ocxEmbedUI::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ocxEmbedUI, CDialogEx)
END_MESSAGE_MAP()


// ocxEmbedUI message handlers
