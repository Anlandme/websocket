#ifndef __ESR_TYPE_H__
#define __ESR_TYPE_H__
#include <stdint.h>

struct EsrResSet
{
	int32_t		nResID;
	char		szType[16];
};

typedef enum
{
	ESR_AUDIO_BEGIN = 0,
	ESR_AUDIO_CONTINUE,
	ESR_AUDIO_END,
} ESR_AUDIO_STATUS;

typedef enum
{
	ESR_RESULT_NONE = 0,
	ESR_RESULT_CONTINUE,
	ESR_RESULT_FINISH,
}ESR_RESULT_STATUS;

typedef struct EsrResultElem_
{
	const char*	key;
	const char*   sub_key;
	const char*   value;
	int32_t		    seg_id;
	void*			reserved;
}EsrResultElem;

typedef struct EsrResult_
{
	ESR_RESULT_STATUS	status;
	EsrResultElem*		elems;
	int32_t				elem_count;
	void*				reserved;
}EsrResult;

#if defined(_MSC_VER)
#    define CALL_BACK __stdcall
#else
#    define CALL_BACK
#endif

typedef int (*PESRCallBack)(void* pHandle, uint32_t uMsg, uint32_t wParam, void* lParam);

class EsrInstBase
{
public:
	EsrInstBase(){}
	virtual ~EsrInstBase(){}
	virtual int32_t   wEsrSetParameter(const char* pParam, const char* pParamValue)				= 0;
	virtual int32_t   wEsrGetParameter(const char* pParam, char* pParamValue, int32_t nLen)		= 0;
	virtual int32_t   wEsrStart(const EsrResSet* pResSet, int32_t nRes)							= 0;
	virtual int32_t   wEsrStop()																= 0;
	virtual int32_t   wEsrWrite(const char* pSamples, int32_t nLen, ESR_AUDIO_STATUS status)	= 0;
	virtual int32_t   wEsrGetResult(EsrResult **result, const char *format)						= 0;
	virtual int32_t	  wEsrRegisterCallBack(const char* szFuncType, const PESRCallBack pFunc, void* pUserParam)	= 0;
};
typedef	EsrInstBase*	pEsrInstBase;

class EsrInterface
{
public:
	EsrInterface(){}
	virtual ~EsrInterface(){}
	virtual int32_t   wEsrCreate(pEsrInstBase* pWEsrInst)			= 0;
	virtual int32_t   wEsrDestroy(pEsrInstBase wEsrInst)			= 0;
	virtual int32_t   wEsrResourceAdd(const EsrResSet& resSet, const char* pRes, const char* option, uint32_t offset, uint32_t nLen, const EsrResSet* pDependentResSet, int32_t dependencyResCount) = 0;
	virtual int32_t   wEsrResourceDelete(const EsrResSet& resSet) = 0;
	virtual int32_t   wEsrResourceUpdate(const EsrResSet& resSet,const char* address, uint32_t offset, uint32_t nLen, const char* option)	  = 0;
	virtual int32_t   wEsrResourceSave(const EsrResSet& resSet, char* pDst, int nMaxLen,int& nResSize)										  = 0;
	virtual int32_t   wEsrResourceSetParameter(const EsrResSet& resSet, const char* pParam, const char* pValue)							      = 0;
	virtual int32_t   wEsrResourceGetParameter(const EsrResSet& resSet, const char* pParam, char* pParamValue, int32_t nLen)				  = 0;
	virtual int32_t   wEsrSetParameter(const char* param,const char*value)																	  = 0;
	virtual int32_t   wEsrGetParameter(const char* param, char* paramValue, int32_t len)													  = 0;
private:
	EsrInterface(EsrInterface const&){}           // copy ctor hidden
	EsrInterface& operator=(EsrInterface const&){return *this;}// assign op. hidden	
};
#endif