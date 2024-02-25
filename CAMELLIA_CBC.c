#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_SIZE 16 // For Camellia-256
#define IV_SIZE 16  // Standard IV size for CBC mode
#define BUFFER_SIZE 1024
#define TAG_SIZE 16 // Tag size for GCM, not used here but shown for completeness

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

void initialize_key_iv(unsigned char *key, unsigned char *iv) {
    if (!RAND_bytes(key, KEY_SIZE) || !RAND_bytes(iv, IV_SIZE)) {
        printf("Error generating key or IV\n");
        exit(1);
    }
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Initialize the encryption operation for Camellia-256-CBC
    if(1 != EVP_EncryptInit_ex(ctx, EVP_camellia_128_cbc(), NULL, key, iv)) handleErrors();

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) handleErrors();
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Initialize the decryption operation for Camellia-256-CBC
    if(1 != EVP_DecryptInit_ex(ctx, EVP_camellia_128_cbc(), NULL, key, iv)) handleErrors();

    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) handleErrors();
    plaintext_len = len;

    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

int main(void) {
    unsigned char key[KEY_SIZE], iv[IV_SIZE];
    initialize_key_iv(key, iv);
    clock_t start, end;
    double cpu_time;


    
    unsigned char buffer[BUFFER_SIZE];
    unsigned char processed[BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH]; 
    int processed_len;

    // Encryption
   
   FILE *file = fopen("10MB.txt", "rb");
    //FILE *file = fopen("100MB.txt", "rb");
    FILE *encryptedfile = fopen("Encrypted10MB.txt", "wb");
  //  FILE *encryptedfile = fopen("Encrypted100MB.txt", "wb");
  if (!file || !encryptedfile){
        printf("Error opening file");
        return EXIT_FAILURE;
    }

    start = clock();
    
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        processed_len = encrypt(buffer, bytes_read, key, iv, processed);
        fwrite(processed, 1, processed_len, encryptedfile);
    }

 // while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, encryptedfile)) > 0) {
  //    processed_len = decrypt(buffer, bytes_read, key, iv, processed);
  // }

    end = clock();
    cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU TIME USED: %f", cpu_time);

    fclose(file);
    fclose(encryptedfile);


    return 0;
}
