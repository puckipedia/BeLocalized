#ifndef CATKEY_TRANSLATIONVIEW_H
#define CATKEY_TRANSLATIONVIEW_H

#include <View.h>

class BButton;
class BGroupLayout;
class BListView;
class BScrollView;
class BStringView;
class BTextView;

class TranslationStore;
class TranslationUnit;

const int32 kMsgGotUnit = 'Unit';

class TranslationView : public BView {
public:
			 TranslationView();
	void	 MessageReceived(BMessage *);
	void	 AttachedToWindow();
	void	 SetStore(TranslationStore *);
	
	bool	 HidesTranslated() { return mHideTranslated; }
	void	 HideTranslated(bool);

private:
	bool				 mHideTranslated;

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

	TranslationStore	*mStore;
	TranslationUnit		*mUnit;
	int					 mReceivedUnits;
};

#endif
