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
	BWindow(rect, "Log in", B_MODAL_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
	mServer = new BTextControl("server", "Server:", "http://i18n-next.haiku-os.org", NULL);
	mUsername = new BTextControl("username", "Username:", "", NULL);

	mPassword = new BTextControl("password", "Password:", "", NULL);
	mPassword->TextView()->HideTyping(true);

	mLoginButton = new BButton("login", "Log in", new BMessage(kMsgLogInPressed));
	mLoginButton->MakeDefault(true);

	mStatusView = new BTextView("status");
	mStatusView->SetText("Waiting for login...");
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
		mStatusView->SetText("Failed!");
		mLoginButton->SetEnabled(true);
		break;
	default:
		BWindow::MessageReceived(msg);
	}
}
