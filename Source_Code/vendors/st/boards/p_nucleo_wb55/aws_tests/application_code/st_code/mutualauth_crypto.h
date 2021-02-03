/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MUTUALAUTH_CRYPTO_H
#define __MUTUALAUTH_CRYPTO_H

#include "optiga/optiga_crypt.h"
///size of public key for NIST-P256
#define LENGTH_PUB_KEY_NISTP256     65
#define CRC_VERIFIED_PASS 1
#define CRC_VERIFIED_FAIL -1
#define PUBLIC_KEY_HEADER_SIZE		3

#define BLE_MUTUAL_AUTH_SERVICES    ( 0 )

typedef enum
{
  DEVICE_CERTIFICATE = 1,
  DEVICE_PUBKEY,
  TOKEN,
  SIGN_TOKEN,
  ECDH_PUBKEY,
  ECDH_SIGN,
  ECDH_SECRET,
} MUTUALAUTH_param_t;

typedef struct
{
  uint8_t random_data_buffer[16];
  uint16_t  Length;
}Random_Data_t;

typedef struct
{
  uint8_t public_key [LENGTH_PUB_KEY_NISTP256 + PUBLIC_KEY_HEADER_SIZE];
  uint16_t  Length;
  optiga_key_id_t optiga_key_id;
  uint8_t key_usage;
}ECC_Pubkey_t;

typedef struct
{
  uint8_t signature [80];
  uint8_t digest [32];
  optiga_key_id_t optiga_key_id;
  uint16_t signature_length;
}ECC_Sign_t;

typedef struct
{
  uint8_t shared_secret [32];
  uint8_t peer_public_key [LENGTH_PUB_KEY_NISTP256 + PUBLIC_KEY_HEADER_SIZE];
  uint8_t key_type;
  optiga_key_id_t optiga_key_id;
}ECDH_Secret_t;

typedef struct
{
  uint8_t public_key [LENGTH_PUB_KEY_NISTP256 + PUBLIC_KEY_HEADER_SIZE];
  uint8_t signature [80];
  uint16_t sig_length;
  uint8_t digest [32];
}ECC_Verify_t;

typedef struct
{
  uint8_t *data_to_hash;
  uint32_t  Length;
  uint8_t digest [32];
}Hash_Data_t;

typedef struct
{
  uint8_t * pPayload;
  uint16_t  Length;
}MUTUALAUTH_Edge_Data_t;

const uint8_t ecc_pubkey_header[] =
{
		//BitString Format
		0x03,
		//Remaining length
		0x42,
		//unused bits
		0x00,
};
const uint8_t ca_certificate[] = {0};
#if 0 /*production CA */
const uint8_t ca_certificate[] = {
		0x30,0x82,0x02,0x0a,0x30,0x82,0x01,0xaf,0xa0,0x03,0x02,0x01,0x02,0x02,0x14,0x17,
		0x9a,0x31,0x5e,0xd8,0x0f,0x52,0x2a,0x72,0x1b,0xda,0x36,0xf9,0x80,0x1a,0x17,0xbc,
		0xa6,0x06,0xb6,0x30,0x0a,0x06,0x08,0x2a,0x86,0x48,0xce,0x3d,0x04,0x03,0x02,0x30,
		0x5a,0x31,0x0b,0x30,0x09,0x06,0x03,0x55,0x04,0x06,0x13,0x02,0x49,0x4e,0x31,0x10,
		0x30,0x0e,0x06,0x03,0x55,0x04,0x08,0x0c,0x07,0x47,0x75,0x6a,0x61,0x72,0x61,0x74,
		0x31,0x0c,0x30,0x0a,0x06,0x03,0x55,0x04,0x07,0x0c,0x03,0x41,0x48,0x4d,0x31,0x0e,
		0x30,0x0c,0x06,0x03,0x55,0x04,0x0a,0x0c,0x05,0x41,0x72,0x72,0x6f,0x77,0x31,0x0c,
		0x30,0x0a,0x06,0x03,0x55,0x04,0x0b,0x0c,0x03,0x45,0x69,0x63,0x31,0x0d,0x30,0x0b,
		0x06,0x03,0x55,0x04,0x03,0x0c,0x04,0x53,0x45,0x45,0x44,0x30,0x1e,0x17,0x0d,0x32,
		0x30,0x30,0x38,0x32,0x36,0x30,0x37,0x32,0x31,0x35,0x32,0x5a,0x17,0x0d,0x33,0x39,
		0x31,0x30,0x32,0x36,0x30,0x37,0x32,0x31,0x35,0x32,0x5a,0x30,0x5a,0x31,0x0b,0x30,
		0x09,0x06,0x03,0x55,0x04,0x06,0x13,0x02,0x49,0x4e,0x31,0x10,0x30,0x0e,0x06,0x03,
		0x55,0x04,0x08,0x0c,0x07,0x47,0x75,0x6a,0x61,0x72,0x61,0x74,0x31,0x0c,0x30,0x0a,
		0x06,0x03,0x55,0x04,0x07,0x0c,0x03,0x41,0x48,0x4d,0x31,0x0e,0x30,0x0c,0x06,0x03,
		0x55,0x04,0x0a,0x0c,0x05,0x41,0x72,0x72,0x6f,0x77,0x31,0x0c,0x30,0x0a,0x06,0x03,
		0x55,0x04,0x0b,0x0c,0x03,0x45,0x69,0x63,0x31,0x0d,0x30,0x0b,0x06,0x03,0x55,0x04,
		0x03,0x0c,0x04,0x53,0x45,0x45,0x44,0x30,0x59,0x30,0x13,0x06,0x07,0x2a,0x86,0x48,
		0xce,0x3d,0x02,0x01,0x06,0x08,0x2a,0x86,0x48,0xce,0x3d,0x03,0x01,0x07,0x03,0x42,
		0x00,0x04,0xb0,0xb6,0x4f,0x0d,0x79,0x73,0x08,0x47,0x3c,0xb6,0xb6,0x74,0x91,0x3a,
		0x3e,0xa4,0x0b,0x6a,0xaa,0x00,0x36,0x76,0xaf,0x4b,0x41,0x5e,0xfc,0xf1,0x17,0x2a,
		0x84,0xb6,0x8f,0xdb,0xf4,0xc6,0x54,0x64,0xc4,0x0d,0xb1,0x1f,0x72,0xd0,0x17,0x1a,
		0x94,0xe6,0xae,0x51,0x1e,0x62,0x16,0x68,0x50,0x88,0x3a,0x35,0x41,0x5d,0x81,0x13,
		0xc9,0x18,0xa3,0x53,0x30,0x51,0x30,0x1d,0x06,0x03,0x55,0x1d,0x0e,0x04,0x16,0x04,
		0x14,0xd1,0x92,0xab,0x65,0xed,0xa7,0x44,0x7f,0x27,0x41,0x9c,0x39,0xbd,0x39,0x61,
		0xf2,0xe6,0xcd,0x6a,0xd6,0x30,0x1f,0x06,0x03,0x55,0x1d,0x23,0x04,0x18,0x30,0x16,
		0x80,0x14,0xd1,0x92,0xab,0x65,0xed,0xa7,0x44,0x7f,0x27,0x41,0x9c,0x39,0xbd,0x39,
		0x61,0xf2,0xe6,0xcd,0x6a,0xd6,0x30,0x0f,0x06,0x03,0x55,0x1d,0x13,0x01,0x01,0xff,
		0x04,0x05,0x30,0x03,0x01,0x01,0xff,0x30,0x0a,0x06,0x08,0x2a,0x86,0x48,0xce,0x3d,
		0x04,0x03,0x02,0x03,0x49,0x00,0x30,0x46,0x02,0x21,0x00,0xd7,0x8a,0xac,0x2f,0x89,
		0x0c,0xd0,0x04,0x55,0x34,0x12,0x2b,0x50,0x4d,0xb8,0x54,0x15,0xca,0x63,0x57,0xdd,
		0xa6,0x62,0xca,0xe2,0x53,0xb1,0x79,0xae,0x1d,0x68,0xac,0x02,0x21,0x00,0xd1,0x9d,
		0x7a,0x8f,0x28,0xa9,0x0a,0xec,0x6b,0xfe,0xe5,0xb1,0xa6,0x76,0x67,0x58,0x38,0xbc,
		0x03,0x46,0x7f,0xf3,0xc2,0xa9,0xfc,0xdb,0x27,0x0b,0x50,0x90,0xad,0x5d
};
#endif

optiga_lib_status_t mutualauth_optiga_crypt_random(Random_Data_t *);
optiga_lib_status_t mutualauth_optiga_crypt_ecc_generate_keypair(ECC_Pubkey_t *);
optiga_lib_status_t mutualauth_optiga_crypt_ecdsa_sign(ECC_Sign_t *);
optiga_lib_status_t mutualauth_optiga_crypt_ecdsa_verify(ECC_Verify_t *);
optiga_lib_status_t mutualauth_optiga_crypt_hash(Hash_Data_t *);
optiga_lib_status_t mutualauth_optiga_crypt_ecdh(ECDH_Secret_t *);
optiga_lib_status_t mutualauth_optiga_write(uint16_t, uint8_t*, uint16_t);
optiga_lib_status_t mutualauth_optiga_read(uint16_t, uint8_t*, uint16_t* );

optiga_lib_status_t read_device_cert(uint16_t , uint8_t* , uint16_t* );
uint16_t crc16(uint8_t *, uint32_t );
uint8_t aes_encrypt(uint8_t *payload_in, uint8_t *payload_out, uint32_t length);
uint8_t aes_decrypt(uint8_t *payload_in, uint8_t *payload_out, uint32_t length);
void create_payload_frame(MUTUALAUTH_Edge_Data_t * , uint8_t *, uint16_t);
int8_t parse_and_validate_frame(uint8_t *);
void ByteArrayToShort(uint8_t *, uint16_t, uint16_t *);
void ShortToByteArray(uint16_t ,uint8_t *);
#endif