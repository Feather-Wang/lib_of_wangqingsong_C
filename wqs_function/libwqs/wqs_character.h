#ifndef __WQS_CHARACTER_H__
#define __WQS_CHARACTER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void Byte2Hex(const unsigned char *ByteBuf, unsigned int ByteLen, unsigned char* HexBuf, unsigned int HexSize);
void Hex2Byte(const unsigned char *HexBuf, unsigned int HexLen, unsigned char* ByteBuf, unsigned int ByteSize);

#endif
