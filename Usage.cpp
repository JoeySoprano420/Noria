#include <openssl/aes.h>
#include <openssl/rand.h>

void encryptData() {
    AES_KEY enc_key;
    unsigned char key[16]; // 128-bit key
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char plaintext[] = "Your data here";
    unsigned char ciphertext[sizeof(plaintext)];

    // Generate random key and IV
    RAND_bytes(key, sizeof(key));
    RAND_bytes(iv, AES_BLOCK_SIZE);

    AES_set_encrypt_key(key, 128, &enc_key);
    AES_cbc_encrypt(plaintext, ciphertext, sizeof(plaintext), &enc_key, iv, AES_ENCRYPT);
    // Use ciphertext as needed
}
