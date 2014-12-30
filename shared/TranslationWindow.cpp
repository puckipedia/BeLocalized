#include "TranslationWindow.h"

#include "TranslationView.h"

#include <Application.h>
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

	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.Add(mView);
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
	BWindow::MessageReceived(msg);
}
