#ifndef _VIS_APP_H
#define _VIS_APP_H


/// dummy object
class CVisApp : public CWinApp
{
public:
	CVisApp();

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


#endif