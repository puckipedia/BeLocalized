#include "BeLocalizedApp.h"

#include "PootleAPI/Pootle.h"

#include <stdio.h>

void
BeLocalizedApp::ArgvReceived(int32 argc, char **argv)
{
	mPootle = new Pootle(BUrl(argv[1]), BMessenger(this), "user", "bitnami");
}


void
BeLocalizedApp::MessageReceived(BMessage *msg)
{
	msg->PrintToStream();
	switch (msg->what) {
	case kMsgPootleInited: {
		BObjectList<PootleProject> msg = mPootle->Projects()->Get();
		for (int32 i = 0; i < msg.CountItems(); i++) {
			printf("Item %d: %s\n", i, msg.ItemAt(i)->SourceLanguage().FullName().String());
		}
		Quit();
		break;
	}
	default:
		BApplication::MessageReceived(msg);
		break;
	}
}


void
BeLocalizedApp::ReadyToRun()
{
}
