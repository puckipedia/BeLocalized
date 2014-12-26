#include "CatKeyApp.h"

#include "CatKeyStore.h"
#include "TranslationView.h"
#include "TranslationWindow.h"

#include <Archivable.h>
#include <File.h>
#include <LayoutBuilder.h>
#include <Message.h>
#include <Messenger.h>
#include <Rect.h>
#include <String.h>
#include <Window.h>

#include <stdio.h>

void
CatKeyApp::ReadyToRun()
{
	TranslationWindow *w = new TranslationWindow(BRect(0, 0, 680, 480));
	w->CenterOnScreen();

	TranslationView *v = w->Translation();
	BMessenger m(v);
	CatKeyStore *s = new CatKeyStore("/boot/home/nl.catkeys", m, kMsgGotUnit);
	w->SetTitle(s->Title());
	v->SetStore(s);
	v->HideTranslated(true);
	s->StartLoading();
	w->Show();

	printf("Got %d units\n", s->TotalUnits());
}


void
CatKeyApp::MessageReceived(BMessage *msg)
{
	if (msg->what == 'ABCD') {
		TranslationUnit *u;
		if (msg->FindPointer("unit", (void **)&u) != B_OK) {
			printf("Done.\n");
			Quit();
		} else
			printf("'%s', '%s': '%s'\n", u->Source().String(), u->Context().String(), u->Translated().String());
	} else {
		msg->PrintToStream();
		BApplication::MessageReceived(msg);
	}
}
