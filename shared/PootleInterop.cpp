#include "PootleInterop.h"

PootleTranslationUnit::PootleTranslationUnit(Pootle *p, PootleUnit u)
	:
	mUnit(u),
	mPootle(p)
{
	mSuggestion.SetUnit(u);
	mSuggestion.SetTarget(u.Target().text);
	mCreatedSuggestion = false;
}


BString
PootleTranslationUnit::Source()
{
	return mUnit.Source().text;
}


BString
PootleTranslationUnit::Context()
{
	return mUnit.Context();
}


BString
PootleTranslationUnit::DeveloperComment()
{
	return mUnit.DeveloperComment();
}


BString
PootleTranslationUnit::Translated()
{
	return mSuggestion.Target();
}


void
PootleTranslationUnit::SetTranslated(BString str)
{
	mSuggestion.SetTarget(str);
}


bool
PootleTranslationUnit::Suggest()
{
	if (!mCreatedSuggestion) {
		mSuggestion.Create(mPootle->Suggestions());
		mCreatedSuggestion = true;
	} else {
		mSuggestion.Put();
		mCreatedSuggestion = false;
	}

	return true;
}


PootleTranslationStore::PootleTranslationStore(Pootle *pootle,
	PootleStore *store, BMessenger &messenger, int32 message)
	:
	TranslationStore(messenger, message),
	mStore(store),
	mPootle(pootle)
{
	mBackgroundThread = spawn_thread(DoBackgroundUnitLoading,
		"Pootle translation getter", B_NORMAL_PRIORITY, this);
	mToGet = store->UnitUrls();
}


void
PootleTranslationStore::StartLoading()
{
	resume_thread(mBackgroundThread);
}


int32
PootleTranslationStore::TotalUnits()
{
	int32 units;
	mListLocker.Lock();
	units = mToGet.CountItems() + mGotten.CountItems();
	mListLocker.Unlock();
	return units;
}


int32
PootleTranslationStore::LoadedUnits()
{
	int32 loaded = 0;
	mListLocker.Lock();
	loaded = mGotten.CountItems();
	mListLocker.Unlock();
	return loaded;
}


int32
PootleTranslationStore::DoBackgroundUnitLoading(void *data)
{
	PootleTranslationStore *store = (PootleTranslationStore *)data;
	while (store->mToGet.CountItems() > 0) {
		BObjectList<BString> toGet;
		for (int32 i = 0; i < 20 && i < store->mToGet.CountItems(); i++) {
			toGet.AddItem(store->mToGet.ItemAt(i));
		}
		
		BObjectList<PootleUnit> ret = store->mPootle->Units()->GetByList(toGet);
		store->mListLocker.Lock();
		int32 start = store->mGotten.CountItems();
		for (int32 i = 0; i < ret.CountItems(); i++) {
			store->mToGet.RemoveItemAt(0);
			store->mGotten.AddItem(new PootleTranslationUnit(store->mPootle, *ret.ItemAt(i)));
		}
		store->mListLocker.Unlock();

		for (int32 i = 0; i < ret.CountItems(); i++) {
			BMessage msg(store->mLoadMessage);
			msg.AddPointer("store", store);
			msg.AddPointer("unit", store->mGotten.ItemAt(start + i));
			store->mLoadMessenger.SendMessage(&msg);
		}
	}
	return 0;
}
