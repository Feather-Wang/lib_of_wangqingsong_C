#ifndef __WQS_RSA_H__
#define __WQS_RSA_H__

#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/pem.h>


int make_private_key_by_create(RSA **rsa_pri_key);
int make_public_key_by_create(RSA **rsa_pub_key);
int make_public_key_by_private_key(RSA **rsa_pub_key, RSA *rsa_pri_key);
int make_keys_by_create(RSA **rsa_pri_key, RSA **rsa_pub_key);

int make_keys_to_file(char *pri_key_file, char *pub_key_file);

int make_private_key_by_file(RSA **rsa_pri_key, char *PrivateKeyFile);
int make_public_key_by_file(RSA **rsa_pub_key, char *PublicKeyFile);
int make_keys_by_file(RSA **rsa_pri_key, RSA **rsa_pub_key, char *PrivateKeyFile, char *PublicKeyFile);

int rsa_encryption(RSA **rsa_pub_key, unsigned char *msg, unsigned int msg_len, unsigned char *encryBuf);
int rsa_decryption(RSA **rsa_pri_key, unsigned char *ByteBuf, unsigned char *sourdata);

int rsa_public_encryption(RSA **rsa_pub_key, unsigned char *msg, unsigned int msg_len, unsigned char *encryBuf);
int rsa_private_encryption(RSA **rsa_pri_key, unsigned char *msg, unsigned int msg_len, unsigned char *encryBuf);

int rsa_private_decryption(RSA **rsa_pri_key, unsigned char *ByteBuf, unsigned char *sourdata);
int rsa_public_decryption(RSA **rsa_pub_key, unsigned char *ByteBuf, unsigned char *sourdata);

void rsa_free(RSA *rsa);

#endif
