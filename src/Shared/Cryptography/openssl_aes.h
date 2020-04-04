#ifndef OPENSSL_AES_H
#define OPENSSL_AES_H

#include "openssl_zalocator.h"

class openssl_aes
{
public:
    openssl_aes(const EVP_CIPHER* evp_ciper);
    static const unsigned int KEY_SIZE_256_BITS = 32;
    static const unsigned int BLOCK_SIZE_128_BITS = 16;
    typedef unsigned char byte;
    typedef std::basic_string<char, std::char_traits<char>, zallocator<char> > secure_string;
    void aes_256_cbc_encrypt(const byte key[KEY_SIZE_256_BITS], const byte iv[BLOCK_SIZE_128_BITS], const secure_string& ptext, secure_string& ctext);
    void aes_256_cbc_decrypt(const byte key[KEY_SIZE_256_BITS], const byte iv[BLOCK_SIZE_128_BITS], const secure_string& ctext, secure_string& rtext);

private:
    using EVP_CIPHER_CTX_free_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;
};

#endif // OPENSSL_AES_H
