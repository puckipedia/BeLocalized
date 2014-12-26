#include "LogInWindow.h"

#include "Pootle.h"

#include <Application.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <Message.h>
#include <Rect.h>
#include <TextControl.h>
#include <TextView.h>

#include <stdio.h>

const uint32 kMsgLogInPressed = 'LgIn';

LogInWindow::LogInWindow(BRect rect)
	:
	BWindow(rect, "Log in", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	mServer = new BTextControl("server", "Server:", "http://i18n-next.haiku-os.org", NULL);
	mServer->TextView()->SetExplicitMinSize(BSize(mServer->TextView()->StringWidth(mServer->Text()) + 20, mServer->MinSize().Height()));
	mServer->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);

	mUsername = new BTextControl("username", "Username:", "", NULL);
	mUsername->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);

	mPassword = new BTextControl("password", "Password:", "", NULL);
	mPassword->TextView()->HideTyping(true);
	mPassword->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);

	mLoginButton = new BButton("login", "Log in", new BMessage(kMsgLogInPressed));
	mLoginButton->MakeDefault(true);

	mStatusView = new BTextView("status");
	mStatusView->SetText("Waiting for login...");
	mStatusView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	BLayoutBuilder::Grid<>(this)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(mServer->CreateLabelLayoutItem(), 0, 0)
		.Add(mServer->CreateTextViewLayoutItem(), 1, 0)
		.Add(mUsername->CreateLabelLayoutItem(), 0, 1)
		.Add(mUsername->CreateTextViewLayoutItem(), 1, 1)
		.Add(mPassword->CreateLabelLayoutItem(), 0, 2)
		.Add(mPassword->CreateTextViewLayoutItem(), 1, 2)
		.AddGroup(B_HORIZONTAL, B_USE_WINDOW_SPACING, 0, 3, 2)
			.Add(mStatusView)
			.Add(mLoginButton);
	
	BSize preferred = GetLayout()->PreferredSize();
	ResizeTo(preferred.Width(), preferred.Height());

	mUsername->MakeFocus();
}


void
LogInWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what) {
	case kMsgLogInPressed:
		mStatusView->SetText("Logging in...");
		mPootle = new Pootle(BUrl(mServer->Text()), BMessenger(this), mUsername->Text(), mPassword->Text());
		mLoginButton->SetEnabled(false);
		break;
	case kMsgPootleInited: {
		mStatusView->SetText("Getting languages...");
		mPootle->Languages()->Get();
		mStatusView->SetText("Success!");
		BMessage success(kMsgPootleInited);
		success.AddPointer("pootle", mPootle);
		be_app->PostMessage(&success);
		Quit();
		break;
	}
	case kMsgPootleInitFailed:
		mStatusView->SetText("Couldn't log in!");
		mLoginButton->SetEnabled(true);
		break;
	default:
		BWindow::MessageReceived(msg);
	}
	return;
}
