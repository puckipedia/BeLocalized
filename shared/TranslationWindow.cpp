#include "TranslationWindow.h"

#include "TranslationView.h"

#include <Application.h>
#include <ControlLook.h>
#include <Entry.h>
#include <LayoutBuilder.h>
#include <Message.h>
#include <Rect.h>
#include <MenuBar.h>
#include <FilePanel.h>

const int32 kMsgHideTranslated = 'HiTr';

TranslationWindow::TranslationWindow(BRect rect, uint32 mask)
	:
	BWindow(rect, "BeLocalized", B_TITLED_WINDOW,
		B_AUTO_UPDATE_SIZE_LIMITS | mask)
{
	mView = new TranslationView();
	BMenuBar *menuBar = new BMenuBar("menu");
	BMenu *toolsMenu = new BMenu("Tools");
	toolsMenu->AddItem(new BMenuItem("Import" B_UTF8_ELLIPSIS, new BMessage(kMsgImportMenuitem)));
	menuBar->AddItem(toolsMenu);
	
	mImportFilePanel = new BFilePanel(B_OPEN_PANEL, NULL, NULL, B_FILE_NODE, false, new BMessage(kImportRef));
	mImportFilePanel->SetTarget(this);

	float spacing = be_control_look->DefaultItemSpacing();
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.Add(menuBar)
		.AddGroup(B_HORIZONTAL, 0)
			.Add(mView)
			.SetInsets(spacing);
}


TranslationView *
TranslationWindow::Translation()
{
	return mView;
}


bool
TranslationWindow::QuitRequested()
{
	be_app->PostMessage(kMsgWindowClosed);
	return BWindow::QuitRequested();
}


void
TranslationWindow::MessageReceived(BMessage *msg)
{
	if (msg->WasDropped()) {
		entry_ref ref;
		if (msg->FindRef("refs", 0, &ref)==B_OK) {
			msg->what = B_REFS_RECEIVED;
			be_app->PostMessage(msg);
		}
	}
	else if(msg->what == kMsgImportMenuitem)
	{
		if(mImportFilePanel)
			mImportFilePanel->Show();
		return;
	}
	else if(msg->what == kImportRef)
	{
		entry_ref fileRef;
		status_t result = msg->FindRef("refs", &fileRef);
		if(result == B_OK)
			mView->ImportKeys(fileRef);
		return;
	}
	BWindow::MessageReceived(msg);
}
