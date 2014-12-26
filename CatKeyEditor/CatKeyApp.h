#ifndef CAT_KEY_APP_H
#define CAT_KEY_APP_H

#include <Application.h>

class CatKeyApp : public BApplication {
public:
	CatKeyApp() : BApplication("application/x-puckipedia.CatKeyEditor") {}

	void	ReadyToRun();
	void	MessageReceived(BMessage *);
};

#endif
