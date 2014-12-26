#ifndef CAT_KEY_CATKEYS_STORE_H
#define CAT_KEY_CATKEYS_STORE_H

#include "TranslationStore.h"

#include <File.h>
#include <ObjectList.h>

class CatKeyTranslationUnit;

class CatKeyStore : public TranslationStore {
public:
	CatKeyStore(BString, BMessenger &, int32);
	
	int32			 TotalUnits() { return mUnits.CountItems(); }
	int32			 LoadedUnits() { return TotalUnits(); }
	
	TranslationUnit	*UnitAt(int32 i) { return mUnits.ItemAt(i); }

	BString			 Title();

	void			 StartLoading();
	bool			 CanSetAsTranslation() { return true; }
	void			 Synchronize();

private:
	status_t		_WriteField(BString field, bool last = false);
	BFile							 mFile;
	BString							 mPath;
	BObjectList<TranslationUnit>	 mUnits;
	BString							 mSignature;
	BString							 mLanguage;
	BString							 mChecksum;
};

#endif
