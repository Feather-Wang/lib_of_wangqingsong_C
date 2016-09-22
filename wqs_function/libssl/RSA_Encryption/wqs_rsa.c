#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wqs_rsa.h"

#define RSA_KEY_BITES 1024

int make_private_key_by_create(RSA **rsa_pri_key)
{
    if( !rsa_pri_key )
        return -1;

    if(*rsa_pri_key != NULL) {
        RSA_free( *rsa_pri_key );
        *rsa_pri_key = NULL;
    }

    int ret = -1;

    //密钥长度( RSA key length )
    int bites = RSA_KEY_BITES;
    unsigned long e = RSA_F4; // RSA_F4 == 0x010001
    BIGNUM *bne = NULL; //store e

    do
    {
        // get Private Key
        bne = BN_new();
        ret = BN_set_word( bne, e );
        *rsa_pri_key = RSA_new();
        ret = RSA_generate_key_ex( *rsa_pri_key, bites, bne, NULL);
        if( ret != 1 )
        {
            fprintf(stderr, "RSA_generate_key_ex error 0x%lx\n", ERR_get_error());
            ret = -1;
            break;
        }

        //将密钥信息打印出来，输出到屏幕上，整体距离左边距为11
        //RSA_print_fp(stdout, rsa_pri_key, 11);
        //RSA_print_fp(stdout, rsa_pub_key, 11);

        ret = 0;
    }while(0);

    if( bne )
        BN_free(bne);

    return ret;
}

int make_public_key_by_create(RSA **rsa_pub_key)
{
    if( !rsa_pub_key )
        return -1;

    if(*rsa_pub_key != NULL) {
        RSA_free( *rsa_pub_key );
        *rsa_pub_key = NULL;
    }

    int ret = -1;

    //密钥长度( RSA key length )
    int bites = RSA_KEY_BITES;
    unsigned long e = RSA_F4; // RSA_F4 == 0x010001
    BIGNUM *bne = NULL; //store e

    RSA *rsa_pri_key = NULL;
    do
    {
        // get Private Key
        bne = BN_new();
        ret = BN_set_word( bne, e );
        rsa_pri_key = RSA_new();
        ret = RSA_generate_key_ex( rsa_pri_key, bites, bne, NULL);
        if( ret != 1 )
        {
            fprintf(stderr, "RSA_generate_key_ex error 0x%lx\n", ERR_get_error());
            ret = -1;
            break;
        }

        // get Public Key
        BIO *mem = BIO_new(BIO_s_mem());
        PEM_write_bio_RSA_PUBKEY(mem, rsa_pri_key);
        *rsa_pub_key = PEM_read_bio_RSA_PUBKEY(mem, NULL, NULL, NULL);
        BIO_free(mem);

        ret = 0;
    }while(0);

    if( rsa_pri_key )
        rsa_free(rsa_pri_key);

    if( bne )
        BN_free(bne);

    return ret;
}

int make_public_key_by_private_key(RSA **rsa_pub_key, RSA *rsa_pri_key)
{
    int ret = -1;

    if( !rsa_pub_key || !rsa_pri_key)
        return ret;

    if( *rsa_pub_key )
    {
        rsa_free(*rsa_pub_key);
        *rsa_pub_key = NULL;
    }

    do{
        BIO *mem = BIO_new(BIO_s_mem());
        PEM_write_bio_RSA_PUBKEY(mem, rsa_pri_key);
        *rsa_pub_key = PEM_read_bio_RSA_PUBKEY(mem, NULL, NULL, NULL);
        BIO_free(mem);

        ret = 0;

    }while(0);

    return ret;
}

int make_keys_by_create(RSA **rsa_pri_key, RSA **rsa_pub_key)
{

    if( !rsa_pri_key || !rsa_pub_key )
        return -1;

    if(*rsa_pri_key != NULL) {
        RSA_free( *rsa_pri_key );
        *rsa_pri_key = NULL;
    }

    if(*rsa_pub_key != NULL) {
        RSA_free( *rsa_pub_key );
        *rsa_pub_key = NULL;
    }

    int ret = -1;

    do
    {
        ret = make_private_key_by_create(rsa_pri_key);
        if( 0 != ret )
        {
            ret = -1;
            break;
        }

        ret = make_public_key_by_private_key(rsa_pub_key, *rsa_pri_key);
        if( 0 != ret )
        {
            ret = -1;
            break;
        }

        ret = 0;
    }while(0);

    return ret;
}

int make_keys_to_file(char *pri_key_file, char *pub_key_file)
{
    int ret = -1;

    if( !pri_key_file || !pub_key_file )
        return ret;

    RSA *rsa_pri_key = NULL;
    do
    {
        ret = make_private_key_by_create(&rsa_pri_key);
        if( 0 != ret )
        {
            ret = -1;
            break;
        }

        // write Private Key to pri_key_file
        FILE *fp = NULL;
        fp = fopen(pri_key_file, "wb");
        if( NULL == fp )
        {
            fprintf(stderr, "fopen %s failed\n", pri_key_file);
            ret = -1;
            break;
        }
        PEM_write_RSAPrivateKey(fp, rsa_pri_key, NULL, NULL, RSA_KEY_BITES, NULL, NULL);
        fclose(fp);
        fp = NULL;

        // write Public Key to pub_key_file
        fp = fopen(pub_key_file, "wb");
        if( NULL == fp )
        {
            fprintf(stderr, "fopen %s failed\n", pub_key_file);
            ret = -1;
            break;
        }
        PEM_write_RSAPublicKey(fp, rsa_pri_key);
        fclose(fp);

        ret = 0;
    }while(0);

    if( rsa_pri_key )
        rsa_free(rsa_pri_key);

    return ret;
}

int make_private_key_by_file(RSA **rsa_pri_key, char *PrivateKeyFile)
{
    if( !rsa_pri_key || !PrivateKeyFile )
        return -1;

    if(*rsa_pri_key != NULL) {
        rsa_free( *rsa_pri_key );
        *rsa_pri_key = NULL;
    }

    int ret = -1;
    do
    {
        // read Private Key File
        FILE *fp = NULL;
        fp = fopen(PrivateKeyFile, "rb");
        if( NULL == fp )
        {
            fprintf(stderr, "fopen %s failed\n", PrivateKeyFile);
            return -1;
        }
        *rsa_pri_key = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL);
        fclose(fp);
        fp = NULL;

        ret = 0;
    }while(0);

    return ret;
}

int make_public_key_by_file(RSA **rsa_pub_key, char *PublicKeyFile)
{
    if( !rsa_pub_key || !PublicKeyFile )
        return -1;

    if(*rsa_pub_key != NULL) {
        rsa_free( *rsa_pub_key );
        *rsa_pub_key = NULL;
    }

    int ret = -1;
    do
    {
        // read Public Key File
        FILE *fp = fopen(PublicKeyFile, "rb");
        if( NULL == fp )
        {
            fprintf(stderr, "fopen %s failed\n", PublicKeyFile);
            return -1;
        }
        //*rsa_pub_key = PEM_read_RSAPublicKey(fp, NULL, NULL, NULL);
        *rsa_pub_key = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
        fclose(fp);

        ret = 0;
    }while(0);

    return ret;
}

int make_keys_by_file(RSA **rsa_pri_key, RSA **rsa_pub_key, char *PrivateKeyFile, char *PublicKeyFile)
{
    if( !rsa_pri_key || !rsa_pub_key || !PrivateKeyFile || !PublicKeyFile )
        return -1;

    int ret = -1;
    do{

        ret = make_private_key_by_file(rsa_pri_key, PrivateKeyFile);
        if( 0 != ret )
        {
            ret = -1;
            break;
        }

        ret = make_public_key_by_file(rsa_pub_key, PublicKeyFile);
        if( 0 != ret )
        {
            ret = -1;
            break;
        }

        ret = 0;

    }while(0);

    return ret;
}

int rsa_public_encryption(RSA **rsa_pub_key, unsigned char *msg, unsigned int msg_len, unsigned char *encryBuf)
{
    unsigned int rsa_key_len = 0;
    unsigned int padding = RSA_PKCS1_PADDING;
    unsigned int rsa_return = 0;

    //因为加密后的数据的长度必须等于密钥的长度，这在生成密钥的时候就决定了
    rsa_key_len = RSA_size(*rsa_pub_key);
    if( msg_len > (rsa_key_len - padding) )
    {
        fprintf(stderr, "this msg is too long, this rsa key can't encryption all of the data\n");
        return -1;
    }

    //最后一个参数是填充方式，因为有时候数据不够长度的时候，就会根据填充方式进行数据填充，所以加密和解密的填充方式一定要一致。
    //第一个参数：待加密的数据的长度
    //返回值，是加密后的长度，可以作为转换成16进制时的长度使用，否则直接strlen计算不出长度的，也可以作为解密时的第一个参数
    rsa_return = RSA_public_encrypt(msg_len, msg, encryBuf, *rsa_pub_key, padding);
    if( rsa_return != rsa_key_len )
    {
        fprintf(stderr, "RSA_public_encrypt failed : Error Code : %s\n", ERR_get_error());
        return -1;
    }
    return 0;
}

int rsa_private_encryption(RSA **rsa_pri_key, unsigned char *msg, unsigned int msg_len, unsigned char *encryBuf)
{
    unsigned int rsa_key_len = 0;
    unsigned int padding = RSA_PKCS1_PADDING;
    unsigned int rsa_return = 0;

    //因为加密后的数据的长度必须等于密钥的长度，这在生成密钥的时候就决定了
    rsa_key_len = RSA_size(*rsa_pri_key);
    if( msg_len > (rsa_key_len - padding) )
    {
        fprintf(stderr, "this msg is too long, this rsa key can't encryption all of the data\n");
        return -1;
    }

    //最后一个参数是填充方式，因为有时候数据不够长度的时候，就会根据填充方式进行数据填充，所以加密和解密的填充方式一定要一致。
    //第一个参数：待加密的数据的长度
    //返回值，是加密后的长度，可以作为转换成16进制时的长度使用，否则直接strlen计算不出长度的，也可以作为解密时的第一个参数
    rsa_return = RSA_private_encrypt(msg_len, msg, encryBuf, *rsa_pri_key, padding);
    if( rsa_return != rsa_key_len )
    {
        fprintf(stderr, "RSA_public_encrypt failed : Error Code : %s\n", ERR_get_error());
        return -1;
    }
    return 0;
}

int rsa_private_decryption(RSA **rsa_pri_key, unsigned char *ByteBuf, unsigned char *sourdata)
{
    unsigned int rsa_key_len = 0;
    unsigned int padding = RSA_PKCS1_PADDING;
    unsigned int rsa_return = 0;

    //因为加密后的数据的长度必须等于密钥的长度，这在生成密钥的时候就决定了
    rsa_key_len = RSA_size(*rsa_pri_key);
    rsa_return = RSA_private_decrypt(rsa_key_len, ByteBuf, sourdata, *rsa_pri_key, padding);
    if( 0 > rsa_return )
    {
        fprintf(stderr, "RSA_public_encrypt failed : Error Code : %s\n", ERR_get_error());
        return -1;
    }

    return 0;
}

int rsa_public_decryption(RSA **rsa_pub_key, unsigned char *ByteBuf, unsigned char *sourdata)
{
    unsigned int rsa_key_len = 0;
    unsigned int padding = RSA_PKCS1_PADDING;
    unsigned int rsa_return = 0;

    //因为加密后的数据的长度必须等于密钥的长度，这在生成密钥的时候就决定了
    rsa_key_len = RSA_size(*rsa_pub_key);
    rsa_return = RSA_public_decrypt(rsa_key_len, ByteBuf, sourdata, *rsa_pub_key, padding);
    if( 0 > rsa_return )
    {
        fprintf(stderr, "RSA_public_encrypt failed : Error Code : %s\n", ERR_get_error());
        return -1;
    }

    return 0;
}

void rsa_free(RSA *rsa)
{
    if( rsa )
        RSA_free(rsa);
}
