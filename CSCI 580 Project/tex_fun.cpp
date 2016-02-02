/* Texture functions for cs580 GzLib    */
#include    "stdafx.h" 
#include        "stdio.h"
#include        "math.h"
#include        "Gz.h"
#include        "CustomizablePerlinNoise.h"
#include        "WorleyNoise.h"

float RGBCode(float a);

GzColor *image;
int xs, ys;
int reset = 1;

#define IMG_ARRAY( x, y, xres ) ( x + ( y * xres ) )    /* simplify image indexing */
#define MAXALT 64.0
#define SYMMETRICAL_STONE 0

enum StoneType
{
	COLORFUL,
	REALISTIC
};

StoneType stoneType = REALISTIC;

/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
	unsigned char         pixel[3];
	unsigned char     dummy;
	char                  foo[8];
	int                   i, j;
	FILE                  *fd;

	if (reset) {          /* open and load texture file */
		fd = fopen("texture", "rb");
		if (fd == NULL) {
			fprintf(stderr, "texture file not found\n");
			exit(-1);
		}
		fscanf(fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
		image = (GzColor*)malloc(sizeof(GzColor)*(xs + 1)*(ys + 1));
		if (image == NULL) {
			fprintf(stderr, "malloc for texture image failed\n");
			exit(-1);
		}

		for (i = 0; i < xs*ys; i++) {       /* create array of GzColor values */
			fread(pixel, sizeof(pixel), 1, fd);
			image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
			image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
			image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
		}

		reset = 0;          /* init is done */
		fclose(fd);
	}

	/* bounds-test u,v to make sure nothing will overflow image array bounds */
	/* determine texture cell corner values and perform bilinear interpolation */
	/* set color to interpolated GzColor value and return */

	// u and v should vary in range [0,1]. Clamp values if they are outside of bounds.
	if (u < 0)
		u = 0;
	if (u > 1)
		u = 1;
	if (v < 0)
		v = 0;
	if (v > 1)
		v = 1;

	// compute texel index. Note that the maximum texel X is (xs - 1) and the maximum texel Y is (xy - 1)
	float texelX = u * (xs - 1);
	float texelY = v * (ys - 1);

	// find texel that form square around this texture coordinate location (e.g. texture cell corner values)
	float x_min = floor(texelX);
	float x_max = ceil(texelX);
	float y_min = floor(texelY);
	float y_max = ceil(texelY);

	/*
	* The variable names below correspond to this layout:
	*    color0                          color1
	*    (x_min, y_min)          (x_max, y_min)
	*
	*
	*    color2                          color3
	*    (x_min, y_max)          (x_max, y_max)
	*/
	// now retrieve the colors at the four cell corner values
	GzColor color0, color1, color2, color3;
	memcpy(color0, image[(int)IMG_ARRAY(x_min, y_min, xs)], sizeof(GzColor));
	memcpy(color1, image[(int)IMG_ARRAY(x_max, y_min, xs)], sizeof(GzColor));
	memcpy(color2, image[(int)IMG_ARRAY(x_min, y_max, xs)], sizeof(GzColor));
	memcpy(color3, image[(int)IMG_ARRAY(x_max, y_max, xs)], sizeof(GzColor));

	// now we need to perform bilinear interpolation on these colors. First interpolate the rows
	GzColor colorTopRow, colorBottomRow;
	float left_weight = x_max - texelX;
	float top_weight = y_max - texelY;
	for (i = 0; i < 3; i++)
	{
		// interpolate this color component for the top and bottom row
		colorTopRow[i] = (left_weight * color0[i]) + ((1 - left_weight) * color1[i]);
		colorBottomRow[i] = (left_weight * color2[i]) + ((1 - left_weight) * color3[i]);

		// now interpolate between the top and bottom row
		color[i] = (top_weight * colorTopRow[i]) + ((1 - top_weight) * colorBottomRow[i]);
	}

	return GZ_SUCCESS;
}

void perlinNoise(float u, float v, GzColor color1, GzColor color2, int scaleFactor, GzColor result){


		static CustomizablePerlinNoise noiseMaker;
		static bool initialized = false;

		if (!initialized)
		{
			noiseMaker.setParams(1, 4, 4, 1, 94);
			noiseMaker.setParams(3, 8, 4, 1, 290);
			/*noiseMaker.setParams(2, 6, 4, 1, 190);
			noiseMaker.setParams(3, 8, 4, 1, 290);
			noiseMaker.setParams(4, 4, 4, 1, 934);*/
			initialized = true;
		}

		// note: in order to make the texture continuous, we need the edges of the texture to all be the same color
		float modifiedU, modifiedV;

		// not sure what this does/ why?
		modifiedU = (u < 0.5) ? u : 1 - u;
		modifiedV = (v < 0.5) ? v : 1 - v;

		float perlinNoise;

			// Multiply modifiedU and modifiedV by different numbers to change the scale
			perlinNoise = (1 + noiseMaker.Get(modifiedU * scaleFactor, modifiedV * scaleFactor)) / 2;

			result[RED] = color1[RED] * perlinNoise + color2[RED] * (1 - perlinNoise);
			result[BLUE] = color1[BLUE] * perlinNoise + color2[BLUE] * (1 - perlinNoise);
			result[GREEN] = color1[GREEN] * perlinNoise + color2[GREEN] * (1 - perlinNoise);
		
}

/* Procedural texture function */
int ptex_fun(float u, float v, float altitude, float slope, GzColor color)
{
	// scale the screen x and z to be 0-1
	// These need to be based on the heightmap width and height
	//u = u / 16.0;
	//v = v / 16.0;
	u = u / 128.0;
	v = v / 128.0;

	// Note: altitude and slope will be between 0 and 1 from the model.

	if (slope < 0)
		slope = 0;
	if (slope > 1)
		slope = 1;
	if (altitude < 0)
		altitude = 0;
	if (altitude > 1)
		altitude = 1;

	altitude = altitude / MAXALT; // between 0 and 1


	// THIS IS WHAT WE STILL NEED TO IMPLEMENT //
	GzColor grassColor;
	GzColor rockColor;
	GzColor snowColor;
	GzColor waterColor;
	//GzColor sandColor;
	GzColor finalColor;

	float gWeight = 0.0;
	float rWeight = 0.0;
	float sWeight = 0.0;
	float wWeight = 0.0;
	//float sandWeight = 0.0;

	// set grass color to perlin noise w/ two greens
	GzColor grass1 = { 0.35, 0.53, 0.33 };
	GzColor grass2 = { 0.38, 0.68, 0.35 };
	perlinNoise(u, v, grass1, grass2, 6, grassColor);


	// set rock color to perlin noise w/ two browns or greys
	GzColor rock1 = { 0.5, 0.34, 0.15 };
	GzColor rock2 = { 0.5, 0.45, 0.38 };
	perlinNoise(u, v, rock1, rock2, 8, rockColor);


	//set snow color to perlin noise w/ white/ light grey
	GzColor snow1 = { 0.98, 0.98, 0.98 };
	GzColor snow2 = { 0.90, 0.90, 0.90 };
	perlinNoise(u, v, snow1, snow2, 100, snowColor);


	// set water color to perlin noise w/ two blue-greens
	GzColor water1 = { 0.24, 0.35, 0.8 };
	//GzColor water2 = { 0.4, 0.49, 0.7 };
	GzColor water2 = { RGBCode(64.0), RGBCode(164.0), RGBCode(223.25) };
	perlinNoise(u, v, water1, water2, 4, waterColor);

	//GzColor sand1 = { 0.5, 0.34, 0.15 };
	//GzColor sand2 = { 0.5, 0.45, 0.38 }; 
	/*GzColor sand1 = { RGBCode(84.0), RGBCode(84.0), RGBCode(84.0) };
	GzColor sand2 = { RGBCode(192.0), RGBCode(192.0), RGBCode(192.25) };
	perlinNoise(u, v, sand1, sand2, 2, sandColor);*/

	float total = 1.0;
	// set the snow color
	sWeight = pow(altitude, 2.5f);

	// set the rock color
	total -= sWeight;
	rWeight = (1 - slope) * 9;
	if (rWeight > total){
		rWeight = total;
	}

	total -= rWeight;
	gWeight = total;

	if (altitude >= 0.0 && altitude < 0.00028){
		wWeight = 1.0;
		sWeight = 0.0;
		rWeight = 0.0;
		gWeight = 0.0;
		//sandWeight = 0.0;
	}
	else if (altitude >= 0.005 && altitude < 1.0){
		wWeight = 0.0;
		sWeight = 1.0;
		rWeight = 0.0;
		gWeight = 0.0;
		//sandWeight = 0.0;
	}
	/*else if (altitude >= 0.004 && altitude < 0.005){
		wWeight = 0.0;
		sWeight = 0.01;
		rWeight = 0.0;
		gWeight = 0.0;
		sandWeight = 1.0;
	}*/

	finalColor[0] = gWeight*grassColor[0] + rWeight*rockColor[0] + sWeight*snowColor[0] + wWeight*waterColor[0]; // +sandWeight*sandColor[0];
	finalColor[1] = gWeight*grassColor[1] + rWeight*rockColor[1] + sWeight*snowColor[1] + wWeight*waterColor[1]; // +sandWeight*sandColor[1];
	finalColor[2] = gWeight*grassColor[2] + rWeight*rockColor[2] + sWeight*snowColor[2] + wWeight*waterColor[2]; // +sandWeight*sandColor[2];

	color[0] = finalColor[0];
	color[1] = finalColor[1];
	color[2] = finalColor[2];
	// then set finalColor to color


	return GZ_SUCCESS;
}
float RGBCode(float a){
	return a / 255.0;
}


/* Free texture memory */
int GzFreeTexture()
{
	if (image != NULL)
		free(image);
	return GZ_SUCCESS;
}
