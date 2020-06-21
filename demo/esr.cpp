#include <stdio.h>
#include <string.h>
#include <iostream>

#include "include/esr/esr_api.h"
#include "include/esr/isr_defines.h"


using namespace std;

#define HMODULE void*

char EsrLicense[] = "5760ca771900a880";

#define CHECK_FUNC_IF_SUCCESS(exp,func) if(!exp)\
{\
	printf("%s Failed!\n",#func);\
	return -1;\
}

const char *w_esr_so="./esr_resource/w_esr.so";
//const char *wfstRes="./esr_resource/wfst.bin";
const char *wfstRes="./esr_resource/RSH006_7G_20190910.bin";
const char *acmodelRes="./esr_resource/shengxue.bin";
const char *vadRes="./esr_resource/vad.bin";
const char *fsaRes="./esr_resource/fsa.txt";
const char *hotRes="./esr_resource/hot.txt";

Proc_wEsrInitialize			wEsrInitialize_			= NULL;
Proc_wEsrUninitialize		wEsrUninitialize_		= NULL;

EsrInterface* pEsrMgr = NULL;
EsrResult* rec_result = NULL;

#define LEN_ESR 5
EsrResSet resSet[LEN_ESR]={{0,"MLP_XN"},{0,"WFST"},{0,"MLP_VAD"},{0,"FSA"},{0,"PERSONAL"}};	
EsrResSet decSet[LEN_ESR]={{0,"MLP_XN"},{0,"WFST"},{0,"MLP_VAD"},{0,"FSA"},{0,"PERSONAL"}};
int decReslen = LEN_ESR;


unsigned int get_file_length(const char* file)
{
	unsigned int ret = 0;
	int nsize = 0;
	FILE * pTest = fopen(file,"r");
	if(pTest != NULL)
	{
		fseek(pTest, 0, SEEK_END);
		nsize = ftell(pTest);
		fclose(pTest);
	}
	if (nsize >= 0)
	{
		ret = nsize;
	}
	return ret;
}

int ESRInit()
{
	HMODULE hand = LoadLibraryA("./esr_resource/w_esr.so");
	if (hand == NULL)
	{
		printf("<%s><%s:%d>error!\n",__FILE__,__FUNCTION__,__LINE__);
		return -1;

	}	
	wEsrInitialize_		= (Proc_wEsrInitialize)     GetProcAddress(hand,"wEsrInitialize");
	if (wEsrInitialize_ == NULL)
	{
		printf("wEsrInitialize_ is NULL err\n");
		return -1;
	}
	wEsrUninitialize_	= (Proc_wEsrUninitialize)	GetProcAddress(hand,"wEsrUninitialize");
	if (wEsrUninitialize_ == NULL)
	{
		printf("wEsrUninitialize_ is NULL err\n");
		return -1;
	}
	//int ret = wEsrInitialize_(&pEsrMgr,"8d3f4d444bed54ec",NULL);	
	int ret = wEsrInitialize_(&pEsrMgr,EsrLicense,NULL);
	if(ret==ESR_ERROR_INVALID_PARA_VALUE)
	{
		printf("err code,wrong serial number!\n");
		return -1;
	}
	char versionStr[16] = {0};



	pEsrMgr->wEsrGetParameter("wesr_param_engine_version", versionStr, sizeof(versionStr));
	printf("Engine Version:%s\n",versionStr);
	pEsrMgr->wEsrSetParameter("wdec_param_decoderNetType","wfst_fsa");//"wfst_fsa"
	CHECK_FUNC_IF_SUCCESS(ret == ESR_SUCCESS,wEsrInitialize_);







	unsigned int res_size = get_file_length(acmodelRes);
	ret =pEsrMgr->wEsrResourceAdd(resSet[0],acmodelRes,"FromFile",0,res_size,NULL,0);
	CHECK_FUNC_IF_SUCCESS(ret == ESR_SUCCESS,wEsrResourceAdd);
	printf("ResAdd ACmodel success\n");

	res_size = get_file_length(wfstRes);
	ret =pEsrMgr->wEsrResourceAdd(resSet[1],wfstRes,"FromFileNonLoad",0,res_size,NULL,0);
	CHECK_FUNC_IF_SUCCESS(ret == ESR_SUCCESS,wEsrResourceAdd);	
	printf("ResAdd wfst success\n");

	res_size = get_file_length(vadRes);
	ret =pEsrMgr->wEsrResourceAdd(resSet[2],vadRes,"FromFile",0,res_size,NULL,0);
	CHECK_FUNC_IF_SUCCESS(ret == ESR_SUCCESS,wEsrResourceAdd);
	printf("ResAdd vad success\n");


	res_size = get_file_length(fsaRes);
	ret =pEsrMgr->wEsrResourceAdd(resSet[3],fsaRes,"FromFile",0,res_size,&resSet[1],1);
	CHECK_FUNC_IF_SUCCESS(ret == ESR_SUCCESS,wEsrResourceAdd);
	res_size = get_file_length(hotRes);
	ret =pEsrMgr->wEsrResourceAdd(resSet[4],hotRes,"FromFile",0,res_size,&resSet[1],1);
	CHECK_FUNC_IF_SUCCESS(ret == ESR_SUCCESS,wEsrResourceAdd);

	pEsrMgr->wEsrSetParameter("wesr_param_bVadOn","0");
	CHECK_FUNC_IF_SUCCESS(ret == ESR_SUCCESS,wEsrSetParameter);

	return 0;
}

int create_esr(pEsrInstBase* esrinst)
{
	return pEsrMgr->wEsrCreate(esrinst);
}

int start_esr(pEsrInstBase esrinst)
{
	return esrinst->wEsrStart(decSet,decReslen-1);
}

int stop_esr(pEsrInstBase esrinst)
{
	return esrinst->wEsrStop();
}

int write_esr(pEsrInstBase esrinst,const char* audio,int len,ESR_AUDIO_STATUS status)
{
	return esrinst->wEsrWrite(audio,len,status);
}

int read_esr(pEsrInstBase esrinst,std::string &result)
{
	EsrResult* rec_result;

	//esrinst->wEsrGetResult( &rec_result, "pgs,plain,readable,htk");
	//esrinst->wEsrGetResult( &rec_result, "pgs,plain");
	//esrinst->wEsrGetResult( &rec_result, "pgs,plain");
	esrinst->wEsrGetResult( &rec_result, "plain");

	if( rec_result->elem_count)
	{
#if 0
		printf("count:----------------------->%d\n",rec_result->elem_count);
		int j = 0;
		for (j = 0;j < rec_result->elem_count ; j++)
		{
#if 1
			printf("value: %s\n", rec_result->elems[j].value);
			printf("key: %s\n", rec_result->elems[j].key);
			printf("sub key: %s\n", rec_result->elems[j].sub_key);
			printf("status: %d\n", rec_result->status);
#endif
			result.append(rec_result->elems[j].value);

			//if(0 == strcmp("plain",rec_result->elems[j].sub_key) && ESR_RESULT_FINISH  == rec_result->status)
		}
#endif

		result.append(rec_result->elems[rec_result->elem_count-1].value);
	}

	return rec_result->status;

	//return value:
	//ESR_RESULT_NONE   0,
	//ESR_RESULT_CONTINUE 1
	//ESR_RESULT_FINISH  2
}


