#include "CryptModeECB.h"

CryptModeECB::CryptModeECB(BlockCipher* cipher, bool encrypt):CryptMode(cipher, encrypt)
{
    if(encrypt)block_transformer=cipher->CreateEncryptor();
    else block_transformer=cipher->CreateDecryptor();
}

CryptModeECB::~CryptModeECB(void)
{
    if(block_transformer)delete block_transformer;
}

void CryptModeECB::ProcessData(const unsigned char* source, unsigned char* target, unsigned int size)
{
    unsigned int chunk_size=cipher->GetBlockSize();
    while(size>=chunk_size){
        block_transformer->ProcessData(source, target);
        source+=chunk_size;
        target+=chunk_size;
        size-=chunk_size;
    }
}

unsigned int CryptModeECB::GetIVSize()
{
    return 0;
}
