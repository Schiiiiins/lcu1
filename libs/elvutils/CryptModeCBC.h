#ifndef _CRYPT_MODE_CBC_H_
#define _CRYPT_MODE_CBC_H_

#include "CryptMode.h"

class ELVUTILS_DLLEXPORT CryptModeCBC :
    public CryptMode
{
public:
    CryptModeCBC(BlockCipher* cipher, bool encrypt, const unsigned char* iv);
    ~CryptModeCBC(void);
    void Resync(const unsigned char* iv);
protected:
    void Decrypt(const unsigned char* source, unsigned char* target, unsigned int size);
    void Encrypt(const unsigned char* source, unsigned char* target, unsigned int size);
private:
    BlockType feedback_buffer;
    BlockCipher::Transformer* block_transformer;
};

#endif
