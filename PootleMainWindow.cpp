#include "PootleMainWindow.h"

#include "AsyncGetter.h"
#include "Pootle.h"

#include <LayoutBuilder.h>
#include <ListView.h>
#include <ScrollView.h>
#include <Message.h>

const int32 kMsgGotProject = 'pRoJ';
const int32 kMsgGotDataForProject = 'PrOj' + 1;

PootleMainWindow::PootleMainWindow(BRect rect, Pootle *pootle)
	:
	BWindow(rect, "BeLocalized", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS),
	mPootle(pootle)
{
	mProjectsView = new BListView("projects");
	mProjectsView->SetSelectionMessage(new BMessage(kMsgGotProject));
	mTranslationProjectsView = new BListView("translation projects");
	BLayoutBuilder::Group<>(this)
		.Add(mProjectsView, 0.2f)
		.Add(mTranslationProjectsView, 0.2f)
		.Add(new BView("nothing", 0), 0.6f);
	
	mProjects = mPootle->Projects()->Get();
	for (int32 i = 0; i < mProjects.CountItems(); i++) {
		mProjectsView->AddItem(new BStringItem(mProjects.ItemAt(i)->FullName()));
	}
	
}


void
PootleMainWindow::MessageReceived(BMessage *msg)
{
	if (msg->what - kMsgGotDataForProject == mProjectsView->CurrentSelection()) {
		PootleTranslationProject *p;
		msg->FindPointer("pointer", (void **)&p);
		mTranslationProjectsView->AddItem(new BStringItem(p->Language().FullName()));
		return;
	}

	switch(msg->what) {
	case kMsgGotProject: {
		int32 index = msg->GetInt32("index", -1);
		if (index < 0)
			break;
		PootleProject *p = mProjects.ItemAt(index);
		GetAsyncByIds(mPootle->TranslationProjects(), p->GetTranslationProjectUrls(), this, kMsgGotDataForProject + index);
		break;
	}
	default:
		BWindow::MessageReceived(msg);
	}
}
