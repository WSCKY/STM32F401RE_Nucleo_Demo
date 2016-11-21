#include "piclib.h"
#include "jpeg.h"

/* 反Z字形编码表 */
const int Zig_Zag[8][8]=
{
	{0,1,5,6,14,15,27,28},
	{2,4,7,13,16,26,29,42},
	{3,8,12,17,25,30,41,43},
	{9,11,18,24,31,40,44,53},
	{10,19,23,32,39,45,52,54},
	{20,22,33,38,46,51,55,60},
	{21,34,37,47,50,56,59,61},
	{35,36,48,49,57,58,62,63}
};
const uint8_t And[9]={0,1,3,7,0xf,0x1f,0x3f,0x7f,0xff};
/* 在JPEG函数里面用到的变量 */
short 	SampRate_Y_H,SampRate_Y_V;
short 	SampRate_U_H,SampRate_U_V;
short 	SampRate_V_H,SampRate_V_V;
short	H_YtoU,V_YtoU,H_YtoV,V_YtoV;
short 	Y_in_MCU,U_in_MCU,V_in_MCU;
short	BitPos,Curuint8_t;				//uint8_t的第几位,当前uint8_t
short	rrun,vvalue;
short	comp_num;
short 	restart;
short 	ycoef,ucoef,vcoef;
short	*YQtTable,*UQtTable,*VQtTable;
short 	interval = 0;
uint32_t	  	sizei,sizej;
uint8_t		comp_index[3];
uint8_t		YDcIndex,YAcIndex,UVDcIndex,UVAcIndex;
uint8_t		HufTabIndex;
uint8_t 		*lp;		/* 取代lpJpegBuf */
uint8_t	  	IntervalFlag;
long  	*iclp;		    

FIL		f_jtemp;
FIL 	*f_jpeg = &f_jtemp;
short	qt_table[3][64];
short	code_pos_table[4][16],code_len_table[4][16];
uint16_t		huf_max_value[4][16],huf_min_value[4][16];
short	MCUBuffer[10*64];    
short	QtZzMCUBuffer[10*64];
short	BlockBuffer[64];
short	Y[4*64],U[4*64],V[4*64];     
uint16_t		code_value_table[4][256];
long 	iclip[1024];					//4k字节缓存  
uint8_t 		jpg_buffer[1024];				//数据缓存区

/**
  *@brief  解码文件名为filename的jpeg/jpg文件
  *@param  file name
  *@retval The decoding results
  */	   
uint8_t jpg_decode(const uint8_t *filename)
{
	uint8_t res = 0;
	uint16_t byre;
	/* 得到JPEG/JPG图片的开始信息 */
	res = f_open(f_jpeg, (const XCHAR*)filename, FA_OPEN_EXISTING | FA_READ);/* 打开文件 */
	if(res == 0)
	{	 						  
		/* 开始时读入1024个字节到缓存里面.方便后面提取JPEG解码的信息 */
		f_read(f_jpeg, jpg_buffer, 1024, (UINT*)&byre);/* 读取文件 */							   
		jpeg_inittable();/* 初始化各个数据表 */
		res = jpeg_inittag();	 
		if(res == 0)
		{
			if((SampRate_Y_H != 0)&&(SampRate_Y_V != 0))
			{	  
				ai_draw_init();	/* 初始化picinfo.Div_Fac,启动智能画图 */	   
				res = jpeg_decode();/* 开始解码JPEG */
			}else res = 0XFD;/* 采样率错误 */	    
		}else res = 0XFE;/* 初始化表头失败 */
		f_close(f_jpeg); 	  
	}
	return res;
}

/**
  *@brief  移动文件指针
  *@param  *pbase:数据存储区首地址
  *		   **pnow:指向当前字节指针
  *@retval None
  */			    
void jpg_seek(uint8_t *pbase, uint8_t **pnow)
{
	uint32_t pos;
	uint16_t offset;
	offset = *pnow - pbase;/* 记录当前的偏移量 */
	if(offset > 1000)//将要结束了,做下一次读取
	{
		pos=f_tell(f_jpeg);				//得到当前位置
		f_lseek(f_jpeg, pos - 1024 + offset);//偏移
		f_read(f_jpeg,pbase, 1024, &br);	//读取1024个字节.
		*pnow = pbase;/* 复位 */
	}
}

/* 初始化d_buffer的数据 */  														 
int jpeg_inittag(void)
{
	uint8_t finish = FALSE;
	uint8_t id;
	short  llength;
	short  i,j,k;
	short  huftab1,huftab2;
	short  huftabindex = 0;
	uint8_t hf_table_index;
	uint8_t qt_table_index;
	uint8_t comnum;//最长为256个字节

	uint8_t  *lptemp;		 
	short  colorount;
		 			   	   	  
	lp = jpg_buffer + 2;/* 跳过两个字节SOI(0xFF，0xD8 Start of Image) */
	jpg_seek(jpg_buffer, &lp);			 
	while (!finish)
	{			 
		id = *(lp+1);//取出低位字节(高位在前，低位在后)
		lp += 2;     //跳过取出的字节
		jpg_seek(jpg_buffer, &lp);    	   
		switch(id)
		{
			case M_APP0: //JFIF APP0 segment marker (0xE0)
			             //标志应用数据段的开始 	    
				llength = MAKEWORD(*(lp+1),*lp);//得到应用数据段长度	
				lp += llength; 
				jpg_seek(jpg_buffer, &lp); 
				break;
			case M_DQT: //定义量化表标记(0xFF,0xDB) 
				llength = MAKEWORD(*(lp + 1), *lp);//(量化表长度)两个字节 
				qt_table_index = (*(lp + 2))&0x0f;//量化表信息bit 0..3: QT 号(0..3, 否则错误) 
											  //bit 4..7: QT 精度, 0 = 8 bit, 否则 16 bit
				lptemp = lp+3;  				  //n 字节的 QT, n = 64*(精度+1) 
				//d_buffer里面至少有有512个字节的余度,这里最大用到128个字节
				if(llength<80)				  //精度为 8 bit 
				{
					for(i = 0;i < 64;i ++)qt_table[qt_table_index][i] = (short)*(lptemp ++);  
				}
				else						  //精度为 16 bit 
				{
					for(i = 0;i < 64;i++)qt_table[qt_table_index][i] = (short)*(lptemp ++);   
	                qt_table_index = (*(lptemp ++))&0x0f;
	  				for(i = 0;i < 64;i++)qt_table[qt_table_index][i] = (short)*(lptemp ++);	    
	  			}		  
				lp += llength;  //跳过量化表
				jpg_seek(jpg_buffer, &lp); 
				break;
			case M_SOF0:	//帧开始 (baseline JPEG 0xFF,0xC0)
				/* 长度 (高字节, 低字节), 8+components*3	*/
		 		llength = MAKEWORD(*(lp+1), *lp);
				/* 图片高度 (高字节, 低字节), 如果不支持 DNL 就必须 >0 */
		 		picinfo.ImgHeight = MAKEWORD(*(lp+4), *(lp+3));
				/* 图片宽度 (高字节, 低字节), 如果不支持 DNL 就必须 >0 */
		 		picinfo.ImgWidth = MAKEWORD(*(lp+6), *(lp+5));
				/* components 数量(1 uint8_t), 灰度图是 1, YCbCr/YIQ 彩色图是 3, CMYK 彩色图是 4 */ 
	            comp_num = *(lp+7); 																						   
				if((comp_num != 1)&&(comp_num != 3))return FUNC_FORMAT_ERROR;/* 格式错误 */
				if(comp_num == 3)//YCbCr/YIQ 彩色图
				{	
					/* component id (1 = Y, 2 = Cb, 3 = Cr, 4 = I, 5 = Q) */
					comp_index[0] = *(lp+8); 
		  			SampRate_Y_H = (*(lp+9))>>4;	//水平采样系数 
		  			SampRate_Y_V = (*(lp+9))&0x0f;//垂直采样系数 
		  			YQtTable = (short *)qt_table[*(lp+10)];//通过量化表号取得量化表地址 
	
					comp_index[1] = *(lp+11);				 //component id 
					SampRate_U_H = (*(lp+12))>>4;			 //水平采样系数
		  			SampRate_U_V = (*(lp+12))&0x0f;		 //垂直采样系数 
		  			UQtTable = (short *)qt_table[*(lp+13)];//通过量化表号取得量化表地址
	
		  			comp_index[2] = *(lp+14);				 //component id 
		  			SampRate_V_H = (*(lp+15))>>4;			 //水平采样系数
		  			SampRate_V_V = (*(lp+15))&0x0f;		 //垂直采样系数 
					VQtTable = (short *)qt_table[*(lp+16)];//通过量化表号取得量化表地址
		  		}
				else								     //component id 
				{
		  			comp_index[0] = *(lp+8);
					SampRate_Y_H = (*(lp+9))>>4;
		  			SampRate_Y_V = (*(lp+9))&0x0f;
		  			YQtTable = (short *)qt_table[*(lp+10)];//灰度图的量化表都一样
	
					comp_index[1] = *(lp+8);
		  			SampRate_U_H = 1;
		  			SampRate_U_V = 1;
		  			UQtTable = (short *)qt_table[*(lp+10)];
	
					comp_index[2] = *(lp+8);
					SampRate_V_H = 1;
		  			SampRate_V_V = 1;
		  			VQtTable = (short *)qt_table[*(lp+10)];
				}   
				lp += llength;  
				jpg_seek(jpg_buffer, &lp);  			    
				break;
			case M_DHT: //定义哈夫曼表(0xFF,0xC4)             
				llength = MAKEWORD(*(lp+1), *lp);//长度 (高字节, 低字节)
				if (llength<0xd0)			  // Huffman Table信息 (1 uint8_t) 
				{
					huftab1 = (short)(*(lp+2))>>4;    //huftab1=0,1(HT 类型,0 = DC 1 = AC)
			 		huftab2 = (short)(*(lp+2))&0x0f;  //huftab2=0,1(HT 号  ,0 = Y  1 = UV)
					huftabindex=huftab1*2+huftab2;//0 = YDC 1 = UVDC 2 = YAC 3 = UVAC 
			 		lptemp=lp+3;
					/* 在这里可能出现余度不够,多于512字节,则会导致出错! */
					for (i = 0; i < 16; i ++)        /* 16 uint8_ts: 长度是 1..16 代码的符号数 */
						code_len_table[huftabindex][i] = (short)(*(lptemp++));/* 码长为i的码字个数 */   
					j = 0;
					for (i = 0; i < 16; i ++)/* 得出HT的所有码字的对应值 */
					{
						if(code_len_table[huftabindex][i] != 0)
						{
							k = 0;
							while(k < code_len_table[huftabindex][i])
							{	//最可能的出错地方
								code_value_table[huftabindex][k + j] = (short)(*(lptemp++));
								k++;
							}
							j += k;	
						}
					}
					i = 0;
					while (code_len_table[huftabindex][i] == 0)i ++;	   
					for (j = 0; j < i; j ++)
					{
						huf_min_value[huftabindex][j] = 0;
						huf_max_value[huftabindex][j] = 0;
					}
					huf_min_value[huftabindex][i] = 0;
					huf_max_value[huftabindex][i] = code_len_table[huftabindex][i]-1;
					for (j = i + 1; j < 16; j ++)
					{
						huf_min_value[huftabindex][j] = (huf_max_value[huftabindex][j-1]+1)<<1;
						huf_max_value[huftabindex][j] = \
							huf_min_value[huftabindex][j]+code_len_table[huftabindex][j]-1;
					}
					code_pos_table[huftabindex][0] = 0;
					for (j = 1; j < 16; j ++)
			  			code_pos_table[huftabindex][j] = \
							code_len_table[huftabindex][j-1]+code_pos_table[huftabindex][j-1];
					lp += llength;  		
					jpg_seek(jpg_buffer, &lp); 
				}
				else
				{		  
				   	hf_table_index = *(lp + 2);
					lp += 2;  
					jpg_seek(jpg_buffer, &lp);  
					while (hf_table_index != 0xff)
					{
						huftab1 = (short)hf_table_index>>4;   //huftab1=0,1
				 		huftab2 = (short)hf_table_index&0x0f; //huftab2=0,1
						huftabindex = huftab1 * 2 + huftab2;
						lptemp = lp + 1;
						colorount = 0;
						for (i = 0; i < 16; i ++)
						{
							code_len_table[huftabindex][i] = (short)(*(lptemp++));
							colorount += code_len_table[huftabindex][i];
						}
						colorount += 17;	
						j = 0;
						for (i = 0; i < 16; i ++)
						{
							if(code_len_table[huftabindex][i] != 0) 
							{
								k = 0;
								while(k<code_len_table[huftabindex][i])
								{	//最可能出错的地方,余度不够
									code_value_table[huftabindex][k+j] = (short)(*(lptemp++));
									k ++;
								}
								j += k;
							}
						}
/*						
	j=0;
	for(i=0;i<16;i++)
	{
		j+=code_len_table[huftabindex][i];
	}
	for(i=0;i<j;i++)
	{
		code_value_table[huftabindex][i]=(short)(*(lptemp++));
	}
																  */
						i = 0;
						while (code_len_table[huftabindex][i] == 0)i ++;
						for (j = 0;j < i;j ++)
						{
							huf_min_value[huftabindex][j] = 0;
							huf_max_value[huftabindex][j] = 0;
						}
						huf_min_value[huftabindex][i] = 0;
						huf_max_value[huftabindex][i] = code_len_table[huftabindex][i] - 1;
						for (j = i + 1; j < 16;j ++)
						{
							huf_min_value[huftabindex][j] = \
								(huf_max_value[huftabindex][j-1]+1)<<1;
							huf_max_value[huftabindex][j] = \
								huf_min_value[huftabindex][j]+code_len_table[huftabindex][j]-1;
						}
						code_pos_table[huftabindex][0] = 0;
						for (j = 1; j < 16; j ++)
							code_pos_table[huftabindex][j] = \
								code_len_table[huftabindex][j-1]+code_pos_table[huftabindex][j-1];
						lp += colorount;  
						jpg_seek(jpg_buffer, &lp);	 
						hf_table_index = *lp;
					}
				}
				break;
			case M_DRI://定义差分编码累计复位的间隔
				llength = MAKEWORD(*(lp + 1), *lp);
				restart = MAKEWORD(*(lp + 3), *(lp + 2));  
				lp += llength;  
				jpg_seek(jpg_buffer, &lp);	 
				break;
			case M_SOS:	 //扫描开始 12字节
				llength = MAKEWORD(*(lp + 1), *lp);
				comnum = *(lp + 2);
				if(comnum != comp_num)return FUNC_FORMAT_ERROR; //格式错误   
				lptemp = lp + 3;//这里也可能出现错误
				//这里也可能出错,但是几率比较小了
				for (i = 0; i < comp_num; i ++)//每组件的信息 
				{
					if(*lptemp == comp_index[0])
					{
						YDcIndex = (*(lptemp + 1))>>4;   //Y 使用的 Huffman 表 
						YAcIndex = ((*(lptemp + 1))&0x0f) + 2;
					}
					else
					{
						UVDcIndex = (*(lptemp + 1))>>4;   //U,V
						UVAcIndex = ((*(lptemp + 1))&0x0f) + 2;
					}
					lptemp += 2;//comp_num<256,但是2*comp_num+3可能>=512
				}		  
				lp += llength;  
				jpg_seek(jpg_buffer, &lp);   
				finish = TRUE;
				break;
			case M_EOI:return FUNC_FORMAT_ERROR;//图片结束 标记     
			default:
	 			if ((id&0xf0) != 0xd0)
				{	   
				   	llength = MAKEWORD(*(lp + 1), *lp);
					lp += llength;  
					jpg_seek(jpg_buffer, &lp);	   
				}
				else lp += 2;
				break;
  		}
	}
	return FUNC_OK;
}

/**
  * @brief  初始化清零量化表
  * @param  None
  * @retval None
  */
void jpeg_inittable(void)
{
	short i,j;
	sizei = 0;
	sizej = 0;
	picinfo.ImgWidth = 0;
	picinfo.ImgHeight = 0;
	rrun=vvalue = 0;
	BitPos = 0;
	Curuint8_t = 0;
	IntervalFlag = FALSE;
	restart = 0;
	for(i = 0; i < 3; i ++)/* 量化表 */
		for(j = 0; j < 64; j ++)
			qt_table[i][j] = 0;
	comp_num = 0;
	HufTabIndex = 0;
	for(i = 0; i < 3; i ++)
		comp_index[i] = 0;
	for(i = 0; i < 4; i ++)
		for(j = 0; j < 16; j ++)
		{
			code_len_table[i][j] = 0;
			code_pos_table[i][j] = 0;
			huf_max_value[i][j] = 0;
			huf_min_value[i][j] = 0;
		}
	for(i = 0; i < 4; i ++)
		for(j = 0; j < 256; j ++)
			code_value_table[i][j] = 0;
	
	for(i = 0; i < 10 * 64; i ++)
	{
		MCUBuffer[i] = 0;
		QtZzMCUBuffer[i] = 0;
	}
	for(i = 0; i < 64; i ++)
	{
		Y[i] = 0;
		U[i] = 0;
		V[i] = 0;
		BlockBuffer[i] = 0;
	}
	ycoef = ucoef = vcoef = 0;
	br = 0;
}

/**
  * @brief  调用顺序:	jpeg_initfastidct()			初始化 
  *						jpeg_decodemcublock()       Huffman Decode 
  *						jpeg_iqtizzmcucomponent()   反量化、反DCT 
  *						jpeg_getyuv()               Get Y U V 
  *						jpeg_storebuffer()          YUV to RGB 
  * @param  None
  * @retval 
  */
int jpeg_decode(void)
{
	int funcret;    
	Y_in_MCU = SampRate_Y_H * SampRate_Y_V;/* YDU YDU YDU YDU */
	U_in_MCU = SampRate_U_H * SampRate_U_V;/* cRDU */ 
	V_in_MCU = SampRate_V_H * SampRate_V_V;/* cBDU */
	H_YtoU = SampRate_Y_H / SampRate_U_H;
	V_YtoU = SampRate_Y_V / SampRate_U_V;
	H_YtoV = SampRate_Y_H / SampRate_V_H;
	V_YtoV = SampRate_Y_V / SampRate_V_V;
	jpeg_initfastidct();
	while((funcret=jpeg_decodemcublock()) == FUNC_OK) /* After Call DecodeMCUBUBlock() */
	{
		interval ++;							/* The Digital has been Huffman Decoded and */
		if((restart)&&(interval % restart == 0))/* be stored in MCUBuffer(YDU,YDU,YDU,YDU */	        
			 IntervalFlag = TRUE;				/* ,UDU,VDU) Every DU := 8*8 */
		else
			IntervalFlag = FALSE;
		jpeg_iqtizzmcucomponent(0); /* 反量化 and IDCT The Data in QtZzMCUBuffer */ 
		jpeg_iqtizzmcucomponent(1);
		jpeg_iqtizzmcucomponent(2);
		jpeg_getyuv(0);			   /* 得到Y cR cB */ 
		jpeg_getyuv(1);
		jpeg_getyuv(2);
		jpeg_storebuffer();		   /* To RGB */
		sizej+=SampRate_Y_H*8;
		if(sizej >= picinfo.ImgWidth)
		{
			sizej = 0;
			sizei+=SampRate_Y_V*8;
		}
		if((sizej==0)&&(sizei>=picinfo.ImgHeight))break;	  
	}
	return funcret;
}

/** -入口 QtZzMCUBuffer
  * -出口 Y[] U[] V[] 
  * -得到YUV色彩空间
  */
void  jpeg_getyuv(short flag)
{
	short	H = 0, VV = 0;
	short	i, j, k, h;
	short	*buf = 0;
	short	*pQtZzMCU = 0;	  
	switch(flag)
	{
		case 0:/* 亮度分量 */
			H = SampRate_Y_H;
			VV = SampRate_Y_V;
			buf = Y;
			pQtZzMCU = QtZzMCUBuffer;
			break;
		case 1:/* 红色分量 */
			H = SampRate_U_H;
			VV = SampRate_U_V;
			buf = U;
			pQtZzMCU = QtZzMCUBuffer + Y_in_MCU * 64;
			break;
		case 2:/* 蓝色分量 */
			H = SampRate_V_H;
			VV = SampRate_V_V;
			buf = V;
			pQtZzMCU = QtZzMCUBuffer + (Y_in_MCU+U_in_MCU) * 64;
			break;
	}
	for (i = 0; i < VV; i ++)
		for(j = 0; j < H; j ++)
			for(k = 0; k < 8; k ++)
				for(h = 0; h < 8; h ++)
					buf[(i * 8 + k) * SampRate_Y_H * 8 + j * 8 + h] = *pQtZzMCU ++;
}
  
/** 
  * @brief  将解出的字按RGB形式存储 lpbmp (BGR),(BGR)
  *			-入口Y[] U[] V[]
  *			-出口lpPtr
  * @param  None
  * @retval None
  */
void jpeg_storebuffer(void)
{
	short i = 0, j = 0;			    
	uint8_t R, G, B;
	int y, u, v, rr, gg, bb;
	uint16_t color;
	uint16_t realx = sizej;/* x,y的实际坐标 */
	uint16_t realy = 0;    
	for(i = 0; i < SampRate_Y_V * 8; i ++)
	{	    
		if((sizei + i) < picinfo.ImgHeight)// sizei表示行 sizej 表示列 
		{										   
			realy = (picinfo.Div_Fac * (sizei + i))>>13;//实际Y坐标											 
			/*-在这里不改变picinfo.staticx和picinfo.staticy的值,
			  -如果在这里改变,则会造成每块的第一个点不显示!		-*/
			if(!is_element_ok(realx, realy, 0))
				continue;//列值是否满足条件? 寻找满足条件的列
			for(j = 0; j < SampRate_Y_H * 8; j ++)
			{
				if((sizej + j) < picinfo.ImgWidth)
				{
					realx = (picinfo.Div_Fac * (sizej + j))>>13;//实际X坐标	   
					//在这里改变picinfo.staticx和picinfo.staticy的值
					if(!is_element_ok(realx, realy, 1))
						continue;//列值是否满足条件? 寻找满足条件的行

					y = Y[i * 8 * SampRate_Y_H + j];
					u = U[(i / V_YtoU) * 8 * SampRate_Y_H + j / H_YtoU];
					v = V[(i / V_YtoV) * 8 * SampRate_Y_H + j / H_YtoV];
					rr = ((y<<8) + 18 * u + 367 * v)>>8;
					gg = ((y<<8) - 159 * u - 220 * v)>>8;
					bb = ((y<<8) + 411 * u - 29 * v)>>8;
					R = (uint8_t)rr;
					G = (uint8_t)gg;
					B = (uint8_t)bb;
					if (rr&0xffffff00) if (rr > 255) R = 255; else if (rr<0) R = 0;
					if (gg&0xffffff00) if (gg > 255) G = 255; else if (gg<0) G = 0;
					if (bb&0xffffff00) if (bb > 255) B =255; else if (bb<0) B = 0;  
					color = R>>3;
					color = color<<6;
					color |= (G>>2);
					color = color<<5;
					color |= (B>>3);			   
					/* 交LCD显示 */ 
					pic_phy.draw_point(realx + picinfo.S_XOFF,\
						realy + picinfo.S_YOFF, color);/* 显示图片★ */	
				}
				else break;
			} 	    
		}
		else break;
	}
}

//Huffman Decode   MCU 出口 MCUBuffer  入口Blockbuffer[  ] 
int jpeg_decodemcublock(void)
{
	short *lpMCUBuffer;
	short i, j;
	int funcret;  
	if (IntervalFlag)//差值复位 
	{			    
		lp += 2;  
		jpg_seek(jpg_buffer, &lp);	 
		ycoef = ucoef = vcoef = 0;
		BitPos = 0;
		Curuint8_t = 0;
	}
	switch(comp_num)
	{
		case 3:	 //comp_num 指图的类型（彩色图、灰度图） 
			lpMCUBuffer = MCUBuffer;
			for (i = 0; i < SampRate_Y_H * SampRate_Y_V; i ++)  //Y
			{
				funcret = jpeg_hufblock(YDcIndex, YAcIndex);//解码4 * (8*8) 
				if (funcret != FUNC_OK)
					return funcret;
				BlockBuffer[0] = BlockBuffer[0] + ycoef;//直流分量是差值，所以要累加。
				ycoef = BlockBuffer[0];
				for (j = 0; j < 64; j ++)
					*lpMCUBuffer++ = BlockBuffer[j];
			}
			for (i = 0; i < SampRate_U_H * SampRate_U_V; i ++)  //U
			{
				funcret = jpeg_hufblock(UVDcIndex, UVAcIndex);
				if (funcret!=FUNC_OK)
					return funcret;
				BlockBuffer[0] = BlockBuffer[0] + ucoef;
				ucoef = BlockBuffer[0];
				for (j = 0; j < 64; j ++)
					*lpMCUBuffer++ = BlockBuffer[j];
			}
			for (i = 0; i < SampRate_V_H * SampRate_V_V; i ++)  //V
			{
				funcret = jpeg_hufblock(UVDcIndex, UVAcIndex);
				if (funcret != FUNC_OK)
					return funcret;
				BlockBuffer[0] = BlockBuffer[0] + vcoef;
				vcoef = BlockBuffer[0];
				for (j = 0; j < 64; j ++)
					*lpMCUBuffer++ = BlockBuffer[j];
			}
			break;
		case 1:	//Gray Picture 
			lpMCUBuffer = MCUBuffer;
			funcret = jpeg_hufblock(YDcIndex, YAcIndex);
			if (funcret != FUNC_OK)
				return funcret;
			BlockBuffer[0] = BlockBuffer[0] + ycoef;
			ycoef = BlockBuffer[0];
			for (j = 0; j < 64; j ++)
				*lpMCUBuffer++ = BlockBuffer[j];
			for (i = 0; i < 128; i ++)
				*lpMCUBuffer++ = 0;
			break;
		default:
			return FUNC_FORMAT_ERROR;
	}
	return FUNC_OK;
}

//Huffman Decode （8*8） DU   出口 Blockbuffer[ ] 入口 vvalue 
int jpeg_hufblock(uint8_t dchufindex, uint8_t achufindex)
{
	short count = 0;
	short i;
	int funcret;   
	//dc
	HufTabIndex = dchufindex;
	funcret = jpeg_decodeelement();
	if(funcret != FUNC_OK)return funcret;	  
	BlockBuffer[count ++] = vvalue;//解出的直流系数 
	//ac
	HufTabIndex = achufindex;
	while (count < 64)
	{
		funcret = jpeg_decodeelement();
		if(funcret != FUNC_OK)
			return funcret;
		if ((rrun == 0)&&(vvalue == 0))
		{
			for (i = count;i < 64;i ++)BlockBuffer[i] = 0;	 
			count = 64;
		}
		else
		{						    
			for (i = 0;i < rrun;i ++)BlockBuffer[count ++] = 0;//前面的零	 
			BlockBuffer[count ++] = vvalue;//解出的值 
		}
	}
	return FUNC_OK;
}

/**	Huffman 解码  每个元素
  *	出口:vvalue
  *	入口:读文件Readuint8_t
  */
int jpeg_decodeelement()
{
	int thiscode, tempcode;
	uint16_t temp, valueex;
	short codelen;
	uint8_t hufexuint8_t, runsize, tempsize, sign;
	uint8_t newuint8_t, lastuint8_t;

	if(BitPos >= 1) //BitPos指示当前比特位置 
	{
		BitPos --;
		thiscode = (uint8_t)Curuint8_t>>BitPos;//取一个比特
		Curuint8_t = Curuint8_t&And[BitPos];   //清除取走的比特位 
	}
	else							   //取出的一个字节已用完
	{								   //新取
		lastuint8_t = jpeg_readuint8_t();		   //读出一个字节
		BitPos --;					   //and[]:=0x0,0x1,0x3,0x7,0xf,0x1f,0x2f,0x3f,0x4f 
		newuint8_t = Curuint8_t&And[BitPos];
		thiscode = lastuint8_t>>7;
		Curuint8_t = newuint8_t;
	}
	codelen = 1;
	/* 与Huffman表中的码字匹配，直自找到为止 */ 
	while ((thiscode < huf_min_value[HufTabIndex][codelen - 1])||
		  (code_len_table[HufTabIndex][codelen - 1] == 0)||
		  (thiscode > huf_max_value[HufTabIndex][codelen - 1]))
	{
		if(BitPos >= 1)
		{
			BitPos --;
			tempcode = (uint8_t)Curuint8_t >> BitPos;
			Curuint8_t = Curuint8_t&And[BitPos];
		}
		else
		{
			lastuint8_t = jpeg_readuint8_t();
			BitPos --;
			newuint8_t = Curuint8_t&And[BitPos];
			tempcode = (uint8_t)lastuint8_t>>7;
			Curuint8_t = newuint8_t;
		}
		thiscode = (thiscode<<1) + tempcode;
		codelen ++;
		if(codelen > 16)return FUNC_FORMAT_ERROR;	   
	}
	temp = thiscode - huf_min_value[HufTabIndex][codelen - 1] + \
		code_pos_table[HufTabIndex][codelen - 1];
	hufexuint8_t = (uint8_t)code_value_table[HufTabIndex][temp];
	rrun = (short)(hufexuint8_t>>4);	 /* 一个字节中，高四位是其前面的零的个数 */
	runsize = hufexuint8_t&0x0f;		 /* 后四位为后面字的尺寸 */
	if(runsize == 0)
	{
		vvalue = 0;
		return FUNC_OK;
	}
	tempsize = runsize;
	if(BitPos >= runsize)
	{
		BitPos -= runsize;
		valueex = (uint8_t)Curuint8_t>>BitPos;
		Curuint8_t = Curuint8_t&And[BitPos];
	}
	else
	{
		valueex = Curuint8_t;
		tempsize -= BitPos;
		while(tempsize > 8)
		{
			lastuint8_t = jpeg_readuint8_t();
			valueex = (valueex<<8) + (uint8_t)lastuint8_t;
			tempsize -= 8;
		}
		lastuint8_t = jpeg_readuint8_t();
		BitPos -= tempsize;
		valueex = (valueex<<tempsize) + (lastuint8_t>>BitPos);
		Curuint8_t = lastuint8_t&And[BitPos];
	}
	sign = valueex>>(runsize - 1);
	if(sign)vvalue = valueex;/* 解出的码值 */
	else
	{
		valueex = valueex^0xffff;
		temp = 0xffff<<runsize;
		vvalue =- (short)(valueex^temp);
	}
	return FUNC_OK;
}

/**
  * @brief  反量化MCU中的每个组件 
  *			入口:MCUBuffer
  *			出口:QtZzMCUBuffer
  */
void jpeg_iqtizzmcucomponent(short flag)
{
	short H = 0, VV = 0;
	short i, j;
	short *pQtZzMCUBuffer = 0;
	short  *pMCUBuffer = 0;

	switch(flag)
	{
		case 0:
			H = SampRate_Y_H;
			VV = SampRate_Y_V;
			pMCUBuffer = MCUBuffer;
			pQtZzMCUBuffer = QtZzMCUBuffer;
			break;
		case 1:
			H = SampRate_U_H;
			VV = SampRate_U_V;
			pMCUBuffer = MCUBuffer + Y_in_MCU*64;
			pQtZzMCUBuffer = QtZzMCUBuffer + Y_in_MCU*64;
			break;
		case 2:
			H = SampRate_V_H;
			VV = SampRate_V_V;
			pMCUBuffer=MCUBuffer + (Y_in_MCU + U_in_MCU) * 64;
			pQtZzMCUBuffer = QtZzMCUBuffer + (Y_in_MCU + U_in_MCU) * 64;
			break;
	}
	for(i = 0; i < VV; i ++)
		for (j = 0; j < H; j ++)
			jpeg_iqtizzblock(pMCUBuffer + (i * H + j) * 64, pQtZzMCUBuffer +\
			(i * H + j) * 64, flag);
}

/**
  *	要量化的字 
  *	反量化 8*8 DU 
  */
void jpeg_iqtizzblock(short *s ,short * d,short flag)
{
	short i, j;
	short tag;
	short *pQt = 0;
	int buffer2[8][8];
	int *buffer1;
	short offset = 0;

	switch(flag)
	{
		case 0:
			pQt = YQtTable;/* 亮度 */
			offset = 128;
			break;
		case 1:
			pQt = UQtTable;/* 红 */
			offset = 0;
			break;
		case 2:
			pQt = VQtTable;/* 蓝 */
			offset = 0;
			break;
	}

	for(i = 0; i < 8; i ++)
		for(j = 0; j < 8; j ++)
		{
			tag = Zig_Zag[i][j];
			buffer2[i][j] = (int)s[tag] * (int)pQt[tag];
		}
	buffer1 = (int *)buffer2;
	jpeg_fastidct(buffer1);/* 反DCT */
	for(i = 0; i < 8; i ++)
		for(j = 0; j < 8; j ++)
			d[i * 8 + j] = buffer2[i][j] + offset;
}

/**
  * @brief  快速反DCT  快速反离散傅立叶变换
  * @param  数据块的指针
  * @retval None
  */
void jpeg_fastidct(int * block)
{
	short i;		 
	for (i = 0; i < 8; i ++)jpeg_idctrow(block + 8 * i);	    
	for (i = 0; i < 8; i ++)jpeg_idctcol(block + i);    
}

/**
  * @brief  从源文件读取一个字节
  * @param  None
  * @retval 读得的字节
  */
uint8_t jpeg_readuint8_t(void)
{
	uint8_t i;   						   
	i = *lp++;  
	jpg_seek(jpg_buffer, &lp);					    	  
	if(i == 0xff)lp++;   
	BitPos = 8;
	Curuint8_t = i;
	return i;
}

/**
  * @brief  初始化快速反DCT
  * @param  None
  * @retval None
  */
void jpeg_initfastidct(void)
{
	short i;
	iclp = iclip + 512;
	for (i= -512; i < 512; i ++)
		iclp[i] = (i < -256) ? -256 : ((i > 255) ? 255 : i);
}

void jpeg_idctrow(int * blk)
{
	int x0, x1, x2, x3, x4, x5, x6, x7, x8;

	if (!((x1 = blk[4]<<11) | (x2 = blk[6]) | (x3 = blk[2]) |
		(x4 = blk[1]) | (x5 = blk[7]) | (x6 = blk[5]) | (x7 = blk[3])))
	{
		blk[0] = blk[1] = blk[2] = blk[3] = blk[4] = blk[5] = blk[6] = blk[7] = blk[0]<<3;
		return;
	}
	x0 = (blk[0]<<11) + 128;/* 在第四阶段适当地四舍五入 */
	/* 第一级 */
	x8 = W7 * (x4 + x5);
	x4 = x8 + (W1 - W7) * x4;
	x5 = x8 - (W1 + W7) * x5;
	x8 = W3 * (x6 + x7);
	x6 = x8 - (W3 - W5) * x6;
	x7 = x8 - (W3 + W5) * x7;
	/* 第二级 */
	x8 = x0 + x1;
	x0 -= x1;
	x1 = W6 * (x3 + x2);
	x2 = x1 - (W2 + W6) * x2;
	x3 = x1 + (W2 - W6) * x3;
	x1 = x4 + x6;
	x4 -= x6;
	x6 = x5 + x7;
	x5 -= x7;
	/* 第三级 */
	x7 = x8 + x3;
	x8 -= x3;
	x3 = x0 + x2;
	x0 -= x2;
	x2 = (181 * (x4 + x5) + 128)>>8;
	x4 = (181 * (x4 - x5) + 128)>>8;
	/* 第四级 */
	blk[0] = (x7 + x1)>>8;
	blk[1] = (x3 + x2)>>8;
	blk[2] = (x0 + x4)>>8;
	blk[3] = (x8 + x6)>>8;
	blk[4] = (x8 - x6)>>8;
	blk[5] = (x0 - x4)>>8;
	blk[6] = (x3 - x2)>>8;
	blk[7] = (x7 - x1)>>8;
}

void jpeg_idctcol(int * blk)
{
	int x0, x1, x2, x3, x4, x5, x6, x7, x8;

	if (!((x1 = (blk[8*4]<<8)) | (x2 = blk[8*6]) | (x3 = blk[8*2]) |
		(x4 = blk[8*1]) | (x5 = blk[8*7]) | (x6 = blk[8*5]) | (x7 = blk[8*3])))
	{
		blk[8 * 0] = blk[8 * 1] = blk[8 * 2] = blk[8 * 3] = blk[8 * 4] = blk[8 * 5]
			= blk[8 * 6] = blk[8 * 7] = iclp[(blk[8 * 0] + 32)>>6];
		return;
	}
	x0 = (blk[8 * 0]<<8) + 8192;
	/* 第一级 */
	x8 = W7 * (x4 + x5) + 4;
	x4 = (x8 + (W1 - W7) * x4)>>3;
	x5 = (x8 - (W1 + W7) * x5)>>3;
	x8 = W3 * (x6 + x7) + 4;
	x6 = (x8 - (W3 - W5) * x6)>>3;
	x7 = (x8 - (W3 + W5) * x7)>>3;
	/* 第二级 */
	x8 = x0 + x1;
	x0 -= x1;
	x1 = W6 * (x3 + x2) + 4;
	x2 = (x1 - (W2 + W6) * x2)>>3;
	x3 = (x1 + (W2 - W6) * x3)>>3;
	x1 = x4 + x6;
	x4 -= x6;
	x6 = x5 + x7;
	x5 -= x7;
	/* 第三级 */
	x7 = x8 + x3;
	x8 -= x3;
	x3 = x0 + x2;
	x0 -= x2;
	x2 = (181 * (x4 + x5) + 128)>>8;
	x4 = (181 * (x4 - x5) + 128)>>8;
	/* 第四级 */
	blk[8 * 0] = iclp[(x7 + x1)>>14];
	blk[8 * 1] = iclp[(x3 + x2)>>14];
	blk[8 * 2] = iclp[(x0 + x4)>>14];
	blk[8 * 3] = iclp[(x8 + x6)>>14];
	blk[8 * 4] = iclp[(x8 - x6)>>14];
	blk[8 * 5] = iclp[(x0 - x4)>>14];
	blk[8 * 6] = iclp[(x3 - x2)>>14];
	blk[8 * 7] = iclp[(x7 - x1)>>14];
}
