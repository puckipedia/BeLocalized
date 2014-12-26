#ifndef ASYNC_GETTER_H
#define ASYNC_GETTER_H

#include <Looper.h>
#include <ObjectList.h>

template<typename T>
struct GetAsyncData {
	T *endpoint;
	BObjectList<BString> ids;
	BLooper *target;
	int32 what;
};

template<typename T>
int32
_GetAsync(void *data)
{
	GetAsyncData<T> *as = (GetAsyncData<T> *)data;
	for (int32 i = 0; i < as->ids.CountItems(); i++) {
		typename T::Data retdata = as->endpoint->GetByUrl(as->ids.ItemAt(i)->String());
		typename T::Data *pointer = new typename T::Data();
		*pointer = retdata;
		BMessage result(as->what);
		result.AddPointer("pointer", pointer);
		as->target->PostMessage(&result);
		printf("Sent a message!\n");
	}

	delete as;
	return 0;
}

template<class T>
void
GetAsyncByIds(T *endpoint, BObjectList<BString> ids, BLooper *target, int32 what)
{
	GetAsyncData<T> *d = new GetAsyncData<T>();
	d->endpoint = endpoint;
	d->ids = ids;
	d->target = target;
	d->what = what;
	thread_id t = spawn_thread(&_GetAsync<T>, "Async Data Getter", B_NORMAL_PRIORITY, d);
	if (t >= 0)
		resume_thread(t);
	printf("Returned from GABI\n");
}

#endif
