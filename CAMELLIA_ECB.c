#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_SIZE 16 //32 for 256 bit
#define BUFFER_SIZE 1024 //1kb

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

void initialize_key(unsigned char *key) {
    if (!RAND_bytes(key, KEY_SIZE)) {
        printf("Error generating key\n");
        exit(1);
    }
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Initialize the encryption operation. Use Camellia in ECB mode
    if(1 != EVP_EncryptInit_ex(ctx, EVP_camellia_128_ecb(), NULL, key, NULL)) handleErrors();

    // Provide the message to be encrypted, and obtain the encrypted output.
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) handleErrors();
    ciphertext_len = len;

    // Finalize the encryption. Further ciphertext bytes may be written at this stage.
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}


int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;
    int ret;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Initialize the decryption operation. Use Camellia in ECB mode
    if(1 != EVP_DecryptInit_ex(ctx, EVP_camellia_128_ecb(), NULL, key, NULL)) handleErrors();

    // Provide the message to be decrypted, and obtain the plaintext output
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) handleErrors();
    plaintext_len = len;

    // Finalize the decryption. Further plaintext bytes may be written at this stage
    ret = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    if(ret > 0) {
        plaintext_len += len;
        EVP_CIPHER_CTX_free(ctx);
        return plaintext_len; // Decryption successful
    } else {
        handleErrors(); // Decryption failed
        EVP_CIPHER_CTX_free(ctx);
    
    }
}

int main(void) {
    clock_t start, end;
    double cpu_time;
    unsigned char key[KEY_SIZE];
    initialize_key(key);

    
    unsigned char buffer[BUFFER_SIZE];
    unsigned char processed[BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH]; 
    int processed_len;

    // Encryption
   
    FILE *file = fopen("10MB.txt", "rb");
   // FILE *file = fopen("100MB.txt", "rb");
    FILE *encryptedfile = fopen("Encrypted10MB.txt", "wb");
   // FILE *encryptedfile = fopen("Encrypted100MB.txt", "wb");
  if (!file || !encryptedfile){
        printf("Error opening file");
        return EXIT_FAILURE;
    }

    start = clock();
    
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        processed_len = encrypt(buffer, bytes_read, key, processed);
        fwrite(processed, 1, processed_len, encryptedfile);
    }

   while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, encryptedfile)) > 0) {
      processed_len = decrypt(buffer, bytes_read, key, processed);
   }

    end = clock();
    cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU TIME USED: %f", cpu_time);

    fclose(file);
    fclose(encryptedfile);

    return 0;
}
