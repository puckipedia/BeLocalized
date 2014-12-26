#include "TranslationView.h"

#include "TranslationStore.h"

#include <Alert.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <ListView.h>
#include <Message.h>
#include <ScrollView.h>
#include <SpaceLayoutItem.h>
#include <StringItem.h>
#include <StringView.h>
#include <TextView.h>

#include <stdio.h>

const int32 kMsgSuggest = 'Sgst';
const int32 kMsgSetTranslation = 'Trns';
const int32 kMsgSelectUnit = 'SelU';

TranslationView::TranslationView()
	:
	BView("translation view", 0)
{
	mWordsView = new BListView("words view");
	mWordsScrollView = new BScrollView("words scroller", mWordsView, 0, false, true);

	mSourceLabel = new BStringView("source label", "Source:");
	mSource = new BTextView("source");
	mSourceScroll = new BScrollView("source scroller", mSource, 0, false, true);
	mSource->MakeEditable(false);

	mContext = new BStringView("context", "");
	mContextLabel = new BStringView("context label", "Context: ");

	mDeveloperCommentLabel = new BStringView("developer comment label", "Developer comment:");
	mDeveloperComment = new BTextView("developer comment");
	mDeveloperCommentScroll = new BScrollView("developer comment scroller", mDeveloperComment, 0, false, true);

	mTranslated = new BTextView("translated");
	mTranslatedScroll = new BScrollView("translated scroller", mTranslated, 0, false, true);
	mTranslatedLabel = new BStringView("translated label", "Translated:");

	mSuggest = new BButton("suggest", "Suggest", new BMessage(kMsgSuggest));
	mSetAsTranslation = new BButton("set translation", "Set as translation",
		new BMessage(kMsgSetTranslation));

	mButtonsLayout = new BGroupLayout(B_HORIZONTAL);

	BSplitView *v = 
		BLayoutBuilder::Split<>(B_VERTICAL)
			.SetInsets(B_USE_WINDOW_INSETS)
			.Add(mWordsScrollView)
			.AddGroup(B_HORIZONTAL)
				.AddGroup(B_VERTICAL)
					.AddGroup(B_HORIZONTAL)
						.Add(mSourceLabel)
						.AddGlue()
					.End()
					.Add(mSourceScroll)
					.AddGroup(B_HORIZONTAL)
						.Add(mDeveloperCommentLabel)
						.AddGlue()
					.End()
					.Add(mDeveloperCommentScroll)
					.AddGroup(B_HORIZONTAL)
						.Add(mContextLabel)
						.Add(mContext)
						.AddGlue()
					.End()
				.End()
				.AddGroup(B_VERTICAL)
					.AddGroup(B_VERTICAL)
						.AddGroup(B_HORIZONTAL)
							.Add(mTranslatedLabel)
							.AddGlue()
						.End()
						.Add(mTranslatedScroll)
					.End()
					.AddGroup(mButtonsLayout)
					.End()
				.End()
			.End()
		.View();
	
	BLayoutBuilder::Group<>(this, B_HORIZONTAL)
		.Add(v);

	mWordsView->SetSelectionMessage(new BMessage(kMsgSelectUnit));
	mContextLabel->SetFont(be_bold_font);
	mDeveloperCommentLabel->SetFont(be_bold_font);
	mSourceLabel->SetFont(be_bold_font);
	mTranslatedLabel->SetFont(be_bold_font);
}


void
TranslationView::SetStore(TranslationStore *s)
{
	mStore = s;

	while (mButtonsLayout->CountItems() > 0)
		mButtonsLayout->RemoveItem((int32)0);

	mButtonsLayout->AddItem(BSpaceLayoutItem::CreateGlue());

	if (s->CanSetAsTranslation()) {
		mButtonsLayout->AddView(mSetAsTranslation);
		mSetAsTranslation->MakeDefault(true);
	}
	
	if (s->CanSuggest()) {
		mButtonsLayout->AddView(mSuggest);
		mSuggest->MakeDefault(true);
	}
}

void
TranslationView::AttachedToWindow()
{
	mWordsView->SetTarget(this);
	mSuggest->SetTarget(this);
	mSetAsTranslation->SetTarget(this);
}

void
TranslationView::MessageReceived(BMessage *msg)
{
	switch(msg->what) {
	case kMsgGotUnit: {
		Window()->Lock();
		TranslationUnit *u;
		if (msg->FindPointer("unit", (void **)&u) != B_OK) {
			break;
		}
		
		mWordsView->AddItem(new BStringItem(u->Source()));
		if (mWordsView->CurrentSelection() == -1)
			mWordsView->Select(0);
		Window()->Lock();
		break;
	}
	case kMsgSelectUnit: {
		TranslationUnit *u = mStore->UnitAt(msg->GetInt32("index", 0));
		mSource->SetText(u->Source());
		mContext->SetText(u->Context());
		mDeveloperComment->SetText(u->DeveloperComment());
		mTranslated->SetText(u->Translated());
		break;
	}
	case kMsgSetTranslation: {
		TranslationUnit *u = mStore->UnitAt(msg->GetInt32("index", 0));
		u->SetTranslated(mTranslated->Text());
		if (!u->SetAsTranslation()) {
			BAlert *a = new BAlert("Failed to set translation", "Failed to set the translation.\nDo you have the right permissions?", "Close");
			a->Go();
			delete a;
		}
		break;
	}
	case kMsgSuggest: {
		TranslationUnit *u = mStore->UnitAt(msg->GetInt32("index", 0));
		u->SetTranslated(mTranslated->Text());
		if (!u->Suggest()) {
			BAlert *a = new BAlert("Failed to suggest translation", "Failed to suggest the translation.\nDo you have the right permissions?", "Close");
			a->Go();
			delete a;
		}
		break;
	}
	default:
		msg->PrintToStream();
		BView::MessageReceived(msg);
	}
}
