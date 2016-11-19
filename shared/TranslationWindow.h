#ifndef CATKEY_TRANSLATION_WINDOW_H
#define CATKEY_TRANSLATION_WINDOW_H

#include <Window.h>

class TranslationView;

const int32 kMsgWindowClosed = 'WINC';
const int32 kMsgImportMenuitem = 'IMPM';
const int32 kImportRef = 'IMPR';

class TranslationWindow : public BWindow {
public:
					 TranslationWindow(BRect, uint32 = 0);
	TranslationView	*Translation();
	
	void			 MessageReceived(BMessage *);
	bool			 QuitRequested();

private:
	TranslationView	*mView;
	BFilePanel		*mImportFilePanel;
};

#endif
