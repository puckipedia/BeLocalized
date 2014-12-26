#ifndef POOTLE_MAIN_WINDOW_H
#define POOTLE_MAIN_WINDOW_H

#include <ObjectList.h>
#include <Window.h>

class BListView;
class Pootle;
class PootleProject;
class PootleTranslationProject;

class PootleMainWindow : public BWindow {
public:
					PootleMainWindow(BRect, Pootle *);
	void			MessageReceived(BMessage *);
private:
	Pootle			*mPootle;
	BObjectList<PootleProject> mProjects;
	BObjectList<PootleTranslationProject> mTranslationProjects;
	BListView		*mProjectsView;
	BListView		*mTranslationProjectsView;
};

#endif
