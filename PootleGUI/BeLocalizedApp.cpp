#include "BeLocalizedApp.h"

#include "LogInWindow.h"
#include "PootleAPI/Pootle.h"
#include "PootleMainWindow.h"

#include <stdio.h>

void
BeLocalizedApp::ArgvReceived(int32 argc, char **argv)
{
}


void
BeLocalizedApp::MessageReceived(BMessage *msgrecv)
{
	switch (msgrecv->what) {
	case kMsgPootleInited: {
		msgrecv->FindPointer("pootle", (void **)&mPootle);

		PootleMainWindow *w = new PootleMainWindow(BRect(0, 0, 1000, 500), mPootle);
		w->CenterOnScreen();
		w->Show();
		break;
	}
	default:
		BApplication::MessageReceived(msgrecv);
		break;
	}
}


void
BeLocalizedApp::ReadyToRun()
{
	LogInWindow *w = new LogInWindow(BRect(0, 0, 100, 100));
	w->CenterOnScreen();
	w->Show();
}
