#include "stdafx.h"
#include "HeightMapMaker.h"
#include <string>
// Call from the application:

void ExecuteMapMaker()
{
	Volcano allVolcanoes[HOTSPOTCNT];
	FaultSS allFaultsSS[FAULTSSCNT];
	FaultDIV allFaultsDIV[FAULTDIVCNT];

	// Seed the random number generator with user input:
	srand((unsigned)SEEDER);

	// MAP DATA:
	static float mapData[WIDTH][HEIGHT];
	static float saveData[WIDTH][HEIGHT];
	InitializeData(mapData, allVolcanoes, allFaultsSS, allFaultsDIV);

	for (int i = 1; i <= NUMBEROFMILLENIA; i++)
	{
		GenerateHeightmapData(mapData, i, allVolcanoes, allFaultsSS, allFaultsDIV);
		SaveData(mapData, saveData);

		// MODEL DATA:
		static float modelData[WIDTH][HEIGHT][POINTSNORMSSLOPE];
		makeModel(saveData, modelData);

		// MAKE ASC:
		saveASC(modelData, i);

		// MAKE BMF:
		saveBMP(saveData, i);
	}
}

/*********************************************************************/
/*********************  MAP GENERATION METHODS  **********************/
/*********************************************************************/

// Initialize the data:
void InitializeData(static float data[WIDTH][HEIGHT], Volcano vols[HOTSPOTCNT], FaultSS sss[FAULTSSCNT], FaultDIV divs[FAULTDIVCNT])
{
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			data[i][j] = 0.0;
		}
	}

	// Make the hotspots:
	for (int i = 0; i < HOTSPOTCNT; i++)
	{
		// Random:
		// Direction of translation:
		int leftPos = rand() % 2;
		if (leftPos == 0) leftPos = -1;
		else leftPos = 1;
		int rightPos = rand() % 2;
		if (rightPos == 0) rightPos = -1;
		else rightPos = 1;

		// Create the volcano that will be moving:
		vols[i].volcanoCenter[0] = rand() % WIDTH;
		vols[i].volcanoCenter[1] = rand() % HEIGHT;
		//vols[i].totalMagma = 75000 + (rand() % 225000);
		vols[i].totalMagma = WIDTH * HEIGHT * ((MAXALT / 8) + (rand() % (MAXALT / 8)));
		vols[i].startMillenia = (rand() % (NUMBEROFMILLENIA + 1 / 2)) + 1;
		vols[i].endMillenia = vols[i].startMillenia + ((rand() % 4) + 1);
		vols[i].flowsPerMillenia = 30 + (rand() % 50);
		vols[i].movePerMillenia[0] = leftPos * ((rand() % (WIDTH / 16)) + 3);
		vols[i].movePerMillenia[1] = rightPos * ((rand() % (HEIGHT / 16)) + 3);
	}

	// Make the stike slip faults:
	for (int i = 0; i < FAULTSSCNT; i++)
	{
		sss[i].mX = (rand() % 10) + 1; // Range: [1, 10]
		sss[i].mY = (rand() % 10) + 1; // Range: [1, 10]
		if (rand() % 2 == 0)
		{
			sss[i].mX *= -1;
		}
		if (rand() % 2 == 0)
		{
			sss[i].mY *= -1;
		}
		sss[i].b = rand() % HEIGHT;
		// Positive slopes:
		if (sss[i].mX*sss[i].mY > 0)
		{
			sss[i].b -= (0.5 * HEIGHT);
		}
		else
		{
			sss[i].b += (0.5 * HEIGHT);
		}

		sss[i].startMillenia = (rand() % NUMBEROFMILLENIA) + 1;
		sss[i].endMillenia = sss[i].startMillenia + ((rand() % 4)); // Active up to 4 millenia
	}

	// Make the divergent faults:
	for (int i = 0; i < FAULTDIVCNT; i++)
	{
		divs[i].mX = (rand() % 8) + 1; // Range: [1, 8]
		divs[i].mY = (rand() % 8) + 1; // Range: [1, 8]
		if (rand() % 2 == 0)
		{
			divs[i].mX *= -1;
		}
		if (rand() % 2 == 0)
		{
			divs[i].mY *= -1;
		}
		divs[i].b = rand() % HEIGHT;
		// Positive slopes:
		if (divs[i].mX*sss[i].mY > 0)
		{
			divs[i].b -= (0.5 * HEIGHT);
		}
		else
		{
			divs[i].b += (0.5 * HEIGHT);
		}

		divs[i].startMillenia = (rand() % NUMBEROFMILLENIA) + 1;
		divs[i].endMillenia = divs[i].startMillenia;
	}
}

void SaveData(static float mapData[WIDTH][HEIGHT], static float imageData[WIDTH][HEIGHT])
{
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			imageData[i][j] = mapData[i][j];
		}
	}

}

// Make the date:
void GenerateHeightmapData(static float data[WIDTH][HEIGHT], int year, Volcano vols[HOTSPOTCNT], FaultSS sss[FAULTSSCNT], FaultDIV divs[FAULTDIVCNT])
{
	// Execute stike slip faults:
	for (int i = 0; i < FAULTSSCNT; i++)
	{
		if (year >= sss[i].startMillenia && year <= sss[i].endMillenia)
		{
			runFaultSS(data, sss[i]);
		}
	}
	// Execute divergent faults:
	for (int i = 0; i < FAULTDIVCNT; i++)
	{
		if (year >= divs[i].startMillenia && year <= divs[i].endMillenia)
		{
			runFaultDIV(data, divs[i]);
		}
	}

	// Execute volcanoes:
	for (int i = 0; i < HOTSPOTCNT; i++)
	{
		if (year >= vols[i].startMillenia && year <= vols[i].endMillenia)
		{
			runVolcano(data, vols[i], year);
		}
	}

	// Execute erosions:
	runErosion(data);
}

void runVolcano(static float data[WIDTH][HEIGHT], Volcano vol, int year)
{
	// Remove the used magma:
	int numberOfMillenia = 1 + (vol.endMillenia - vol.startMillenia);
	int milleniaPast = year - vol.startMillenia;
	for (int i = vol.startMillenia; i < year; i++)
	{
		vol.totalMagma -= (int)(vol.totalMagma * (1.0 / (numberOfMillenia)));
	}

	int magmaToSpread = (int)(vol.totalMagma * (1.0 / (numberOfMillenia)));
	int magmaPerMillenia = magmaToSpread / vol.flowsPerMillenia;

	for (int j = 0; j < vol.flowsPerMillenia; j++)
	{
		magmaToSpread = magmaPerMillenia;
		int curLoc[2] = { vol.volcanoCenter[0] + (milleniaPast*vol.movePerMillenia[0]), vol.volcanoCenter[1] + (milleniaPast*vol.movePerMillenia[1]) };
		int newLoc[2] = { 0, 0 };
		while (magmaToSpread > 0)
		{
			int trys = 3;
			while (trys > 0)
			{
				// pick one of the 8 directions:
				int dir = rand() % 8;
				switch (dir)
				{
				case 0:
				{
					newLoc[0] = curLoc[0] - 1;
					newLoc[1] = curLoc[1];
					break;
				}
				case 1:
				{
					newLoc[0] = curLoc[0] - 1;
					newLoc[1] = curLoc[1] - 1;
					break;
				}
				case 2:
				{
					newLoc[0] = curLoc[0] - 1;
					newLoc[1] = curLoc[1] + 1;
					break;
				}
				case 3:
				{
					newLoc[0] = curLoc[0] + 1;
					newLoc[1] = curLoc[1] - 1;
					break;
				}
				case 4:
				{
					newLoc[0] = curLoc[0] + 1;
					newLoc[1] = curLoc[1];
					break;
				}
				case 5:
				{
					newLoc[0] = curLoc[0] + 1;
					newLoc[1] = curLoc[1] + 1;
					break;
				}
				case 6:
				{
					newLoc[0] = curLoc[0];
					newLoc[1] = curLoc[1] - 1;
					break;
				}
				case 7:
				{
					newLoc[0] = curLoc[0];
					newLoc[1] = curLoc[1] + 1;
					break;
				}
				}
				//newLoc[0] = curLoc[0] + ((rand() % 2 == 0) ? 1 : -1);
				//newLoc[1] = curLoc[1] + ((rand() % 2 == 0) ? 1 : -1);

				if (newLoc[0] >= 0 && newLoc[0] < WIDTH && newLoc[1] >= 0 && newLoc[1] < HEIGHT)
				{
					if (data[newLoc[0]][newLoc[1]] <= data[curLoc[0]][curLoc[1]])
					{
						if (data[newLoc[0]][newLoc[1]] < (MAXALT - 1))
						{
							data[newLoc[0]][newLoc[1]] += 1;
						}
						trys = 0;
					}
					else
					{
						trys -= 1;
						if (trys == 0)
						{
							if (curLoc[0] >= 0 && curLoc[0] < WIDTH && curLoc[1] >= 0 && curLoc[1] < HEIGHT)
							{
								if (data[curLoc[0]][curLoc[1]] < (MAXALT - 1))
								{
									data[curLoc[0]][curLoc[1]] += 1;
								}
							}
						}
					}
				}
				else
				{
					trys = 0;
				}
			}

			magmaToSpread -= 1;
			curLoc[0] = newLoc[0];
			curLoc[1] = newLoc[1];

			//// Go to one of the 8 adjacent locations:
			//int flowto[2] = { (rand() % 3) - 1, (rand() % 3) - 1 };

			//// Modify on the map
			//if (currentLoc[0] + flowto[0] >= 0 &&
			//	currentLoc[0] + flowto[0] < WIDTH &&
			//	currentLoc[1] + flowto[1] >= 0 &&
			//	currentLoc[1] + flowto[1] < HEIGHT)
			//{
			//	if (data[currentLoc[0] + flowto[0]][currentLoc[1] + flowto[1]] <= data[currentLoc[0]][currentLoc[1]])
			//	{
			//		// Move to the lower location and raise it:
			//		currentLoc[0] += flowto[0];
			//		currentLoc[1] += flowto[1];

			//		if (data[currentLoc[0]][currentLoc[1]] < 255)
			//		{
			//			data[currentLoc[0]][currentLoc[1]] += 1;
			//		}
			//		magmaToSpread -= 1;
			//	}
			//	else
			//	{
			//		// Raise the elevation:
			//		if (data[currentLoc[0]][currentLoc[1]] < MAXALT)
			//		{
			//			data[currentLoc[0]][currentLoc[1]] += 1;
			//		}
			//		magmaToSpread -= 1;
			//	}
			//}
			//// Continue the flow, but dont change the map
			//else
			//{
			//	magmaToSpread -= 1;
			//}
		}
	}
}

void runFaultSS(static float data[WIDTH][HEIGHT], FaultSS flt)
{
	// Positive X shift:
	if (flt.mX > 0)
	{
		// Positive Y shift:
		if (flt.mY > 0)
		{
			for (int i = (WIDTH - 1); i >= 0; i--)
			{
				for (int j = (HEIGHT - 1); j >= 0; j--)
				{
					int faultHeight = (((float)flt.mY / (float)flt.mX) * i) + flt.b;
					if (j < faultHeight)
					{
						int getX = (int)(i - (flt.mX * ((float)i / (WIDTH - 1))));
						int getY = (int)(j - (flt.mY * ((float)j / (HEIGHT - 1))));

						if (getX >= 0 && getX < WIDTH &&
							getY >= 0 && getY < HEIGHT)
						{
							data[i][j] = data[getX][getY];
						}
					}
				}
			}
		}
		// Negative Y shift:
		else
		{
			for (int i = 0; i < WIDTH; i++)
			{
				for (int j = (HEIGHT - 1); j >= 0; j--)
				{
					int faultHeight = (((float)flt.mY / (float)flt.mX) * i) + flt.b;
					if (j < faultHeight)
					{
						int getX = (int)(i + (flt.mX * (1 - ((float)i / (WIDTH - 1)))));
						int getY = (int)(j + (flt.mY * ((float)j / (HEIGHT - 1))));

						if (getX >= 0 && getX < WIDTH &&
							getY >= 0 && getY < HEIGHT)
						{
							data[i][j] = data[getX][getY];
						}
					}
				}
			}
		}
	}
	// Negative X shift:
	else
	{
		// Positive Y shift:
		if (flt.mY > 0)
		{
			for (int i = 0; i < WIDTH; i++)
			{
				for (int j = (HEIGHT - 1); j >= 0; j--)
				{
					int faultHeight = (((float)flt.mY / (float)flt.mX) * i) + flt.b;
					if (j > faultHeight)
					{
						int getX = (int)(i - (flt.mX * (1 - ((float)i / (WIDTH - 1)))));
						int getY = (int)(j - (flt.mY * ((float)j / (HEIGHT - 1))));

						if (getX >= 0 && getX < WIDTH &&
							getY >= 0 && getY < HEIGHT)
						{
							data[i][j] = data[getX][getY];
						}
					}
				}
			}
		}
		// Negative Y shift:
		else
		{
			for (int i = (WIDTH - 1); i >= 0; i--)
			{
				for (int j = (HEIGHT - 1); j >= 0; j--)
				{
					int faultHeight = (((float)flt.mY / (float)flt.mX) * i) + flt.b;
					if (j > faultHeight)
					{
						int getX = (int)(i + (flt.mX * ((float)i / (WIDTH - 1))));
						int getY = (int)(j + (flt.mY * ((float)j / (HEIGHT - 1))));

						if (getX >= 0 && getX < WIDTH &&
							getY >= 0 && getY < HEIGHT)
						{
							data[i][j] = data[getX][getY];
						}
					}
				}
			}
		}
	}
}

void runFaultDIV(static float data[WIDTH][HEIGHT], FaultDIV flt)
{
	if (flt.mX > 0)
	{
		if (flt.mY > 0)
		{
			// Pull right down:
			for (int i = (WIDTH - 1); i >= 0; i--)
			{
				for (int j = 0; j < HEIGHT; j++)
				{
					float slope = ((float)flt.mY / (float)flt.mX);
					float faultY = (slope * i) + flt.b;
					float faultX = (j - flt.b) / slope;
					if (j < faultY)
					{
						if (faultY - j >(2 * flt.mY) && i - faultX >(2 * flt.mX))
						{
							int getX = (int)(i - flt.mX);
							int getY = (int)(j + flt.mY);
							if (getX >= 0 && getX < WIDTH &&
								getY >= 0 && getY < HEIGHT)
							{
								data[i][j] = data[getX][getY];
							}
						}
						// Make the trough:
						else
						{
							data[i][j] = 0.0;
						}
					}
				}
			}
		}
		else
		{
			// Pull left down:
			for (int i = 0; i < WIDTH; i++)
			{
				for (int j = 0; j < HEIGHT; j++)
				{
					float slope = ((float)flt.mY / (float)flt.mX);
					float faultY = (slope * i) + flt.b;
					float faultX = (j - flt.b) / slope;
					if (j < faultY)
					{
						if (faultY - j > abs(2 * flt.mY) && faultX - i >(2 * flt.mX))
						{
							int getX = (int)(i + flt.mX);
							int getY = (int)(j - flt.mY);
							if (getX >= 0 && getX < WIDTH &&
								getY >= 0 && getY < HEIGHT)
							{
								data[i][j] = data[getX][getY];
							}
						}
						// Make the trough:
						else
						{
							data[i][j] = 0.0;
						}
					}
				}
			}
		}
	}
	else
	{
		// Push right up:
		if (flt.mY > 0)
		{
			for (int i = (WIDTH - 1); i >= 0; i--)
			{
				for (int j = (HEIGHT - 1); j >= 0; j--)
				{
					float slope = ((float)flt.mY / (float)flt.mX);
					float faultY = (slope * i) + flt.b;
					float faultX = (j - flt.b) / slope;
					if (j > faultY)
					{
						if (j - faultY > (2 * flt.mY) && i - faultX > abs(2 * flt.mX))
						{
							int getX = (int)(i + flt.mX);
							int getY = (int)(j - flt.mY);
							if (getX >= 0 && getX < WIDTH &&
								getY >= 0 && getY < HEIGHT)
							{
								data[i][j] = data[getX][getY];
							}
						}
						// Make the trough:
						else
						{
							data[i][j] = 0.0;
						}
					}
				}
			}
		}
		// Push left up:
		else
		{
			for (int i = 0; i < WIDTH; i++)
			{
				for (int j = (HEIGHT - 1); j >= 0; j--)
				{
					float slope = ((float)flt.mY / (float)flt.mX);
					float faultY = (slope * i) + flt.b;
					float faultX = (j - flt.b) / slope;
					if (j > faultY)
					{
						if (j - faultY > abs(2 * flt.mY) && faultX - i > abs(2 * flt.mX))
						{
							int getX = (int)(i - flt.mX);
							int getY = (int)(j + flt.mY);
							if (getX >= 0 && getX < WIDTH &&
								getY >= 0 && getY < HEIGHT)
							{
								data[i][j] = data[getX][getY];
							}
						}
						// Make the trough:
						else
						{
							data[i][j] = 0.0;
						}
					}
				}
			}
		}
	}
}

void runErosion(static float data[WIDTH][HEIGHT])
{
	runRivers(data);
	runSmoothness(data);
}

void runSmoothness(static float data[WIDTH][HEIGHT])
{
	// Intialize:
	static float smoothData[WIDTH][HEIGHT];
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			smoothData[i][j] = 0;
		}
	}

	// Get smoothy:
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			float avg = 0;
			int count = 0;
			if (i > 0)
			{
				avg += data[i - 1][j];
				count++;
				if (j > 0)
				{
					avg += data[i - 1][j - 1];
					count++;
				}
				if (j < (HEIGHT - 1))
				{
					avg += data[i - 1][j + 1];
					count++;
				}
			}
			if (i < (WIDTH - 1))
			{
				avg += data[i + 1][j];
				count++;
				if (j > 0)
				{
					avg += data[i + 1][j - 1];
					count++;
				}
				if (j < (HEIGHT - 1))
				{
					avg += data[i + 1][j + 1];
					count++;
				}
			}
			if (j > 0)
			{
				avg += data[i][j - 1];
				count++;
			}
			if (j < (HEIGHT - 1))
			{
				avg += data[i][j + 1];
				count++;
			}
			if (count > 0)
			{
				avg /= count;
				if (data[i][j] > WATERELEVATION)
				{
					if (data[i][j] - avg > 2) // Greater than 45 degrees
					{
						smoothData[i][j] = data[i][j] - ((data[i][j] - avg) / 2.0);
					}
					else if (data[i][j] > avg)
					{
						smoothData[i][j] = data[i][j] - ((data[i][j] - avg) / 2.5);
					}
					else
					{
						smoothData[i][j] = data[i][j] + ((avg - data[i][j]) / 1.1);
					}
				}
				else
				{
					// Extremely smooth below water:
					smoothData[i][j] = avg;
				}
			}
			else
			{
				smoothData[i][j] = data[i][j];
			}

		}
	}

	// Apply the smoothyness:
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			data[i][j] = smoothData[i][j];
		}
	}
}

void runRivers(static float data[WIDTH][HEIGHT])
{
	static float peakdata[WIDTH][HEIGHT];
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			peakdata[i][j] = data[i][j];
		}
	}

	int allPeaks[RIVERCNT][2];
	for (int peaks = 0; peaks < RIVERCNT; peaks++)
	{
		int peakIndex[2] = { 0, 0 };
		for (int i = 0; i < WIDTH; i++)
		{
			for (int j = 0; j < HEIGHT; j++)
			{
				if (peakdata[i][j] > peakdata[peakIndex[0]][peakIndex[1]])
				{
					peakIndex[0] = i;
					peakIndex[1] = j;
				}
			}
		}

		allPeaks[peaks][0] = peakIndex[0];
		allPeaks[peaks][1] = peakIndex[1];
		int radius = 8;
		int r2 = radius*radius;
		int startX, endX, startY, endY;
		startX = peakIndex[0] - radius;
		if (startX < 0) startX = 0;
		endX = peakIndex[0] + radius;
		if (endX > WIDTH) endX = WIDTH;
		startY = peakIndex[1] - radius;
		if (startY < 0) startY = 0;
		endY = peakIndex[1] + radius;
		if (endY > HEIGHT) endX = HEIGHT;
		for (int i = startX; i < endX; i++)
		{
			for (int j = startY; j < endY; j++)
			{
				int x = peakIndex[0] - i;
				int y = peakIndex[1] - j;
				if (x * x + y * y < r2)
				{
					peakdata[i][j] = 0;
				}
			}
		}
	}

	// For each peak, find the closest peak:
	int allRiverSource[RIVERCNT][2];
	for (int i = 0; i < RIVERCNT; i++)
	{
		int thisPeak[2] = { allPeaks[i][0], allPeaks[i][1] };
		int closestPeak[2] = { 0, 0 };
		float closestDistance = -1;
		for (int j = 0; j < RIVERCNT; j++)
		{
			if (i != j)
			{
				float thisDist2 = pow((thisPeak[0] - allPeaks[j][0]), 2) + pow((thisPeak[1] - allPeaks[j][1]), 2);
				if (closestDistance == -1)
				{
					closestDistance = thisDist2;
					closestPeak[0] = allPeaks[j][0];
					closestPeak[1] = allPeaks[j][1];
				}
				else if (thisDist2 < closestDistance)
				{
					closestDistance = thisDist2;
					closestPeak[0] = allPeaks[j][0];
					closestPeak[1] = allPeaks[j][1];
				}
			}
		}
		if (closestPeak[0] > thisPeak[0])
		{
			allRiverSource[i][0] = (int)(thisPeak[0] + ((closestPeak[0] - thisPeak[0]) / 2.0));
		}
		else
		{
			allRiverSource[i][0] = (int)(closestPeak[0] + ((thisPeak[0] - closestPeak[0]) / 2.0));
		}
		if (closestPeak[1] > thisPeak[1])
		{
			allRiverSource[i][1] = (int)(thisPeak[1] + ((closestPeak[1] - thisPeak[1]) / 2.0));
		}
		else
		{
			allRiverSource[i][1] = (int)(closestPeak[1] + ((thisPeak[1] - closestPeak[1]) / 2.0));
		}

	}

	for (int i = 0; i < RIVERCNT; i++)
	{
		int loc[2] = { allRiverSource[i][0], allRiverSource[i][1] };
		int dir = rand() % 4;
		int redirects = 3;
		bool keepGoing = TRUE;
		while (keepGoing)
		{
			int path[2] = { 0, 0 };
			switch (dir)
			{
			case 0:
			{
				path[0] = -1;
				path[1] = rand() % 3 - 1;
				break;
			}
			case 1:
			{
				path[0] = 1;
				path[1] = rand() % 3 - 1;
				break;
			}
			case 2:
			{
				path[0] = rand() % 3 - 1;
				path[1] = -1;
				break;
			}
			case 3:
			{
				path[0] = rand() % 3 - 1;
				path[1] = 1;
				break;
			}
			}
			if (loc[0] + path[0] < 0 || loc[0] + path[0] >= WIDTH || loc[1] + path[1] < 0 || loc[1] + path[1] >= HEIGHT)
			{
				keepGoing = FALSE;
			}
			else
			{
				if (data[loc[0] + path[0]][loc[1] + path[1]] > data[loc[0]][loc[1]] && redirects > 0)
				{
					dir = rand() % 4;
					redirects--;
					continue;

				}
				redirects = 3;
				data[loc[0]][loc[1]] -= (MAXALT / (4.0 * MAXALT));
				loc[0] += path[0];
				loc[1] += path[1];
				if (data[loc[0]][loc[1]] <= WATERELEVATION)
				{
					keepGoing = FALSE;
				}
			}
		}
	}


}
/*********************************************************************/
/*********************  .BMP SAVING METHODS  *************************/
/*********************************************************************/

// Generate the bitmap information header:
BITMAPINFOHEADER makeBMIH(unsigned long padding, unsigned long pixel_data_size)
{
	BITMAPINFOHEADER bih = { 0 };

	// Set the size  
	bih.biSize = sizeof(BITMAPINFOHEADER);

	// Bit count  
	bih.biBitCount = 24;

	// Use all colors  
	bih.biClrImportant = 0;

	// Use as many colors according to bits per pixel  
	bih.biClrUsed = 0;

	// Store as un Compressed  
	bih.biCompression = BI_RGB;

	// Set the height in pixels  
	bih.biHeight = LONG(HEIGHT);

	// Width of the Image in pixels  
	bih.biWidth = (LONG)WIDTH;

	// Default number of planes  
	bih.biPlanes = 1;

	// Calculate the image size in bytes  
	bih.biSizeImage = pixel_data_size;

	return bih;
}

// Generate the bitmap file header:
BITMAPFILEHEADER makeBMFH(unsigned long headers_size, unsigned long pixel_data_size)
{
	BITMAPFILEHEADER bfh = { 0 };

	// This value should be values of BM letters i.e 0x4D42  
	bfh.bfType = 0x4D42;

	// Offset to the RGBQUAD  
	bfh.bfOffBits = headers_size;

	// Total size of image including size of headers  
	bfh.bfSize = headers_size + pixel_data_size;

	return bfh;
}

// Save the .bmp file from the data:
void saveBMP(static float data[WIDTH][HEIGHT], int year)
{

	std::string filename = HEIGHTMAPPREFIX;
	int leadingZeros = 5 - floor(log10(abs(year))) + 1;
	for (int i = 0; i < leadingZeros; i++)
	{
		filename += "0";
	}
	filename += std::to_string(year);
	filename += ".bmp";

	//LPCTSTR lpszFileName = (LPCTSTR)(HEIGHTMAPPREFIX + std::to_string(year) ;
	// Some basic bitmap parameters 
	LPCTSTR lpszFileName = (LPCTSTR)filename.c_str();
	unsigned long padding = (4 - ((WIDTH * 3) % 4)) % 4;
	unsigned long headers_size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	unsigned long pixel_data_size = HEIGHT * ((WIDTH * 3) + padding);
	unsigned long size;

	BITMAPINFOHEADER bmpInfoHeader = makeBMIH(padding, pixel_data_size);
	BITMAPFILEHEADER bfh = makeBMFH(headers_size, pixel_data_size);

	// Create the file in disk to write  
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Return if error opening file  
	if (!hFile) return;

	// Make the empty buffer:
	size = bfh.bfSize - bfh.bfOffBits;
	BYTE* mapData = new BYTE[size];
	std::fill(&mapData[0], &mapData[size], 0);

	int total_scanlinebytes = (WIDTH * 3) + padding;
	long newpos = 0;
	int val = 0;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			newpos = (HEIGHT - y - 1) * total_scanlinebytes + 3 * x;
			mapData[newpos] = data[x][y] * 255 / MAXALT;
			mapData[newpos + 1] = data[x][y] * 255 / MAXALT;
			mapData[newpos + 2] = data[x][y] * 255 / MAXALT;
		}
	}

	DWORD dwWritten = 0;

	// Write the File header  
	WriteFile(hFile, &bfh, sizeof(bfh), &dwWritten, NULL);

	// Write the bitmap info header  
	WriteFile(hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &dwWritten, NULL);

	// Write the RGB Data  
	WriteFile(hFile, mapData, bmpInfoHeader.biSizeImage, &dwWritten, NULL);

	// Close the file handle  
	CloseHandle(hFile);
}


/*********************************************************************/
/*********************  MODEL MAKING METHODS  ************************/
/*********************************************************************/

// Generate the model:
void makeModel(static float data[WIDTH][HEIGHT], static float modelData[WIDTH][HEIGHT][POINTSNORMSSLOPE])
{
	// Initialize the model data:
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			for (int k = 0; k < POINTSNORMSSLOPE; k++)
			{
				modelData[i][j][k] = 0.0F;
			}
		}
	}

	// Store the verts:
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			modelData[i][j][X] = i;
			modelData[i][j][Y] = data[i][j];
			//modelData[i][j][Y] = MAXALT * (data[i][j] / 255.0);
			modelData[i][j][Z] = j;
		}
	}

	// Store the norms:
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			float avg[3] = { 0, 0, 0 };
			float slope = 0;
			if (i > 0)
			{
				// Left:
				slope = data[i][j] - data[i - 1][j];
				avg[X] -= slope;
				avg[Y] += 1;

				if (j > 0)
				{
					// Top:
					slope = data[i][j] - data[i][j - 1];
					avg[Y] += 1;
					avg[Z] -= slope;

					// Top Left:
					slope = data[i][j] - data[i - 1][j - 1];
					avg[X] -= slope;
					avg[Y] += 1;
					avg[Z] -= slope;
				}
				if (j < HEIGHT - 1)
				{
					// Bottom
					slope = data[i][j] - data[i][j + 1];
					avg[Y] += 1;
					avg[Z] += slope;

					// Bottom Left:
					slope = data[i][j] - data[i - 1][j + 1];
					avg[X] -= slope;
					avg[Y] += 1;
					avg[Z] += slope;
				}
			}
			if (i < WIDTH - 1)
			{
				// Right:
				slope = data[i][j] - data[i + 1][j];
				avg[X] += slope;
				avg[Y] += 1;

				if (j > 0)
				{
					// Top Right:
					slope = data[i][j] - data[i + 1][j - 1];
					avg[X] += slope;
					avg[Y] += 1;
					avg[Z] -= slope;
				}
				if (j < HEIGHT - 1)
				{
					// Bottom Right:
					slope = data[i][j] - data[i + 1][j + 1];
					avg[X] += slope;
					avg[Y] += 1;
					avg[Z] += slope;
				}
			}

			// normalize and Store:
			float mag = sqrt(avg[X] * avg[X] + avg[Y] * avg[Y] + avg[Z] * avg[Z]);
			for (int k = 0; k < 3; k++)
			{
				modelData[i][j][k + 3] = avg[k] / mag;
			}

			// store the elevation and slope at this point:
			modelData[i][j][6] = modelData[i][j][1] / MAXALT; // [ranges from 0 to 1]
			modelData[i][j][7] = modelData[i][j][4]; // [ranges from 0-1]
		}
	}
}


/*********************************************************************/
/*********************  .ASC SAVING METHODS  *************************/
/*********************************************************************/
void saveASC(static float modelData[WIDTH][HEIGHT][POINTSNORMSSLOPE], int year)
{
	std::string filename = MODELPREFIX;
	int leadingZeros = 5 - floor(log10(abs(year))) + 1;
	for (int i = 0; i < leadingZeros; i++)
	{
		filename += "0";
	}
	filename += std::to_string(year);
	filename += ".asc";

	FILE *outfile;
	outfile = fopen(filename.c_str(), "w");

	if (outfile == NULL){
		printf("failed to open file.\n");
		return;
	}

	for (int i = 0; i < WIDTH - 1; i++)
	{
		for (int j = 0; j < HEIGHT - 1; j++)
		{
			fprintf(outfile, "triangle\n");
			fprintf(outfile, "%f %f %f %f %f %f %f %f \n",
				modelData[i][j][0], modelData[i][j][1], modelData[i][j][2], // Verts
				modelData[i][j][3], modelData[i][j][4], modelData[i][j][5],	// Normals
				modelData[i][j][6], modelData[i][j][7]);					// UV coords

			fprintf(outfile, "%f %f %f %f %f %f %f %f \n",
				modelData[i + 1][j][0], modelData[i + 1][j][1], modelData[i + 1][j][2],	// Verts
				modelData[i + 1][j][3], modelData[i + 1][j][4], modelData[i + 1][j][5],	// Normals
				modelData[i + 1][j][6], modelData[i + 1][j][7]);						// UV coords

			fprintf(outfile, "%f %f %f %f %f %f %f %f \n",
				modelData[i + 1][j + 1][0], modelData[i + 1][j + 1][1], modelData[i + 1][j + 1][2],	// Verts
				modelData[i + 1][j + 1][3], modelData[i + 1][j + 1][4], modelData[i + 1][j + 1][5],	// Normals
				modelData[i + 1][j + 1][6], modelData[i + 1][j + 1][7]);							// UV coords

			fprintf(outfile, "triangle\n");
			fprintf(outfile, "%f %f %f %f %f %f %f %f \n",
				modelData[i][j][0], modelData[i][j][1], modelData[i][j][2], // Verts
				modelData[i][j][3], modelData[i][j][4], modelData[i][j][5],	// Normals
				modelData[i][j][6], modelData[i][j][7]);					// UV coords

			fprintf(outfile, "%f %f %f %f %f %f %f %f \n",
				modelData[i][j + 1][0], modelData[i][j + 1][1], modelData[i][j + 1][2],	// Verts
				modelData[i][j + 1][3], modelData[i][j + 1][4], modelData[i][j + 1][5],	// Normals
				modelData[i][j + 1][6], modelData[i][j + 1][7]);						// UV coords

			fprintf(outfile, "%f %f %f %f %f %f %f %f \n",
				modelData[i + 1][j + 1][0], modelData[i + 1][j + 1][1], modelData[i + 1][j + 1][2],	// Verts
				modelData[i + 1][j + 1][3], modelData[i + 1][j + 1][4], modelData[i + 1][j + 1][5],	// Normals
				modelData[i + 1][j + 1][6], modelData[i + 1][j + 1][7]);							// UV coords
		}
	}
	fclose(outfile);
}