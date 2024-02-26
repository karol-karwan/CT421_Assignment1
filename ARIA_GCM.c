#include "header20320721.h"

#define KEY_SIZE 32
#define IV_SIZE 12 
#define BUFFER_SIZE 1024
#define TAG_SIZE 16

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

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext, unsigned char *tag) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aria_256_gcm(), NULL, NULL, NULL)) handleErrors();

    if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, IV_SIZE, NULL)) handleErrors();

    if(1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

 
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) handleErrors();
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, TAG_SIZE, tag)) handleErrors();

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *tag, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;
    int ret;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if(1 != EVP_DecryptInit_ex(ctx, EVP_aria_256_gcm(), NULL, NULL, NULL)) handleErrors();

    if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, IV_SIZE, NULL)) handleErrors();

    if(1 != EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) handleErrors();
    plaintext_len = len;

    if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, TAG_SIZE, tag)) handleErrors();

    ret = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    if(ret > 0) {
        plaintext_len += len;
        EVP_CIPHER_CTX_free(ctx);
        return plaintext_len;
    } else {
        handleErrors();
        EVP_CIPHER_CTX_free(ctx);
        return -1; 
    }
}

int main(void) {
    unsigned char key[KEY_SIZE], iv[IV_SIZE];
    unsigned char tag[TAG_SIZE]; 
    initialize_key_iv(key, iv);

    clock_t start, end;
    double cpu_time;
   
    unsigned char buffer[BUFFER_SIZE];
    unsigned char processed[BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH]; 
    int processed_len;

    // Encryption
   
    //FILE *file = fopen("10MB.txt", "rb");
    FILE *file = fopen("100MB.txt", "rb");
  // FILE *encryptedfile = fopen("Encrypted10MB.txt", "wb");
   FILE *encryptedfile = fopen("Encrypted100MB.txt", "wb");
  if (!file || !encryptedfile){
        printf("Error opening file");
        return EXIT_FAILURE;
    }

    start = clock();
    
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        processed_len = encrypt(buffer, bytes_read, key, iv, processed, tag);
        fwrite(processed, 1, processed_len, encryptedfile);
    }

  while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, encryptedfile)) > 0) {
    processed_len = decrypt(buffer, bytes_read, key, iv, processed, tag) ;
   }

    end = clock();
    cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU TIME USED: %f", cpu_time);

    fclose(file);
    fclose(encryptedfile);
    
    return 0;
}
