#include "Gz.h"

#include <Windows.h>
#include <algorithm>
#include <memory>
#include <time.h>
#include <fstream>

// Parameters:
#define MAXALT 64 // 6400 m
#define WATERELEVATION 6.4 // 140 m

//#define WIDTH 16	// 12800 m
//#define HEIGHT 16	// 12800 m
#define WIDTH 128	// 12800 m
#define HEIGHT 128	// 12800 m

#define POINTSNORMSSLOPE 8

//#define MODELFILE "P:\\bmpTester\\test1.asc"
#define MODELPREFIX "models\\model"             // "testasc.asc"
#define HEIGHTMAPPREFIX "heightmaps\\heightmap" // "test1.bmp"

#define NUMBEROFMILLENIA 5
#define SEEDER 42489


#ifndef VOLCANO
typedef	struct
{
	int volcanoCenter[2];
	int movePerMillenia[2];
	int startMillenia;
	int endMillenia;
	int flowsPerMillenia;
	int totalMagma;
} Volcano;
#define VOLCANO
#endif;

// Strike slip faults:
#ifndef FAULTSS
typedef	struct
{
	// y = (mY/mX)x + b...
	int mX;
	int mY;
	int b;
	int startMillenia;
	int endMillenia;
} FaultSS;
#define FAULTSS
#endif;

// Divergent fault:
#ifndef FAULTDIV
typedef	struct
{
	// y = (mY/mX)x + b...
	int mX;
	int mY;
	int b;
	int startMillenia;
	int endMillenia;
} FaultDIV;
#define FAULTDIV
#endif;

// Convergent fault:


#define HOTSPOTCNT 3
#define FAULTSSCNT 2
#define FAULTDIVCNT 1
#define RIVERCNT 8


void GenerateHeightmapData(static float data[WIDTH][HEIGHT]);

// Define as GZ_SUCCESS?
void ExecuteMapMaker();

// Map generation methods:
void InitializeData(static float data[WIDTH][HEIGHT], Volcano vols[HOTSPOTCNT], FaultSS sss[FAULTSSCNT], FaultDIV divs[FAULTDIVCNT]);
void SaveData(static float mapData[WIDTH][HEIGHT], static float imageData[WIDTH][HEIGHT]);
void GenerateHeightmapData(static float data[WIDTH][HEIGHT], int year, Volcano vols[HOTSPOTCNT], FaultSS sss[FAULTSSCNT], FaultDIV divs[FAULTDIVCNT]);
void runVolcano(static float data[WIDTH][HEIGHT], Volcano vol, int year);
void runFaultSS(static float data[WIDTH][HEIGHT], FaultSS flt);
void runFaultDIV(static float data[WIDTH][HEIGHT], FaultDIV flt);
void runErosion(static float data[WIDTH][HEIGHT]);
void runSmoothness(static float data[WIDTH][HEIGHT]);
void runRivers(static float data[WIDTH][HEIGHT]);

// .bmp saving methods:
BITMAPINFOHEADER makeBMIH(unsigned long padding, unsigned long pixel_data_size);
BITMAPFILEHEADER makeBMFH(unsigned long headers_size, unsigned long pixel_data_size);
void saveBMP(static float data[WIDTH][HEIGHT], int year);

// Model making methods:
void makeModel(static float data[WIDTH][HEIGHT], static float modelData[WIDTH][HEIGHT][POINTSNORMSSLOPE]);

// .asc saving methods:
void saveASC(static float modelData[WIDTH][HEIGHT][POINTSNORMSSLOPE], int year);
