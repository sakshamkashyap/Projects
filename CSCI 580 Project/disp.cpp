/*   CS580 HW1 display functions to be completed   */

#include   "stdafx.h"  
#include	"Gz.h"
#include	"disp.h"


int GzNewFrameBuffer(char** framebuffer, int width, int height) // GOOD
{
/* HW1.1 create a framebuffer for MS Windows display:
 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
 -- pass back pointer 
 */

	// check for width and height validity
	if (width <= 0 || height <= 0 || width > MAXXRES || height > MAXYRES) {
		return GZ_FAILURE;
	}

	// allocate
	char * newFrameBuffer = new char[3*width*height]; 

	// check if null 
	if (newFrameBuffer == NULL){
		return GZ_FAILURE;
	}

	// pass back
	*framebuffer = newFrameBuffer;
	
	return GZ_SUCCESS;
}

int GzNewDisplay(GzDisplay	**display, int xRes, int yRes) //OK
{
	/* HW1.2 create a display:
	  -- allocate memory for indicated resolution
	  -- pass back pointer to GzDisplay object in display
	  */
	// check the given xRes and yRes
	if (xRes <= 0 || yRes <= 0 || xRes > MAXXRES || yRes > MAXYRES){
		return GZ_FAILURE;
	}
	// create a new display pointer
	GzDisplay * newDisplay = new GzDisplay;
	newDisplay->xres = xRes;
	newDisplay->yres = yRes;
	newDisplay->fbuf = new GzPixel[xRes*yRes];

	// pass back the pointer
	*display = newDisplay;
	return GZ_SUCCESS;
}


int GzFreeDisplay(GzDisplay	*display)
{
/* HW1.3 clean up, free memory */

	// if the display exists, delete it
	if (display != NULL){
		delete display;
	}
	return GZ_SUCCESS;
}


int GzGetDisplayParams(GzDisplay *display, int *xRes, int *yRes)
{
/* HW1.4 pass back values for a display */

	// set xRes and yRes to values in given display
	*xRes = display->xres;
	*yRes = display->yres;

	return GZ_SUCCESS;
}


int GzInitDisplay(GzDisplay	*display)
{
/* HW1.5 set everything to some default values - start a new frame */

	// check display
	if (display == NULL){
		return GZ_FAILURE;
	}

	// loop through the display's frame buffer
	int bufferSize = (display->xres)*(display->yres);

	for (int i = 0; i < bufferSize; i++){
		// for each pixel, set default values
		display->fbuf[i].blue = 1992;
		display->fbuf[i].green = 1992;
		display->fbuf[i].red = 1992;

		display->fbuf[i].alpha = 1;
		display->fbuf[i].z = INT_MAX;  // set z to max int
	}
	

	return GZ_SUCCESS;
}


int GzPutDisplay(GzDisplay *display, int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	/* HW1.6 write pixel values into the display */

	// check display
	if (display == NULL) {
		return GZ_FAILURE;
	}

	// check the indices for validity
	//int buffersize = (display->xres)*(display->yres);
	//if (ARRAY(i, j) < 0 || ARRAY(i, j) > buffersize){
	//	return GZ_FAILURE;
	//}
	if (i < 0 || i >= display->xres || j < 0 || j >= display->yres){
		return GZ_FAILURE;
	}



	// check the rgb values and clamp if too large
	if (r < 0) r = 0;
	if (g < 0) g = 0;
	if (b < 0) b = 0;


	if (r > 4095) {
		r = 4095;
	}

	if (g > 4095) {
		g = 4095;
	}

	if (b > 4095) {
		b = 4095;
	}

	// put rgb values into display
	display->fbuf[ARRAY(i, j)].red = r;
	display->fbuf[ARRAY(i, j)].green = g;
	display->fbuf[ARRAY(i, j)].blue = b;

	// put alpha and z values
	display->fbuf[ARRAY(i, j)].alpha = a;
	display->fbuf[ARRAY(i, j)].z = z;

	return GZ_SUCCESS;
}


int GzGetDisplay(GzDisplay *display, int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
/* HW1.7 pass back a pixel value to the display */

	// again check the display
	if (display == NULL) {
		return GZ_FAILURE;
	}

	// check the indices for validity
	int buffersize = (display->xres)*(display->yres);
	if (ARRAY(i, j) < 0 || ARRAY(i, j) > buffersize){
		return GZ_FAILURE;
	}

	// pass back the pixel values

    *r = display->fbuf[ARRAY(i, j)].red;
	*b = display->fbuf[ARRAY(i, j)].blue;
	*g = display->fbuf[ARRAY(i, j)].green;

	*a = display->fbuf[ARRAY(i, j)].alpha;
	*z = display->fbuf[ARRAY(i, j)].z;
	return GZ_SUCCESS;
}


int GzFlushDisplay2File(FILE* outfile, GzDisplay *display)
{

/* HW1.8 write pixels to ppm file -- "P6 %d %d 255\r" */

	// check the display
	if (display == NULL){
		return GZ_FAILURE;
	}

	// check the file pointer
	if (outfile == NULL){
		return GZ_FAILURE;
	}


	// the file pointer comes in already opened

	// write the first line of the .ppm file
	fprintf(outfile, "P6 %d %d 255\r", display->xres, display->yres);

	int bufferSize = (display->xres) * (display->yres);

	// loop through all pixels
	for (int i = 0; i < bufferSize; i++){

		// current pixel
		GzPixel current = display->fbuf[i];
		current.red = current.red >> 4;
		current.green = current.green >> 4;
		current.blue = current.blue >> 4;

		// address of color, how many bytes, file pointer
		fwrite(&current.red, 1, 1, outfile);
		fwrite(&current.green, 1, 1, outfile);
		fwrite(&current.blue, 1, 1, outfile);

	}



	return GZ_SUCCESS;
}

int GzFlushDisplay2FrameBuffer(char* framebuffer, GzDisplay *display)
{

/* HW1.9 write pixels to framebuffer: 
	- put the pixels into the frame buffer
	- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red 
	- NOT red, green, and blue !!!
*/

	// check framebuffer and display

	if (framebuffer == NULL || display == NULL ){
		return GZ_FAILURE;
	}

	// loop through all the pixels
	int bufferSize = (display->xres)*(display->yres);
	int fb_index = 0;

	for (int i = 0; i < bufferSize-1; i++){

		// grab the current pixel
		GzPixel current = display->fbuf[i];

		// convert to 0 - 255
		char blue = current.blue >> 4;
		char green = current.green >> 4;
		char red = current.red >> 4;

		sprintf(framebuffer + fb_index, "%c", blue);
		sprintf(framebuffer + fb_index + 1, "%c", green);
		sprintf(framebuffer + fb_index + 2 , "%c", red);

		fb_index += 3;

	}
	return GZ_SUCCESS;
}