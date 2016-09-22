#ifndef __WQS_SIGNATURE_H__
#define __WQS_SIGNATURE_H__

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <assert.h>

typedef unsigned char byte;

void EVP_RSA_SIGN_Init();

/* Returns 0 for success, non-0 otherwise */
int EVP_RSA_SIGN_make_keys_by_create(EVP_PKEY** skey, EVP_PKEY** vkey);
int EVP_RSA_SIGN_make_keys_by_hmac(EVP_PKEY** pri_key, EVP_PKEY** pub_key, unsigned char *hmac_key);
int EVP_RSA_SIGN_make_keys_by_file(EVP_PKEY** pri_key, EVP_PKEY** pub_key, char *pri_key_file, char *pub_key_file);
int EVP_RSA_SIGN_make_keys_to_file(char *pri_key_file, char *pub_key_file);

/* Returns 0 for success, non-0 otherwise */
int EVP_RSA_SIGN_Signature(const byte* msg, size_t mlen, byte** sig, size_t* slen, EVP_PKEY* pri_key);

/* Returns 0 for success, non-0 otherwise */
int EVP_RSA_SIGN_Verify(const byte* msg, size_t mlen, byte* sig, size_t slen, EVP_PKEY* pub_key);

/* Prints a buffer to stdout. Label is optional */
void print_it(const char* label, const byte* buff, size_t len);

#endif
