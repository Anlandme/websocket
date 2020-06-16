#include "include/esr/esr_api.h"


typedef struct connection_info_s
{
	char ip[20];
	char res[100];
	pEsrInstBase esr_inst;
}connection_info_t;

