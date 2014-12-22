#ifndef BELOCALIZED_LOGIN_WINDOW_H
#define BELOCALIZED_LOGIN_WINDOW_H

#include <Window.h>

class BButton;
class BTextControl;
class BTextView;

class Pootle;

class LogInWindow : public BWindow {
public:
			LogInWindow(BRect);
	void	MessageReceived(BMessage *);
private:
	BTextView		*mStatusView;
	BButton			*mLoginButton;
	BTextControl	*mServer;
	BTextControl	*mUsername;
	BTextControl	*mPassword;
	Pootle			*mPootle;
};

#endif
