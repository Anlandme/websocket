#ifndef __ISR_DEFINES_H__
#define __ISR_DEFINES_H__

#ifndef MAX_PATH
#  define MAX_PATH 260
#endif

#ifdef __GNUC__
	typedef unsigned int        DWORD;
	typedef unsigned short		WORD;
	typedef void*				HINSTANCE;
	#define HANDLE 				void*
	typedef unsigned char       BYTE;
	typedef BYTE*				PBYTE;
	typedef unsigned char		byte;
	typedef DWORD				DWORD_PTR;
	typedef const char			PCSTR;
	typedef DWORD				ULONG;
	typedef unsigned int		ULONG_PTR, *PULONG_PTR;
	typedef wchar_t				TCHAR;
	typedef int 				LONG;


	#define PtrToUlong( p )		((ULONG)(ULONG_PTR) (p) )
	#define INFINITE			0xFFFFFFFF

	#define INVALID_HANDLE_VALUE ((HANDLE)(long)-1)

	//SSE
	#define SSE_MM_ALIGN(x)	__attribute__ ((aligned (x)))
	#define SSE_MM_ALIGN16	__attribute__ ((aligned (16)))


	#include <malloc.h>
	#define _aligned_free		free
	#define _aligned_malloc(size, align)		memalign(align, size)
	//string
	#include <string.h>
	#define _stricmp	strcasecmp
	#define sprintf_s	snprintf
	#define strcpy_s	strncpy
	#define strcat_s	strncat

	//time
	#include <unistd.h>
	static void Sleep(int ms)
	{
		usleep(ms*1000);
	}
	//file
	#include <stdio.h>
	#define _fseeki64	fseeko
	#define _ftelli64	ftello64
	//INT_MAX
	#include <limits.h>
		//dll
	#include <dlfcn.h>
	#include <assert.h>
	#define HMODULE	void*
	#ifdef __cplusplus
		#include <string>
		static HMODULE LoadLibraryA(const char* file)
		{
		
//			printf("file = %s\n",file);
//			printf("hello world!\n");

			std::string normalized_file = file;
			int pos = -1;
			pos = normalized_file.rfind('.', -1);
			assert( pos != -1);
			if( normalized_file.substr(pos, -1) != ".so")
			{
				normalized_file = normalized_file.substr(0, pos) + ".so";
			}
			HMODULE handle = ::dlopen(normalized_file.c_str(), RTLD_LAZY);
			if(char* err = ::dlerror())
			{
				if(err)
				{
					printf("load dll %s with err %s\n", normalized_file.c_str(), err);
				}
				//assert(0 && "dll open err");
				return 0;
			}
			return handle;
		}

		static void* GetProcAddress(HMODULE handle, const char* procAddress)
		{
			void* p = ::dlsym(handle, procAddress);
			if(::dlerror())
			{
				assert(0 && "GetProcAddress err");
				return 0;
			}
			return p;
		}

		static void FreeLibrary(HMODULE handle)
		{
			::dlclose(handle);
		}

		#include <sys/time.h>
		#define _PTHREADS
		//#include <util/fast_mutex.hpp>
		//static fast_mutex tickcount_lock;
	//	static int GetTickCount()
//		{
		//	struct timeval	cur;
		//	tickcount_lock.lock();
		//	gettimeofday(&cur, 0);
		//	tickcount_lock.unlock();
		//	return cur.tv_sec * 1000 +  cur.tv_usec/1000;
	//	}
	#endif
#elif defined(_WIN32)
	#define SSE_MM_ALIGN(x)	__declspec(align(x))
	#define SSE_MM_ALIGN16	__declspec(align(16))
#endif // __GNUC__




#endif // __ISR_DEFINES_H__
