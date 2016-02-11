#include "CryptModeCBC.h"
#include <string.h>

CryptModeCBC::CryptModeCBC(BlockCipher* cipher, bool encrypt, const unsigned char* iv):CryptMode(cipher, encrypt)
{
    if(encrypt)block_transformer=cipher->CreateEncryptor();
    else block_transformer=cipher->CreateDecryptor();
    Resync(iv);
}

CryptModeCBC::~CryptModeCBC(void)
{
    if(block_transformer)delete block_transformer;
}

void CryptModeCBC::Resync(const unsigned char* iv)
{
    feedback_buffer.assign(iv, cipher->GetBlockSize());
}

void CryptModeCBC::Encrypt(const unsigned char* source, unsigned char* target, unsigned int size)
{
    unsigned int chunk_size=cipher->GetBlockSize();
    while(size>=chunk_size){
        memcpy(target, source, chunk_size);
        XorBytes(target, feedback_buffer.c_str(), chunk_size);
        block_transformer->ProcessData(target, target);
        feedback_buffer.assign(target, chunk_size);
        source+=chunk_size;
        target+=chunk_size;
        size-=chunk_size;
    }
}

void CryptModeCBC::Decrypt(const unsigned char* source, unsigned char* target, unsigned int size)
{
    unsigned int chunk_size=cipher->GetBlockSize();
    while(size>=chunk_size){
        feedback_buffer.assign(source, chunk_size);
        block_transformer->ProcessData(source, target);
        XorBytes(target, feedback_buffer.c_str(), chunk_size);
        source+=chunk_size;
        target+=chunk_size;
        size-=chunk_size;
    }
}
