/*   CS580 HW1 display functions to be completed
	 Submitted by- Saksham Kashyap */

#include   "stdafx.h"  
#include	"Gz.h"
#include	"disp.h"


int GzNewFrameBuffer(char** framebuffer, int width, int height)
{
	//Memory allocation for framebuffer
	*framebuffer = new char[3 * width * height];
	return GZ_SUCCESS;
}

int GzNewDisplay(GzDisplay	**display, int xRes, int yRes)
{
	//Memory allocation for indicated resolution
	if (xRes < 0 || xRes > MAXXRES || yRes < 0 || yRes > MAXYRES)
		return GZ_FAILURE;

	GzDisplay * my_display = new GzDisplay;

	my_display->xres = xRes;
	my_display->yres = yRes;
	my_display->fbuf = new GzPixel[24 * xRes * yRes];

	*display = my_display;
	return GZ_SUCCESS;
}


int GzFreeDisplay(GzDisplay	*display)
{
	//free memory
	delete[] display->fbuf;
	//display->fbuf = NULL;
	delete display;
	return GZ_SUCCESS;
}


int GzGetDisplayParams(GzDisplay *display, int *xRes, int *yRes)
{
	//values passed back for display
	*xRes = display->xres;
	*yRes = display->yres;
	return GZ_SUCCESS;
}


int GzInitDisplay(GzDisplay	*display)
{
	//Initialising new frame and fill background color
	int disp_row, disp_col;
	for (disp_row = 0; disp_row < display->yres; disp_row++)  //One loop each for x and y coodrinates
	{
		for (disp_col = 0; disp_col < display->xres; disp_col++)
		{
			int index = ARRAY(disp_col, disp_row);

			display->fbuf[index].red = 1992; //value of RGB for background color
			display->fbuf[index].blue = 1992;
			display->fbuf[index].green = 1992;
			display->fbuf[index].alpha = 1;
			display->fbuf[index].z = INT_MAX;
		}
	}
	return GZ_SUCCESS;
}


int GzPutDisplay(GzDisplay *display, int disp_col, int disp_row, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	//writing pixel values into the display

	if (!display)
		return GZ_FAILURE;

	if (disp_col < 0 || disp_row < 0 || disp_col >= display->xres || disp_row >= display->yres)
		return GZ_SUCCESS;

	if (r < 0) //bound the coordinate components in range 0-4095
		r = 0;
	else if (r > 4095)
		r = 4095;

	if (g < 0)
		g = 0;
	else if (g > 4095)
		g = 4095;

	if (b < 0)
		b = 0;
	else if (b > 4095)
		b = 4095;

	int index = ARRAY(disp_col, disp_row);
	display->fbuf[index].red = r;
	display->fbuf[index].green = g;
	display->fbuf[index].blue = b;
	display->fbuf[index].alpha = a;
	display->fbuf[index].z = z;
	return GZ_SUCCESS;
	
}


int GzGetDisplay(GzDisplay *display, int disp_col, int disp_row, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
	//Pass back value to display
	if (!display) 
		return GZ_FAILURE;
	
	if (ARRAY(disp_col, disp_row) >= (display->xres * display->yres) || ARRAY(disp_col, disp_row) <= 0) 
		return GZ_FAILURE;
	
	GzPixel pixel = display->fbuf[ARRAY(disp_col, disp_row)];
	*r = pixel.red;
	*g = pixel.green;
	*b = pixel.blue;
	*a = pixel.alpha;
	*z = pixel.z;
	return GZ_SUCCESS;
}

int GzFlushDisplay2File(FILE* outfile, GzDisplay *display)
{

	//Writing pixel value to ppm file
	int disp_row, disp_col;
	char ppm_red, ppm_green, ppm_blue;

	fprintf(outfile, "P6 %d %d 255\r", display->xres, display->yres); //initialising print to external file

	for (disp_row = 0; disp_row < display->yres; disp_row++)
	{
		for (disp_col = 0; disp_col < display->xres; disp_col++)
		{
			int index = ARRAY(disp_col, disp_row);

			ppm_red = display->fbuf[index].red >> 4; //8 bits usage instead of 12 bits - Right shift 4 bits
			ppm_green = display->fbuf[index].green >> 4;
			ppm_blue = display->fbuf[index].blue >> 4;

			fprintf(outfile, "%c%c%c", ppm_red, ppm_green, ppm_blue); //writing to external file
		}
	}
	return GZ_SUCCESS;
}

int GzFlushDisplay2FrameBuffer(char* framebuffer, GzDisplay *display)
{

	//Writing pixels into frame buffer
	if (!framebuffer)
		GZ_FAILURE;

	int disp_row, disp_col;
	char fb_red, fb_green, fb_blue;

	if (display != NULL)
	for (disp_row = 0; disp_row < display->yres; disp_row++)
	{
		for (disp_col = 0; disp_col < display->xres; disp_col++)
		{
			int fb_index = ARRAY(disp_col, disp_row);

			fb_red = display->fbuf[fb_index].red >> 4;
			fb_green = display->fbuf[fb_index].green >> 4;
			fb_blue = display->fbuf[fb_index].blue >> 4;

			framebuffer[fb_index * 3] = fb_blue; //writing into frame buffer sequentially one by one
			framebuffer[fb_index * 3 + 1] = fb_green;
			framebuffer[fb_index * 3 + 2] = fb_red;

		}
	}
	return GZ_SUCCESS;
}
