#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AES_128_KEY_SIZE 16
#define AES_GCM_IV_LENGTH 12
#define AES_GCM_TAG_LENGTH 16
#define BUFFER_SIZE 1024 

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext, unsigned char *tag) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL)) handleErrors();

    if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, AES_GCM_IV_LENGTH, NULL)) handleErrors();

    if(1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) handleErrors();
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, AES_GCM_TAG_LENGTH, tag)) handleErrors();

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *tag, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;
    int ret;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL)) handleErrors();

    if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, AES_GCM_IV_LENGTH, NULL)) handleErrors();

    if(1 != EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) handleErrors();
    plaintext_len = len;

    if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, AES_GCM_TAG_LENGTH, tag)) handleErrors();

    ret = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);

    EVP_CIPHER_CTX_free(ctx);

    if(ret > 0) {
        plaintext_len += len;
        return plaintext_len;
    } else {
        printf("Decryption failed\n");
        return -1; // Indicate failure
    }
}



int main(void) {
    clock_t start, end;
    double cpu_time;
    unsigned char key[AES_128_KEY_SIZE];
    unsigned char iv[AES_GCM_IV_LENGTH];
    unsigned char tag[AES_GCM_TAG_LENGTH];
    unsigned char buffer[BUFFER_SIZE];
    unsigned char ciphertext[BUFFER_SIZE + AES_GCM_TAG_LENGTH]; // Allocate space for potential expansion

    RAND_bytes(key, sizeof(key));
    RAND_bytes(iv, sizeof(iv));

    //FILE *file = fopen("10MB.txt", "rb");
    FILE *file = fopen("100MB.txt", "rb");
   // FILE *encryptedfile = fopen("Encrypted10MB.txt", "wb");
    FILE *encryptedfile = fopen("Encrypted100MB.txt", "wb");

    if (!file || !encryptedfile){
        printf("Error opening file");
        return EXIT_FAILURE;
    }
    start = clock();
    int bytes_read, ciphertext_len;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        ciphertext_len = encrypt(buffer, bytes_read, key, iv, ciphertext, tag);
        fwrite(ciphertext, 1, ciphertext_len, encryptedfile);
    }
    // Write the tag at the end of the file
    fwrite(tag, 1, AES_GCM_TAG_LENGTH, encryptedfile);

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        ciphertext_len = decrypt(buffer, bytes_read, key, iv, ciphertext, tag);
    }

    end = clock();
    cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU TIME USED: %f", cpu_time);

    fclose(file);
    fclose(encryptedfile);

    return 0;
}
