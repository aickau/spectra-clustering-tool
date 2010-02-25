#include "vis_types.h"

/// simple cursor handling class to set and load cursor shapes
class CVisCursor
{
public:
	CVisCursor();
	~CVisCursor();

	void			SetCursor(TVIS_CURSORSHAPE cursor);	

private:
	
	static TVIS_CURSORSHAPE	current;
};