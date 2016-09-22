#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wqs_signature.h"
#include <openssl/rsa.h>

int test_signature()
{
    printf("Testing RSA functions with EVP_DigestSign and EVP_DigestVerify\n");

    EVP_RSA_SIGN_Init();

    int return_value = -1;

    /* Sign and Verify HMAC keys */
    EVP_PKEY *pri_key = NULL, *pub_key = NULL;
    unsigned char* sig = NULL;

    do
    {
        int rc = EVP_RSA_SIGN_make_keys_by_create(&pri_key, &pub_key);
        //EVP_RSA_SIGN_make_keys_to_file("RSAPrivateKey.pem", "RSAPublicKey.pem");
        //int rc = EVP_RSA_SIGN_make_keys_by_file(&pri_key, &pub_key, "clientPriKeyNew.pem", "clientPubKeyNew.pem");
        //int rc = EVP_RSA_SIGN_make_keys_by_file(&pri_key, &pub_key, "clientPriKeyOld.pem", "clientPubKeyOld.pem");
        //int rc = EVP_RSA_SIGN_make_keys_by_hmac(&pri_key, &pub_key, "wangqingsong");
        assert(rc == 0);
        if(rc != 0)
            break;

        assert(pri_key != NULL);
        if(pri_key == NULL)
            break;

        assert(pub_key != NULL);
        if(pub_key == NULL)
            break;

        const byte msg[] = "Now is the time for all good men to come to the aide of their country";
        size_t slen = 0;

        /* Using the skey or signing key */
        rc = EVP_RSA_SIGN_Signature(msg, strlen(msg), &sig, &slen, pri_key);
        assert(rc == 0);
        if(rc == 0) {
            printf("Signature OK\n");
        } else {
            printf("Failed to create signature, return code %d\n", rc);
            break;
        }

        print_it("Signature", sig, slen);
#if 0
        /* Tamper with signature */
        printf("Tampering with signature\n");
        sig[0] ^= 0x01;
#endif

#if 0
        /* Tamper with signature */
        printf("Tampering with signature\n");
        sig[slen - 1] ^= 0x01;
#endif

        /* Using the vkey or verifying key */
        rc = EVP_RSA_SIGN_Verify(msg, strlen(msg), sig, slen, pub_key);
        /* if the key is created by hmac , you need use this function to verify
         * signature*/
        //rc = EVP_RSA_SIGN_Verify_Hmac(msg, strlen(msg), sig, slen, pub_key);
        if(rc == 0) {
            printf("Verified OK\n");
        } else {
            printf("Failed to verify signature, return code %d\n", rc);
            break;
        }
        print_it("Verify", sig, slen);

        return_value = 0;
    }while(0);

    if(sig)
        OPENSSL_free(sig);

    if(pri_key)
        EVP_PKEY_free(pri_key);

    if(pub_key)
        EVP_PKEY_free(pub_key);

    return return_value;
}

int main(int argc, const char *argv[])
{
    test_signature();
    return 0;
}
