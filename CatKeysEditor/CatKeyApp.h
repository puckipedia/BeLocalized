#ifndef CAT_KEY_APP_H
#define CAT_KEY_APP_H

#include <Application.h>
#include <FilePanel.h>

class CatKeyApp : public BApplication {
public:
	CatKeyApp(const char *a)
		:
		BApplication("application/x-vnd.puckipedia-CatKeysEditor"),
		mOpenWindows(0),
		mOpenPanel(NULL),
		mCalledAs(a) {}
	~CatKeyApp() {delete mOpenPanel;}

	void	ArgvReceived(int32, char **);
	void	ReadyToRun();
	void	MessageReceived(BMessage *);
	void	RefsReceived(BMessage *);
private:
	int32	mOpenWindows;
	void	_OpenWindow(BString);
	const char *mCalledAs;
	BFilePanel *mOpenPanel;
};

#endif
