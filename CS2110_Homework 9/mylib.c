#include "mylib.h"
#include "game.h"

u16 *videoBuffer = (u16 *)0x6000000;
double secs_per_tick[4] = {59.604E-9, 3.811E-6, 15.259E-6, 59.382E-6};

//functions
// A function to set pixel (r, c) to the color passed in.
void setPixel(int r, int c, u16 color)
{
	videoBuffer[OFFSET(r, c, 240)] = color;
}

// A function to draw a FILLED rectangle starting at (r, c)
void drawRect(int row, int col, int height, int width, volatile u16 color)
{
	int r;
	for(r=0; r<height; r++)
	{
		DMA[3].src = &color;
		DMA[3].dst = videoBuffer+OFFSET(row+r, col, 240);
		DMA[3].cnt = DMA_ON | DMA_SOURCE_FIXED | width;
	}	
}

// A function to draw a HOLLOW rectangle starting at (r, c)
// NOTE: It has to run in O(w+h) time.
void drawHollowRect(int r, int c, int width, int height, u16 color)
{
	int row, col;
	for(row=0; row<height; row++) {
		//draw top pixel
		videoBuffer[OFFSET(r+row, c, 240)] = color;
		//draw bottom pixel in same c
		videoBuffer[OFFSET(r+row, c+width, 240)] = color;
	}
	for (col=0; col<=width; col++) {
		//draw left
		videoBuffer[OFFSET(r, c+col, 240)] = color;
		//draw right using same r
		videoBuffer[OFFSET(r+height, c+col, 240)] = color;
	}
}

void drawChar(int row, int col, char ch, u16 color)
{
	int r,c;
	for (r=0; r<8; r++)
	{
		for (c=0; c<6; c++)
		{
			if (fontdata_6x8[OFFSET(r, c, 6)+ch*48])
			{
				setPixel(row+r, col+c, color);
			}
		}
	}
}

void drawString(int row, int col, char *str, u16 color)
{
	while(*str) //dont need != '\0' in condition
	{
		drawChar(row, col, *str++, color);
		col += 6;
	}
}

void stringWithBack(int row, int col, char *str, u16 color, u16 color2)
{
	while(*str) //dont need != '\0' in condition
	{
		drawCharBack(row, col, *str++, color, color2);
		col += 6;
	}
}

void drawCharBack(int row, int col, char ch, u16 color, u16 color2)
{
	int r,c;
	for (r=0; r<8; r++)
	{
		for (c=0; c<6; c++)
		{
			if (fontdata_6x8[OFFSET(r, c, 6)+ch*48])
			{
				setPixel(row+r, col+c, color);
			} else {
				setPixel(row+r, col+c, color2);
			}
		}
	}
}

void drawTri(int row, int col, int base, int height, u16 color)
{	
	int r,c;
	for (c=0, r=0; c<=base; c++) {
		setPixel(row, col+c, color); //base line
		if (c <= (base/2)){
			setPixel(row-r, col+c-1, color); //left diag
			r++;
		} else {
			setPixel(row-r, col+c-1, color); //right diag
			r--;
		}
	}
	setPixel(row-r, col+c-1, color); //right diag
	r--;
}

void drawStar(int row, int col, int rad, u16 color)
{
	int r,c,i;
	for (c=0, r=0; c<=(2*rad); c++)
	{
		setPixel(row+rad+r, col+c, color);
		setPixel(row+c ,col+rad+r, color);
		for (i=0; i<=(2*rad); i++)
		{
			setPixel(row+i, col+i, color);
			setPixel(row+(2*rad)-i, col+i, color);
		}
	}	
}

/* drawimage3
* A function that will draw an arbitrary sized image
* onto the screen (with DMA).
* @param r row to draw the image
* @param c column to draw the image
* @param width width of the image
* @param height height of the image
* @param image Pointer to the first element of the image.
*/
void drawImage3(int r, int c, int width, int height, const u16* image)
{
	int row;
	for(row=0; row<height; row++)
	{
		DMA[DMA_CHANNEL_3].src = image + OFFSET(row, 0, width);	
		DMA[DMA_CHANNEL_3].dst = videoBuffer + OFFSET(row+r, c, 240);
		DMA[DMA_CHANNEL_3].cnt = DMA_ON | width;
	}
}



void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void redrawbackground(int r, int c, int width, int height, const u16* image) {
	int row;
	for(row=0; row<height; row++)
	{
		DMA[DMA_CHANNEL_3].src = image + c + (r*240)+ (row*240);	
		DMA[DMA_CHANNEL_3].dst = videoBuffer + c + (r*240)+ row*240; 
		DMA[DMA_CHANNEL_3].cnt = DMA_ON | width;
		 
	}
}
