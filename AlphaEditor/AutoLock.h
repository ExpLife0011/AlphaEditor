//		�Զ�����֧�ֿ��߳�
// AutoLock.h: interface for the CAutoLock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOLOCK_H__1082B027_8A7D_4388_B215_B97E5229476A__INCLUDED_)
#define AFX_AUTOLOCK_H__1082B027_8A7D_4388_B215_B97E5229476A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>

// dll����Ϊ��
#if (defined WIN32 || defined _WIN32 || defined WINCE) && defined CALOCK_EXPORTS
#  define ALOCK_EXPORTS __declspec(dllexport)
#else
#  define ALOCK_EXPORTS
#endif

namespace ALOCK
{
///////////////////////////////////////////
// ����Ϊ��ĳ�Ա����������ȫ�ֱ���
class ALOCK_EXPORTS Mutex
{
public:
	Mutex();
	~Mutex();
	Mutex(const Mutex& m);
	Mutex& operator = (const Mutex& m);

	void lock();
	BOOL trylock();
	void unlock();

	struct Impl;
protected:
	Impl* impl;
};
///////////////////////////////////////////
struct Mutex::Impl
{
	Impl()
	{
#if (_WIN32_WINNT >= 0x0600)
		::InitializeCriticalSectionEx(&cs, 1000, 0);
#else
		::InitializeCriticalSection(&cs);
#endif
		refcount = 1;
	}
	~Impl() { DeleteCriticalSection(&cs); }

	void lock() { EnterCriticalSection(&cs); }
	BOOL trylock() { return TryEnterCriticalSection(&cs) != 0; }
	void unlock() { LeaveCriticalSection(&cs); }

	CRITICAL_SECTION cs;
	long refcount;
};

///////////////////////////////////////////
class ALOCK_EXPORTS AutoLock
{
public:
	AutoLock(Mutex& m) : mutex(&m) { mutex->lock(); }
	~AutoLock() { mutex->unlock(); }
protected:
	Mutex* mutex;
private:
	AutoLock(const AutoLock&);
	AutoLock& operator = (const AutoLock&);
};
///////////////////////////////////////////

} // namespace LOCK





#endif // !defined(AFX_AUTOLOCK_H__1082B027_8A7D_4388_B215_B97E5229476A__INCLUDED_)
