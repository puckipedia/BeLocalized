#ifndef CAT_KEY_TRANSLATION_STORE_H
#define CAT_KEY_TRANSLATION_STORE_H

#include <Messenger.h>
#include <String.h>

class TranslationUnit {
public:
	virtual	BString			Source() = 0;
	virtual	BString			Context() = 0;
	virtual	BString			DeveloperComment() = 0;
	virtual	BString			Translated() = 0;

	virtual	void			SetTranslated(BString) = 0;
	virtual	bool			Suggest() { return false; }
	virtual	bool			SetAsTranslation() { return false; }
};

class TranslationStore {
public:
							 TranslationStore(BMessenger &messenger,
							 	int32 message)
							 	: mLoadMessenger(messenger),
							 	  mLoadMessage(message) {}

	virtual	int32			 TotalUnits() = 0;
	virtual	int32			 LoadedUnits() = 0;
	virtual	TranslationUnit	*UnitAt(int32) = 0;
	virtual	BString			 Title() = 0;

	virtual	void			 Synchronize() {}
	virtual	bool			 CanSuggest() { return false; }
	virtual	bool			 CanSetAsTranslation() { return false; }
protected:
	BMessenger				 mLoadMessenger;
	int32					 mLoadMessage;
};
#endif
