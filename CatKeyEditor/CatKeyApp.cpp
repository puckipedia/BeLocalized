#include "CatKeyApp.h"

#include "CatKeyStore.h"
#include "TranslationView.h"

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
	BWindow *win = new BWindow(BRect(0, 0, 680, 480), "BeLocalized", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS | B_QUIT_ON_WINDOW_CLOSE);
	TranslationView *v = new TranslationView();

	BLayoutBuilder::Group<>(win)
		.Add(v);

	win->CenterOnScreen();

	BMessenger m(v);
	CatKeyStore *s = new CatKeyStore("/boot/home/nl.catkeys", m, kMsgGotUnit);
	win->SetTitle(s->Title());
	v->SetStore(s);
	s->StartLoading();
	win->Show();

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
