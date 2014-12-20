#ifndef BE_LOCALIZED_APP_H
#define BE_LOCALIZED_APP_H

#include <Application.h>

class Pootle;

class BeLocalizedApp : public BApplication {
public:
	BeLocalizedApp() : BApplication("application/x-puckipedia.BeLocalized") {}

	void ArgvReceived(int32 argc, char **argv);
	void ReadyToRun();

	void MessageReceived(BMessage *msg);

private:
	Pootle *mPootle;
};

#endif
