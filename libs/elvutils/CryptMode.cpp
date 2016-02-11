#include "CryptMode.h"

CryptMode::CryptMode(BlockCipher* cipher, bool encrypt):cipher(cipher)
{
    this->encrypt=encrypt;
}

CryptMode::~CryptMode(void)
{
    delete cipher;
}

unsigned int CryptMode::GetChunkGranularity()
{
    return cipher->GetBlockSize();
}

void CryptMode::Resync(const unsigned char* iv)
{
}

unsigned int CryptMode::GetIVSize()
{
    return cipher->GetBlockSize();
}

void CryptMode::ProcessData(const unsigned char* source, unsigned char* target, unsigned int size)
{
    if(encrypt)Encrypt(source, target, size);
    else Decrypt(source, target, size);
}

/*static*/ void CryptMode::XorBytes(unsigned char* bytes1, const unsigned char * bytes2, unsigned int count )
{
	do {
		*bytes1 ^= *bytes2;
		bytes1++;
		bytes2++;
	} while( --count );
}


