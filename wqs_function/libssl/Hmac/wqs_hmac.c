#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "../e_os.h"

#include <openssl/hmac.h>
#include <openssl/md5.h>

#include <openssl/ebcdic.h>

#include <libwqs/wqs_character.h>

unsigned char msg[] = "this is a simple message";
unsigned char Hmac_key[] = "this is the hmac key";

int method_1()
{
    int i = 0;
    unsigned char HexBuff[1024] = {0};
    unsigned char *ByteBuff = NULL;

    memset(HexBuff, 0x00, sizeof(HexBuff));

    ByteBuff = HMAC(EVP_md5(), Hmac_key, strlen(Hmac_key), msg, strlen(msg), NULL, NULL);
    Byte2Hex(ByteBuff, MD5_DIGEST_LENGTH, HexBuff, sizeof(HexBuff));

    printf("HexBuff --> %d [ %s ]\n", strlen(HexBuff), HexBuff);

    return 0;
}

int method_2()
{
    int ret = -1;

    HMAC_CTX ctx;
    unsigned char HexBuff[1024] = {0};
    unsigned int len;
    unsigned char ByteBuff[1024] = {0};
    
    memset(ByteBuff, 0x00, sizeof(ByteBuff));
    memset(HexBuff, 0x00, sizeof(HexBuff));

    HMAC_CTX_init(&ctx);

    do{
        if (!HMAC_Init_ex(&ctx, Hmac_key, strlen(Hmac_key), EVP_md5(), NULL)) {
        //if (!HMAC_Init_ex(&ctx, Hmac_key, strlen(Hmac_key), EVP_sha256(), NULL)) {
            printf("Failed to reinitialise HMAC with key (test 5)\n");
            break;
        }
        if (!HMAC_Update(&ctx, msg, strlen(msg))) {
            printf("Error updating HMAC with data (new key) (test 5)\n");
            break;
        }
        if (!HMAC_Final(&ctx, ByteBuff, &len)) {
            printf("Error finalising data (new key) (test 5)\n");
            break;
        }
        Byte2Hex(ByteBuff, len, HexBuff, sizeof(HexBuff));
        printf("HexBuff --> %d [ %s ]\n", strlen(HexBuff), HexBuff);

    }while(0);
    printf("msg : %s, hmac key : %s\n", msg, Hmac_key);
    HMAC_CTX_cleanup(&ctx);

}
