#ifndef _AES_CIPHER_H_
#define _AES_CIPHER_H_

#include "BlockCipher.h"
#include <string>

class ELVUTILS_DLLEXPORT AesCipher : public BlockCipher
{
private:
    enum{
        BLOCKSIZE=16,
        BPOLY=0x1b            //!< Lower 8 bits of (x^8+x^4+x^3+x+1), ie. (x^4+x^3+x+1).
    };

    static void CycleLeft( unsigned char * row );
    static void MixColumn( unsigned char * column );
    static void InvMixColumn( unsigned char * column );
    static void SubBytes( unsigned char * bytes, unsigned char count );
    static void InvSubBytesAndXOR( unsigned char * bytes, unsigned char * key, unsigned char count );
    static void ShiftRows( unsigned char * state );
    static void InvShiftRows( unsigned char * state );
    static void MixColumns( unsigned char * state );
    static void InvMixColumns( unsigned char * state );
    static void XORBytes( unsigned char * bytes1, unsigned char * bytes2, unsigned char count );
    static void CopyBytes( unsigned char * to, unsigned char * from, unsigned char count );
    static unsigned char Multiply( unsigned char num, unsigned char factor );
    static unsigned char DotProduct( unsigned char * vector1, unsigned char * vector2 );
    static unsigned char CalcRounds(unsigned char keysize);

    static const unsigned char sBox[256];
    static const unsigned char sBoxInv[256];
    std::basic_string<unsigned char> key;

public:

    AesCipher(unsigned char* key, unsigned int keysize);
    BlockCipher::Transformer* CreateEncryptor();
    BlockCipher::Transformer* CreateDecryptor();

    inline unsigned int GetBlockSize(){return BLOCKSIZE;};

    class ELVUTILS_DLLEXPORT Encryptor : public BlockCipher::Transformer
    {
    public:
        Encryptor(std::basic_string<unsigned char> key);
        virtual ~Encryptor();
        void ProcessData(const unsigned char* source, unsigned char* target);
    private:
        void KeyExpansion( const unsigned char* key );
        unsigned char expandedKey[256];
        unsigned char rounds;
        unsigned int keysize;
    };

    class ELVUTILS_DLLEXPORT Decryptor : public BlockCipher::Transformer
    {
    public:
        Decryptor(std::basic_string<unsigned char> key);
        virtual ~Decryptor();
        void ProcessData(const unsigned char* source, unsigned char* target);
        inline unsigned int GetBlockSize(){return BLOCKSIZE;};
    private:
        void KeyExpansion( const unsigned char* key );
        unsigned char expandedKey[256];
        unsigned char rounds;
        unsigned int keysize;
    };
};

#endif
