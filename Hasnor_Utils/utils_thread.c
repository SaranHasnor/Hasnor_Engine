#include "utils_thread.h"

#include "utils_memory.h"
#include "utils_list.h"
#include "utils_string.h"

#include <windows.h>
#include <process.h>

typedef struct {
	uint		id;
	char		*name;
	HANDLE		obj;
} mutex_t;

list_t *_mutexes = NULL;
uint _mutexCount = 0;

uint _threadCount = 0;

uint thread_newMutex(const char *name)
{
	mutex_t *mutex = alloc(mutex_t);
	mutex->id = ++_mutexCount;
	mutex->name = name ? String.create(name) : NULL;
	mutex->obj = CreateMutex(NULL, FALSE, NULL);

	List.add(&_mutexes, mutex);

	return mutex->id;
}

void thread_killMutex(uint id)
{
	list_t **mutexIterator = &_mutexes;
	while ((*mutexIterator)->content)
	{
		mutex_t *mutex = (mutex_t*)(*mutexIterator)->content;
		if (mutex->id == id)
		{
			List.removeAt(mutexIterator, 0);
			CloseHandle(mutex->obj);
			dealloc(mutex);
			break;
		}
		mutexIterator = &(*mutexIterator)->next;
	}
}

mutex_t *_getMutexWithID(uint id)
{
	list_t *mutexIterator = _mutexes;
	while (mutexIterator->content)
	{
		mutex_t *mutex = (mutex_t*)mutexIterator->content;
		if (mutex->id == id)
		{
			return mutex;
		}
		mutexIterator = mutexIterator->next;
	}
	return NULL;
}

void thread_lockMutex(uint id)
{
	WaitForSingleObject(_getMutexWithID(id)->obj, INFINITE);
}

void thread_unlockMutex(uint id)
{
	ReleaseMutex(_getMutexWithID(id)->obj);
}

thread_t *thread_new(void)
{
	thread_t *newThread = alloc(thread_t);
	newThread->id = ++_threadCount;
	newThread->function = NULL;
	newThread->loops = false;
	newThread->delayBetweenLoops = 0;
	newThread->destroyWhenDone = true;
	return newThread;
}

void _singleRun(void *arg)
{
	thread_t *thread = (thread_t*)arg;

	thread->function();

	if (thread->destroyWhenDone)
	{
		dealloc(thread);
	}
}

void _loopRun(void *arg)
{
	thread_t *thread = (thread_t*)arg;
	bool keepGoing;

	do {
		keepGoing = thread->function();
		if (thread->delayBetweenLoops)
		{
			Sleep(thread->delayBetweenLoops);
		}
	} while (keepGoing);

	if (thread->destroyWhenDone)
	{
		dealloc(thread);
	}
}

void thread_run(thread_t *thread)
{
	if (thread->loops)
	{
		_beginthread(_loopRun, 0, thread);
	}
	else
	{
		_beginthread(_singleRun, 0, thread);
	}
}

void initThreadFunctions(void)
{
	Thread.newThread = thread_new;
	Thread.newMutex = thread_newMutex;
	Thread.deleteMutex = thread_killMutex;
	Thread.lockMutex = thread_lockMutex;
	Thread.unlockMutex = thread_unlockMutex;
	Thread.run = thread_run;
}