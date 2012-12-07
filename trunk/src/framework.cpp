//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 1/19/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  framework.cpp
//! \brief OpenGL init code. 


#include "framework.h"
#include "keycodes.h"
#include "keymap.h"

#include "sdsslib/helpers.h"

#include <string>

extern int		InitGL();
extern void		DrawGLScene();
extern void		KillGL();
extern void		UpdateGLView(int, int);

#define		FR_MAXKEYTABLE		320

HDC			fr_hDC			= NULL;		
HGLRC		fr_hRC			= NULL;		
HWND		fr_hWnd			= NULL;		
HINSTANCE	fr_hInstance;		

BOOL		fr_keys[FR_MAXKEYTABLE];	
POINT		fr_mouse_pos	= {0, 0};
	
BOOL		fr_active		= TRUE;	
BOOL		fr_fullscreen	= TRUE;	

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	


void ProcessInput()
{
	int c;
	int fr_keymapcount = sizeof(fr_keymap) / sizeof(TKEYMAP);

	for(c=0;c < fr_keymapcount;c++)
	{
		if (fr_keys[fr_keymap[c].scancode]) {
			fr_keymap[c].ptProc();
			fr_keys[fr_keymap[c].scancode] = FALSE;
		}
	}

}


void KillGLWindow(GLvoid)								
{
	KillGL();

	if (fr_fullscreen)									
	{
		ChangeDisplaySettings(NULL,0);					
		ShowCursor(TRUE);							
	}

	if (fr_hDC)										
	{
		if (!wglMakeCurrent(NULL,NULL))				
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(fr_hRC))						
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		fr_hRC=NULL;										
	}

	if (fr_hDC && !ReleaseDC(fr_hWnd,fr_hDC))					
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		fr_hDC=NULL;										
	}

	if (fr_hWnd && !DestroyWindow(fr_hWnd))					
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		fr_hWnd=NULL;										
	}

	if (!UnregisterClass("OpenGL",fr_hInstance))			
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		fr_hInstance=NULL;									
	}
}

 
BOOL CreateGLWindow(char* title, int width, int height, int bits, int freq, bool fullscreenflag, LPSTR lpCmdLine)
{
	GLuint		PixelFormat;			
	WNDCLASS	wc;						
	DWORD		dwExStyle;				
	DWORD		dwStyle;				
	RECT		WindowRect;				

	WindowRect.left		= 0;			
	WindowRect.right	= width;		
	WindowRect.top		= 0;				
	WindowRect.bottom	= height;		

	fr_fullscreen		= fullscreenflag;			

	fr_hInstance		= GetModuleHandle(NULL);				
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wc.lpfnWndProc		= (WNDPROC) WndProc;					
	wc.cbClsExtra		= 0;									
	wc.cbWndExtra		= 0;									
	wc.hInstance		= fr_hInstance;							
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground	= NULL;									
	wc.lpszMenuName		= NULL;									
	wc.lpszClassName	= "OpenGL";								

	if (!RegisterClass(&wc))									
	{
		MessageBox(NULL,"Failed to register window class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
	
	if (fr_fullscreen)												
	{
		DEVMODE dmScreenSettings;								
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
	
		dmScreenSettings.dmSize				=sizeof(dmScreenSettings);		
		dmScreenSettings.dmPelsWidth		= width;				
		dmScreenSettings.dmPelsHeight		= height;				
		dmScreenSettings.dmBitsPerPel		= bits;		
		dmScreenSettings.dmDisplayFrequency	= freq;
		dmScreenSettings.dmFields			= DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT|DM_DISPLAYFREQUENCY;

		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			if (MessageBox(NULL,"The requested fullscreen mode is not supported. Use windowed mode?","",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fr_fullscreen=FALSE;		
			}
			else
			{
				return FALSE;								
			}
		}
	}

	if (fr_fullscreen)												
	{
		dwExStyle=WS_EX_APPWINDOW;								
		dwStyle=WS_POPUP;										
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
		dwStyle=WS_OVERLAPPEDWINDOW;	
	}

//	ShowCursor(FALSE);
	SetCursorPos(FR_WIDTH/2, FR_HEIGHT/2);

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		

	if (!(fr_hWnd=CreateWindowEx(dwExStyle,							
								"OpenGL",							
								title,								
								dwStyle | WS_CLIPSIBLINGS |	WS_CLIPCHILDREN,					
								0,
								0,								
								WindowRect.right-WindowRect.left,	
								WindowRect.bottom-WindowRect.top,	
								NULL,								
								NULL,								
								fr_hInstance,							
								NULL)))								
	{
		KillGLWindow();								
		MessageBox(NULL,"Window creation error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	static	PIXELFORMATDESCRIPTOR pfd=				
	{
		sizeof(PIXELFORMATDESCRIPTOR),				
		1,											
		PFD_DRAW_TO_WINDOW |						
		PFD_SUPPORT_OPENGL |						
		PFD_DOUBLEBUFFER,							
		PFD_TYPE_RGBA,								
		bits,										
		0, 0, 0, 0, 0, 0,							
		0,											
		0,											
		0,											
		0, 0, 0, 0,									
		16,											
		0,											
		0,											
		PFD_MAIN_PLANE,								
		0,											
		0, 0, 0										
	};
	
	if (!(fr_hDC=GetDC(fr_hWnd)))							
	{
		KillGLWindow();								
		MessageBox(NULL,"Cannot create GL device context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(fr_hDC,&pfd)))	
	{
		KillGLWindow();							
		MessageBox(NULL,"Cannot select GL pixel format.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!SetPixelFormat(fr_hDC,PixelFormat,&pfd))		
	{
		KillGLWindow();								
		MessageBox(NULL,"Cannot set GL pixel format.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(fr_hRC=wglCreateContext(fr_hDC)))				
	{
		KillGLWindow();							
		MessageBox(NULL,"Cannot create GL render context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(fr_hDC,fr_hRC))				
	{
		KillGLWindow();							
		MessageBox(NULL,"Cannot activate GL rendering context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(fr_hWnd,SW_SHOW);					
	SetForegroundWindow(fr_hWnd);					
	SetFocus(fr_hWnd);									
	UpdateGLView(width, height);					

	if (!InitGL())									
	{
		KillGLWindow();							
		MessageBox(NULL,"Initialization failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}


	int c;
	for(c=0;c < FR_MAXKEYTABLE;c++)
	{
		fr_keys[c] = FALSE;
	}
	
	return TRUE;									
}

LRESULT CALLBACK WndProc(HWND hWnd,	UINT uMsg, WPARAM wParam, LPARAM lParam)			
{
	switch (uMsg)									
	{
		case WM_ACTIVATE:							
		{
			if (!HIWORD(wParam))					
			{
				fr_active=TRUE;						
			}
			else
			{
				fr_active=FALSE;						
			}

			return 0;								
		}

		case WM_SYSCOMMAND:							
		{
			switch (wParam)							
			{
				case SC_SCREENSAVE:					
				case SC_MONITORPOWER:				
				return 0;							
			}
			break;									
		}

		case WM_CLOSE:								
		{
			PostQuitMessage(0);						
			return 0;							
		}

/*		case WM_MOUSEMOVE:	
		{
			return 0;
		}
*/		
		case WM_KEYDOWN:							
		{
			int kbscan_code = ((lParam >> 16) & 0x0ff) | ((lParam & 0x1000000) >> 17);
			fr_keys[kbscan_code] = TRUE;	
			return 0;								
		}

		case WM_KEYUP:								
		{
			int kbscan_code = ((lParam >> 16) & 0x0ff) | ((lParam & 0x1000000) >> 17);
			fr_keys[kbscan_code] = FALSE;	
			return 0;								
		}

		case WM_LBUTTONDOWN:								
		{
			fr_keys[MBB_LEFT] = TRUE;	
			return 0;
		}

		case WM_RBUTTONDOWN:								
		{
			fr_keys[MBB_RIGHT] = TRUE;	
			return 0;
		}

		case WM_LBUTTONUP:								
		{
			fr_keys[MBB_LEFT] = FALSE;	
			return 0;
		}

		case WM_RBUTTONUP:								
		{
			fr_keys[MBB_RIGHT] = FALSE;	
			return 0;
		}		

		case WM_SIZE:								
		{
			UpdateGLView(LOWORD(lParam),HIWORD(lParam));  
			return 0;								
		}
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}



int WINAPI WMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)			
{
	BOOL	done = FALSE;								
	MSG		msg;									

	fr_fullscreen=FALSE;

	if (!CreateGLWindow(FR_NAME, FR_WIDTH, FR_HEIGHT, FR_BPP, FR_FRQ, fr_fullscreen, lpCmdLine))
	{
		return 0;									
	}

	while(!done)									
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)				
			{
				done=TRUE;							
			}
			else									
			{
				TranslateMessage(&msg);				
				DispatchMessage(&msg);				
			}
		}
		else										
		{

			if (fr_keys[KB_ESCAPE])				
			{
				done = TRUE;						
			}
			else								
			{
				ProcessInput();
				DrawGLScene();					
				SwapBuffers(fr_hDC);				
			}


			if (fr_keys[KB_F1])						
			{
				fr_keys[KB_F1]=FALSE;					
				KillGLWindow();						
				fr_fullscreen=!fr_fullscreen;				

				if (!CreateGLWindow(FR_NAME, FR_WIDTH, FR_HEIGHT, FR_BPP, FR_FRQ, fr_fullscreen, lpCmdLine))
				{
					return 0;						
				}
			}
		}
	}

	KillGLWindow();								
	return (msg.wParam);						
}


void main(int argc, char* argv[])
{
	__try
	{
		//
		// we use void main as entry point to get console print feedback to caller window
		WMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOW);
	}
	__except( Helpers::writeMiniDump( GetExceptionInformation() ), EXCEPTION_EXECUTE_HANDLER )
	{
		printf("Program Termination. Crashdump (.dmp) written.");
	}

};