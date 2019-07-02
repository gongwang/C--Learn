// vlc demo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>  
#include <time.h>
#include "vlc/vlc.h"  
#include "vlc/libvlc_media.h"

struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_t;

typedef struct tagKDRECT
{
	long    left;
	long    top;
	long    right;
	long    bottom;
} KDRECT;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void	DestroyWindowRect(void* hWnd)
{
	DestroyWindow((HWND)hWnd); 
}

void*	CreateWindowRect( int iWinX,int iWinY, int iWinXSum,int iWinYSum)
{
	WNDCLASS window = {0};
	HWND hWnd = NULL;

	KDRECT displayRegion = {0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
	KDRECT RectWindow = {0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
	window.lpfnWndProc= (WNDPROC)WindowProc;
	window.hInstance= GetModuleHandle(NULL);
	window.hCursor= LoadCursor(NULL, IDC_ARROW);
	window.style = CS_OWNDC;
	window.lpszClassName= (LPCWSTR)_T("YY");
	
#define VIDEO_ADPTER_0_WIDTH  0//1366
#define VIDEO_ADPTER_0_HEIGHT 0//768

	displayRegion.right  = (RectWindow.right - RectWindow.left)  / iWinXSum;
	displayRegion.bottom = (RectWindow.bottom - RectWindow.top)  / iWinYSum;

	displayRegion.left   = displayRegion.right * iWinX + RectWindow.left;
	displayRegion.top    = displayRegion.bottom * iWinY + RectWindow.top;
	
	RegisterClass(&window);
	DWORD dwStyle =  WS_OVERLAPPEDWINDOW;
	hWnd = CreateWindowEx(NULL,
		(LPCWSTR)_T("YY"),
		NULL,
		dwStyle,
		displayRegion.left+VIDEO_ADPTER_0_WIDTH ,
		displayRegion.top ,
		displayRegion.right,
		displayRegion.bottom ,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	if (NULL == hWnd)
	{
		return NULL;
	}
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	printf("<%s> Wnd: %x %d\n",__FUNCTION__,hWnd,hWnd);
	return hWnd;
}

int _tmain(int argc, _TCHAR* argv[])
{
	libvlc_instance_t * inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;

	// init vlc modules, should be done only once
	inst = libvlc_new(0, NULL);

	// Create a new item

	//m = libvlc_media_new_path (inst, "test.mp4");
    m = libvlc_media_new_location(inst, "rtsp://172.16.128.51/id=00");

	// Create a media player playing environement
	mp = libvlc_media_player_new_from_media(m);
	
	libvlc_media_parse(m);

	libvlc_time_t duration = libvlc_media_get_duration(m);

	// No need to keep the media now
	libvlc_media_release(m);
#if 0
	// This is a non working code that show how to hooks into a window,
	// if we have a window around
	// libvlc_drawable_t drawable = xdrawable;
	// or on windows
	//void *drawable = &g_hwnd;
	 //void * m_Hwnd = CreateWindowRect( 0,0,1,1);
	 //libvlc_media_player_set_hwnd (mp, m_Hwnd);
#endif
	// play the media_player
	libvlc_media_player_play (mp);

	Sleep(100*1000); // Let it play a bit

	// Stop playing
	libvlc_media_player_stop (mp);

	// Free the media_player
	libvlc_media_player_release (mp);
	
	libvlc_release (inst);

	//DestroyWindowRect(m_Hwnd);
	return 0;
}

