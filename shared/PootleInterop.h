#ifndef POOTLE_BELOCALIZED_INTEROP_H
#define POOTLE_BELOCALIZED_INTEROP_H

#include <Locker.h>
#include <ObjectList.h>

#include "Pootle.h"
#include "PootleStoresEndpoint.h"
#include "PootleSuggestionsEndpoint.h"
#include "PootleUnitsEndpoint.h"

#include "TranslationStore.h"

class PootleTranslationUnit : public TranslationUnit {
public:
	PootleTranslationUnit(Pootle *, PootleUnit);
	
	BString	Source();
	BString	Context();
	BString	DeveloperComment();
	BString	Translated();
	
	void	SetTranslated(BString);
	bool	Suggest();
private:
	PootleSuggestion	 mSuggestion;
	PootleUnit			 mUnit;
	Pootle				*mPootle;
	bool				 mCreatedSuggestion;
};

class PootleTranslationStore : public TranslationStore {
public:
	PootleTranslationStore(Pootle *, PootleStore *, BMessenger &, int32);

	int32			 TotalUnits();
	int32			 LoadedUnits();
	TranslationUnit *UnitAt(int32 i) { return mGotten.ItemAt(i); }
	BString			 Title() { return mStore->PootlePath(); }
	
	void			 Synchronize() {}
	bool			 CanSuggest() { return true; }
	
	void			 StartLoading();
private:
	static	int32	 DoBackgroundUnitLoading(void *data);
	thread_id							 mBackgroundThread;
	BLocker								 mListLocker;
	BObjectList<BString>				 mToGet;	
	PootleStore							*mStore;
	Pootle								*mPootle;
	BObjectList<PootleTranslationUnit>	 mGotten;
};

#endif
