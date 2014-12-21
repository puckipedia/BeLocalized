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
BeLocalizedApp::MessageReceived(BMessage *msgrecv)
{
	msgrecv->PrintToStream();
	switch (msgrecv->what) {
	case kMsgPootleInited: {
		BObjectList<PootleProject> msg = mPootle->Projects()->Get();
		for (int32 i = 0; i < msg.CountItems(); i++) {
			PootleProject *pr = msg.ItemAt(i);
			printf("Project %d: %s (from %s)\n", i, pr->FullName().String(), pr->SourceLanguage().FullName().String());\
		}
		int index;
		printf("Choose project: ");
		scanf("%d", &index);
		PootleProject *pr = msg.ItemAt(index);
		for (int32 i = 0; i < pr->CountTranslationProjects(); i++) {
			PootleTranslationProject p = pr->GetTranslationProject(i);
			printf("%4d: %s\n", i, p.Language().FullName().String());
		}
		printf("Choose translation project: ");
		scanf("%d", &index);
		PootleTranslationProject tp = pr->GetTranslationProject(index);
		for (int32 i = 0; i < tp.CountStores(); i++) {
			PootleStore st = tp.GetStore(i);
			printf("%4d: %s\n", i, st.File().String());
		}
		printf("Choose store: ");
		scanf("%d", &index);
		PootleStore st = tp.GetStore(index);
		
		for (int32 i = 0; i < st.CountUnits(); i++) {
			PootleUnit u = st.GetUnit(i);
			printf("%4d: %s\n", i, u.Source().text.String());
		}
		
		
		Quit();
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
	if (mPootle == NULL)
		Quit();
}
