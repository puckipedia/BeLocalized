#include "TranslationWindow.h"

#include "TranslationView.h"

#include <Application.h>
#include <ControlLook.h>
#include <Entry.h>
#include <LayoutBuilder.h>
#include <Message.h>
#include <Rect.h>

const int32 kMsgHideTranslated = 'HiTr';

TranslationWindow::TranslationWindow(BRect rect, uint32 mask)
	:
	BWindow(rect, "BeLocalized", B_TITLED_WINDOW,
		B_AUTO_UPDATE_SIZE_LIMITS | mask)
{
	mView = new TranslationView();

	float spacing = be_control_look->DefaultItemSpacing();
	BLayoutBuilder::Group<>(this, B_VERTICAL)
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
	BWindow::MessageReceived(msg);
}
