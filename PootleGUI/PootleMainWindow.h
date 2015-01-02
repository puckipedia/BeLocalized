#ifndef POOTLE_MAIN_WINDOW_H
#define POOTLE_MAIN_WINDOW_H

#include <Messenger.h>
#include <ObjectList.h>
#include <Window.h>

class BListView;
class BMenu;
class Pootle;
class PootleProject;
class PootleStore;
class PootleTranslationProject;
class TranslationView;

struct AsyncTranslationProjectGet {
	PootleProject	*project;
	BMessenger		 messenger;
	bool			 dont_message;
};

struct AsyncStoreGet {
	PootleTranslationProject	*project;
	BMessenger		 			messenger;
	bool			 			dont_message;
};

class PootleMainWindow : public BWindow {
public:
					PootleMainWindow(BRect, Pootle *);
	void			MessageReceived(BMessage *);
private:

	Pootle			*mPootle;
	BObjectList<PootleProject> mProjects;
	BObjectList<PootleTranslationProject> mTranslationProjects;
	BObjectList<PootleStore> mStores;

	AsyncTranslationProjectGet *mCurrentTranslationGet;
	AsyncStoreGet *mCurrentStoreGet;

	BListView		*mStoresView;
	BMenu			*mLanguagesMenu;
	BMenu			*mProjectsMenu;
	int32			 mSelectedLanguage;
	int32			 mSelectedProject;

	TranslationView	*mTranslationView;
};

#endif
