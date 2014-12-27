#include "TranslationWindow.h"

#include "TranslationView.h"

#include <Application.h>
#include <LayoutBuilder.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Message.h>
#include <Rect.h>

const int32 kMsgHideTranslated = 'HiTr';

TranslationWindow::TranslationWindow(BRect rect)
	:
	BWindow(rect, "BeLocalized", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	mView = new TranslationView();
	mMenu = new BMenuBar("menu bar");
	mHideTranslated = new BMenuItem("Hide Translated", new BMessage(kMsgHideTranslated));

	BMenu *subMenu = new BMenu("Options");
	subMenu->AddItem(mHideTranslated);

	BMenuItem *subMenuItem = new BMenuItem(subMenu);
	mMenu->AddItem(subMenuItem);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL)
		.Add(mMenu)
		.Add(mView);
}


TranslationView *
TranslationWindow::Translation()
{
	return mView;
}


void
TranslationWindow::MessageReceived(BMessage *msg)
{
	if (msg->what == kMsgHideTranslated) {
		mView->HideTranslated(!mView->HidesTranslated());
		mHideTranslated->SetMarked(mView->HidesTranslated());
	} else if(msg->what == B_QUIT_REQUESTED) {
		be_app->PostMessage(kMsgWindowClosed);
	} else {
		BWindow::MessageReceived(msg);
	}
}
