#ifndef POOTLE_MAIN_WINDOW_H
#define POOTLE_MAIN_WINDOW_H

#include <Messenger.h>
#include <ObjectList.h>
#include <Window.h>

class BListView;
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

	BListView		*mProjectsView;
	BListView		*mTranslationProjectsView;
	BListView		*mStoresView;

	BMenuItem		*mShowTranslated;
	TranslationView	*mTranslationView;
};

#endif
