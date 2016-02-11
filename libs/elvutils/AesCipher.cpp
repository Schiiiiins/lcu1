#include "AesCipher.h"

#include <string.h>

/*static*/ const unsigned char AesCipher::sBox[256]={
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, 
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

/*static*/ const unsigned char AesCipher::sBoxInv[256]={
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, 
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, 
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, 
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, 
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, 
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, 
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, 
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, 
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d 
};

AesCipher::AesCipher(unsigned char* key, unsigned int keysize)
{
    this->key.assign(key, keysize);
    switch(keysize){
        case 16:
        case 24:
        case 32:
            break;
        default:
            this->key.resize(16);
        break;
    }
}

BlockCipher::Transformer* AesCipher::CreateEncryptor()
{
    Encryptor* enc=new Encryptor(key);
    return enc;
}

BlockCipher::Transformer* AesCipher::CreateDecryptor()
{
    Decryptor* dec=new Decryptor(key);
    return dec;
}

AesCipher::Encryptor::Encryptor(const std::basic_string<unsigned char> key)
{
    keysize=key.size();
    rounds=CalcRounds(key.size());
    KeyExpansion(key.c_str());
}

AesCipher::Encryptor::~Encryptor()
{
}

void AesCipher::Encryptor::ProcessData(const unsigned char* source, unsigned char* target)
{
    if(source!=target)memcpy(target, source, BLOCKSIZE);

	unsigned char round = rounds-1;

	XORBytes( target, expandedKey, 16 );
	unsigned char* round_key = expandedKey + BLOCKSIZE;

	do {
		SubBytes( target, 16 );
		ShiftRows( target );
		MixColumns( target );
		XORBytes( target, round_key, 16 );
		round_key += BLOCKSIZE;
	} while( --round );

	SubBytes( target, 16 );
	ShiftRows( target );
	XORBytes( target, round_key, 16 );
}

AesCipher::Decryptor::Decryptor(const std::basic_string<unsigned char> key)
{
    keysize=key.size();
    rounds=CalcRounds(key.size());
    KeyExpansion(key.c_str());
}

AesCipher::Decryptor::~Decryptor()
{
}

void AesCipher::Encryptor::KeyExpansion( const unsigned char * key )
{
	unsigned char temp[4];
	unsigned char Rcon[4] = { 0x01, 0x00, 0x00, 0x00 }; // Round constant.

	// Copy key to start of expanded key.
    memcpy(expandedKey, key, keysize);

	// Prepare last 4 bytes of key in temp.
    memcpy(temp, expandedKey+keysize-4, 4);

	// Expand key.
	for(unsigned char i=keysize; i < BLOCKSIZE*(rounds+1); i+=4 ) {
        unsigned char* p=expandedKey+i;
		// Are we at the start of a multiple of the key size?
		if( (i % keysize) == 0 ) {
			CycleLeft( temp ); // Cycle left once.
			SubBytes( temp, 4 ); // Substitute each byte.
			XORBytes( temp, Rcon, 4 ); // Add constant in GF(2).
			*Rcon = Multiply( *Rcon, 0x02 ); // First byte in Rcon *= 2 in GF(2^8).
		}

		// Keysize larger than 24 bytes, ie. larger that 192 bits?
		// Are we right past a block size?
		else if( keysize > 24 && (i % keysize) == BLOCKSIZE ) {
			SubBytes( temp, 4 ); // Substitute each byte.
		}

		// Add bytes in GF(2) one KEYLENGTH away.
		XORBytes( temp, p - keysize, 4 );

		// Copy result to current 4 bytes.
        memcpy(p, temp, 4);
	}	
}


void AesCipher::Decryptor::KeyExpansion( const unsigned char * key )
{
	unsigned char temp[4];
	unsigned char Rcon[4] = { 0x01, 0x00, 0x00, 0x00 }; // Round constant.
	
	// Copy key to start of expanded key.
    memcpy(expandedKey, key, keysize);

	// Prepare last 4 bytes of key in temp.
    memcpy(temp, expandedKey+keysize+4, 4);

	// Expand key.
	for(unsigned char i=keysize; i < BLOCKSIZE*(rounds+1); i+=4 ) {
        unsigned char* p=expandedKey+i;
		// Are we at the start of a multiple of the key size?
		if( (i % keysize) == 0 ) {
			CycleLeft( temp ); // Cycle left once.
			SubBytes( temp, 4 ); // Substitute each byte.
			XORBytes( temp, Rcon, 4 ); // Add constant in GF(2).
			*Rcon = (*Rcon << 1) ^ (*Rcon & 0x80 ? BPOLY : 0);
		}

		// Are we right past a block size?
		else if( keysize > 24 && (i % keysize) == BLOCKSIZE ) {
			SubBytes( temp, 4 ); // Substitute each byte.
		}

		// Add bytes in GF(2) one KEYLENGTH away.
		XORBytes( temp, expandedKey - keysize, 4 );

		// Copy result to current 4 bytes.
        memcpy(p, temp, 4);
	}	
}

void AesCipher::Decryptor::ProcessData(const unsigned char* source, unsigned char* target)
{
    if(source!=target)memcpy(target, source, BLOCKSIZE);

	unsigned char round = rounds-1;
	unsigned char* round_key = expandedKey + BLOCKSIZE * rounds;

	XORBytes( target, round_key, 16 );
	round_key -= BLOCKSIZE;

	do {
		InvShiftRows( target );
		InvSubBytesAndXOR( target, round_key, 16 );
		round_key -= BLOCKSIZE;
		InvMixColumns( target );
	} while( --round );

	InvShiftRows( target );
	InvSubBytesAndXOR( target, round_key, 16 );
}

void AesCipher::CycleLeft( unsigned char * row )
{
	// Cycle 4 bytes in an array left once.
	unsigned char temp = row[0];
	row[0] = row[1];
	row[1] = row[2];
	row[2] = row[3];
	row[3] = temp;
}


void AesCipher::MixColumn( unsigned char * column )
{
	unsigned char row[8] = {
		0x02, 0x03, 0x01, 0x01,
		0x02, 0x03, 0x01, 0x01
	}; // Prepare first row of matrix twice, to eliminate need for cycling.

	unsigned char result[4];
	
	// Take dot products of each matrix row and the column vector.
	result[0] = DotProduct( row+0, column );
	result[1] = DotProduct( row+3, column );
	result[2] = DotProduct( row+2, column );
	result[3] = DotProduct( row+1, column );

	// Copy temporary result to original column.
	column[0] = result[0];
	column[1] = result[1];
	column[2] = result[2];
	column[3] = result[3];
}


void AesCipher::InvMixColumn( unsigned char * column )
{
	unsigned char r0, r1, r2, r3;

	r0 = column[1] ^ column[2] ^ column[3];
	r1 = column[0] ^ column[2] ^ column[3];
	r2 = column[0] ^ column[1] ^ column[3];
	r3 = column[0] ^ column[1] ^ column[2];

 	column[0] = (column[0] << 1) ^ (column[0] & 0x80 ? BPOLY : 0);
 	column[1] = (column[1] << 1) ^ (column[1] & 0x80 ? BPOLY : 0);
 	column[2] = (column[2] << 1) ^ (column[2] & 0x80 ? BPOLY : 0);
 	column[3] = (column[3] << 1) ^ (column[3] & 0x80 ? BPOLY : 0);

	r0 ^= column[0] ^ column[1];
	r1 ^= column[1] ^ column[2];
	r2 ^= column[2] ^ column[3];
	r3 ^= column[0] ^ column[3];

 	column[0] = (column[0] << 1) ^ (column[0] & 0x80 ? BPOLY : 0);
 	column[1] = (column[1] << 1) ^ (column[1] & 0x80 ? BPOLY : 0);
 	column[2] = (column[2] << 1) ^ (column[2] & 0x80 ? BPOLY : 0);
 	column[3] = (column[3] << 1) ^ (column[3] & 0x80 ? BPOLY : 0);

	r0 ^= column[0] ^ column[2];
	r1 ^= column[1] ^ column[3];
	r2 ^= column[0] ^ column[2];
	r3 ^= column[1] ^ column[3];

 	column[0] = (column[0] << 1) ^ (column[0] & 0x80 ? BPOLY : 0);
 	column[1] = (column[1] << 1) ^ (column[1] & 0x80 ? BPOLY : 0);
 	column[2] = (column[2] << 1) ^ (column[2] & 0x80 ? BPOLY : 0);
 	column[3] = (column[3] << 1) ^ (column[3] & 0x80 ? BPOLY : 0);

	column[0] ^= column[1] ^ column[2] ^ column[3];
	r0 ^= column[0];
	r1 ^= column[0];
	r2 ^= column[0];
	r3 ^= column[0];

	column[0] = r0;
	column[1] = r1;
	column[2] = r2;
	column[3] = r3;
}



void AesCipher::SubBytes( unsigned char * bytes, unsigned char count )
{
	do {
		*bytes = sBox[ *bytes ]; // Substitute every byte in state.
		bytes++;
	} while( --count );
}


void AesCipher::InvSubBytesAndXOR( unsigned char * bytes, unsigned char * key, unsigned char count )
{
	do {
		*bytes = sBoxInv[ *bytes ] ^ *key; // Inverse substitute every byte in state and add key.
		bytes++;
		key++;
	} while( --count );
}



void AesCipher::ShiftRows( unsigned char * state )
{
	unsigned char temp;

	// Note: State is arranged column by column.

	// Cycle second row left one time.
	temp = state[ 1 + 0*4 ];
	state[ 1 + 0*4 ] = state[ 1 + 1*4 ];
	state[ 1 + 1*4 ] = state[ 1 + 2*4 ];
	state[ 1 + 2*4 ] = state[ 1 + 3*4 ];
	state[ 1 + 3*4 ] = temp;

	// Cycle third row left two times.
	temp = state[ 2 + 0*4 ];
	state[ 2 + 0*4 ] = state[ 2 + 2*4 ];
	state[ 2 + 2*4 ] = temp;
	temp = state[ 2 + 1*4 ];
	state[ 2 + 1*4 ] = state[ 2 + 3*4 ];
	state[ 2 + 3*4 ] = temp;

	// Cycle fourth row left three times, ie. right once.
	temp = state[ 3 + 3*4 ];
	state[ 3 + 3*4 ] = state[ 3 + 2*4 ];
	state[ 3 + 2*4 ] = state[ 3 + 1*4 ];
	state[ 3 + 1*4 ] = state[ 3 + 0*4 ];
	state[ 3 + 0*4 ] = temp;
}


void AesCipher::InvShiftRows( unsigned char * state )
{
	unsigned char temp;

	// Note: State is arranged column by column.

	// Cycle second row right one time.
	temp = state[ 1 + 3*4 ];
	state[ 1 + 3*4 ] = state[ 1 + 2*4 ];
	state[ 1 + 2*4 ] = state[ 1 + 1*4 ];
	state[ 1 + 1*4 ] = state[ 1 + 0*4 ];
	state[ 1 + 0*4 ] = temp;

	// Cycle third row right two times.
	temp = state[ 2 + 0*4 ];
	state[ 2 + 0*4 ] = state[ 2 + 2*4 ];
	state[ 2 + 2*4 ] = temp;
	temp = state[ 2 + 1*4 ];
	state[ 2 + 1*4 ] = state[ 2 + 3*4 ];
	state[ 2 + 3*4 ] = temp;

	// Cycle fourth row right three times, ie. left once.
	temp = state[ 3 + 0*4 ];
	state[ 3 + 0*4 ] = state[ 3 + 1*4 ];
	state[ 3 + 1*4 ] = state[ 3 + 2*4 ];
	state[ 3 + 2*4 ] = state[ 3 + 3*4 ];
	state[ 3 + 3*4 ] = temp;
}

void AesCipher::MixColumns( unsigned char * state )
{
	MixColumn( state + 0*4 );
	MixColumn( state + 1*4 );
	MixColumn( state + 2*4 );
	MixColumn( state + 3*4 );
}

void AesCipher::InvMixColumns( unsigned char * state )
{
	InvMixColumn( state + 0*4 );
	InvMixColumn( state + 1*4 );
	InvMixColumn( state + 2*4 );
	InvMixColumn( state + 3*4 );
}

void AesCipher::XORBytes( unsigned char * bytes1, unsigned char * bytes2, unsigned char count )
{
	do {
		*bytes1 ^= *bytes2; // Add in GF(2), ie. XOR.
		bytes1++;
		bytes2++;
	} while( --count );
}



void AesCipher::CopyBytes( unsigned char * to, unsigned char * from, unsigned char count )
{
	do {
		*to = *from;
		to++;
		from++;
	} while( --count );
}



unsigned char AesCipher::Multiply( unsigned char num, unsigned char factor )
{
	unsigned char mask = 1;
	unsigned char result = 0;

	while( mask != 0 ) {
		// Check bit of factor given by mask.
		if( mask & factor ) {
			// Add current multiple of num in GF(2).
			result ^= num;
		}

		// Shift mask to indicate next bit.
		mask <<= 1;

		// Double num.
       		num = (num << 1) ^ (num & 0x80 ? BPOLY : 0);
	}

	return result;
}



unsigned char AesCipher::DotProduct( unsigned char * vector1, unsigned char * vector2 )
{
	unsigned char result = 0;

	result ^= Multiply( *vector1++, *vector2++ );
	result ^= Multiply( *vector1++, *vector2++ );
	result ^= Multiply( *vector1++, *vector2++ );
	result ^= Multiply( *vector1  , *vector2   );

	return result;
}

unsigned char AesCipher::CalcRounds( unsigned char keysize )
{
    switch(keysize)
    {
    case 16:
        return 10;
    case 24:
        return 12;
    case 32:
        return 14;
    default:
        return 0;
    }
}

