// MdeModulePkg/Library/FrameBufferBltLib/FrameBufferBltLib.c
#include"internal.h"
typedef struct fb_cfg{
	uint32_t scan,bpp;
	uint32_t width,height;
	uint8_t*buffer;
	efi_graphics_pixel_format format;
	efi_pixel_bitmask masks;
	int8_t shl[4],shr[4];
	uint8_t line[0];
}fb_cfg;
static const efi_pixel_bitmask rgb_mask={0x000000ff,0x0000ff00,0x00ff0000,0xff000000};
static const efi_pixel_bitmask bgr_mask={0x00ff0000,0x0000ff00,0x000000ff,0xff000000};
static inline intn_t h32(uint32_t val){
	intn_t i;
	if(val==0)return -1;
	for(i=31;(int32_t)val>0;i--,val<<=1);
	return i;
}
static void fb_blt_setup_format(
	const efi_pixel_bitmask*mask,
	uint32_t*bpp,int8_t*shl,int8_t*shr
){
	uint32_t*masks=(uint32_t*)mask,mm=0;
	for(uint8_t i=0;i<3;i++){
		shl[i]=(int8_t)h32(masks[i])-23+(i*8);
		if(shl[i]<0){
			shr[i]=-shl[i];
			shl[i]=0;
		}else shr[i]=0;
		mm=(uint32_t)(mm|masks[i]);
	}
	mm=(uint32_t)(mm|masks[3]);
	*bpp=(uint32_t)((h32(mm)+7)/8);
}
efi_status efiapi fb_blt_setup(
	void*buffer,
	efi_graphics_output_mode_information*info,
	fb_cfg*cfg,uintn_t*size
){
	uint32_t bpp;
	int8_t shl[4],shr[4];
	const efi_pixel_bitmask*mask;
	if(!buffer||!info||!size)return efi_invalid_parameter;
	switch(info->format){
		case PIXEL_RGBA8888:mask=&rgb_mask;break;
		case PIXEL_BGRA8888:mask=&bgr_mask;break;
		case PIXEL_BIT_MASK:mask=&info->mask;break;
		case PIXEL_BLT_ONLY:return efi_unsupported;
		default:return efi_invalid_parameter;
	}
	if(info->scan<info->width)return efi_unsupported;
	fb_blt_setup_format(mask,&bpp,shl,shr);
	if(*size<sizeof(fb_cfg)+info->width*bpp){
		*size=sizeof(fb_cfg)+info->width*bpp;
		return efi_buffer_too_small;
	}
	if(!cfg)return efi_invalid_parameter;
	memcpy(&cfg->masks,mask, sizeof(*mask));
	memcpy(cfg->shl,shl,sizeof(shl));
	memcpy(cfg->shr,shr,sizeof(shr));
	cfg->bpp=bpp;
	cfg->format=info->format;
	cfg->buffer=(uint8_t*)buffer;
	cfg->width=info->width;
	cfg->height=info->height;
	cfg->scan=info->scan;
	return efi_success;
}
static efi_status fb_blt_video_fill(
	fb_cfg*cfg,
	efi_graphics_output_blt_pixel*color,
	uintn_t dst_x,uintn_t dst_y,
	uintn_t width,uintn_t height
){
	uint8_t*dst,u8;
	uint32_t u32;
	uint64_t wf;
	bool xwf=true,ready;
	uintn_t ix,iy,off,ws,size;
	if(
		!color||width<=0||height<=0||
		dst_y+height>cfg->height||dst_x+width>cfg->width
	)return efi_invalid_parameter;
	ws=width*cfg->bpp;
	u32=*(uint32_t*)color;
	wf=(uint32_t)(
		(((u32<<cfg->shl[0])>>cfg->shr[0])&cfg->masks.red_mask)|
		(((u32<<cfg->shl[1])>>cfg->shr[1])&cfg->masks.green_mask)|
		(((u32<<cfg->shl[2])>>cfg->shr[2])&cfg->masks.blue_mask)
	);
	if((sizeof(wf)%cfg->bpp)!=0){
		for(ix=1,u8=((uint8_t*)&wf)[0];ix<cfg->bpp;ix++)if(u8!=((uint8_t*)&wf)[ix]){
			xwf=false;
			break;
		}
		if(xwf)for(size_t i=0;i<sizeof(wf);i++)((uint8_t*)wf)[i]=u8;
	}else for(ix=cfg->bpp;ix<sizeof(wf);ix++)
		((uint8_t*)&wf)[ix]=((uint8_t*)&wf)[ix%cfg->bpp];
	if(xwf&&(dst_x==0)&&(width==cfg->scan)){
		off=dst_y*cfg->scan;
		off=cfg->bpp*off;
		dst=cfg->buffer+off;
		size=ws*height;
		if(size>=8){
			for(size_t i=0;i<(size&~3)/4;i++)((uint32_t*)dst)[i]=wf;
			dst+=size&~3,size&=3;
		}
		if(size>0)for(size_t i=0;i<size;i++)((uint8_t*)dst)[i]=wf;
	}else{
		ready=false;
		for(iy=dst_y;iy<(height+dst_y);iy++){
			off=(iy*cfg->scan)+dst_x;
			off=cfg->bpp*off;
			dst=cfg->buffer+off;
			if(xwf&&(((uintn_t)dst&7)==0)){
				size=ws;
				if(size>=8){
					for(size_t i=0;i<(size&~7)/8;i++)
						((uint64_t*)dst)[i]=wf;
					dst+=size&~7,size&=7;
				}
				if(size>0)memcpy(dst,&wf,size);
			}else{
				if(!ready){
					memcpy(cfg->line,&wf,cfg->bpp);
					for(ix=1;ix<width;){
						memcpy(
							(cfg->line+(ix*cfg->bpp)),cfg->line,
							MIN(ix,width-ix)*cfg->bpp
						);
						ix+=MIN(ix,width-ix);
					}
					ready=true;
				}
				memcpy(dst,cfg->line,ws);
			}
		}
	}
	return efi_success;
}
static efi_status fb_blt_video_to_buffer(
	fb_cfg*cfg,
	efi_graphics_output_blt_pixel*buffer,
	uintn_t src_x,uintn_t src_y,
	uintn_t dst_x,uintn_t dst_y,
	uintn_t width,uintn_t height,
	uintn_t delta
){
	efi_graphics_output_blt_pixel*blt;
	uint8_t*src,*dst;
	uint32_t u32;
	uintn_t dy,sy,ix,off,ws;
	if(
		!buffer||width<=0||height<=0||
		src_y+height>cfg->height||src_x+width>cfg->width
	)return efi_invalid_parameter;
	if(delta==0)delta=width*sizeof(efi_graphics_output_blt_pixel);
	ws=width*cfg->bpp;
	for(sy=src_y,dy=dst_y;dy<(height+dst_y);sy++,dy++){
		off=(sy*cfg->scan)+src_x,off=cfg->bpp*off,src=cfg->buffer+off;
		dst=cfg->format==PIXEL_BGRA8888?(uint8_t*)buffer+
			(dy*delta)+(dst_x*sizeof(efi_graphics_output_blt_pixel)):cfg->line;
		memcpy(dst,src,ws);
		if(cfg->format!=PIXEL_BGRA8888)for(ix=0;ix<width;ix++){
			blt=(efi_graphics_output_blt_pixel*)((uint8_t*)buffer+
				(dy*delta)+(dst_x+ix)*sizeof(efi_graphics_output_blt_pixel));
			u32=*(uint32_t*)(cfg->line+(ix*cfg->bpp));
			*(uint32_t*)blt=(uint32_t)(
				(((u32&cfg->masks.red_mask)>>cfg->shl[0])<<cfg->shr[0])|
				(((u32&cfg->masks.green_mask)>>cfg->shl[1])<<cfg->shr[1])|
				(((u32&cfg->masks.blue_mask)>>cfg->shl[2])<<cfg->shr[2])
			);
		}
	}
	return efi_success;
}
static efi_status fb_blt_buffer_to_video(
	fb_cfg*cfg,
	efi_graphics_output_blt_pixel*buffer,
	uintn_t src_x,uintn_t src_y,
	uintn_t dst_x,uintn_t dst_y,
	uintn_t width,uintn_t height,
	uintn_t delta
){
	efi_graphics_output_blt_pixel*blt;
	uint8_t*src,*dst;
	uint32_t u32;
	uintn_t ix,dy,sy,off,ws;
	if(
		!buffer||width<=0||height<=0||
		dst_y+height>cfg->height||dst_x+width>cfg->width
	)return efi_invalid_parameter;
	if(delta==0)delta=width*sizeof(efi_graphics_output_blt_pixel);
	ws=width*cfg->bpp;
	for(sy=src_y,dy=dst_y;sy<(height+src_y);sy++,dy++){
		off=(dy*cfg->scan)+dst_x,off=cfg->bpp*off,dst=cfg->buffer+off;
		if(cfg->format!=PIXEL_BGRA8888){
			for(ix=0;ix<width;ix++){
				blt=(efi_graphics_output_blt_pixel*)((uint8_t*)buffer+
					(sy*delta)+((src_x+ix)*sizeof(efi_graphics_output_blt_pixel)));
				u32=*(uint32_t*)blt;
				*(uint32_t*)(cfg->line+(ix*cfg->bpp))=(uint32_t)(
					(((u32<<cfg->shl[0])>>cfg->shr[0])&cfg->masks.red_mask)|
					(((u32<<cfg->shl[1])>>cfg->shr[1])&cfg->masks.green_mask)|
					(((u32<<cfg->shl[2])>>cfg->shr[2])&cfg->masks.blue_mask)
				);
			}
			src=cfg->line;
		}else src=(uint8_t*)buffer+(sy*delta)+src_x*sizeof(efi_graphics_output_blt_pixel);
		memcpy(dst,src,ws);
	}
	return efi_success;
}
static efi_status fb_blt_video_to_video(
	fb_cfg*cfg,
	uintn_t src_x,uintn_t src_y,
	uintn_t dst_x,uintn_t dst_y,
	uintn_t width,uintn_t height
){
	uint8_t*src,*dst;
	uintn_t off,ws;
	intn_t stride;
	if(
		width<=0||height<=0||
		src_y+height>cfg->height||src_x+width>cfg->width||
		dst_y+height>cfg->height||dst_x+width>cfg->width
	)return efi_invalid_parameter;
	ws=width*cfg->bpp,off=(src_y*cfg->scan)+src_x;
	off=cfg->bpp*off,src=cfg->buffer+off,off=(dst_y*cfg->scan)+dst_x;
	off=cfg->bpp*off,dst=cfg->buffer+off,stride=cfg->bpp*cfg->scan;
	if(dst>src)src+=height*stride,dst+=height*stride,stride=-stride;
	while(height-->0){
		memcpy(dst,src,ws);
		src+=stride,dst+=stride;
	}
	return efi_success;
}
efi_status fb_blt(
	fb_cfg*cfg,
	efi_graphics_output_blt_pixel*buffer,
	efi_graphics_output_blt_operation oper,
	uintn_t src_x,uintn_t src_y,
	uintn_t dst_x,uintn_t dst_y,
	uintn_t width,uintn_t height,
	uintn_t delta
){
	if(!cfg)return efi_invalid_parameter;
	switch(oper){
		case efi_blt_video_to_blt_buffer:return fb_blt_video_to_buffer(
			cfg,buffer,src_x,src_y,dst_x,dst_y,width,height,delta
		);
		case efi_blt_video_to_video:return fb_blt_video_to_video(
			cfg,src_x,src_y,dst_x,dst_y,width,height
		);
		case efi_blt_video_fill:return fb_blt_video_fill(
			cfg,buffer,dst_x,dst_y,width,height
		);
		case efi_blt_buffer_to_video:return fb_blt_buffer_to_video(
			cfg,buffer,src_x,src_y,dst_x,dst_y,width,height,delta
		);
		default:return efi_invalid_parameter;
	}
}
