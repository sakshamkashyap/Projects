#include <stdlib.h>


#define SAMPLE_SIZE 1024

class CustomizablePerlinNoise
{
public:
	/*
	The first parameter is the number of octaves, this is how noisy or smooth the function is.
	This is valid between 1 and 16. A value of 4 to 8 octaves produces fairly conventional noise results.
	The second parameter is the noise frequency. Values betwen 1 and 8 are reasonable here.
	You can try sampling the data and plotting it to the screen to see what numbers you like.
	The third parameter is the amplitude. Setting this to a value of 1 will return randomized samples between -1 and +1.
	The last parameter is the random number seed. This number is what causes this instance of the Perlin
	to be deterministic and distanct from any other instance. The perlin noise function creates
	some random number tables that are sampled during queries.
	*/
	CustomizablePerlinNoise() {}// mStart = false;}
	CustomizablePerlinNoise(int persistance, int octaves, float freq, float amp, int seed);
	void setParams(int persistance, int octaves, float freq, float amp, int seed);

	// 2D version 
	float Get(float x, float y)
	{
		float vec[2];
		vec[0] = x;
		vec[1] = y;
		return perlin_noise_2D(vec);
	};

	// 3D version ( don't think we need ) 
	float Get(float x, float y, float z)
	{
		float vec[3];
		vec[0] = x;
		vec[1] = y;
		vec[2] = z;
		return perlin_noise_3D(vec);
	};

private:
	void init_perlin(int n, float p);
	float perlin_noise_2D(float vec[2]);
	float perlin_noise_3D(float vec[3]);

	float noise1(float arg);
	float noise2(float vec[2]);
	float noise3(float vec[3]);

	void normalize2(float v[2]);
	void normalize3(float v[3]);
	void init(void);

	int	  mPersistance;
	int   mOctaves;
	float mFrequency;
	float mAmplitude;
	int   mSeed;

	int p[SAMPLE_SIZE + SAMPLE_SIZE + 2];
	float g3[SAMPLE_SIZE + SAMPLE_SIZE + 2][3];
	float g2[SAMPLE_SIZE + SAMPLE_SIZE + 2][2];
	float g1[SAMPLE_SIZE + SAMPLE_SIZE + 2];
	bool  mStart;

};
