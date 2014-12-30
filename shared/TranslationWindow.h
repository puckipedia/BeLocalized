#ifndef CATKEY_TRANSLATION_WINDOW_H
#define CATKEY_TRANSLATION_WINDOW_H

#include <Window.h>

class TranslationView;

const int32 kMsgWindowClosed = 'WINC';

class TranslationWindow : public BWindow {
public:
					 TranslationWindow(BRect, uint32 = 0);
	TranslationView	*Translation();
	
	void			 MessageReceived(BMessage *);
	bool			 QuitRequested();

private:
	TranslationView	*mView;
};

#endif
