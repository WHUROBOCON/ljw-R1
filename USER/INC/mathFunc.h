#ifndef __MATHFUNC_H
#define __MATHFUNC_H

#include "stm32f4xx.h"
#include "stdbool.h"
#include <stdlib.h>
#include <string.h> 

#define ABS(x)  ((x>0)? (x):(-(x)))
#define PlusOrMinus(x) ((x>0)? 1: -1 )
#define PEAK(A,B)   if(ABS(A)>ABS(B)) A=PlusOrMinus(A)*B; 


#define EncodeS32Data(f,buff)	{*(s32*)buff = *f;}
#define DecodeS32Data(f,buff)	{*f = *(s32 *)buff;}
#define EncodeS16Data(f,buff)	{*(s16*)buff = *f;}
#define DecodeS16Data(f,buff)	{*f = *(s16*)buff;}
#define EncodeU16Data(f,buff)	{*(u16*)buff = *f;}
#define DecodeU16Data(f,buff)	{*f = *(u16*)buff;}

void Byte2ToS16(s16 *f, u8 *buff);

void BYTE3TOs16(int16_t* f, unsigned char* buff);

void Char2float(float* f, unsigned char* buff);
void Char2s16(int16_t* f, unsigned char* buff);

void ChangeData(u8* buff2,u8* buff1);
void EcodeFloatData_to_4byte(float *f, u8 *buff);
void DecodeFloatData_From_4byte(float *f, u8 *buff);
void buffer_append_int32(uint8_t *buffer, int32_t number, int32_t *index);
s32 get_s32_from_buffer(const u8 *buffer, s32 *index);
s16 get_s16_from_buffer(const u8 *buffer, s32 *index);
float buffer_32_to_float(const uint8_t *buffer, float scale, int32_t *index);
float buffer_16_to_float(const uint8_t *buffer, float scale, int32_t *index);
void buffer32tofloat(const uint8_t *buffer, float *f);

int utils_truncate_number(float *number, float min, float max);


#endif
