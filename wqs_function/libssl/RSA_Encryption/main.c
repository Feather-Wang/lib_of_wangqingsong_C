#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wqs_rsa.h"
#include <openssl/rsa.h>

int test_rsa_encryption()
{
    RSA *rsa_pri_key = NULL;
    RSA *rsa_pub_key = NULL;

    unsigned char encryDataByte[1024] = {0};
    unsigned char decryDataString[1024] = {0};
    unsigned int decryStringLen = 0;

    /****************** create RSA Key **********************/
    puts("make_keys_by_create");
    make_keys_by_create(&rsa_pri_key, &rsa_pub_key);
    /***********************************************************/


    unsigned char msg[1024] = {0};
    int i = 0;
    for(i = 0; i < 11; ++i)
        msg[i] = 'A' + i / 10;

    /********************* encrypted msg ********************************/
    printf("rsa_encryption msg %d ( rsa_len:%d)--> %s\n", strlen(msg), RSA_size(rsa_pub_key), msg);
    //if( 0 != rsa_public_encryption(&rsa_pub_key, msg, strlen(msg), encryDataByte) )
    if( 0 != rsa_private_encryption(&rsa_pri_key, msg, strlen(msg), encryDataByte) )
    {
        return -1;
    }
    /*********************************************************************/
    
    /******************** decrypted msg ****************************************/
    puts("rsa_decryption");
    //rsa_private_decryption(&rsa_pri_key, encryDataByte, decryDataString);
    rsa_public_decryption(&rsa_pub_key, encryDataByte, decryDataString);
    /*************************************************************************/

    printf("decryDataString --> %s\n", decryDataString);

    if( rsa_pri_key )
        rsa_free( rsa_pri_key );
    if( rsa_pub_key )
        rsa_free( rsa_pub_key );

    return 0;
}

int main(int argc, const char *argv[])
{
    test_rsa_encryption();     
    return 0;
}
