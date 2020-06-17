#ifndef __ESR_H__
#define __ESR_H__

extern int ESRInit();
extern int create_esr(pEsrInstBase* esrinst);
extern int start_esr(pEsrInstBase esrinst);
extern int write_esr(pEsrInstBase esrinst,const char* audio,int len,ESR_AUDIO_STATUS status);
extern int read_esr(pEsrInstBase esrinst,std::string &result);


#endif
