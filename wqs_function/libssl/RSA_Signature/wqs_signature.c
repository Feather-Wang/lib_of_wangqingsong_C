#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "wqs_signature.h"

#define UNUSED(x) ((void)x)
const char hn[] = "SHA256";

#define RSA_KEY_BITES 1024

int CRYPTO_memcmp( const void *in_a, const void *in_b, size_t len )
{
    size_t i = 0;
    const unsigned char *a = in_a;
    const unsigned char *b = in_b;
    unsigned char x = 0;

    for( i = 0; i < len; ++i )
        x |= a[i] ^ b[i];

    return x;
}

void EVP_RSA_SIGN_Init()
{
    OpenSSL_add_all_algorithms();
}

int EVP_RSA_SIGN_make_keys_by_create(EVP_PKEY** pri_key, EVP_PKEY** pub_key)
{
    if(!pri_key || !pub_key)
        return -1;
    
    if(*pri_key != NULL) {
        EVP_PKEY_free(*pri_key);
        *pri_key = NULL;
    }
    
    if(*pub_key != NULL) {
        EVP_PKEY_free(*pub_key);
        *pub_key = NULL;
    }
    
    int result = -1;
    RSA* rsa = NULL;
    
    do
    {
        *pri_key = EVP_PKEY_new();
        assert(*pri_key != NULL);
        if(*pri_key == NULL) {
            printf("EVP_PKEY_new failed (1), error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        *pub_key = EVP_PKEY_new();
        assert(*pub_key != NULL);
        if(*pub_key == NULL) {
            printf("EVP_PKEY_new failed (2), error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        rsa = RSA_generate_key(RSA_KEY_BITES, RSA_F4, NULL, NULL);
        assert(rsa != NULL);
        if(rsa == NULL) {
            printf("RSA_generate_key failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        /* Set signing key */
        int rc = EVP_PKEY_assign_RSA(*pri_key, RSAPrivateKey_dup(rsa));
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_PKEY_assign_RSA (1) failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        /* Sanity check. Verify private exponent is present */
        /* assert(EVP_PKEY_get0_RSA(*pri_key)->d != NULL); */

        /* Set verifier key */
        rc = EVP_PKEY_assign_RSA(*pub_key, RSAPublicKey_dup(rsa));
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_PKEY_assign_RSA (2) failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        /* Sanity check. Verify private exponent is missing */
        /* assert(EVP_PKEY_get0_RSA(*pub_key)->d == NULL); */
        
        result = 0;
        
    } while(0);
    
    if(rsa) {
        RSA_free(rsa);
        rsa = NULL;
    }
    
    return !!result;
}

int EVP_RSA_SIGN_make_keys_by_hmac(EVP_PKEY** pri_key, EVP_PKEY** pub_key, unsigned char *hmac_key)
{
    if(!pri_key || !pub_key)
        return -1;
    
    if(*pri_key != NULL) {
        EVP_PKEY_free(*pri_key);
        *pri_key = NULL;
    }
    
    if(*pub_key != NULL) {
        EVP_PKEY_free(*pub_key);
        *pub_key = NULL;
    }
    
    int result = -1;
    /* HMAC key */
    byte hkey[EVP_MAX_MD_SIZE];
    
    do
    {
        const EVP_MD* md = EVP_get_digestbyname(hn);
        assert(md != NULL);
        if(md == NULL) {
            printf("EVP_get_digestbyname failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        int size = EVP_MD_size(md);
        assert(size >= 16);
        if(!(size >= 16)) {
            printf("EVP_MD_size failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        assert(size <= sizeof(hkey));
        if(!(size <= sizeof(hkey))) {
            printf("EVP_MD_size is too large\n");
            break; /* failed */
        }
        
        /* Generate bytes */
        int rc = RAND_bytes(hkey, size);
        assert(rc == 1);
        if(rc != 1) {
            printf("RAND_bytes failed, error 0x%lx\n", ERR_get_error());
            break;
        }
        
        snprintf(hkey, sizeof(hkey), "%s", hmac_key);
        size = strlen(hkey);
        print_it("HMAC key", hkey, size);

        
        *pri_key = EVP_PKEY_new_mac_key(EVP_PKEY_HMAC, NULL, hkey, size);
        assert(*pri_key != NULL);
        if(*pri_key == NULL) {
            printf("EVP_PKEY_new_mac_key failed, error 0x%lx\n", ERR_get_error());
            break;
        }
        
        *pub_key = EVP_PKEY_new_mac_key(EVP_PKEY_HMAC, NULL, hkey, size);
        assert(*pub_key != NULL);
        if(*pub_key == NULL) {
            printf("EVP_PKEY_new_mac_key failed, error 0x%lx\n", ERR_get_error());
            break;
        }
        
        result = 0;
        
    } while(0);
    
    OPENSSL_cleanse(hkey, sizeof(hkey));
    
    return !!result;
}

int EVP_RSA_SIGN_make_keys_by_file(EVP_PKEY** pri_key, EVP_PKEY** pub_key, char *pri_key_file, char *pub_key_file)
{
    int result = -1;
    
    if(!pri_key || !pub_key)
        return -1;
    
    if(*pri_key != NULL) {
        EVP_PKEY_free(*pri_key);
        *pri_key = NULL;
    }
    
    if(*pub_key != NULL) {
        EVP_PKEY_free(*pub_key);
        *pub_key = NULL;
    }

    RSA *rsa_pri_key = NULL;
    RSA *rsa_pub_key = NULL;
    
    do
    {
        *pri_key = EVP_PKEY_new();
        assert(*pri_key != NULL);
        if(*pri_key == NULL) {
            printf("EVP_PKEY_new failed (1), error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        *pub_key = EVP_PKEY_new();
        assert(*pub_key != NULL);
        if(*pub_key == NULL) {
            printf("EVP_PKEY_new failed (2), error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        // read Private Key File
        FILE *fp = NULL;
        fp = fopen(pri_key_file, "rb");
        if( NULL == fp )
        {
            fprintf(stderr, "fopen %s failed\n", pri_key_file);
            break;
        }
        rsa_pri_key = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
        fclose(fp);
        fp = NULL;

        assert(rsa_pri_key != NULL);
        if(rsa_pri_key == NULL) {
            printf("RSA_generate_key failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        /* Set signing key */
        int rc = EVP_PKEY_assign_RSA(*pri_key, RSAPrivateKey_dup(rsa_pri_key));
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_PKEY_assign_RSA (1) failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        /* Sanity check. Verify private exponent is present */
        /* assert(EVP_PKEY_get0_RSA(*pri_key)->d != NULL); */
        
        // read Public Key File
        fp = fopen(pub_key_file, "rb");
        if( NULL == fp )
        {
            fprintf(stderr, "fopen %s failed\n", pub_key_file);
            break;
        }
        rsa_pub_key = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
        //rsa_pub_key = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL);
        fclose(fp);

        /* Set verifier key */
        rc = EVP_PKEY_assign_RSA(*pub_key, RSAPublicKey_dup(rsa_pub_key));
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_PKEY_assign_RSA (2) failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        /* Sanity check. Verify private exponent is missing */
        /* assert(EVP_PKEY_get0_RSA(*pub_key)->d == NULL); */
        
        result = 0;
        
    } while(0);
    
    if(rsa_pri_key) {
        RSA_free(rsa_pri_key);
        rsa_pri_key = NULL;
    }
 
    if(rsa_pub_key) {
        RSA_free(rsa_pub_key);
        rsa_pub_key = NULL;
    }
   
    return !!result;
}

int EVP_RSA_SIGN_make_keys_to_file(char *pri_key_file, char *pub_key_file)
{
    int return_value = -1;

    //密钥长度( RSA key length )
    int bites = 1024;
    unsigned long e = RSA_F4; // RSA_F4 == 0x010001
    BIGNUM *bne = NULL; //store e
    RSA *rsa_key = NULL;

    int ret = 0;

    do{
        // get Private Key
        bne = BN_new();
        ret = BN_set_word( bne, e );
        rsa_key = RSA_new();
        ret = RSA_generate_key_ex( rsa_key, bites, bne, NULL);
        if( ret != 1 )
        {
            fprintf(stderr, "RSA_generate_key_ex error 0x%lx\n", ERR_get_error());
            break;
        }
        BN_free(bne);

        //将密钥信息打印出来，输出到屏幕上，整体距离左边距为11
        //RSA_print_fp(stdout, rsa_key, 11);

        // write Private Key to pri_key_file
        FILE *fp = NULL;
        fp = fopen(pri_key_file, "wb");
        if( NULL == fp )
        {
            fprintf(stderr, "fopen %s failed\n", pri_key_file);
            break;
        }
        PEM_write_RSAPrivateKey(fp, rsa_key, NULL, NULL, RSA_KEY_BITES, NULL, NULL);
        fclose(fp);
        fp = NULL;

        // write Public Key to pub_key_file
        fp = fopen(pub_key_file, "wb");
        if( NULL == fp )
        {
            fprintf(stderr, "fopen %s failed\n", pub_key_file);
            break;
        }
        PEM_write_RSAPublicKey(fp, rsa_key);
        fclose(fp);

        return_value = 0;
    }while(0);

    if( bne )
        BN_free(bne);

    if( rsa_key )
        RSA_free(rsa_key);

    return return_value;
}


int EVP_RSA_SIGN_Signature(const byte* msg, size_t mlen, byte** sig, size_t* slen, EVP_PKEY* pri_key)
{
    /* Returned to caller */
    int result = -1;
    
    if(!msg || !mlen || !sig || !pri_key) {
        assert(0);
        return -1;
    }
    
    if(*sig)
        OPENSSL_free(*sig);
    
    *sig = NULL;
    *slen = 0;
    
    EVP_MD_CTX* ctx = NULL;
    
    do
    {
        ctx = EVP_MD_CTX_create();
        assert(ctx != NULL);
        if(ctx == NULL) {
            printf("EVP_MD_CTX_create failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        const EVP_MD* md = EVP_get_digestbyname(hn);
        assert(md != NULL);
        if(md == NULL) {
            printf("EVP_get_digestbyname failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        int rc = EVP_DigestInit_ex(ctx, md, NULL);
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_DigestInit_ex failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        rc = EVP_DigestSignInit(ctx, NULL, md, NULL, pri_key);
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_DigestSignInit failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        rc = EVP_DigestSignUpdate(ctx, msg, mlen);
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_DigestSignUpdate failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        size_t req = 0;
        rc = EVP_DigestSignFinal(ctx, NULL, &req);
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_DigestSignFinal failed (1), error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        assert(req > 0);
        if(!(req > 0)) {
            printf("EVP_DigestSignFinal failed (2), error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        *sig = OPENSSL_malloc(req);
        assert(*sig != NULL);
        if(*sig == NULL) {
            printf("OPENSSL_malloc failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        *slen = req;
        rc = EVP_DigestSignFinal(ctx, *sig, slen);
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_DigestSignFinal failed (3), return code %d, error 0x%lx\n", rc, ERR_get_error());
            break; /* failed */
        }
        
        assert(req == *slen);
        if(rc != 1) {
            printf("EVP_DigestSignFinal failed, mismatched signature sizes %ld, %ld", req, *slen);
            break; /* failed */
        }
        
        result = 0;
        
    } while(0);
    
    if(ctx) {
        EVP_MD_CTX_destroy(ctx);
        ctx = NULL;
    }
    
    return !!result;
}

int EVP_RSA_SIGN_Verify(const byte* msg, size_t mlen, byte* sig, size_t slen, EVP_PKEY* pub_key)
{
    /* Returned to caller */
    int result = -1;
    
    if(!msg || !mlen || !sig || !slen || !pub_key) {
        assert(0);
        return -1;
    }
    
    EVP_MD_CTX* ctx = NULL;
    
    do
    {
        ctx = EVP_MD_CTX_create();
        assert(ctx != NULL);
        if(ctx == NULL) {
            printf("EVP_MD_CTX_create failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        const EVP_MD* md = EVP_get_digestbyname(hn);
        assert(md != NULL);
        if(md == NULL) {
            printf("EVP_get_digestbyname failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        int rc = EVP_DigestInit_ex(ctx, md, NULL);
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_DigestInit_ex failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        rc = EVP_DigestVerifyInit(ctx, NULL, md, NULL, pub_key);
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_DigestVerifyInit failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        rc = EVP_DigestVerifyUpdate(ctx, msg, mlen);
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_DigestVerifyUpdate failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        /* Clear any errors for the call below */
        ERR_clear_error();
        
        rc = EVP_DigestVerifyFinal(ctx, sig, slen);
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_DigestVerifyFinal failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        result = 0;
        
    } while(0);
    
    if(ctx) {
        EVP_MD_CTX_destroy(ctx);
        ctx = NULL;
    }
    
    return !!result;

}

/* if the key is created by hmac , you need use this function to verify
 * signature*/
int EVP_RSA_SIGN_Verify_Hmac(const byte* msg, size_t mlen, const byte* sig, size_t slen, EVP_PKEY* pub_key)
{
    /* Returned to caller */
    int result = -1;
    
    if(!msg || !mlen || !sig || !slen || !pub_key) {
        assert(0);
        return -1;
    }

    EVP_MD_CTX* ctx = NULL;
    
    do
    {
        ctx = EVP_MD_CTX_create();
        assert(ctx != NULL);
        if(ctx == NULL) {
            printf("EVP_MD_CTX_create failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        const EVP_MD* md = EVP_get_digestbyname(hn);
        assert(md != NULL);
        if(md == NULL) {
            printf("EVP_get_digestbyname failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        int rc = EVP_DigestInit_ex(ctx, md, NULL);
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_DigestInit_ex failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        rc = EVP_DigestSignInit(ctx, NULL, md, NULL, pub_key);
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_DigestSignInit failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        rc = EVP_DigestSignUpdate(ctx, msg, mlen);
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_DigestSignUpdate failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        byte buff[EVP_MAX_MD_SIZE];
        size_t size = sizeof(buff);
        
        rc = EVP_DigestSignFinal(ctx, buff, &size);
        assert(rc == 1);
        if(rc != 1) {
            printf("EVP_DigestVerifyFinal failed, error 0x%lx\n", ERR_get_error());
            break; /* failed */
        }
        
        assert(size > 0);
        if(!(size > 0)) {
            printf("EVP_DigestSignFinal failed (2)\n");
            break; /* failed */
        }
        
        const size_t m = (slen < size ? slen : size);
        result = !!CRYPTO_memcmp(sig, buff, m);
        
        OPENSSL_cleanse(buff, sizeof(buff));
        
    } while(0);
    
    if(ctx) {
        EVP_MD_CTX_destroy(ctx);
        ctx = NULL;
    }
    
    /* Convert to 0/1 result */
    return !!result;
}

void print_it(const char* label, const byte* buff, size_t len)
{
    if(!buff || !len)
        return;
    
    if(label)
        printf("%s: ", label);
    
    size_t i;
    for(i=0; i < len; ++i)
        printf("%02X", buff[i]);
    
    printf("\n");
}
