#include "TranslationView.h"

#include "TranslationStore.h"

#include <Alert.h>
#include <Button.h>
#include <CheckBox.h>
#include <LayoutBuilder.h>
#include <ListView.h>
#include <Message.h>
#include <ScrollView.h>
#include <SpaceLayoutItem.h>
#include <StringItem.h>
#include <StringView.h>
#include <TextControl.h>
#include <TextView.h>

#include <stdio.h>

const int32 kMsgSuggest = 'Sgst';
const int32 kMsgSetTranslation = 'Trns';
const int32 kMsgSelectUnit = 'SelU';
const int32 kMsgUpdateSelection = 'USel';

const int32 kMsgUpdateView = 'UVuw';

class UnitItem : public BStringItem {
public:
			 UnitItem(const char *text, int32 index)
				:
				BStringItem(text),
				mUnitIndex(index) {}
	int32	 UnitIndex() { return mUnitIndex; }
private:
	int32	 mUnitIndex;
};

TranslationView::TranslationView()
	:
	BView("translation view", 0),
	mHideTranslated(false)
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

	mSearchControl = new BTextControl("Search for:", "", new BMessage(kMsgUpdateView));
	mSearchControl->SetModificationMessage(new BMessage(kMsgUpdateView));

	mHideTranslatedCheckbox = new BCheckBox("Hide translated", new BMessage(kMsgUpdateView));

	mButtonsLayout = new BGroupLayout(B_HORIZONTAL);

	mDevCommentView =
		BLayoutBuilder::Group<>(B_VERTICAL)
			.AddGroup(B_HORIZONTAL)
				.Add(mDeveloperCommentLabel)
				.AddGlue()
			.End()
			.Add(mDeveloperCommentScroll).View();


	BSplitView *v = 
		BLayoutBuilder::Split<>(B_VERTICAL)
			.SetInsets(B_USE_WINDOW_INSETS)
			.AddGroup(B_VERTICAL)
				.AddGroup(B_HORIZONTAL)
					.Add(mHideTranslatedCheckbox)
					.AddGlue()
					.Add(mSearchControl)
				.End()
				.Add(mWordsScrollView)
			.End()
			.AddGroup(B_HORIZONTAL)
				.AddGroup(B_VERTICAL)
					.AddGroup(B_HORIZONTAL)
						.Add(mSourceLabel)
						.AddGlue()
					.End()
					.Add(mSourceScroll)
					.Add(mDevCommentView)
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

	mDevCommentView->Hide();

	mWordsView->SetSelectionMessage(new BMessage(kMsgSelectUnit));
	mContextLabel->SetFont(be_bold_font);
	mDeveloperCommentLabel->SetFont(be_bold_font);
	mSourceLabel->SetFont(be_bold_font);
	mTranslatedLabel->SetFont(be_bold_font);
}


void
TranslationView::HideTranslated(bool set)
{
	mHideTranslated = set;
	mHideTranslatedCheckbox->SetValue(set ? B_CONTROL_ON : B_CONTROL_OFF);

	_UpdateView();
}


void
TranslationView::_UpdateView()
{
	if (!mStore)
		return;

	UnitItem *u = (UnitItem *)mWordsView->ItemAt(mWordsView->CurrentSelection());

	int32 selection = -1;
	int32 currentSelection = -1;
	if (u)
		currentSelection = u->UnitIndex();

	mWordsView->RemoveItems(0, mWordsView->CountItems());

	for (int32 i = 0; i < mStore->LoadedUnits(); i++) {
		if (_Filter(mStore->UnitAt(i))) {
			mWordsView->AddItem(new UnitItem(mStore->UnitAt(i)->Source(), i));
			if (i == currentSelection) {
				selection = mWordsView->CountItems() - 1;
			}
		}
	}

	if (mWordsView->CountItems() > 0 && selection >= 0) {
		mWordsView->Select(selection);
		mCurrentSelection = selection;
	}
}


bool
TranslationView::_Filter(TranslationUnit *unit)
{
	BString translated = unit->Translated();
	BString text = unit->Source();
	BString searchText = mSearchControl->Text();
	if (mHideTranslated && translated.Length() > 0)
		return false;

	if (text.IFindFirst(searchText) < 0 && translated.IFindFirst(searchText) < 0)
		return false;
	
	return true;
}


int32
TranslationView::_IndexForIndex(int32 i)
{
	if (mCurrentSelection > 0) {
		i = mCurrentSelection;
		mCurrentSelection = 0;
	}

	UnitItem *u = (UnitItem *)mWordsView->ItemAt(i);
	if (u == NULL)
		return -1;

	return u->UnitIndex();
}


void
TranslationView::SetStore(TranslationStore *s)
{
	mStore = s;

	while (mButtonsLayout->CountItems() > 0)
		mButtonsLayout->RemoveItem((int32)0);

	mButtonsLayout->AddItem(BSpaceLayoutItem::CreateGlue());

	if (s == 0) {
		mSource->SetText("");
		mContext->SetText("");
		mDeveloperComment->SetText("");
		mTranslated->SetText("");
		return;
	}

	if (s->CanSetAsTranslation()) {
		mButtonsLayout->AddView(mSetAsTranslation);
		mSetAsTranslation->MakeDefault(true);
	}
	
	if (s->CanSuggest()) {
		mButtonsLayout->AddView(mSuggest);
		mSuggest->MakeDefault(true);
	}
	
	mReceivedUnits = 0;
	_UpdateView();
}

void
TranslationView::AttachedToWindow()
{
	mWordsView->SetTarget(this);
	mSuggest->SetTarget(this);
	mSetAsTranslation->SetTarget(this);
	mSearchControl->SetTarget(this);
	mHideTranslatedCheckbox->SetTarget(this);
}

void
TranslationView::MessageReceived(BMessage *msg)
{
	switch(msg->what) {
	case kMsgGotUnit: {
		TranslationUnit *u;
		if (msg->FindPointer("unit", (void **)&u) != B_OK) {
			break;
		}
		
		TranslationStore *s;
		msg->FindPointer("store", (void **)&s);
		if (s != mStore)
			break;

		if (_Filter(u)) {
			mWordsView->AddItem(new UnitItem(u->Source(), mReceivedUnits));
			if (mWordsView->CurrentSelection() == -1)
				mWordsView->Select(0);
		}
		
		mReceivedUnits++;
		break;
	}
	case kMsgSelectUnit: {
		int32 index = _IndexForIndex(mWordsView->CurrentSelection());
		if (index < 0)
			break;

		mUnit = mStore->UnitAt(index);
		mSource->SetText(mUnit->Source());
		mContext->SetText(mUnit->Context());
		mDeveloperComment->SetText(mUnit->DeveloperComment());
		mTranslated->SetText(mUnit->Translated());
		if (mUnit->DeveloperComment().Length() == 0)
			mDevCommentView->Hide();
		else
			mDevCommentView->Show();

		mWordsView->ScrollToSelection();
		break;
	}
	case kMsgSetTranslation: {
		mUnit->SetTranslated(mTranslated->Text());
		if (!mUnit->SetAsTranslation()) {
			BAlert *a = new BAlert("Failed to set translation", "Failed to set the translation.\nDo you have the right permissions?", "Close");
			a->Go();
		} else {
			mWordsView->Select(mWordsView->CurrentSelection() + 1);
			_UpdateView();
		}
		break;
	}
	case kMsgSuggest: {
		mUnit->SetTranslated(mTranslated->Text());
		if (!mUnit->Suggest()) {
			BAlert *a = new BAlert("Failed to suggest translation", "Failed to suggest the translation.\nDo you have the right permissions?", "Close");
			a->Go();
		} else {
			mWordsView->Select(mWordsView->CurrentSelection() + 1);
			_UpdateView();
		}
		break;
	}
	case kMsgUpdateView: {
		mHideTranslated = mHideTranslatedCheckbox->Value() == B_CONTROL_ON;
		_UpdateView();
		break;
	}

	default:
		BView::MessageReceived(msg);
	}
}
