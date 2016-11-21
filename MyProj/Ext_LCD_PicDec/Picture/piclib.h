#ifndef __PICLIB_H
#define __PICLIB_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_ExtLCD.h"
#include "ff.h"
#include "bmp.h"
#include "jpeg.h"
#include "gif.h"
#include "string.h"

typedef char XCHAR;

#define f_tell(fp) ((fp)->fptr)
#define f_size(fp) ((fp)->fsize)

#define PIC_FORMAT_ERR		0x27	//��ʽ����
#define PIC_SIZE_ERR		0x28	//ͼƬ�ߴ����
#define PIC_WINDOW_ERR		0x29	//�����趨����
#define PIC_MEM_ERR			0x11	//�ڴ����

//f_typetell���ص����Ͷ���
//���ݱ�FILE_TYPE_TBL���.��exfuns.c���涨��
#define T_BIN		0X00	//bin�ļ�
#define T_LRC		0X10	//lrc�ļ�
#define T_NES		0X20	//nes�ļ�
#define T_TEXT		0X30	//.txt�ļ�
#define T_C			0X31	//.c�ļ�
#define T_H			0X32    //.h�ļ�
#define T_FLAC		0X4C	//flac�ļ�
#define T_BMP		0X50	//bmp�ļ�
#define T_JPG		0X51	//jpg�ļ�
#define T_JPEG		0X52	//jpeg�ļ�		 
#define T_GIF		0X53	//gif�ļ�

extern UINT br;

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

//ͼƬ��ʾ�����ӿ�  
//����ֲ��ʱ��,�������û��Լ�ʵ���⼸������
typedef struct 
{
	uint16_t(*read_point)(uint8_t,uint8_t);			//uint16_t read_point(uint16_t x,uint16_t y)						���㺯��
	void(*draw_point)(uint8_t,uint8_t,uint16_t);		//void draw_point(uint16_t x,uint16_t y,uint16_t color)		    ���㺯��
 	void(*fill)(uint16_t,uint16_t,uint16_t,uint16_t,uint16_t);	///void fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color) ��ɫ��亯�� 	 
 	void(*draw_hline)(uint16_t,uint16_t,uint16_t,uint16_t);	//void draw_hline(uint16_t x0,uint16_t y0,uint16_t len,uint16_t color)  ��ˮƽ�ߺ���	 
}_pic_phy; 
extern _pic_phy pic_phy;

//ͼ����Ϣ
typedef struct
{			 
	uint32_t ImgWidth; 	//ͼ���ʵ�ʿ�Ⱥ͸߶�
	uint32_t ImgHeight;

	uint32_t Div_Fac;  	//����ϵ�� (������8192����)
	
	uint32_t S_Height; 	//�趨�ĸ߶ȺͿ��
	uint32_t S_Width;
	
	uint32_t	S_XOFF;	  	//x���y���ƫ����
	uint32_t S_YOFF;

	uint32_t staticx; 	//��ǰ��ʾ���ģ�������
	uint32_t staticy;																 	
}_pic_info;
extern _pic_info picinfo;//ͼ����Ϣ

void piclib_init(void);													//��ʼ����ͼ
uint16_t piclib_alpha_blend(uint16_t src,uint16_t dst,uint8_t alpha);						//alphablend����
void ai_draw_init(void);												//��ʼ�����ܻ�ͼ
uint8_t is_element_ok(uint16_t x,uint16_t y,uint8_t chg);									//�ж������Ƿ���Ч
uint8_t char_upper(uint8_t c);
uint8_t f_typetell(uint8_t *fname);
uint8_t ai_load_picfile(const uint8_t *filename,uint16_t x,uint16_t y,uint16_t width,uint16_t height);//���ܻ�ͼ

#endif
