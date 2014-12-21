#include "BeLocalizedApp.h"

#include "PootleAPI/Pootle.h"

#include <stdio.h>

void
BeLocalizedApp::ArgvReceived(int32 argc, char **argv)
{
	if (argc < 4)
		printf("Usage: %s url username password\n", argv[0]);
	else
		mPootle = new Pootle(BUrl(argv[1]), BMessenger(this), argv[2], argv[3]);
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
			printf("Project %d: %s (%s)\n", i, pr->FullName().String(), pr->SourceLanguage().FullName().String());\
			for (int32 j = 0; j < pr->CountTranslationProjects(); j++) {
				PootleTranslationProject p = pr->GetTranslationProject(j);
				PootleLanguage lang = p.Language();
				printf("           %02d. %s\n", j, lang.FullName().String());
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
	if (mPootle == NULL)
		Quit();
}
