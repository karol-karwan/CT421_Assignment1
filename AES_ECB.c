
#include "header20320721.h"

//initialising key for encryption
void initialize_aes_key_enc(AES_KEY *aes_key) {
    //unsigned char key_bits128[16]; //128 bits
   unsigned char key_bits256[32]; //256bits
    if (!RAND_bytes(key_bits256, sizeof(key_bits256))) {
        printf("Error generating key");
        exit(1);
    }
    
    if (AES_set_encrypt_key(key_bits256, 256, aes_key) < 0) {
        printf("Error initializing AES key");
        exit(1);
    }
}

//similar to encryption we just change AES_set_encrypt_key => AES_set_decrypt_key
void initialize_aes_key_dyc(AES_KEY *aes_key) {
    //unsigned char key_bits128[16]; //128 bits
    unsigned char key_bits256[32]; //256bits
    if (!RAND_bytes(key_bits256, sizeof(key_bits256))) {
        printf("Error generating key");
        exit(1);
    }
    
    if (AES_set_decrypt_key(key_bits256, 256, aes_key) < 0) {
        printf("Error initializing AES key");
        exit(1);
    }
}


void encrypt_ecb(AES_KEY *aes_key, const unsigned char *data, unsigned char *encrypted_data, size_t data_len){
    //block size is 16bytes by default
    for(size_t i=0; i < data_len; i+= AES_BLOCK_SIZE){
        AES_ecb_encrypt(data + i, encrypted_data + i, aes_key, AES_ENCRYPT);
    }
}

//similar function to encrytp_ecb, we just change it from AES_ENCRYPT => AES_DECRYPT
void decrypt_ecb(AES_KEY *aes_key, const unsigned char *data, unsigned char *encrypted_data, size_t data_len){
    for(size_t i=0; i < data_len; i+= AES_BLOCK_SIZE){
        AES_ecb_encrypt(data + i, encrypted_data + i, aes_key, AES_DECRYPT);
    }
}

int main(){
    clock_t start, end;
    double cpu_time;

    //FILE *file = fopen("10MB.txt", "rb");
    FILE *file = fopen("100MB.txt", "rb");
    //FILE *encryptedfile = fopen("Encrypted10MB.txt", "wb");
    FILE *encryptedfile = fopen("Encrypted100MB.txt", "wb");

    if (!file || !encryptedfile){
        printf("Error opening file");
        return EXIT_FAILURE;
    }

    //start the timer to calculate CPU time.
    start = clock();
    AES_KEY aes_key256;
    initialize_aes_key_enc(&aes_key256);


    //1024 is 1kb
    const size_t bufferSize = 1024;
    unsigned char buffer[bufferSize];
    unsigned char encrypted[bufferSize];
    size_t bytesRead;

    //encrypt the file 
    while ((bytesRead = fread(buffer, 1, bufferSize, file)) > 0) {
        encrypt_ecb(&aes_key256, buffer, encrypted, bytesRead);
        fwrite(encrypted, 1, bytesRead, encryptedfile);
    }
    initialize_aes_key_dyc(&aes_key256);
    //decrypt the file
    while ((bytesRead = fread(buffer, 1, bufferSize, encryptedfile)) > 0) {
        decrypt_ecb(&aes_key256, buffer, encrypted, bytesRead);
    }
       
    end = clock();

    //stop the time after calculations.
    cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU TIME USED: %f", cpu_time);


    fclose(file);
    fclose(encryptedfile);
    return 0;
}