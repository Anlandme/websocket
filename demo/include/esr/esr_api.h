#ifndef __ESR_API_H__
#define __ESR_API_H__

#include "esr_type.h"
#include "esr_err.h"

#if defined(_MSC_VER)
#    define ESR_CALL __stdcall
#    define ESR_ATTR
#else
#    define ESR_CALL
#    define ESR_ATTR __attribute__ ((visibility ("default")))
#endif

#if defined(_MSC_VER)                 /* Microsoft Visual C++ */
#	if !defined(ESRAPI)
#		define ESRAPI ESR_ATTR ESR_CALL
#	endif
#	pragma pack(push, 8)
#else                                          /* Any other including Unix */
#	if !defined(ESRAPI)
#		define ESRAPI ESR_ATTR ESR_CALL
#	endif
#endif


#define API_LIST_ESR(func)\
	func(wEsrInitialize)\
	func(wEsrUninitialize)

#ifdef __cplusplus
extern "C" {
#endif
	int32_t ESRAPI wEsrInitialize(EsrInterface** ppEsrMgr,const char* pserial, const char *pWorkDire);
	typedef int32_t (ESRAPI *Proc_wEsrInitialize)(EsrInterface** ppEsrMgr,const char* pserial, const char* pWorkDir);

	int32_t ESRAPI wEsrUninitialize(EsrInterface* pEsrMgr);
	typedef int32_t (ESRAPI *Proc_wEsrUninitialize)(EsrInterface* pEsrMgr);

#ifdef __cplusplus
};
#endif

#if defined(_MSC_VER)                /* Microsoft Visual C++ */
#	pragma pack(pop)
#endif

#endif /* __SAD_H__ */
