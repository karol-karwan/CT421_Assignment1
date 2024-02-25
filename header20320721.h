
#ifndef AES_ENCRYPTION_H
#define AES_ENCRYPTION_H

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// Function prototypes
void initialize_aes_key(AES_KEY *aes_key);
void encrypt_ecb(AES_KEY *aes_key, const unsigned char *data, unsigned char *encrypted_data, size_t data_len);
void decrypt_ecb(AES_KEY *aes_key, const unsigned char *data, unsigned char *encrypted_data, size_t data_len);
void decrypt_cbc(AES_KEY *aes_key, unsigned char *input, unsigned char *output, size_t length, unsigned char *iv);
void encrypt_cbc(AES_KEY *aes_key, unsigned char *input, unsigned char *output, size_t length, unsigned char *iv);
#endif // AES_ENCRYPTION_H
