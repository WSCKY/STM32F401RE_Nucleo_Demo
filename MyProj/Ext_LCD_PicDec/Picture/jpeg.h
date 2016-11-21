#ifndef __JPEG_H__
#define __JPEG_H__

#include "stm32f4xx_hal.h"

#define M_SOF0  0xc0
#define M_DHT   0xc4
#define M_EOI   0xd9
#define M_SOS   0xda
#define M_DQT   0xdb
#define M_DRI   0xdd
#define M_APP0  0xe0  	  


#define W1 		2841 		// 2048*sqrt(2)*cos(1*pi/16) 
#define W2 		2676 		// 2048*sqrt(2)*cos(2*pi/16)
#define W3 		2408 		// 2048*sqrt(2)*cos(3*pi/16) 
#define W5 		1609 		// 2048*sqrt(2)*cos(5*pi/16) 
#define W6 		1108 		// 2048*sqrt(2)*cos(6*pi/16)
#define W7 		565  		// 2048*sqrt(2)*cos(7*pi/16)
												 
	  					    		  
#define MAKEWORD(a, b)		((WORD)(((uint8_t)(a)) | ((WORD)((uint8_t)(b))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOuint8_t(w)           	((uint8_t)(w))
#define HIuint8_t(w)           	((uint8_t)(((WORD)(w) >> 8) & 0xFF))
	   
//宏定义					   
#define WIDTHuint8_tS(i)    ((i+31)/32*4)
#define PI 3.1415926535				

//jpeg解码函数返回值定义
#define FUNC_OK 0
#define FUNC_MEMORY_ERROR 1
#define FUNC_FILE_ERROR 2
#define FUNC_FORMAT_ERROR 3

uint8_t jpg_decode(const uint8_t *filename);					//解码某个jpeg/jpg文件
int  jpeg_inittag(void);
void jpeg_inittable(void);					   		//初始化数据表
int  jpeg_decode(void);							   	//解码
int  jpeg_decodemcublock(void);
int  jpeg_hufblock(uint8_t dchufindex,uint8_t achufindex);	//哈夫曼解码
int  jpeg_decodeelement(void);					  	//解码一个像素
void jpeg_iqtizzmcucomponent(short flag);		   	//反量化
void jpeg_iqtizzblock(short  *s ,short * d,short flag);
void jpeg_getyuv(short flag);		 				//色彩转换的实现,得到色彩空间数据
void jpeg_storebuffer(void);
uint8_t   jpeg_readuint8_t(void);             				//从文件里面读取一个字节出来
void jpeg_initfastidct(void); 						//初始化反离散傅立叶变换
void jpeg_fastidct(int * block);	 				//快速反离散傅立叶变换
void jpeg_idctrow(int * blk);
void jpeg_idctcol(int * blk);
void jpg_seek(uint8_t *pbase, uint8_t *(*pnow));  									    	    

#endif
