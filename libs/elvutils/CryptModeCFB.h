#ifndef _CRYPT_MODE_CFB_H_
#define _CRYPT_MODE_CFB_H_

#include "CryptMode.h"

class ELVUTILS_DLLEXPORT CryptModeCFB :
    public CryptMode
{
public:
    CryptModeCFB(BlockCipher* cipher, bool encrypt, const unsigned char* iv);
    ~CryptModeCFB(void);
    unsigned int GetChunkGranularity();
    void Resync(const unsigned char* iv);
protected:
    void Decrypt(const unsigned char* source, unsigned char* target, unsigned int size);
    void Encrypt(const unsigned char* source, unsigned char* target, unsigned int size);
private:
    BlockCipher::Transformer* block_transformer;
    unsigned int cur_block_bytes_left;
    BlockType feedback_buffer;
};

#endif
