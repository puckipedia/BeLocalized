#include "CatKeyApp.h"

#include "CatKeyStore.h"
#include "TranslationView.h"
#include "TranslationWindow.h"

#include <Alert.h>
#include <Archivable.h>
#include <Catalog.h>
#include <Entry.h>
#include <File.h>
#include <LayoutBuilder.h>
#include <Message.h>
#include <Messenger.h>
#include <Path.h>
#include <Rect.h>
#include <String.h>
#include <Window.h>

#include <stdio.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "CatKeyApp"

void
CatKeyApp::_OpenWindow(BString path)
{
	TranslationWindow *window = new TranslationWindow(BRect(0, 0, 680, 480));
	window->CenterOnScreen();
	window->MoveBy(mOpenWindows * 20, mOpenWindows * 20);

	TranslationView *view = window->Translation();
	BMessenger messenger(view);
	CatKeyStore *store = new CatKeyStore(path, messenger, kMsgGotUnit);
	window->SetTitle(store->Title());
	view->SetStore(store);
	if (!store->StartLoading()) {
		delete window;
		delete store;

		BAlert *alert = new BAlert(B_TRANSLATE("Failed to load file"),
			B_TRANSLATE("The file failed to load. Are you sure it's a catkey file?"),
			B_TRANSLATE("Close"));
		alert->Go();

		if (mOpenWindows == 0)
			OpenFilePanel();
	} else {
		view->SetAutomaticallyConfirm(true);
		window->Show();
		mOpenWindows++;
	}
}


void
CatKeyApp::ReadyToRun()
{
	if (mOpenWindows == 0) {
		fprintf(stderr, "Usage: %s [file] [file] [...]\n", mCalledAs);
		OpenFilePanel();
	}
}


void
CatKeyApp::OpenFilePanel()
{
	if (!mOpenPanel)
		mOpenPanel = new BFilePanel(B_OPEN_PANEL, NULL, NULL,
			B_FILE_NODE | B_DIRECTORY_NODE);
	mOpenPanel->SetTarget(this);
	mOpenPanel->Show();
}


void
CatKeyApp::ArgvReceived(int32 argc, char **argv)
{
	for (int i = 1; i < argc; i++)
		_OpenWindow(argv[i]);
}

void
CatKeyApp::MessageReceived(BMessage *msg)
{
	switch (msg->what) {
		case kMsgWindowClosed:
			mOpenWindows--;
		// Fallthrough on purpose
		case B_CANCEL: {
			if (mOpenWindows == 0)
				Quit();
			break;
		}
		default:
		{
			BApplication::MessageReceived(msg);
		}
	}
}


void
CatKeyApp::RefsReceived(BMessage *refs)
{
	int32 count;
	refs->GetInfo("refs", NULL, &count);

	for (int32 i = 0; i < count; i++) {
		entry_ref ref;
		refs->FindRef("refs", i, &ref);
		BEntry e(&ref);
		BPath p;
		e.GetPath(&p);
		_OpenWindow(p.Path());
	}
}
