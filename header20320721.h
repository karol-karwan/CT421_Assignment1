
#ifndef AES_ENCRYPTION_H
#define AES_ENCRYPTION_H

#include <openssl/des.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function prototypes
void initialize_aes_key(AES_KEY *aes_key);
void encrypt_ecb(AES_KEY *aes_key, const unsigned char *data, unsigned char *encrypted_data, size_t data_len);
void decrypt_ecb(AES_KEY *aes_key, const unsigned char *data, unsigned char *encrypted_data, size_t data_len);
void decrypt_cbc(AES_KEY *aes_key, unsigned char *input, unsigned char *output, size_t length, unsigned char *iv);
void encrypt_cbc(AES_KEY *aes_key, unsigned char *input, unsigned char *output, size_t length, unsigned char *iv);
int decrypt_3des_ecb(unsigned char *ciphertext, int ciphertext_len, const_DES_cblock key1, const_DES_cblock key2, const_DES_cblock key3, unsigned char *plaintext);
int encrypt_3des_ecb(unsigned char *plaintext, int plaintext_len, const_DES_cblock key1, const_DES_cblock key2, const_DES_cblock key3, unsigned char *ciphertext);
int decrypt_3des_cbc(unsigned char *ciphertext, int ciphertext_len, const_DES_cblock key1, const_DES_cblock key2, const_DES_cblock key3, DES_cblock iv, unsigned char *plaintext);
int encrypt_3des_cbc(unsigned char *plaintext, int plaintext_len, const_DES_cblock key1, const_DES_cblock key2, const_DES_cblock key3, DES_cblock iv, unsigned char *ciphertext);

#endif // AES_ENCRYPTION_H
