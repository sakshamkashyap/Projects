#include "disp.h" /* include your own disp.h file (e.g. hw1)*/

/* Camera defaults */
#define	DEFAULT_FOV		35.0
#define	DEFAULT_IM_Z	(-10.0)  /* world coords for image plane origin */
#define	DEFAULT_IM_Y	(5.0)    /* default look-at point = 0,0,0 */
#define	DEFAULT_IM_X	(-10.0)

#define	DEFAULT_AMBIENT	{0.1, 0.1, 0.1}
#define	DEFAULT_DIFFUSE	{0.7, 0.6, 0.5}
#define	DEFAULT_SPECULAR	{0.2, 0.3, 0.4}
#define	DEFAULT_SPEC		32

#define	MATLEVELS	100		/* how many matrix pushes allowed */
#define	MAX_LIGHTS	10		/* how many lights allowed */

#ifndef GZRENDER
#define GZRENDER
typedef struct {			/* define a renderer */
  GzDisplay		*display;
  GzCamera		camera;
  short		    matlevel;	        /* top of stack - current xform */
  GzMatrix		Ximage[MATLEVELS];	/* stack of xforms (Xsm) */
  GzMatrix		Xnorm[MATLEVELS];	/* xforms for norms (Xim) */
  GzMatrix		Xsp;		        /* NDC to screen (pers-to-screen) */
  GzColor		flatcolor;          /* color state for flat shaded triangles */
  int			interp_mode;
  int			numlights;
  GzLight		lights[MAX_LIGHTS];
  GzLight		ambientlight;
  GzColor		Ka, Kd, Ks;
  float		    spec;		/* specular power */
  GzTexture		tex_fun;    /* tex_fun(float u, float v, GzColor color) */
}  GzRender;
#endif

// Function declaration
// HW2
int GzNewRender(GzRender **render, GzDisplay *display);
int GzFreeRender(GzRender *render);
int GzBeginRender(GzRender	*render);
int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList, 
	GzPointer *valueList);
int GzPutTriangle(GzRender *render, int	numParts, GzToken *nameList,
	GzPointer *valueList);

// HW3
int GzPutCamera(GzRender *render, GzCamera *camera);
int GzPushMatrix(GzRender *render, GzMatrix	matrix);
int GzPopMatrix(GzRender *render);

// HW5
int GzFreeTexture();

// Object Translation
int GzRotXMat(float degree, GzMatrix mat);
int GzRotYMat(float degree, GzMatrix mat);
int GzRotZMat(float degree, GzMatrix mat);
int GzTrxMat(GzCoord translate, GzMatrix mat);
int GzScaleMat(GzCoord scale, GzMatrix mat);

// vertex class
#ifndef VERT
#define VERT
class Vert
{
public:
	float x;
	float y;
	float z;

	float screenX;
	float screenZ;

	// add support for normals at each vertex
	float normX;
	float normY;
	float normZ;

	// support for color
	float red;
	float green;
	float blue;

	// support for texture
	float texU;
	float texV;

	// constructor. take in GzCoord
	Vert(const GzCoord& vertIn){
		x = vertIn[0];
		y = vertIn[1];
		z = vertIn[2];
	}
	Vert(){}

	// destructor
	~Vert(){}
};
#endif

#ifndef DDAEDGE
#define DDAEDGE
class DDAEdge{

public:
	Vert start;
	Vert end;
	Vert current;

	float slopeX;
	float slopeZ;

	float slopeR;
	float slopeG;
	float slopeB;

	float slopeNX;
	float slopeNY;
	float slopeNZ;

	float slopeTU;
	float slopeTV;

	float slopeScreenX;
	float slopeScreenZ; 

	DDAEdge(const Vert& inStart, const Vert& inEnd){
		start = inStart;
		end = inEnd;
		current = inStart;

		// calculate all the slopes with respect to change in y
		slopeX = (end.x - start.x) / (end.y - start.y);
		slopeZ = (end.z - start.z) / (end.y - start.y);

		slopeR = (end.red - start.red) / (end.y - start.y);
		slopeG = (end.green - start.green) / (end.y - start.y);
		slopeB = (end.blue - start.blue) / (end.y - start.y);

		slopeNX = (end.normX - start.normX) / (end.y - start.y);
		slopeNY = (end.normY - start.normY) / (end.y - start.y);
		slopeNZ = (end.normZ - start.normZ) / (end.y - start.y);

		slopeTU = (end.texU - start.texU) / (end.y - start.y);
		slopeTV = (end.texV - start.texV) / (end.y - start.y);

		slopeScreenX = (end.screenX - start.screenX) / (end.y - start.y);
		slopeScreenZ = (end.screenZ - start.screenZ) / (end.y - start.y);
		


	}

};
#endif

#ifndef DDASPAN
#define DDASPAN
class DDASpan{
public:
	Vert start;
	Vert end;
	Vert current;

	float slopeZ = 0;
	float slopeR;
	float slopeG;
	float slopeB;

	float slopeNX;
	float slopeNY;
	float slopeNZ;

	float slopeTU;
	float slopeTV; 

	float slopeScreenX;
	float slopeScreenZ;


	DDASpan(const Vert& inStart, const Vert& inEnd){
		start = inStart;
		end = inEnd;
		current = start;

		// calculate all slopes with respect to x
		slopeZ = (end.z - start.z) / (end.x - start.x);

		slopeR = (end.red - start.red) / (end.x - start.x);
		slopeG = (end.green - start.green) / (end.x - start.x);
		slopeB = (end.blue - start.blue) / (end.x - start.x);

		slopeNX = (end.normX - start.normX) / (end.x - start.x);
		slopeNY = (end.normY - start.normY) / (end.x - start.x);
		slopeNZ = (end.normZ - start.normZ) / (end.x - start.x);

		slopeTU = (end.texU - start.texU) / (end.x - start.x);
		slopeTV = (end.texV - start.texV) / (end.x - start.x);

		slopeScreenX = (end.screenX - start.screenX) / (end.x - start.x);
		slopeScreenZ = (end.screenZ - start.screenZ) / (end.x - start.x);
		
	}
};

#endif
