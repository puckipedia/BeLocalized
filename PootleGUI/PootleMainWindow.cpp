#include "PootleMainWindow.h"

#include "AsyncGetter.h"
#include "Pootle.h"
#include "PootleInterop.h"
#include "TranslationView.h"

#include <LayoutBuilder.h>
#include <ListView.h>
#include <ScrollView.h>
#include <Message.h>

const int32 kMsgGotProject = 'pRoJ';
const int32 kMsgGotDataForProject = 'PrOj';
const int32 kMsgTranslationProjectChosen = 'TrPr';
const int32 kMsgGotStores = 'StRs';
const int32	kMsgChoseStore = 'ChSt';

PootleMainWindow::PootleMainWindow(BRect rect, Pootle *pootle)
	:
	BWindow(rect, "BeLocalized", B_TITLED_WINDOW,
		B_AUTO_UPDATE_SIZE_LIMITS | B_QUIT_ON_WINDOW_CLOSE),
	mPootle(pootle),
	mCurrentTranslationGet(NULL),
	mCurrentStoreGet(NULL)
{
	mProjectsView = new BListView("projects");
	mProjectsView->SetSelectionMessage(new BMessage(kMsgGotProject));

	mTranslationProjectsView = new BListView("translation projects");
	mTranslationProjectsView->SetSelectionMessage(new BMessage(kMsgTranslationProjectChosen));

	mTranslationView = new TranslationView();

	mStoresView = new BListView("stores");
	mStoresView->SetSelectionMessage(new BMessage(kMsgChoseStore));

	BLayoutBuilder::Group<>(this)
		.SetInsets(B_USE_WINDOW_INSETS)
		.AddSplit(B_HORIZONTAL, 1.0f)
			.AddGroup(B_VERTICAL)
				.Add(new BScrollView("projects scroller", mProjectsView, 0, false, true), 0.2f)
				.Add(new BScrollView("translation projects scroller", mTranslationProjectsView, 0, false, true), 0.2f)
				.Add(new BScrollView("stores scroller", mStoresView, 0, false, true), 0.2f)
			.End()
			.Add(mTranslationView);
	
	mProjects = mPootle->Projects()->Get();
	for (int32 i = 0; i < mProjects.CountItems(); i++) {
		mProjectsView->AddItem(new BStringItem(mProjects.ItemAt(i)->FullName()));
	}
	
}


int32
GetTranslationProjects(void *data) {
	AsyncTranslationProjectGet *d = (AsyncTranslationProjectGet *)data;
	BObjectList<PootleTranslationProject> *projects
		= new BObjectList<PootleTranslationProject>(d->project->TranslationProjects());

	BMessage msg(kMsgGotDataForProject);
	msg.AddPointer("data", (void *)projects);
	if (!d->dont_message)
		d->messenger.SendMessage(&msg);
	delete d;
	return 0;
}


int32
GetStores(void *data) {
	AsyncStoreGet *d = (AsyncStoreGet *)data;
	BObjectList<PootleStore> *stores
		= new BObjectList<PootleStore>(d->project->Stores());

	BMessage msg(kMsgGotStores);
	msg.AddPointer("data", (void *)stores);
	if (!d->dont_message)
		d->messenger.SendMessage(&msg);
	delete d;
	return 0;
}


void
PootleMainWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what) {
	case kMsgGotProject: {
		int32 index = msg->GetInt32("index", -1);
		if (index < 0)
			break;
		PootleProject *p = mProjects.ItemAt(index);
		AsyncTranslationProjectGet *g = new AsyncTranslationProjectGet();
		g->project = p;
		g->messenger = BMessenger(this);
		g->dont_message = false;
		if (mCurrentTranslationGet)
			mCurrentTranslationGet->dont_message = true;
		mCurrentTranslationGet = g;

		thread_id t = spawn_thread(GetTranslationProjects,
			"translation project getter", B_NORMAL_PRIORITY, g);
		resume_thread(t);

		mTranslationProjectsView->RemoveItems(0, mTranslationProjectsView->CountItems());
		mTranslationProjectsView->AddItem(new BStringItem("Loading..."));
		mStoresView->RemoveItems(0, mStoresView->CountItems());
		break;
	}
	case kMsgTranslationProjectChosen: {
		int32 index = msg->GetInt32("index", -1);
		if (index < 0 || mCurrentTranslationGet)
			break;
		PootleTranslationProject *p = mTranslationProjects.ItemAt(index);
		AsyncStoreGet *g = new AsyncStoreGet();
		g->project = p;
		g->messenger = BMessenger(this);
		g->dont_message = false;
		if (mCurrentStoreGet)
			mCurrentStoreGet->dont_message = true;
		mCurrentStoreGet = g;

		thread_id t = spawn_thread(GetStores,
			"store getter", B_NORMAL_PRIORITY, g);
		resume_thread(t);

		mStoresView->RemoveItems(0, mStoresView->CountItems());
		mStoresView->AddItem(new BStringItem("Loading..."));
		break;
	}
	case kMsgGotDataForProject: {
		BObjectList<PootleTranslationProject> *projects;
		msg->FindPointer("data", (void **)&projects);
		mTranslationProjects = *projects;
		delete projects;
		mTranslationProjectsView->RemoveItems(0, mTranslationProjectsView->CountItems());
		for (int32 i = 0; i < mTranslationProjects.CountItems(); i++) {
			PootleTranslationProject *p = mTranslationProjects.ItemAt(i);
			mTranslationProjectsView->AddItem(new BStringItem(p->Language().FullName()));
		}
		mCurrentTranslationGet = NULL;
		break;
	}
	case kMsgGotStores: {
		BObjectList<PootleStore> *stores;
		msg->FindPointer("data", (void **)&stores);
		mStores = *stores;
		delete stores;
		mStoresView->RemoveItems(0, mStoresView->CountItems());
		for (int32 i = 0; i < mStores.CountItems(); i++) {
			PootleStore *p = mStores.ItemAt(i);
			mStoresView->AddItem(new BStringItem(p->PootlePath().String()));
		}
		mCurrentStoreGet = NULL;
		break;
	}
	case kMsgChoseStore: {
		if (mCurrentStoreGet || mCurrentTranslationGet)
			break;

		BMessenger msg(mTranslationView);
		PootleTranslationStore *s = new PootleTranslationStore(mPootle,
			mStores.ItemAt(mStoresView->CurrentSelection()), msg, kMsgGotUnit);
		
		mTranslationView->SetStore(s);
		s->StartLoading();
		break;
	}
	default:
		BWindow::MessageReceived(msg);
	}
}
