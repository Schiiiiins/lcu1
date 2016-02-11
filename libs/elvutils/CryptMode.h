#ifndef _CRYPT_MODE_H_
#define _CRYPT_MODE_H_

#include "dllexport.h"
#include "BlockCipher.h"
#include <string>

class ELVUTILS_DLLEXPORT CryptMode
{
public:
    typedef std::basic_string<unsigned char> BlockType;
    CryptMode(BlockCipher* cipher, bool encrypt);
    virtual ~CryptMode(void);
    virtual unsigned int GetChunkGranularity();
    virtual void ProcessData(const unsigned char* source, unsigned char* target, unsigned int size);
    virtual unsigned int GetIVSize();
    virtual void Resync(const unsigned char* iv);
protected:
    virtual void Encrypt(const unsigned char* source, unsigned char* target, unsigned int size){};
    virtual void Decrypt(const unsigned char* source, unsigned char* target, unsigned int size){};
    static void XorBytes(unsigned char* bytes1, const unsigned char * bytes2, unsigned int count );
    BlockCipher* cipher;
    bool encrypt;
};

#endif
