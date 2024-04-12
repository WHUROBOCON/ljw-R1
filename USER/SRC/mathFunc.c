#include "mathFunc.h"

void Byte2ToS16(s16 *f, u8 *buff)
{
    *(f) = *(s16 *)(buff);
}

int utils_truncate_number(float *number, float min, float max) {
	int did_trunc = 0;

	if (*number > max) {
		*number = max;
		did_trunc = 1;
	} else if (*number < min) {
		*number = min;
		did_trunc = 1;
	}

	return did_trunc;
}


void Char2float(float* f, unsigned char* buff)
{
	int i;
	char *tmp = (char *)f;
	for (i = 0; i < 4; i++)
		tmp[i] = buff[i]; 
}

void BYTE3TOs16(int16_t* f, unsigned char* buff)
{
	int i;
	char *tmp = (char *)f;
	for (i = 0; i < 3; i++)
		tmp[i] = buff[i]; //688
	
	
//	u32 a= (uint32_t)tmp[0]; 
//		u32 b= (uint32_t)tmp[1]; 
//	u32 c= (uint32_t)tmp[2]; 

	
	
		*f = ((uint32_t)tmp[0]) * 100 + ((uint32_t)tmp[1]) *10 + ((uint32_t)tmp[2]);
}

/* �ı�16λ���ݸߵ�˳�� */
void ChangeData(u8* buff2,u8* buff1)
 {
	u8 temp=0;
	temp= *buff1;
	*buff1 = *buff2;	 
	*buff2 = temp;
 }

/**
	*@brief:��float���͵����ݷŵ��ĸ��ֽ���
	*/
void EcodeFloatData_to_4byte(float *f, u8 *buff)
{									//		f��һ��ָ��float���͵�ִ��
	u16 f_c[2];
	*(float *)f_c = *f;		
//f_c��һ��ָ�� float����ָ�� ��ָ��
	
	buff[0] = (f_c[0] >> 0 & 0x00ff);
	buff[1] = ((f_c[0] >> 8) & 0x00ff);
	buff[2] = (f_c[1] >> 0 & 0x00ff);
	buff[3] = ((f_c[1] >> 8) & 0x00ff);
}
/**
	*@brief:�ĸ��ֽڵ�����ת��Ϊfloat����
	*/
void DecodeFloatData_From_4byte(float *f, u8 *buff)
{
	short int f_c[2];
	f_c[0] = *(short int *)buff;
	f_c[1] = *(short int *)(buff + 2);
	*f = *(float *)f_c;
}
/**
	*@brief:��number�ŵ�����buffer�У�number�ĸ�λ��Ӧbuffer�ĵ��ֽ�
	*/
void buffer_append_int32(uint8_t *buffer, int32_t number, int32_t *index)
{
	buffer[(*index)++] = number >> 24;
	buffer[(*index)++] = number >> 16;
	buffer[(*index)++] = number >> 8;
	buffer[(*index)++] = number;
}
/**
	*@brief:���ĸ��ֽ�u8���͵����ݺϳ�Ϊint���ͷ���
	*/
s32 get_s32_from_buffer(const u8 *buffer, s32 *index)
{
	s32 res = ((uint32_t)buffer[*index]) << 24 |
			  ((uint32_t)buffer[*index + 1]) << 16 |
			  ((uint32_t)buffer[*index + 2]) << 8 |
			  ((uint32_t)buffer[*index + 3]);
	*index += 4;
	return res;
}
/**
  *@brief:�������ֽ�u8���͵����ݺϳ�Ϊint���ͷ���
  */
s16 get_s16_from_buffer(const u8 *buffer, s32 *index)
{
	s16 res = ((uint32_t)buffer[*index]) << 8 |
			  ((uint32_t)buffer[*index + 1]);
	*index += 2;
	return res;
}
/**
	*@brief:���ĸ��ֽ�u8���͵����ݺϳ�Ϊfloat���ͳ���scale֮�󷵻�
	*/
float buffer_32_to_float(const uint8_t *buffer, float scale, int32_t *index)
{
	return (float)get_s32_from_buffer(buffer, index) / scale;
}
/**
	*@brief:�������ֽ�u8���͵����ݺϳ�Ϊfloat���ͳ���scale֮�󷵻�
	*/
float buffer_16_to_float(const uint8_t *buffer, float scale, int32_t *index)
{
	return (float)get_s16_from_buffer(buffer, index) / scale;
}


void buffer32tofloat(const uint8_t *buffer, float *f)
{
	memcpy(f, buffer, sizeof(float));
}
