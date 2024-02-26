
#include "header20320721.h"

#define BUFFER_SIZE 1024
#define KEY_SIZE 8

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}
//since its 3des, we want to setup 3 keys for both encryption and decryption in both CBC and ECM mode
//cbc encryption
int encrypt_3des_cbc(unsigned char *plaintext, int plaintext_len, const_DES_cblock key1, const_DES_cblock key2, const_DES_cblock key3, DES_cblock iv, unsigned char *ciphertext) {
    DES_key_schedule ks1, ks2, ks3;
    DES_set_key_checked((DES_cblock *)key1, &ks1);
    DES_set_key_checked((DES_cblock *)key2, &ks2);
    DES_set_key_checked((DES_cblock *)key3, &ks3);

    int len;
    DES_ede3_cbc_encrypt(plaintext, ciphertext, plaintext_len, &ks1, &ks2, &ks3, &iv, DES_ENCRYPT);
    len = plaintext_len; 
    return len;
}

//cbc decryption
int decrypt_3des_cbc(unsigned char *ciphertext, int ciphertext_len, const_DES_cblock key1, const_DES_cblock key2, const_DES_cblock key3, DES_cblock iv, unsigned char *plaintext) {
    DES_key_schedule ks1, ks2, ks3;
    DES_set_key_checked((DES_cblock *)key1, &ks1);
    DES_set_key_checked((DES_cblock *)key2, &ks2);
    DES_set_key_checked((DES_cblock *)key3, &ks3);


    int len;
    DES_ede3_cbc_encrypt(ciphertext, plaintext, ciphertext_len, &ks1, &ks2, &ks3, &iv, DES_DECRYPT);
    len = ciphertext_len; // For simplicity, assuming no padding is removed
    return len;
}

//encrypt ecb. Only change from cbc is DES_ede3_cbc_enc -> DES_edb3_enc and remove IV since ECB doesnt use that
int encrypt_3des_ecb(unsigned char *plaintext, int plaintext_len, const_DES_cblock key1, const_DES_cblock key2, const_DES_cblock key3, unsigned char *ciphertext) {
    DES_key_schedule ks1, ks2, ks3;
    DES_set_key_checked((DES_cblock *)key1, &ks1);
    DES_set_key_checked((DES_cblock *)key2, &ks2);
    DES_set_key_checked((DES_cblock *)key3, &ks3);

    int len;
    DES_ecb3_encrypt(plaintext, plaintext_len, &ks1, &ks2, &ks3, ciphertext DES_ENCRYPT);
    len = plaintext_len; 
    return len;
}

//decryption for ecb.
//same again, similar to decryption from cbc, just change the function call and remove the IV 
int decrypt_3des_ecb(unsigned char *ciphertext, int ciphertext_len, const_DES_cblock key1, const_DES_cblock key2, const_DES_cblock key3, unsigned char *plaintext) {
    DES_key_schedule ks1, ks2, ks3;
    DES_set_key_checked((DES_cblock *)key1, &ks1);
    DES_set_key_checked((DES_cblock *)key2, &ks2);
    DES_set_key_checked((DES_cblock *)key3, &ks3);

    int len;
    DES_ecb3_encrypt(ciphertext, ciphertext_len, &ks1, &ks2, &ks3, plaintext, DES_DECRYPT);
    len = ciphertext_len; 
    return len;
}

int main() {
    clock_t start, end;
    double cpu_time;

    //key size is sety to 8.
    unsigned char key1[KEY_SIZE], key2[KEY_SIZE], key3[KEY_SIZE], iv[KEY_SIZE];

    unsigned char buffer[BUFFER_SIZE], encrypted[BUFFER_SIZE], decrypted[BUFFER_SIZE];
    size_t bytes_read, bytes_written;
    unsigned char processed[BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH]; 
    int encrypted_len, decrypted_len;

    FILE *file = fopen("100MB.txt", "rb");
    FILE *encryptedfile = fopen("Encrypted100MB.txt", "wb");
  if (!file || !encryptedfile){
        printf("Error opening file");
        return EXIT_FAILURE;
    }
    start = clock();


while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
    encrypted_len = encrypt_3des_cbc(buffer, bytes_read, key1, key2, key3, iv, encrypted);
   // encrypted_len = encrypt_3des_ecb(buffer, bytes_read, key1, key2, key3, encrypted);
    fwrite(encrypted, 1, encrypted_len, encryptedfile);
}

fclose(encryptedfile); 
fclose(file); 

// Reopen encrypted file for reading
FILE *encryptedfile = fopen("Encrypted100MB.txt", "rb");

while ((bytes_read = fread(encrypted, 1, BUFFER_SIZE, encryptedfile)) > 0) {
    decrypted_len = decrypt_3des_cbc(encrypted, bytes_read, key1, key2, key3, iv, decrypted);
   // decrypted_len = decrypt_3des_ecb(encrypted, bytes_read, key1, key2, key3, decrypted);
}
fclose(encryptedfile); 

end = clock();
    cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU TIME USED: %f", cpu_time);

    return 0;
  }

