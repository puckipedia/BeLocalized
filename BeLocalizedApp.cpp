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
			PootleProject *pr = msg.ItemAt(i);
			printf("Item %04d: %s (%s)\n", i, pr->FullName().String(), pr->SourceLanguage().FullName().String());\
			for (int32 j = 0; j < pr->CountTranslationProjects(); j++) {
				PootleTranslationProject p = pr->GetTranslationProject(j);
				PootleLanguage lang = p.Language();
				printf("           %d. %s\n", j, lang.FullName().String());
			}
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
