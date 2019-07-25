#include "esp_system.h"
#include "font.h"

 #include "fonts/FreeMono12pt7b.h"
 #include "fonts/FreeMono9pt7b.h"
 #include "fonts/FreeMonoBold9pt7b.h"
 #include "fonts/FreeSans9pt7b.h"
 //#include "fonts/Font0_11px.h"
 #include "fonts/Font7_15px.h"

 extern uint16_t* framebuffer; 
 extern uint16_t framebuffer_width;
 extern uint16_t framebuffer_height;
 GFXfont* gfxFont = &Font7_15px;

void draw_text(uint16_t x, uint16_t y, const char* message, uint16_t count, uint16_t color){

	uint16_t i = 0;
	//GFXfont* gfxFont = &FreeSans9pt7b;
	uint8_t first = gfxFont->first;
	
	while(message[i]!=0 && i<count){
		uint8_t c = message[i];
		GFXglyph *glyph = &(gfxFont->glyph[c - first]);
		drawGlyph(x, y, glyph,gfxFont->bitmap, color);
		x += glyph->xAdvance;
		//y += gfxFont->yAdvance;
		i++;
	}
}

void drawGlyph(int16_t x, int16_t y, GFXglyph *glyph, uint8_t  *bitmap, uint16_t color){
	
	uint16_t bo = glyph->bitmapOffset;
	uint8_t  w  = glyph->width,
	h  = glyph->height;
	int8_t   xo = glyph->xOffset,
	yo = glyph->yOffset;

//	printf("(10) w,h,xo, yo: %d,%d,%d,%d\n", w,h,xo,yo);

	uint8_t  xx, yy, bits = 0, bit = 0;

	// Todo: Add character clipping here

	// NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
	// THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
	// has typically been used with the 'classic' font to overwrite old
	// screen contents with new data.  This ONLY works because the
	// characters are a uniform size; it's not a sensible thing to do with
	// proportionally-spaced fonts with glyphs of varying sizes (and that
	// may overlap).  To replace previously-drawn text when using a custom
	// font, use the getTextBounds() function to determine the smallest
	// rectangle encompassing a string, erase the area with fillRect(),
	// then draw new text.  This WILL infortunately 'blink' the text, but
	// is unavoidable.  Drawing 'background' pixels will NOT fix this,
	// only creates a new set of problems.  Have an idea to work around
	// this (a canvas object type for MCUs that can afford the RAM and
	// displays supporting setAddrWindow() and pushColors()), but haven't
	// implemented this yet.

//	printf("Before Loop\n");


	int x1,y1;

	//startWrite();
	for(yy=0; yy<h; yy++) {
		for(xx=0; xx<w; xx++) {
			if(!(bit++ & 7)) {
				bits = bitmap[bo++];
			}
			if(bits & 0x80) {
				x1 = x+xo+xx;
				y1 = y+yo+yy;
				if((x1>=0) && (y1>=0) && (x1<framebuffer_width) && (y1<framebuffer_height)){
					//writePixel(x1, y1, color);
					framebuffer[x1+((y1)*framebuffer_width)]=color;
				}
			}
			bits <<= 1;
		}
	}
//	printf("Loop End.\n");

}

uint16_t FntWordWrap(const char* message, uint16_t maxWidth){
	uint16_t lastBreak = 0;
	uint16_t width = 0;
	
	uint16_t i = 0;
	//GFXfont* gfxFont = &FreeSans9pt7b;
	uint8_t first = gfxFont->first;
	
	printf("FntWordWrap(%s,%d)\n",message,maxWidth);
	
	while(message[i]!=0 && width < maxWidth){
		uint8_t c = message[i];
		
		if(c==' '){
			lastBreak = i;
		}
		
		GFXglyph *glyph = &(gfxFont->glyph[c - first]);
		width += glyph->xAdvance;
		i++;
		
		if(width>maxWidth){
			printf("(%d>%d)=> %d\n",width,maxWidth,lastBreak);
			return lastBreak;
		}
	}

	printf("=> %d\n",i);
	return(i);
}


uint16_t FntLineHeight(){
	//GFXfont* gfxFont = &FreeSans9pt7b;
	return(gfxFont->yAdvance);
}

uint16_t FntLineWidth(const char* message, uint16_t count){
	uint16_t width = 0;
	uint16_t i = 0;
	//GFXfont* gfxFont = &FreeSans9pt7b;
	uint8_t first = gfxFont->first;
	
	while(message[i]!=0 && i < count){
		uint8_t c = message[i];
		GFXglyph *glyph = &(gfxFont->glyph[c - first]);
		width += glyph->xAdvance;
		i++;
	}
	
	return(width);
	
}