#include "CatKeyStore.h"

#include <File.h>

class CatKeyTranslationUnit : public TranslationUnit {
public:
	CatKeyTranslationUnit(CatKeyStore *s) : mStore(s) {}
	BString	Source() { return mSource; }
	BString Context() { return mContext; }
	BString DeveloperComment() { return mComment; }
	BString Translated() { return mTranslated; }

	void	SetSource(BString t) { mSource = t; }
	void	SetContext(BString t) { mContext = t; }
	void	SetDeveloperComment(BString t) { mComment = t; }
	void	SetTranslated(BString t) { mTranslated = t; }
	
	bool	Suggest() { return false; }
	bool	SetAsTranslation() { mStore->Synchronize(); }
private:
	BString		 mSource;
	BString		 mContext;
	BString		 mComment;
	BString		 mTranslated;
	CatKeyStore	*mStore;
};


CatKeyStore::CatKeyStore(BString path, BMessenger &messenger, int32 message)
	:
	TranslationStore(messenger, message),
	mFile(path, B_READ_WRITE),
	mPath(path)
{
}

BString
CatKeyStore::Title()
{
	return mPath;
}

bool
CatKeyStore::StartLoading()
{
	char buffer[1024];
	ssize_t readbytes = mFile.Read(buffer, sizeof(buffer));
	char *buffer_pointer;
	
	BString temp;
	CatKeyTranslationUnit *u = NULL;
	bool first = true;
	int index = 0;
	int instring = 0;

	while (readbytes > 0) {
		buffer_pointer = buffer - 1;
		while(readbytes--) {
			buffer_pointer++;
			if(*buffer_pointer == '\t' || *buffer_pointer == '\n' || *buffer_pointer == '\r') {
				if (first) {
					switch(index++) {
					case 0:
						if(temp != "1") {
							return false;
						}
						break;
					case 1:
						mLanguage = temp;
						break;
					case 2:
						mSignature = temp;
						break;
					case 3:
						mChecksum = temp;
						break;
					}
				} else {
					switch(index++) {
					case 0:
						u->SetSource(temp);
						break;
					case 1:
						u->SetContext(temp);
						break;
					case 2:
						u->SetDeveloperComment(temp);
						break;
					case 3:
						u->SetTranslated(temp);
						break;
					}
				}
				
				temp = "";
				if (*buffer_pointer == '\r')
					buffer_pointer++;

				if (*buffer_pointer == '\n') {
					if (u) {
						mUnits.AddItem(u);
						BMessage msg(mLoadMessage);
						msg.AddPointer("unit", u);
						msg.AddPointer("store", this);
						mLoadMessenger.SendMessage(&msg);
					}
					u = new CatKeyTranslationUnit(this);
					index = 0;
					first = false;
				}
			} else {
				temp += *buffer_pointer;
			}
		}
		readbytes = mFile.Read(buffer, sizeof(buffer));
	}

	BMessage msg(mLoadMessage);
	msg.AddPointer("store", this);
	mLoadMessenger.SendMessage(&msg);
	
	delete u;
	return true;
}

status_t
CatKeyStore::_WriteField(BString field, bool last)
{
	if (last)
		field.Append("\n");
	else
		field.Append("\t");	
	
	return mFile.Write(field.String(), field.Length());
}

void
CatKeyStore::Synchronize()
{
	mFile.Seek(0, SEEK_SET);
	_WriteField("1");
	_WriteField(mLanguage);
	_WriteField(mSignature);
	_WriteField(mChecksum, true);

	for (int32 i = 0; i < mUnits.CountItems(); i++) {
		CatKeyTranslationUnit *u = (CatKeyTranslationUnit *)mUnits.ItemAt(i);
		_WriteField(u->Source());
		_WriteField(u->Context());
		_WriteField(u->DeveloperComment());
		_WriteField(u->Translated(), true);
	}
}
