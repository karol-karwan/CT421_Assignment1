
#include "header20320721.h"

//initialise the aes_key for encryption
void initialize_aes_key_enc(AES_KEY *aes_key) {
   //unsigned char key_bits128[16]; //128 bits
   unsigned char key_bits256[32]; //256bits
    if (!RAND_bytes(key_bits256[32], sizeof(key_bits256[32]))) {
        printf("Error generating key");
        exit(1);
    }
    
    if (AES_set_encrypt_key(key_bits256[32], 256, aes_key) < 0) {
        printf("Error initializing AES key");
        exit(1);
    }
}

//similar to encryption we just change AES_set_encrypt_key => AES_set_decrypt_key
void initialize_aes_key_dyc(AES_KEY *aes_key) {
   // unsigned char key_bits128[16]; //128 bits
    unsigned char key_bits256[32]; //256bits
    if (!RAND_bytes(key_bits256[32], sizeof(key_bits256[32]))) {
        printf("Error generating key");
        exit(1);
    }
    
    if (AES_set_decrypt_key(key_bits256[32], 256, aes_key) < 0) {
        printf("Error initializing AES key");
        exit(1);
    }
}


void encrypt_cbc(AES_KEY *aes_key, unsigned char *input, unsigned char *output, size_t data_length, unsigned char *iv) {
    //using a temp_iv first so that we dont alter the original initilisation variable, then we use memcpy to transfer it over to the original IV.
    unsigned char temp_iv[AES_BLOCK_SIZE]; 
    memcpy(temp_iv, iv, AES_BLOCK_SIZE); 
    AES_cbc_encrypt(input, output, data_length, aes_key, temp_iv, AES_ENCRYPT);
}

//similar function to encrytp_cbc, we just change it from AES_ENCRYPT => AES_DECRYPT
void decrypt_cbc(AES_KEY *aes_key, unsigned char *input, unsigned char *output, size_t data_length, unsigned char *iv) {
    unsigned char temp_iv[AES_BLOCK_SIZE]; 
    memcpy(temp_iv, iv, AES_BLOCK_SIZE); 
    AES_cbc_encrypt(input, output, data_length, aes_key, temp_iv, AES_DECRYPT);
}

//generate IV.
void IV(AES_KEY *iv){
unsigned char iv[AES_BLOCK_SIZE]; 
if (!RAND_bytes(iv, sizeof(iv))) {
    printf("Error generating IV");
    exit(1);
 }
}

int main(){
    clock_t start, end;
    double cpu_time;

   FILE *file = fopen("10MB.txt", "rb");
   //FILE *file = fopen("100MB.txt", "rb");
   FILE *encryptedfile = fopen("Encrypted10MB.txt", "wb");
   //FILE *encryptedfile = fopen("Encrypted100MB.txt", "wb");

    if (!file || !encryptedfile){
        printf("Error opening file");
        return EXIT_FAILURE;
    }

    //start the timer to calculate CPU time.
    start = clock();
    AES_KEY aes_key128;
    AES_KEY iv;
    initialize_aes_key_enc(&aes_key128);
    IV(&iv);

    //1024 is 1kb
    const size_t bufferSize = 1024;
    unsigned char buffer[bufferSize];
    unsigned char encrypted[bufferSize];
    size_t bytesRead;

   

    //encrypt the file and save the result into encryptedfile
    while ((bytesRead = fread(buffer, 1, bufferSize, file)) > 0) {
        encrypt_cbc(&aes_key128, buffer, encrypted, bytesRead,&iv);
        fwrite(encrypted, 1, bytesRead, encryptedfile);
    }
    initialize_aes_key_dyc(&aes_key128);
    //decrypt the file
   while ((bytesRead = fread(buffer, 1, bufferSize, encryptedfile)) > 0) {
       decrypt_cbc(&aes_key128, buffer, encrypted, bytesRead, &iv);
    }
       
    end = clock();

    //after calculations stop the clock and print time taken.
    cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU TIME USED: %f", cpu_time);


    fclose(file);
    fclose(encryptedfile);
    return 0;
}