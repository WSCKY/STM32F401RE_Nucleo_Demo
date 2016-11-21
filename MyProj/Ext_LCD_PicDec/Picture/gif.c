#include "piclib.h"
#include "gif.h"	 
#include "ff.h"

const uint16_t _aMaskTbl[16] =
{
	0x0000, 0x0001, 0x0003, 0x0007,
	0x000f, 0x001f, 0x003f, 0x007f,
	0x00ff, 0x01ff, 0x03ff, 0x07ff,
	0x0fff, 0x1fff, 0x3fff, 0x7fff,
};	  
const uint8_t _aInterlaceOffset[]={8,8,4,2};
const uint8_t _aInterlaceYPos  []={0,4,2,1};
 
uint8_t gifdecoding=0;//���GIF���ڽ���.

void mymemset(void *s,uint8_t c,uint32_t count);

gif89a tgif89a;			//gif89a�ļ�
FIL f_gfile;			//gif �ļ�
LZW_INFO tlzw;			//lzw

/**
  * @brief  ��ȡGIF�ļ�ͷ��6���ֽ�
  * @param  
  * @retval 0,��GIF89a/87a
  *			!0,��GIF89a/87a
  */
uint8_t gif_check_head(FIL *file)
{
	uint8_t gifversion[6];
	uint32_t readed;
	uint8_t res;
	res=f_read(file,gifversion,6,(UINT*)&readed);
	if(res)return 1;	   
	if((gifversion[0]!='G')||(gifversion[1]!='I')||(gifversion[2]!='F')||
	(gifversion[3]!='8')||((gifversion[4]!='7')&&(gifversion[4]!='9'))||
	(gifversion[5]!='a'))return 2;
	else return 0;	
}
/**
  * @brief  ��RGB888תΪRGB565
  * @param  ctb:RGB888��ɫ�����׵�ַ
  * @retval RGB565��ɫ
  */
uint16_t gif_getrgb565(uint8_t *ctb) 
{
	uint16_t r,g,b;
	r = (ctb[0]>>3)&0X1F;
	g = (ctb[1]>>2)&0X3F;
	b = (ctb[2]>>3)&0X1F;
	return b + (g<<5) + (r<<11);
}
/**
  * @brief  ��ȡ��ɫ��
  * @param  file:�ļ�
  * @param  gif:gif��Ϣ
  * @param  num:tbl��С
  * @retval 0,OK;����,ʧ��
  */
uint8_t gif_readcolortbl(FIL *file, gif89a *gif, uint16_t num)
{
	uint8_t rgb[3];
	uint16_t t;
	uint8_t res;
	uint32_t readed;
	for(t = 0; t < num; t ++)
	{
		res = f_read(file, rgb, 3, (UINT*)&readed);
		if(res)return 1;						/* Error */
		gif->colortbl[t] = gif_getrgb565(rgb);
	}
	return 0;
} 
/** @brief  �õ��߼���Ļ����,ͼ��ߴ��
  * @param  file:�ļ�
  * @param  gif:gif��Ϣ
  * @retval 0,OK;����,ʧ��
  */
uint8_t gif_getinfo(FIL *file, gif89a *gif)
{
	uint32_t readed;	 
	uint8_t res;   
	res = f_read(file, (uint8_t*)&gif->gifLSD, 7, (UINT*)&readed);
	if(res)return 1;
	if(gif->gifLSD.flag&0x80)/* ����ȫ����ɫ�� */
	{
		gif->numcolors=2<<(gif->gifLSD.flag&0x07);/* �õ���ɫ����С */
		if(gif_readcolortbl(file,gif,gif->numcolors))return 1;/* Error */	
	}	   
	return 0;
}

/**
  * @brief  ����ȫ����ɫ��
  * @param  gif:gif��Ϣ
  * @retval None
  */
void gif_savegctbl(gif89a* gif)
{
	uint16_t i = 0;
	for(i = 0; i < 256; i ++)						 /* ����ȫ����ɫ */
		gif->bkpcolortbl[i] = gif->colortbl[i];
}

/**
  * @brief  �ָ�ȫ����ɫ��
  * @param  gif:gif��Ϣ
  * @retval None
  */
void gif_recovergctbl(gif89a* gif)
{
	uint16_t i = 0;
	for(i = 0; i < 256; i ++)						 /* �ָ�ȫ����ɫ */
		gif->colortbl[i] = gif->bkpcolortbl[i];
}

/**
  * @brief  ��ʼ��LZW��ز���
  * @param  gif:gif��Ϣ
  * @param  codesize:lzw�볤��
  * @retval None
  */
void gif_initlzw(gif89a* gif,uint8_t codesize) 
{
 	mymemset((uint8_t *)gif->lzw, 0, sizeof(LZW_INFO));		  /* �� */
	gif->lzw->SetCodeSize  = codesize;
	gif->lzw->CodeSize     = codesize + 1;
	gif->lzw->ClearCode    = (1 << codesize);
	gif->lzw->EndCode      = (1 << codesize) + 1;
	gif->lzw->MaxCode      = (1 << codesize) + 2;
	gif->lzw->MaxCodeSize  = (1 << codesize) << 1;
	gif->lzw->ReturnClear  = 1;
	gif->lzw->LastByte     = 2;
	gif->lzw->sp           = gif->lzw->aDecompBuffer;
}

/**
  * @brief  ��ȡһ�����ݿ�
  * @param  gfile:gif�ļ�
  * @param  buf:���ݻ�����
  * @param  maxnum:����д��������
  * @retval 
  */
uint16_t gif_getdatablock(FIL *gfile,uint8_t *buf,uint16_t maxnum) 
{
	uint8_t cnt;
	uint32_t readed;
	uint32_t fpos;
	f_read(gfile,&cnt,1,(UINT*)&readed);//�õ�LZW����			 
	if(cnt) 
	{
		if (buf)//��Ҫ��ȡ 
		{
			if(cnt>maxnum)
			{
				fpos=f_tell(gfile);
				f_lseek(gfile,fpos+cnt);//����
				return cnt;//ֱ�Ӳ���
			}
			f_read(gfile,buf,cnt,(UINT*)&readed);//�õ�LZW����	
		}else 	//ֱ������
		{
			fpos=f_tell(gfile);
			f_lseek(gfile,fpos+cnt);//����
		}
	}
	return cnt;
}
/**
  *ReadExtension		 
  *Purpose:
  *Reads an extension block. One extension block can consist of several data blocks.
  *If an unknown extension block occures, the routine failes.
  *return value:0,�ɹ�;
  * 			����,ʧ��
  */
uint8_t gif_readextension(FIL *gfile,gif89a* gif, int *pTransIndex,uint8_t *pDisposal)
{
	uint8_t temp;
	uint32_t readed;	 
	uint8_t buf[4];  
	f_read(gfile,&temp,1,(UINT*)&readed);//�õ�����		 
	switch(temp)
	{
		case GIF_PLAINTEXT:
		case GIF_APPLICATION:
		case GIF_COMMENT:
			while(gif_getdatablock(gfile,0,256)>0);			//��ȡ���ݿ�
			return 0;
		case GIF_GRAPHICCTL://ͼ�ο�����չ��
			if(gif_getdatablock(gfile,buf,4)!=4)return 1;	//ͼ�ο�����չ��ĳ��ȱ���Ϊ4 
 		 	gif->delay=(buf[2]<<8)|buf[1];					//�õ���ʱ 
			*pDisposal=(buf[0]>>2)&0x7; 	    			//�õ���������
			if((buf[0]&0x1)!=0)*pTransIndex=buf[3];			//͸��ɫ�� 
			f_read(gfile,&temp,1,(UINT*)&readed);	 		//�õ�LZW����	
 			if(temp!=0)return 1;							//��ȡ���ݿ����������.
			return 0;
	}
	return 1;//���������
}

/**
  * @brief  ��LZW�����еõ���һ��LZW��,ÿ�������12λ
  * @param  
  * @retval <0,����.
  *			others,����.
  */
int gif_getnextcode(FIL *gfile,gif89a* gif) 
{
	int i,j,End;
	long Result;
	if(gif->lzw->ReturnClear)
	{
		/* The first code should be a clearcode. */
		gif->lzw->ReturnClear=0;
		return gif->lzw->ClearCode;
	}
	End=gif->lzw->CurBit+gif->lzw->CodeSize;
	if(End>=gif->lzw->LastBit)
	{
		int Count;
		if(gif->lzw->GetDone)return-1;/* Error */ 
		gif->lzw->aBuffer[0]=gif->lzw->aBuffer[gif->lzw->LastByte-2];
		gif->lzw->aBuffer[1]=gif->lzw->aBuffer[gif->lzw->LastByte-1];
		if((Count=gif_getdatablock(gfile,&gif->lzw->aBuffer[2],300))==0)gif->lzw->GetDone=1;
		if(Count<0)return -1;/* Error */ 
		gif->lzw->LastByte=2+Count;
		gif->lzw->CurBit=(gif->lzw->CurBit-gif->lzw->LastBit)+16;
		gif->lzw->LastBit=(2+Count)*8;
		End = gif->lzw->CurBit + gif->lzw->CodeSize;
	}
	j = End>>3;
	i = gif->lzw->CurBit>>3;
	if(i==j)
		Result=(long)gif->lzw->aBuffer[i];
	else if(i+1==j)
		Result = (long)gif->lzw->aBuffer[i] | ((long)gif->lzw->aBuffer[i + 1]<<8);
	else
		Result = (long)gif->lzw->aBuffer[i] | \
		((long)gif->lzw->aBuffer[i + 1]<<8) | ((long)gif->lzw->aBuffer[i + 2]<<16);
	Result = (Result>>(gif->lzw->CurBit&0x7))&_aMaskTbl[gif->lzw->CodeSize];
	gif->lzw->CurBit += gif->lzw->CodeSize;
	return(int)Result;
}

/**
  * @brief  �õ���һ��LZW��
  * @param  
  * @retval <0,����(-1,���ɹ�;-2,������������)
		 	>=0,OK.(LZW�ĵ�һ����)
  */	
int gif_getnextbyte(FIL *gfile,gif89a* gif) 
{
	int i,Code,Incode;
	while((Code=gif_getnextcode(gfile,gif))>=0)
	{
		if(Code==gif->lzw->ClearCode)
		{
			/* Corrupt GIFs can make this happen */  
			if(gif->lzw->ClearCode>=(1<<MAX_NUM_LWZ_BITS))return -1;/* Error */ 
			/* Clear the tables */ 
			mymemset((uint8_t*)gif->lzw->aCode,0,sizeof(gif->lzw->aCode));	  /* �� */
			for(i=0;i<gif->lzw->ClearCode;++i)gif->lzw->aPrefix[i]=i;
			/* Calculate the'special codes' independence of the initial code size
			   and initialize the stack pointer */ 
			gif->lzw->CodeSize=gif->lzw->SetCodeSize+1;
			gif->lzw->MaxCodeSize=gif->lzw->ClearCode<<1;
			gif->lzw->MaxCode=gif->lzw->ClearCode+2;
			gif->lzw->sp=gif->lzw->aDecompBuffer;
			/* Read the first code from the stack after clear ingand initializing */
			do
			{
				gif->lzw->FirstCode=gif_getnextcode(gfile,gif);
			}while(gif->lzw->FirstCode==gif->lzw->ClearCode);
			gif->lzw->OldCode=gif->lzw->FirstCode;
			return gif->lzw->FirstCode;
		}
		if(Code==gif->lzw->EndCode)return -2;/* End code */
		Incode=Code;
		if(Code>=gif->lzw->MaxCode)
		{
			*(gif->lzw->sp)++=gif->lzw->FirstCode;
			Code=gif->lzw->OldCode;
		}
		while(Code>=gif->lzw->ClearCode)
		{
			*(gif->lzw->sp)++=gif->lzw->aPrefix[Code];
			if(Code==gif->lzw->aCode[Code])return Code;
			if((gif->lzw->sp-gif->lzw->aDecompBuffer)>=sizeof(gif->lzw->aDecompBuffer))return Code;
			Code=gif->lzw->aCode[Code];
		}
		*(gif->lzw->sp)++=gif->lzw->FirstCode=gif->lzw->aPrefix[Code];
		if((Code=gif->lzw->MaxCode)<(1<<MAX_NUM_LWZ_BITS))
		{
			gif->lzw->aCode[Code]=gif->lzw->OldCode;
			gif->lzw->aPrefix[Code]=gif->lzw->FirstCode;
			++gif->lzw->MaxCode;
			if((gif->lzw->MaxCode>=gif->lzw->MaxCodeSize)&&(gif->lzw->MaxCodeSize<(1<<MAX_NUM_LWZ_BITS)))
			{
				gif->lzw->MaxCodeSize<<=1;
				++gif->lzw->CodeSize;
			}
		}
		gif->lzw->OldCode=Incode;
		if(gif->lzw->sp>gif->lzw->aDecompBuffer)return *--(gif->lzw->sp);
	}
	return Code;
}
/**
  *DispGIFImage		 
	  *Purpose:
		This routine draws a GIF image from the current pointer which should point to a
		valid GIF data block. The size of the desired image is given in the image descriptor.
	  *Return value:
		0 if succeed
		1 if not succeed
	  *Parameters:
		pDescriptor  - Points to a IMAGE_DESCRIPTOR structure, which contains infos about size, colors and interlacing.
		x0, y0       - Obvious.
		Transparency - Color index which should be treated as transparent.
		Disposal     - Contains the disposal method of the previous image. If Disposal == 2, the transparent pixels
						of the image are rendered with the background color.
*/
uint8_t gif_dispimage(FIL *gfile,gif89a* gif,uint16_t x0,uint16_t y0,int Transparency, uint8_t Disposal) 
{
	uint32_t readed;	   
   	uint8_t lzwlen;
	int Index,OldIndex,XPos,YPos,YCnt,Pass,Interlace,XEnd;
	int Width,Height,Cnt,ColorIndex;
	uint16_t bkcolor;
	uint16_t *pTrans;

	Width = gif->gifISD.width;
	Height = gif->gifISD.height;
	XEnd = Width+x0-1;
	bkcolor = gif->colortbl[gif->gifLSD.bkcindex];
	pTrans = (uint16_t*)gif->colortbl;
	f_read(gfile, &lzwlen, 1, (UINT*)&readed);/* ��ȡLZW���� */ 
	gif_initlzw(gif, lzwlen);/* Initialize the LZW stack with the LZW code size */ 
	Interlace = gif->gifISD.flag&0x40;/* �Ƿ�֯���� */
	for(YCnt = 0, YPos = y0, Pass = 0; YCnt < Height; YCnt ++)
	{
		Cnt = 0;
		OldIndex = -1;
		for(XPos = x0; XPos <= XEnd; XPos ++)
		{
			if(gif->lzw->sp>gif->lzw->aDecompBuffer)Index=*--(gif->lzw->sp);
		    else Index=gif_getnextbyte(gfile,gif);	   
			if(Index==-2)return 0;		/* Endcode */     
			if((Index<0)||(Index>=gif->numcolors))
			{
				/* If Index out of legal range stop decompressing */
				return 1;/* Error */
			}
			/* If current index equals old index increment counter */
			if((Index==OldIndex)&&(XPos<=XEnd))Cnt++;
	 		else
			{
				if(Cnt)
				{
					if(OldIndex != Transparency)
					{									    
						pic_phy.draw_hline(XPos-Cnt-1, YPos, Cnt+1, *(pTrans + OldIndex));
					}else if(Disposal==2)
					{
						pic_phy.draw_hline(XPos-Cnt-1, YPos, Cnt+1, bkcolor);
					}
					Cnt = 0;
				}else
				{
					if(OldIndex >= 0)
					{
						if(OldIndex != Transparency)
							pic_phy.draw_point(XPos-1,YPos,*(pTrans + OldIndex));
					 	else if(Disposal == 2)
							pic_phy.draw_point(XPos-1, YPos, bkcolor); 
					}
				}
			}
			OldIndex = Index;
		}
		if((OldIndex != Transparency) || (Disposal == 2))
		{
			if(OldIndex != Transparency) ColorIndex = *(pTrans + OldIndex);
		    else ColorIndex = bkcolor;
	 		if(Cnt)
			{
				pic_phy.draw_hline(XPos-Cnt-1, YPos, Cnt+1, ColorIndex);
			}else pic_phy.draw_point(XEnd, YPos, ColorIndex);	 
		}
		/* Adjust YPos if image is interlaced */ 
		if(Interlace)/* ��֯���� */
		{
			YPos += _aInterlaceOffset[Pass];
			if((YPos - y0) >= Height)
			{
				++ Pass;
				YPos = _aInterlaceYPos[Pass] + y0;
			}
		}else YPos++;	    
	}
	return 0;
}

/**
  * @brief  �ָ��ɱ���ɫ
  * @param  x,y:����
  * @param  gif:gif��Ϣ
  * @param  pimge:ͼ����������Ϣ
  * @retval None
  */  			   
void gif_clear2bkcolor(uint16_t x,uint16_t y,gif89a* gif,ImageScreenDescriptor pimge)
{
	uint16_t x0,y0,x1,y1;
	uint16_t color=gif->colortbl[gif->gifLSD.bkcindex];
	if(pimge.width==0||pimge.height==0)return;/* ֱ�Ӳ��������,ԭ��û��ͼ��! */
	if(gif->gifISD.yoff>pimge.yoff)
	{
		x0 = x + pimge.xoff;
		y0 = y + pimge.yoff;
		x1 = x + pimge.xoff + pimge.width - 1;;
		y1 = y + gif->gifISD.yoff - 1;
		if(x0<x1&&y0<y1&&x1<320&&y1<320)	 /* �趨xy,�ķ�Χ����̫�� */
			pic_phy.fill(x0,y0,x1,y1,color); /* ������ɫ��� */  
	}
	if(gif->gifISD.xoff>pimge.xoff)
	{
   		x0 = x+pimge.xoff;
		y0 = y+pimge.yoff;
		x1 = x+gif->gifISD.xoff-1;;
		y1 = y+pimge.yoff+pimge.height-1;
		if(x0<x1&&y0<y1&&x1<320&&y1<320)
			pic_phy.fill(x0,y0,x1,y1,color); /* ������ɫ��� */
	}
	if((gif->gifISD.yoff+gif->gifISD.height)<(pimge.yoff+pimge.height))
	{
   		x0=x+pimge.xoff;
		y0=y+gif->gifISD.yoff+gif->gifISD.height-1;
		x1=x+pimge.xoff+pimge.width-1;;
		y1=y+pimge.yoff+pimge.height-1;
		if(x0<x1&&y0<y1&&x1<320&&y1<320)
			pic_phy.fill(x0,y0,x1,y1,color); /* ������ɫ��� */
	}
 	if((gif->gifISD.xoff+gif->gifISD.width)<(pimge.xoff+pimge.width))
	{
   		x0=x+gif->gifISD.xoff+gif->gifISD.width-1;
		y0=y+pimge.yoff;
		x1=x+pimge.xoff+pimge.width-1;;
		y1=y+pimge.yoff+pimge.height-1;
		if(x0<x1&&y0<y1&&x1<320&&y1<320)
			pic_phy.fill(x0,y0,x1,y1,color); /* ������ɫ��� */
	}   
}

/**
  * @brief  ��GIFͼ���һ֡
  * @param  gfile:gif�ļ�
  * @param  x0,y0:��ʼ��ʾ������
  * @retval None
  */
uint8_t gif_drawimage(FIL *gfile, gif89a* gif, uint16_t x0, uint16_t y0)
{		  
	uint32_t readed;
	uint8_t res,temp;    
	uint16_t numcolors;
	ImageScreenDescriptor previmg;

	uint8_t Disposal;
	int TransIndex;
	uint8_t Introducer;
	TransIndex=-1;				  
	do
	{
		res=f_read(gfile,&Introducer,1,(UINT*)&readed);//��ȡһ���ֽ�
		if(res)return 1;   
		switch(Introducer)
		{		 
			case GIF_INTRO_IMAGE://ͼ������
				previmg.xoff=gif->gifISD.xoff;
 				previmg.yoff=gif->gifISD.yoff;
				previmg.width=gif->gifISD.width;
				previmg.height=gif->gifISD.height;

				res=f_read(gfile,(uint8_t*)&gif->gifISD,9,(UINT*)&readed);//��ȡһ���ֽ�
				if(res)return 1;			 
				if(gif->gifISD.flag&0x80)//���ھֲ���ɫ��
				{							  
					gif_savegctbl(gif);//����ȫ����ɫ��
					numcolors=2<<(gif->gifISD.flag&0X07);//�õ��ֲ���ɫ����С
					if(gif_readcolortbl(gfile,gif,numcolors))return 1;//������	
				}
				if(Disposal==2)gif_clear2bkcolor(x0,y0,gif,previmg); 
				gif_dispimage(gfile,gif,x0+gif->gifISD.xoff,\
					y0+gif->gifISD.yoff,TransIndex,Disposal);
 				while(1)
				{
					f_read(gfile,&temp,1,(UINT*)&readed);//��ȡһ���ֽ�
					if(temp==0)break;
					readed=f_tell(gfile);/* ��������һ�� */
					if(f_lseek(gfile,readed+temp))break;//�������ƫ��	 
			    }
				if(temp!=0)return 1;/* ���� */
				return 0;
			case GIF_INTRO_TERMINATOR:/* ������� */
				return 2;
			case GIF_INTRO_EXTENSION:
				/* Read image extension ��ȡͼ����չ����Ϣ*/
				res = gif_readextension(gfile, gif, &TransIndex, &Disposal);
				if(res)return 1;
	 			break;
			default:
				return 1;
		}
	}while(Introducer != GIF_INTRO_TERMINATOR);/* ��ȡ�������� */
	return 0;
}

/**
  * @brief  Quit
  * @param  None
  * @retval None
  */
void gif_quit(void)
{
	gifdecoding = 0;
}

/** 
  * @brief  ����һ��gif�ļ�(�����Ӳ�����ʾ�ߴ���ڸ����ߴ��gifͼƬ!)
  * @param  filename:��·����gif�ļ�����
  * @param  x,y,width,height:��ʾ���꼰�����С
  * @retval res:������
  */
uint8_t gif_decode(const uint8_t *filename,uint16_t x,uint16_t y,uint16_t width,uint16_t height)
{
	uint8_t res = 0;
	uint16_t dtime = 0;//������ʱ
	gif89a *mygif89a;
	FIL *gfile;
	gfile = &f_gfile;
	mygif89a = &tgif89a;
	mygif89a->lzw = &tlzw;
	if(res == 0)//OK
	{
		res = f_open(gfile, (XCHAR *)filename, FA_READ);
		if(res == 0)//���ļ�ok
		{
			if(gif_check_head(gfile))res = PIC_FORMAT_ERR;
			if(gif_getinfo(gfile,mygif89a))res = PIC_FORMAT_ERR;
			if(mygif89a->gifLSD.width>width||mygif89a->gifLSD.height>height)
				res = PIC_SIZE_ERR;//�ߴ�̫��.
			else
			{
				x = (width - mygif89a->gifLSD.width)/2 + x;
				y = (height - mygif89a->gifLSD.height)/2 + y;
			}
			gifdecoding = 1;
			while(gifdecoding&&res == 0)//����ѭ��
			{	 
				res = gif_drawimage(gfile, mygif89a,x,y);//��ʾһ��ͼƬ
				if(mygif89a->gifISD.flag&0x80)gif_recovergctbl(mygif89a);//�ָ�ȫ����ɫ��
				if(mygif89a->delay)dtime = mygif89a->delay;
				else dtime = 10;//Ĭ����ʱ
				while(dtime -- && gifdecoding)delay_ms(10);//�ӳ�
				if(res == 2)
				{
					res = 0;
					break;
				}
			}
		}
		f_close(gfile);
	}   
	return res;
}

/**************************************************************************************/
//�����ڴ�
//*s:�ڴ��׵�ַ
//c :Ҫ���õ�ֵ
//count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
void mymemset(void *s,uint8_t c,uint32_t count)  
{  
    uint8_t *xs = s;  
    while(count--)*xs++=c;  
}
/**************************************************************************************/