#ifndef _CRYPT_MODE_ECB_H_
#define _CRYPT_MODE_ECB_H_

#include "CryptMode.h"

class ELVUTILS_DLLEXPORT CryptModeECB :
    public CryptMode
{
public:
    CryptModeECB(BlockCipher* cipher, bool encrypt);
    ~CryptModeECB(void);
    void ProcessData(const unsigned char* source, unsigned char* target, unsigned int size);
    unsigned int GetIVSize();
private:
    BlockCipher::Transformer* block_transformer;
};

#endif
