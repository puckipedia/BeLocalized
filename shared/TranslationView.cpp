#include "TranslationView.h"

#include "TranslationStore.h"

#include <Alert.h>
#include <Button.h>
#include <Catalog.h>
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

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "TranslationView"


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
	mHideTranslated(false),
	mAutomaticallyConfirm(false),
	mUnit(NULL)
{
	mWordsView = new BListView("words view");
	mWordsScrollView = new BScrollView("words scroller", mWordsView, 0, false, true);

	mSourceLabel = new BStringView("source label", B_TRANSLATE("Source:"));
	mSourceLabel->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));
	mSource = new BTextView("source");
	mSource->SetInsets(6, 3, 6, 3);
	mSourceScroll = new BScrollView("source scroller", mSource, 0, false, true);
	mSource->MakeEditable(false);

	mContext = new BStringView("context", "");
	mContextLabel = new BStringView("context label", B_TRANSLATE("Context: "));

	mDeveloperComment = new BStringView("developer comment", "");
	mDeveloperCommentLabel = new BStringView("developer comment label", B_TRANSLATE("Comment: "));

	mTranslated = new BTextView("translated");
	mTranslated->SetInsets(6, 3, 6, 3);
	mTranslatedScroll = new BScrollView("translated scroller", mTranslated, 0, false, true);
	mTranslatedLabel = new BStringView("translated label", B_TRANSLATE("Translated:"));
	mTranslatedLabel->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));

	mSuggest = new BButton("suggest", "Suggest", new BMessage(kMsgSuggest));
	mSetAsTranslation = new BButton("set translation", B_TRANSLATE("Set as translation"),
		new BMessage(kMsgSetTranslation));

	mSearchControl = new BTextControl(B_TRANSLATE("Filter:"), "", new BMessage(kMsgUpdateView));
	mSearchControl->SetModificationMessage(new BMessage(kMsgUpdateView));

	mHideTranslatedCheckbox = new BCheckBox(B_TRANSLATE("Hide translated"), new BMessage(kMsgUpdateView));

	mButtonsLayout = new BGroupLayout(B_HORIZONTAL);

	BSplitView *v = 
		BLayoutBuilder::Split<>(B_VERTICAL)
			.AddGroup(B_VERTICAL)
				.AddGroup(B_HORIZONTAL)
					.Add(mHideTranslatedCheckbox)
					.AddGlue()
					.Add(mSearchControl)
				.End()
				.Add(mWordsScrollView)
			.End()
			.AddGroup(B_VERTICAL)
				.AddGroup(B_HORIZONTAL)
					.AddGroup(B_VERTICAL)
						.Add(mSourceLabel)
						.Add(mSourceScroll)
					.End()
					.AddGroup(B_VERTICAL)
						.Add(mTranslatedLabel)
						.Add(mTranslatedScroll)
					.End()
				.End()
				.AddGroup(B_HORIZONTAL)
					.AddGroup(B_VERTICAL, 0)
						.AddGroup(B_HORIZONTAL, 0)
							.Add(mContextLabel)
							.Add(mContext)
							.AddGlue()
						.End()
						.AddGroup(B_HORIZONTAL, 0)
							.Add(mDeveloperCommentLabel)
							.Add(mDeveloperComment)
							.AddGlue()
						.End()
					.End()
					.Add(mButtonsLayout)
				.End()
			.End()
		.View();

	BLayoutBuilder::Group<>(this, B_HORIZONTAL)
		.SetInsets(0)
		.Add(v);

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
	if (mHideTranslated && (translated.Length() > 0 || translated == text))
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

	if (s == 0) {
		mSource->SetText("");
		mContext->SetText("");
		mDeveloperComment->SetText("");
		mTranslated->SetText("");
		return;
	}

	mReceivedUnits = 0;
	_UpdateView();
	_UpdateButtons();
}

void
TranslationView::_UpdateButtons()
{
	while (mButtonsLayout->CountItems() > 0)
		mButtonsLayout->RemoveItem((int32)0);

	mButtonsLayout->AddItem(BSpaceLayoutItem::CreateGlue());

	if (mStore->CanSetAsTranslation()) {
		mButtonsLayout->AddView(mSetAsTranslation);
		mSetAsTranslation->MakeDefault(true);
	}
	
	if (mStore->CanSuggest()) {
		mButtonsLayout->AddView(mSuggest);
		mSuggest->MakeDefault(true);
	}
}

void
TranslationView::SetAutomaticallyConfirm(bool confirm)
{
	mAutomaticallyConfirm = confirm;
	_UpdateButtons();
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
		
		mReceivedUnits++;

		if (_Filter(u)) {
			mWordsView->AddItem(new UnitItem(u->Source(), mReceivedUnits - 1));
			if (mWordsView->CurrentSelection() == -1)
				mWordsView->Select(0);
			else
				break;
		} else
			break;
	}
	// Yes, fallthrough is on purpose!
	case kMsgSelectUnit: {
		int32 index = _IndexForIndex(mWordsView->CurrentSelection());
		if (index < 0)
			break;

		if (mAutomaticallyConfirm && mUnit) {
			mUnit->SetTranslated(mTranslated->Text());
			mUnit->Suggest();
		}

		mUnit = mStore->UnitAt(index);
		mSource->SetText(mUnit->Source());
		mContext->SetText(mUnit->Context());
		mDeveloperComment->SetText(mUnit->DeveloperComment());
		mTranslated->SetText(mUnit->Translated());

		mWordsView->ScrollToSelection();
		break;
	}
	case kMsgSetTranslation: {
		mUnit->SetTranslated(mTranslated->Text());
		if (!mUnit->SetAsTranslation()) {
			BAlert *a = new BAlert(B_TRANSLATE("Failed to set translation"),
				B_TRANSLATE("Failed to set the translation.\nDo you have "
				"the right permissions?"), B_TRANSLATE("Close"));
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
			BAlert *a = new BAlert(B_TRANSLATE("Failed to suggest translation"),
				B_TRANSLATE("Failed to suggest the translation.\nDo you have "
				"the right permissions?"), B_TRANSLATE("Close"));
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
