#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/md5.h>

#include <wqs_character.h>

#include "wqs_sha.h"

typedef unsigned char u_char;

u_char msg[] = "this is a simple message";

void Byte2Hex(const unsigned char *ByteBuf, unsigned int ByteLen, unsigned char* HexBuf, unsigned int HexSize)
{
    int i;
    for(i=0; i < ByteLen; ++i)
        snprintf(&(HexBuf[i*2]), HexSize-1, "%02X", ByteBuf[i]);
}

int indexa = 0;
int method_first()
{
    u_char ByteBuff[1024] = {0};
    u_char HexBuff[1024] = {0};

    EVP_Digest(msg, strlen(msg), ByteBuff, NULL, EVP_md5(), NULL);

    Byte2Hex(ByteBuff, MD5_DIGEST_LENGTH, HexBuff, sizeof(HexBuff));
    printf("md5 --> %d : %d [ %s ]\n", indexa++, strlen(HexBuff), HexBuff);
    if( strlen(HexBuff) != 32 )
        exit(0);

    return 0;
}



int method_second()
{
    EVP_MD_CTX c;
    u_char ByteBuff[1024] = {0};
    u_char HexBuff[1024] = {0};

    EVP_MD_CTX_init(&c);

    EVP_DigestInit_ex(&c, EVP_sha256(), NULL);
    EVP_DigestUpdate(&c, msg, strlen(msg));
    EVP_DigestFinal_ex(&c, ByteBuff, NULL);
    
    EVP_MD_CTX_cleanup(&c);

    Byte2Hex(ByteBuff, SHA256_DIGEST_LENGTH, HexBuff, sizeof(HexBuff));
    printf("SHA --> %d [ %s ]\n", strlen(HexBuff), HexBuff);

    return 0;
}

int method_third()
{
    u_char ByteBuff[1024] = {0};
    u_char HexBuff[1024] = {0};

    MD5(msg, strlen(msg), ByteBuff);
    
    Byte2Hex(ByteBuff, MD5_DIGEST_LENGTH, HexBuff, sizeof(HexBuff));
    printf("md5 --> %d [ %s ]\n", strlen(HexBuff), HexBuff);

    return 0;

}
