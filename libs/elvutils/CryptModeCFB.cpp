#include "CryptModeCFB.h"
#include <string.h>


CryptModeCFB::CryptModeCFB(BlockCipher* cipher, bool encrypt, const unsigned char* iv):CryptMode(cipher, encrypt)
{
    block_transformer=cipher->CreateEncryptor();
    Resync(iv);
}

CryptModeCFB::~CryptModeCFB(void)
{
    if(block_transformer)delete block_transformer;
}

unsigned int CryptModeCFB::GetChunkGranularity()
{
    return 1;
}

void CryptModeCFB::Resync(const unsigned char* iv)
{
    feedback_buffer.assign(iv, cipher->GetBlockSize());
    cur_block_bytes_left=0;
}

void CryptModeCFB::Encrypt(const unsigned char* source, unsigned char* target, unsigned int size)
{
    unsigned int blocksize=cipher->GetBlockSize();
    unsigned char* temp_buffer=new unsigned char[blocksize];
	while(size)
	{
	    unsigned short count;
		if(!cur_block_bytes_left){
            block_transformer->ProcessData(feedback_buffer.c_str(), temp_buffer);
            feedback_buffer.assign(temp_buffer, blocksize);
            cur_block_bytes_left=blocksize;
        }else{
            memcpy(temp_buffer, feedback_buffer.data()+blocksize-cur_block_bytes_left, cur_block_bytes_left);
        }
	    count=cur_block_bytes_left;
		if(size<count)count=size;
        memcpy(target, source, count);
		XorBytes( target, temp_buffer, count );

		// we now have the ciphertext in target, put the ciphertext into the feedback_buffer
        feedback_buffer.replace( blocksize - cur_block_bytes_left, count, target, count);
		source += count;
        target += count;
		size -= count;
		cur_block_bytes_left-=count;
	}
    delete[] temp_buffer;
}

void CryptModeCFB::Decrypt(const unsigned char* source, unsigned char* target, unsigned int size)
{
    unsigned int blocksize=cipher->GetBlockSize();
    unsigned char* temp_buffer=new unsigned char[blocksize];
	while(size)
	{
	    unsigned short count;
		if(!cur_block_bytes_left){
            block_transformer->ProcessData(feedback_buffer.c_str(), temp_buffer);
            feedback_buffer.assign(temp_buffer, blocksize);
            cur_block_bytes_left=blocksize;
        }else{
            memcpy(temp_buffer, feedback_buffer.data()+blocksize-cur_block_bytes_left, cur_block_bytes_left);
        }
	    count=cur_block_bytes_left;
		if(size<count)count=size;
		XorBytes( temp_buffer, source, count );
		// we now have the plaintext in temp_buffer, put the ciphertext into the feedback_buffer
        feedback_buffer.replace( blocksize - cur_block_bytes_left, count, source, count);
        // OK, now we may copy the plaintext to the target buffer
		memcpy( target, temp_buffer, count );
		source += count;
        target += count;
		size -= count;
		cur_block_bytes_left-=count;
	}
    delete[] temp_buffer;
}
