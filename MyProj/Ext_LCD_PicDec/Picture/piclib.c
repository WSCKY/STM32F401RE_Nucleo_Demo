#include "piclib.h"

_pic_info picinfo;	/* ͼƬ��Ϣ */
_pic_phy pic_phy;	/* ͼƬ��ʾ����ӿ� */

UINT br;

/* �ļ������б� */
const uint8_t *FILE_TYPE_TBL[6][13]=
{
{"BIN"},			//BIN�ļ�
{"LRC"},			//LRC�ļ�
{"NES"},			//NES�ļ�
{"TXT","C","H"},	//�ı��ļ�
{"MP1","MP2","MP3","MP4","M4A","3GP","3G2","OGG","ACC","WMA","WAV","MID","FLAC"},//�����ļ�
{"BMP","JPG","JPEG","GIF"},//ͼƬ�ļ�
};

/* �����ߺ��� */
void piclib_draw_hline(uint16_t x0,uint16_t y0,uint16_t len,uint16_t color)
{
	uint8_t i = 0;
	LCD_SetRamAddr(x0, x0 + len - 1, y0, y0);
	for(i = 0; i < len; i++)
		WriteColor(color);
}
/* ��亯�� */
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{
	uint16_t Index;
	LCD_SetRamAddr(sx, ex, sy, ey);
	for(Index = ((ex - sx + 1) * (ey - sy + 1)); Index > 0; Index --)
		WriteColor(color);
}

/**
  * @brief  ��ͼ��ʼ��,�ڻ�ͼ֮ǰ,�����ȵ��ô˺���(ָ������/���㺯��)
  * @param  None
  * @retval None
  */
void piclib_init(void)
{
	pic_phy.read_point = ReadPoint;  			//���㺯��ʵ��
	pic_phy.draw_point = DrawPiont;				//���㺯��ʵ��
	pic_phy.fill = LCD_Fill;					//��亯��ʵ��
	pic_phy.draw_hline = piclib_draw_hline;  	//���ߺ���ʵ��

	picinfo.ImgWidth = 0;	//��ʼ�����Ϊ0
	picinfo.ImgHeight = 0;	//��ʼ���߶�Ϊ0
	picinfo.Div_Fac = 0;	//��ʼ������ϵ��Ϊ0
	picinfo.S_Height = 0;	//��ʼ���趨�ĸ߶�Ϊ0
	picinfo.S_Width = 0;	//��ʼ���趨�Ŀ��Ϊ0
	picinfo.S_XOFF = 0;		//��ʼ��x���ƫ����Ϊ0
	picinfo.S_YOFF = 0;		//��ʼ��y���ƫ����Ϊ0
	picinfo.staticx = 0;	//��ʼ����ǰ��ʾ����x����Ϊ0
	picinfo.staticy = 0;	//��ʼ����ǰ��ʾ����y����Ϊ0
}

/**
  *����ALPHA BLENDING�㷨.
  *src:Դ��ɫ
  *dst:Ŀ����ɫ
  *alpha:͸���̶�(0~32)
  *����ֵ:��Ϻ����ɫ.
  */
uint16_t piclib_alpha_blend(uint16_t src,uint16_t dst,uint8_t alpha)
{
	uint32_t src2;
	uint32_t dst2;	 
	/* Convert to 32bit |-----GGGGGG-----RRRRR------BBBBB| */
	src2 = ((src<<16)|src)&0x07E0F81F;
	dst2 = ((dst<<16)|dst)&0x07E0F81F;   
	/*--Perform blending R:G:B with alpha in range 0..32
	Note that the reason that alpha may not exceed 32 is that there are only
	5bits of space between each R:G:B value, any higher value will overflow
	into the next component and deliver ugly result.--*/
	dst2 = ((((dst2 - src2) * alpha)>>5) + src2)&0x07E0F81F;
	return (dst2>>16)|dst2;  
}

/**
  * @brief  ��ʼ�����ܻ���(�ڲ�����)
  *	@param  None
  * @retval None
  */
void ai_draw_init(void)
{
	float temp,temp1;	   
	temp = (float)picinfo.S_Width / picinfo.ImgWidth;
	temp1 = (float)picinfo.S_Height / picinfo.ImgHeight;						 
	if(temp < temp1) temp1 = temp;/* ȡ��С������ */	 
	if(temp1 > 1) temp1 = 1;	  
	/* ʹͼƬ��������������м� */
	picinfo.S_XOFF += (picinfo.S_Width - temp1 * picinfo.ImgWidth) / 2;
	picinfo.S_YOFF += (picinfo.S_Height - temp1 * picinfo.ImgHeight) / 2;
	temp1 *= 8192;/* ����8192�� */	 
	picinfo.Div_Fac = temp1;
	picinfo.staticx = 0xffff;
	picinfo.staticy = 0xffff;/* �ŵ�һ�������ܵ�ֵ���� */			 										    
}

/**
  * @brief  �ж���������Ƿ������ʾ
  * @param  (x,y) :����ԭʼ����
  * @param  chg   :���ܱ���. 
  * @retval 0,����Ҫ��ʾ
  *			1,��Ҫ��ʾ
  */
uint8_t is_element_ok(uint16_t x,uint16_t y,uint8_t chg)
{
	if(x!=picinfo.staticx||y!=picinfo.staticy)
	{
		if(chg==1)
		{
			picinfo.staticx=x;
			picinfo.staticy=y;
		}
		return 1;
	}
	else return 0;
}

/**
  * @brief  ��Сд��ĸת��Ϊ��д��ĸ�����ֲ���
  * @param  Ҫת������
  * @retval ת�����
  */
uint8_t char_upper(uint8_t c)
{
	if(c<'A')return c;//����,���ֲ���.
	if(c>='a')return c-0x20;//��Ϊ��д.
	else return c;//��д,���ֲ���
}

/**
  * @brief  �����ļ�������
  * @param  fname:�ļ���
  * @retval 0XFF,��ʾ�޷�ʶ����ļ����ͱ��
  *			others,����λ��ʾ��������,����λ��ʾ����С��
  */
uint8_t f_typetell(uint8_t *fname)
{
	uint8_t tbuf[5];
	uint8_t *attr='\0';//��׺��
	uint8_t i=0,j;
	while(i<250)
	{
		i++;
		if(*fname=='\0')break;//ƫ�Ƶ��������.
		fname++;
	}
	if(i==250)return 0XFF;//������ַ���.
 	for(i=0;i<5;i++)//�õ���׺��
	{
		fname--;
		if(*fname=='.')
		{
			fname++;
			attr=fname;
			break;
		}
  	}
	strcpy((char *)tbuf,(const char*)attr);//copy
 	for(i=0;i<4;i++)tbuf[i]=char_upper(tbuf[i]);//ȫ����Ϊ��д 
	for(i=0;i<6;i++)
	{
		for(j=0;j<13;j++)
		{
			if(*FILE_TYPE_TBL[i][j]==0)break;//�����Ѿ�û�пɶԱȵĳ�Ա��.
			if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//�ҵ���
			{
				return (i<<4)|j;
			}
		}
	}
	return 0XFF;	 			   
}

/**
  *��ʼ��ͼ��֧�������ʽͼƬ�ļ� 
  *FileName:Ҫ��ʾ��ͼƬ�ļ�  BMP/JPG/JPEG/GIF
  *x,y,width,height:���꼰��ʾ����ߴ�
  *acolor  :alphablend����ɫ(���Բ�����128*160��32λbmp��Ч!)
  *abdnum  :alphablend��ֵ(0~32��Ч,����ֵ��ʾ��ʹ��alphablend,���Բ�����128*160��32λbmp��Ч!)
  *ͼƬ�ڿ�ʼ�ͽ���������㷶Χ����ʾ
  */
uint8_t ai_load_picfile(const uint8_t *filename,uint16_t x,uint16_t y,uint16_t width,uint16_t height)
{	
	uint8_t res;
	uint8_t temp;	
	if((x + width) > LCD_PIXEL_WIDTH)return PIC_WINDOW_ERR;		//x���곬��Χ��.
	if((y + height) > LCD_PIXEL_HEIGHT)return PIC_WINDOW_ERR;		//y���곬��Χ��.  
	if(width==0||height==0)return PIC_WINDOW_ERR;	//�����趨����	  	 
	//�õ���ʾ�����С
	picinfo.S_Height=height;
	picinfo.S_Width=width;
	//��ʾ������Ч
	if(picinfo.S_Height==0||picinfo.S_Width==0)
	{
		picinfo.S_Height=LCD_PIXEL_HEIGHT;
		picinfo.S_Width=LCD_PIXEL_WIDTH;
		return FALSE;   
	}
	//��ʾ�Ŀ�ʼ�����
	picinfo.S_YOFF=y;
	picinfo.S_XOFF=x;
	//�ļ�������		 
	temp=f_typetell((uint8_t*)filename);	//�õ��ļ�������
	switch(temp)
	{											  
		case T_BMP:
			res=stdbmp_decode(filename); //����bmp	  	  
			break;
		case T_JPG:
		case T_JPEG:
			res=jpg_decode(filename); 					//����JPG/JPEG	  	  
			break;
		case T_GIF:
			res=gif_decode(filename,x,y,width,height);	//����gif  	  
			break;
		default:
	 		res=PIC_FORMAT_ERR;  						//��ͼƬ��ʽ!!!  
			break;
	}  											   
	return res;
}
