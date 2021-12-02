#include <aes/esp_aes.h>
#include <esp_err.h>
#include <stdio.h>
#include <string.h>

#include "app.h"

/*
  For  Encryption time: 1802.40us  (9.09 MB/s) at 16kB blocks.
*/

static inline int32_t _getCycleCount(void) {
  int32_t ccount;
  __asm__ __volatile__("rsr %0,ccount":"=a" (ccount));
  return ccount;
}

char plaintext[16384];
char encrypted[16384];


void encodetest(int16_t *input, int16_t *output, size_t size)
{
	uint8_t key[32] = "(H+MbQeThWmZq4t7w!z%C*F-J@NcRfUj";
	uint8_t iv[16];
	/* uint16_t *value = (uint16_t *)data; */

	/* for (size_t i = 0; i < size; i++) */
	/* { */
	/* 	printf("buffer song data = %d\n%u sur size = %u\n", (uint16_t)value[i], i, size); */
	/* } */


  //If you have cryptographically random data in the start of your payload, you do not need
	//an IV.  If you start a plaintext payload, you will need an IV.
	memset( iv, 0, sizeof( iv ) );

	//Right now, I am using a key of all zeroes.  This should change.  You should fill the key
  //out with actual data.
	/* memset( key, 0, sizeof( key ) ); */

	/* memset( plaintext, 0, sizeof( plaintext ) ); */
	/* strcpy( plaintext, "Hello, world, how are you doing today?" ); */

	//Just FYI - you must be encrypting/decrypting data that is in BLOCKSIZE chunks!!!

	esp_aes_context ctx;
	esp_aes_init( &ctx );
	esp_aes_setkey( &ctx, key, 256 );
	int32_t start = _getCycleCount();
	esp_aes_crypt_cbc( &ctx, ESP_AES_ENCRYPT, size, iv, (uint8_t*)input, (uint8_t*)output );
	int32_t end = _getCycleCount();
	float enctime = (end-start)/240.0;
	printf( "Encryption time: %.2fus  (%f MB/s)\n", enctime, (size*1.0)/enctime );

	//See encrypted payload, and wipe out plaintext.
	memset( input, 0, size );
	/* int i; */
	/* for( i = 0; i < 128; i++ ) */
	/* { */
	/* 	printf( "%02x[%c]%c", output[i], (output[i]>31)?output[i]:' ', ((i&0xf)!=0xf)?' ':'\n' ); */
	/* } */
	/* printf( "\n" ); */
	//Must reset IV.
	//XXX TODO: Research further: I found out if you don't reset the IV, the first block will fail
	//but subsequent blocks will pass.  Is there some strange cryptoalgebra going on that permits this?
	printf( "IV: %02x %02x\n", iv[0], iv[1] );
	memset( iv, 0, sizeof( iv ) );


	//Use the ESP32 to decrypt the CBC block.
	esp_aes_crypt_cbc( &ctx, ESP_AES_DECRYPT, size, iv, (uint8_t*)output, (uint8_t*)input );


	//Verify output
	/* for( i = 0; i < 128; i++ ) */
	/* { */
	/* 	printf( "%02x[%c]%c", input[i],  (input[i]>31)?input[i]:' ', ((i&0xf)!=0xf)?' ':'\n' ); */
	/* } */
	/* printf( "\n" ); */


	esp_aes_free( &ctx );
}
