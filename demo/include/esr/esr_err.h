#ifndef __WESR_ERROR_H__
#define __WESR_ERROR_H__
#include "errcode_module.h"

enum
{
	/* Generic Error defines */
	ESR_SUCCESS							=SUCCESS,
	/* Common errors */
	ESR_ERROR_GENERAL						= ERROR_W_ESR + 1,
	ESR_ERROR_NOT_INIT,
	ESR_ERROR_ALREADY_START,
	ESR_ERROR_NOT_START,
	ESR_ERROR_RESOURCE_NOT_EXIST,
	ESR_ERROR_INVALID_PARA,
	ESR_ERROR_INVALID_PARA_VALUE,
	ESR_ERROR_NULL_HANDLE,
	ESR_ERROR_BUF_NOT_ENOUGH,
	ESR_ERROR_RES_MISMATCH,
};
#endif	//__WESR_ERROR_H__