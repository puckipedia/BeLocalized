#ifndef CAT_KEY_APP_H
#define CAT_KEY_APP_H

#include <Application.h>

class CatKeyApp : public BApplication {
public:
	CatKeyApp() : BApplication("application/x-puckipedia.CatKeyEditor"),
		mOpenWindows(0) {}

	void	ArgvReceived(int, char **);
	void	ReadyToRun();
	void	MessageReceived(BMessage *);
	void	RefsReceived(BMessage *);
private:
	int32	mOpenWindows;
	void	_OpenWindow(BString);
};

#endif
