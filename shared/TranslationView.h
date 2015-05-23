#ifndef CATKEY_TRANSLATIONVIEW_H
#define CATKEY_TRANSLATIONVIEW_H

#include <View.h>

class BButton;
class BCheckBox;
class BGroupLayout;
class BListView;
class BScrollView;
class BStringView;
class BTextControl;
class BTextView;

class TranslationStore;
class TranslationUnit;

const int32 kMsgGotUnit = 'Unit';

class TranslationView : public BView {
public:
						 TranslationView();
	void				 MessageReceived(BMessage *);
	void				 AttachedToWindow();
	void				 SetStore(TranslationStore *);
	TranslationStore	*Store() { return mStore; }
	
	bool				 HidesTranslated() const { return mHideTranslated; }
	void				 HideTranslated(bool);

	bool				 AutomaticallyConfirm() const {
							return mAutomaticallyConfirm; }
	void				 SetAutomaticallyConfirm(bool);

private:
	void				 _UpdateView();
	void				 _UpdateButtons();
	bool				 _Filter(TranslationUnit *);
	int32				 _IndexForIndex(int32);

	bool				 mHideTranslated;
	int32				 mCurrentSelection;

	BListView			*mWordsView;
	BScrollView			*mWordsScrollView;
	
	BStringView			*mContextLabel;	
	BStringView			*mContext;

	BStringView			*mTranslatedLabel;
	BTextView			*mTranslated;
	BScrollView			*mTranslatedScroll;
	
	BStringView			*mSourceLabel;
	BTextView			*mSource;
	BScrollView			*mSourceScroll;

	BStringView			*mDeveloperCommentLabel;
	BTextView			*mDeveloperComment;
	BScrollView			*mDeveloperCommentScroll;

	BGroupLayout		*mButtonsLayout;
	BButton				*mSuggest;
	BButton				*mSetAsTranslation;

	BTextControl		*mSearchControl;
	BCheckBox			*mHideTranslatedCheckbox;
	
	BView				*mDevCommentView;

	TranslationStore	*mStore;
	TranslationUnit		*mUnit;
	int					 mReceivedUnits;
	bool				 mAutomaticallyConfirm;
};

#endif
