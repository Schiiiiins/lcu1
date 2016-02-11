#ifndef _BLOCKCIPHER_H_
#define _BLOCKCIPHER_H_

#include "dllexport.h"

class ELVUTILS_DLLEXPORT BlockCipher
{
public:
    class ELVUTILS_DLLEXPORT Transformer
    {
    public:
        Transformer();
        virtual ~Transformer();
        virtual void ProcessData(const unsigned char* source, unsigned char* target)=0;
    };

    BlockCipher(void);
    virtual ~BlockCipher(void);
    virtual unsigned int GetBlockSize()=0;
    virtual BlockCipher::Transformer* CreateEncryptor()=0;
    virtual BlockCipher::Transformer* CreateDecryptor()=0;
};

#endif
