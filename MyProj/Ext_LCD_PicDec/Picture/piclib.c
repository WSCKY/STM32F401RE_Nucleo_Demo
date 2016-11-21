#include "piclib.h"

_pic_info picinfo;	/* 图片信息 */
_pic_phy pic_phy;	/* 图片显示物理接口 */

UINT br;

/* 文件类型列表 */
const uint8_t *FILE_TYPE_TBL[6][13]=
{
{"BIN"},			//BIN文件
{"LRC"},			//LRC文件
{"NES"},			//NES文件
{"TXT","C","H"},	//文本文件
{"MP1","MP2","MP3","MP4","M4A","3GP","3G2","OGG","ACC","WMA","WAV","MID","FLAC"},//音乐文件
{"BMP","JPG","JPEG","GIF"},//图片文件
};

/* 画横线函数 */
void piclib_draw_hline(uint16_t x0,uint16_t y0,uint16_t len,uint16_t color)
{
	uint8_t i = 0;
	LCD_SetRamAddr(x0, x0 + len - 1, y0, y0);
	for(i = 0; i < len; i++)
		WriteColor(color);
}
/* 填充函数 */
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{
	uint16_t Index;
	LCD_SetRamAddr(sx, ex, sy, ey);
	for(Index = ((ex - sx + 1) * (ey - sy + 1)); Index > 0; Index --)
		WriteColor(color);
}

/**
  * @brief  画图初始化,在画图之前,必须先调用此函数(指定画点/读点函数)
  * @param  None
  * @retval None
  */
void piclib_init(void)
{
	pic_phy.read_point = ReadPoint;  			//读点函数实现
	pic_phy.draw_point = DrawPiont;				//画点函数实现
	pic_phy.fill = LCD_Fill;					//填充函数实现
	pic_phy.draw_hline = piclib_draw_hline;  	//画线函数实现

	picinfo.ImgWidth = 0;	//初始化宽度为0
	picinfo.ImgHeight = 0;	//初始化高度为0
	picinfo.Div_Fac = 0;	//初始化缩放系数为0
	picinfo.S_Height = 0;	//初始化设定的高度为0
	picinfo.S_Width = 0;	//初始化设定的宽度为0
	picinfo.S_XOFF = 0;		//初始化x轴的偏移量为0
	picinfo.S_YOFF = 0;		//初始化y轴的偏移量为0
	picinfo.staticx = 0;	//初始化当前显示到的x坐标为0
	picinfo.staticy = 0;	//初始化当前显示到的y坐标为0
}

/**
  *快速ALPHA BLENDING算法.
  *src:源颜色
  *dst:目标颜色
  *alpha:透明程度(0~32)
  *返回值:混合后的颜色.
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
  * @brief  初始化智能画点(内部调用)
  *	@param  None
  * @retval None
  */
void ai_draw_init(void)
{
	float temp,temp1;	   
	temp = (float)picinfo.S_Width / picinfo.ImgWidth;
	temp1 = (float)picinfo.S_Height / picinfo.ImgHeight;						 
	if(temp < temp1) temp1 = temp;/* 取较小的数据 */	 
	if(temp1 > 1) temp1 = 1;	  
	/* 使图片处于所给区域的中间 */
	picinfo.S_XOFF += (picinfo.S_Width - temp1 * picinfo.ImgWidth) / 2;
	picinfo.S_YOFF += (picinfo.S_Height - temp1 * picinfo.ImgHeight) / 2;
	temp1 *= 8192;/* 扩大8192倍 */	 
	picinfo.Div_Fac = temp1;
	picinfo.staticx = 0xffff;
	picinfo.staticy = 0xffff;/* 放到一个不可能的值上面 */			 										    
}

/**
  * @brief  判断这个像素是否可以显示
  * @param  (x,y) :像素原始坐标
  * @param  chg   :功能变量. 
  * @retval 0,不需要显示
  *			1,需要显示
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
  * @brief  将小写字母转换为大写字母，数字不变
  * @param  要转换的量
  * @retval 转换结果
  */
uint8_t char_upper(uint8_t c)
{
	if(c<'A')return c;//数字,保持不变.
	if(c>='a')return c-0x20;//变为大写.
	else return c;//大写,保持不变
}

/**
  * @brief  报告文件的类型
  * @param  fname:文件名
  * @retval 0XFF,表示无法识别的文件类型编号
  *			others,高四位表示所属大类,低四位表示所属小类
  */
uint8_t f_typetell(uint8_t *fname)
{
	uint8_t tbuf[5];
	uint8_t *attr='\0';//后缀名
	uint8_t i=0,j;
	while(i<250)
	{
		i++;
		if(*fname=='\0')break;//偏移到了最后了.
		fname++;
	}
	if(i==250)return 0XFF;//错误的字符串.
 	for(i=0;i<5;i++)//得到后缀名
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
 	for(i=0;i<4;i++)tbuf[i]=char_upper(tbuf[i]);//全部变为大写 
	for(i=0;i<6;i++)
	{
		for(j=0;j<13;j++)
		{
			if(*FILE_TYPE_TBL[i][j]==0)break;//此组已经没有可对比的成员了.
			if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//找到了
			{
				return (i<<4)|j;
			}
		}
	}
	return 0XFF;	 			   
}

/**
  *开始画图，支持任意格式图片文件 
  *FileName:要显示的图片文件  BMP/JPG/JPEG/GIF
  *x,y,width,height:坐标及显示区域尺寸
  *acolor  :alphablend的颜色(仅对不大于128*160的32位bmp有效!)
  *abdnum  :alphablend的值(0~32有效,其余值表示不使用alphablend,仅对不大于128*160的32位bmp有效!)
  *图片在开始和结束的坐标点范围内显示
  */
uint8_t ai_load_picfile(const uint8_t *filename,uint16_t x,uint16_t y,uint16_t width,uint16_t height)
{	
	uint8_t res;
	uint8_t temp;	
	if((x + width) > LCD_PIXEL_WIDTH)return PIC_WINDOW_ERR;		//x坐标超范围了.
	if((y + height) > LCD_PIXEL_HEIGHT)return PIC_WINDOW_ERR;		//y坐标超范围了.  
	if(width==0||height==0)return PIC_WINDOW_ERR;	//窗口设定错误	  	 
	//得到显示方框大小
	picinfo.S_Height=height;
	picinfo.S_Width=width;
	//显示区域无效
	if(picinfo.S_Height==0||picinfo.S_Width==0)
	{
		picinfo.S_Height=LCD_PIXEL_HEIGHT;
		picinfo.S_Width=LCD_PIXEL_WIDTH;
		return FALSE;   
	}
	//显示的开始坐标点
	picinfo.S_YOFF=y;
	picinfo.S_XOFF=x;
	//文件名传递		 
	temp=f_typetell((uint8_t*)filename);	//得到文件的类型
	switch(temp)
	{											  
		case T_BMP:
			res=stdbmp_decode(filename); //解码bmp	  	  
			break;
		case T_JPG:
		case T_JPEG:
			res=jpg_decode(filename); 					//解码JPG/JPEG	  	  
			break;
		case T_GIF:
			res=gif_decode(filename,x,y,width,height);	//解码gif  	  
			break;
		default:
	 		res=PIC_FORMAT_ERR;  						//非图片格式!!!  
			break;
	}  											   
	return res;
}
