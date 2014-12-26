#ifndef CATKEY_TRANSLATION_WINDOW_H
#define CATKEY_TRANSLATION_WINDOW_H

#include <Window.h>

class TranslationView;

class TranslationWindow : public BWindow {
public:
					 TranslationWindow(BRect);
	TranslationView	*Translation();
	
	void			 MessageReceived(BMessage *);

private:
	TranslationView	*mView;
	
	BMenuBar		*mMenu;
	BMenuItem		*mHideTranslated;
};

#endif
