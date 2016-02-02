/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
#include <algorithm>
#include <vector>


/* orientation constants */
#define GZ_UPRIGHT_TRI		0
#define GZ_INVERT_TRI		1
#define GZ_RIGHT_TRI		3
#define GZ_LEFT_TRI			4


using namespace std;

int DDAinit(vector<Vert> & sortedVerts, vector<DDAEdge> &Edges);
void raster(vector<DDAEdge> &Edges, int &orientation, GzRender *render);
bool ySmaller(const Vert &v1, const Vert &v2);

void matrixMultiply( const GzMatrix mat1, const GzMatrix mat2, GzMatrix result );

void buildXsp( GzRender * render );
void buildXpi( GzRender * render );
void buildXiw( GzRender * render );

void xFormTris(vector<Vert> & verts, GzMatrix xFormMat);
void xFormNorms(vector< Vert> & normals, GzMatrix xFormMat);


void calcColor(Vert & vert, GzRender * render);


// functions to transform textures
void xFormTex(Vert & vert);
void reverseTex(Vert & vert, float & u, float & v);

// helper math functions
float dotProd(Vert vert1, Vert vert2);
Vert normalizeVert(Vert & vert);
void normalizeGzCoord(GzCoord & coord);



int GzRotXMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along x axis
// Pass back the matrix using mat value
	float radTheta = (float)(degree / 180) * 3.1415926;

	mat[0][0] = 1;
	mat[0][1] = 0;
	mat[0][2] = 0;
	mat[0][3] = 0;

	mat[1][0] = 0;
	mat[1][1] = cos(radTheta);
	mat[1][2] = -sin(radTheta);
	mat[1][3] = 0;

	mat[2][0] = 0;
	mat[2][1] = sin(radTheta);
	mat[2][2] = cos(radTheta);
	mat[2][3] = 0;

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;

	return GZ_SUCCESS;
}


int GzRotYMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along y axis
// Pass back the matrix using mat value
	float radTheta = (float)(degree / 180) * 3.1415926;

	mat[0][0] = cos(radTheta);
	mat[0][1] = 0;
	mat[0][2] = sin(radTheta);
	mat[0][3] = 0;

	mat[1][0] = 0;
	mat[1][1] = 1;
	mat[1][2] = 0;
	mat[1][3] = 0;

	mat[2][0] = -sin(radTheta);
	mat[2][1] = 0;
	mat[2][2] = cos(radTheta);
	mat[2][3] = 0;

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;
	return GZ_SUCCESS;
}


int GzRotZMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along z axis
// Pass back the matrix using mat value
	float radTheta = (float)(degree / 180) * 3.1415926;

	mat[0][0] = cos(radTheta);
	mat[0][1] = -sin(radTheta);
	mat[0][2] = 0;
	mat[0][3] = 0;

	mat[1][0] = sin(radTheta);
	mat[1][1] = cos(radTheta);
	mat[1][2] = 0;
	mat[1][3] = 0;

	mat[2][0] = 0;
	mat[2][1] = 0;
	mat[2][2] = 1;
	mat[2][3] = 0;

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;
	return GZ_SUCCESS;
}


int GzTrxMat(GzCoord translate, GzMatrix mat)
{
// Create translation matrix
// Pass back the matrix using mat value

	mat[0][0] = 1;
	mat[0][1] = 0;
	mat[0][2] = 0;
	mat[0][3] = translate[0];

	mat[1][0] = 0;
	mat[1][1] = 1;
	mat[1][2] = 0;
	mat[1][3] = translate[1];

	mat[2][0] = 0;
	mat[2][1] = 0;
	mat[2][2] = 1;
	mat[2][3] = translate[2];

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;

	

	return GZ_SUCCESS;
}


int GzScaleMat(GzCoord scale, GzMatrix mat)
{
// Create scaling matrix
// Pass back the matrix using mat value
	mat[0][0] = scale[0];
	mat[0][1] = 0;
	mat[0][2] = 0;
	mat[0][3] = 0;

	mat[1][0] = 0;
	mat[1][1] = scale[1];
	mat[1][2] = 0;
	mat[1][3] = 0;

	mat[2][0] = 0;
	mat[2][1] = 0;
	mat[2][2] = scale[2];
	mat[2][3] = 0;

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;

	return GZ_SUCCESS;
}


//----------------------------------------------------------
// Begin main functions

int GzNewRender(GzRender **render, GzDisplay	*display) // OK
{
/*  
- malloc a renderer struct 
- setup Xsp and anything only done once 
- save the pointer to display 
- init default camera 
*/ 
	// check display
	if (display == NULL){
		return GZ_FAILURE;
	}

	// allocate new Renderer
	GzRender * newRender = new GzRender;

	// set the given display to the new renderer's display
	newRender->display = display;

	// initialize the matrix stack to be empty
	newRender->matlevel = 0;

	// init default camera //
	// Field of vision
	newRender->camera.FOV = DEFAULT_FOV;

	// lookat point
	newRender->camera.lookat[0] = 0;
	newRender->camera.lookat[1] = 0;
	newRender->camera.lookat[2] = 0;

	// camera position
	newRender->camera.position[0] = DEFAULT_IM_X;
	newRender->camera.position[1] = DEFAULT_IM_Y;
	newRender->camera.position[2] = DEFAULT_IM_Z;

	// world up vector
	newRender->camera.worldup[0] = 0;
	newRender->camera.worldup[1] = 1;
	newRender->camera.worldup[2] = 0;

	// default interp mode
	newRender->interp_mode = GZ_FLAT;

	// initialize number of lights
	newRender->numlights = 0;

	// initialize Ka, Kd, Ks to defaults
	//Ka
	GzColor defaultKa = DEFAULT_AMBIENT;
	newRender->Ka[0] = defaultKa[0];
	newRender->Ka[1] = defaultKa[1];
	newRender->Ka[2] = defaultKa[2];

	//Kd
	GzColor defaultKd = DEFAULT_DIFFUSE;
	newRender->Kd[0] = defaultKd[0];
	newRender->Kd[1] = defaultKd[1];
	newRender->Kd[2] = defaultKd[2];

	//Ks
	GzColor defaultKs = DEFAULT_SPECULAR;
	newRender->Ks[0] = defaultKs[0];
	newRender->Ks[1] = defaultKs[1];
	newRender->Ks[2] = defaultKs[2];

	// texture function
	newRender->tex_fun = NULL;

	// set up Xsp
	buildXsp(newRender);

	// pass back the pointer
	*render = newRender;
	return GZ_SUCCESS;

}


int GzFreeRender(GzRender *render)
{
/* 
-free all renderer resources
*/
	delete render;
	return GZ_SUCCESS;
}


int GzBeginRender(GzRender *render) // OK
{
/*  
- setup for start of each frame - init frame buffer color,alpha,z
- compute Xiw and projection xform Xpi from camera definition 
- init Ximage - put Xsp at base of stack, push on Xpi and Xiw 
- now stack contains Xsw and app can push model Xforms when needed 
*/ 

	// check renderer
	if (render == NULL){
		return GZ_SUCCESS;
	}

	// initialize the display
	GzInitDisplay(render->display);

	// compute Xiw and Xpi
	buildXiw(render);
	buildXpi(render);

	// put Xsp at the base of the stack
	GzPushMatrix(render, render->Xsp);

	// push Xpi and Xiw
	GzPushMatrix(render, render->camera.Xpi);
	GzPushMatrix(render, render->camera.Xiw);

	// stack now contains Xsw 

	return GZ_SUCCESS;
}

int GzPutCamera(GzRender *render, GzCamera *camera) //OK
{
/*
- overwrite renderer camera structure with new camera definition
*/

	render->camera.FOV = camera->FOV;
	render->camera.lookat[0] = camera->lookat[0];
	render->camera.lookat[1] = camera->lookat[1];
	render->camera.lookat[2] = camera->lookat[2];

	render->camera.position[0] = camera->position[0];
	render->camera.position[1] = camera->position[1];
	render->camera.position[2] = camera->position[2];


	render->camera.worldup[0] = camera->worldup[0];
	render->camera.worldup[1] = camera->worldup[1];
	render->camera.worldup[2] = camera->worldup[2];


	return GZ_SUCCESS;	
}

int GzPushMatrix(GzRender *render, GzMatrix	matrix) 
{
/*
- push a matrix onto the Ximage stack
- check for stack overflow
*/

	if (render->matlevel >= MATLEVELS){
		return GZ_FAILURE;
	}

	// if the stack is empty, just add the matrix  This will be X
	if (render->matlevel == 0) {
		
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				render->Ximage[render->matlevel][i][j] = matrix[i][j];

				// fill the first Xnorm mat with zeros
				render->Xnorm[render->matlevel][i][j] = 0;
			}
		}
		// add ones to form identity matrix
		render->Xnorm[render->matlevel][0][0] = 1;
		render->Xnorm[render->matlevel][1][1] = 1;
		render->Xnorm[render->matlevel][2][2] = 1;
		render->Xnorm[render->matlevel][3][3] = 1;
	}
	else {
		// otherwise
		// multiply the new matrix by the top of the stack
		// push it into the stack

		GzMatrix newTopStack;

		// matrix multiply the incoming matrix to the one at the top of the stack ( XIMAGE) 
		matrixMultiply(render->Ximage[render->matlevel - 1], matrix, newTopStack);

		// push onto stack
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				render->Ximage[render->matlevel][i][j] = newTopStack[i][j];
			}
		}


		// XNORM
		if (render->matlevel == 1) {// Xpi will also be identity matrix
			for (int i = 0; i < 4; i++){
				for (int j = 0; j < 4; j++){
					render->Xnorm[render->matlevel][i][j] = 0;
				}
			}
			render->Xnorm[render->matlevel][0][0] = 1;
			render->Xnorm[render->matlevel][1][1] = 1;
			render->Xnorm[render->matlevel][2][2] = 1;
			render->Xnorm[render->matlevel][3][3] = 1;

		}
		// if appropriate, process the matrix for the normal stack
		else{
			// calculate k 
			float k = sqrt(matrix[0][0] * matrix[0][0] + matrix[0][1] * matrix[0][1] + matrix[0][2] * matrix[0][2] + matrix[0][3] * matrix[0][3]);
	
			// divide each term of 3x3 by k
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 3; j++){
					matrix[i][j] = matrix[i][j] / k;
				}
			}
			// make sure there are no translates
			matrix[0][3] = 0;
			matrix[1][3] = 0;
			matrix[2][3] = 0;
			matrix[3][3] = 1;

			GzMatrix newNormTop;
			// matrix multiply the incoming matrix to the one at the top of the stack
			matrixMultiply(render->Xnorm[render->matlevel - 1], matrix, newNormTop);

			// push onto stack
			for (int i = 0; i < 4; i++){
				for (int j = 0; j < 4; j++){
					render->Xnorm[render->matlevel][i][j] = newNormTop[i][j];
				}
			}
		}
	}

	// increment matlevel
	render->matlevel++;
	return GZ_SUCCESS;
}

int GzPopMatrix(GzRender *render) // OK
{
/*
- pop a matrix off the Ximage stack
- check for stack underflow
*/

	// check for underflow
	if (render->matlevel <= 0){
		return GZ_FAILURE;
	}

	// decrement matlevel
	render->matlevel--; 

	return GZ_SUCCESS;
}


int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList, // OK
	GzPointer	*valueList) /* void** valuelist */
{
/*
- set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
- later set shaders, interpolaters, texture maps, and lights
*/
	// check all pointers given
	if (render == NULL || nameList == NULL || valueList == NULL) {
		return GZ_FAILURE;
	}

	// loop through all the attributes given 
	for (int i = 0; i < numAttributes; i++){ // might need to increment by sizeof eventually???

		// if it's the color token
		if (nameList[i] == GZ_RGB_COLOR){

			// set the color for the triangle. 
			GzColor * color = (GzColor*)valueList[i];
			render->flatcolor[0] = (*color)[0]; // red
			render->flatcolor[1] = (*color)[1]; // green
			render->flatcolor[2] = (*color)[2]; // blue
		}
		// if it's the interpolation mode
		if (nameList[i] == GZ_INTERPOLATE){

			int * mode = (int*)(valueList[i]);
			render->interp_mode = *mode;
		}

		// if it's a directional light
		if (nameList[i] == GZ_DIRECTIONAL_LIGHT){
			GzLight * dLight = (GzLight*)valueList[i];
			
			// first check if the lights are already at maximum
			if (render->numlights == MAX_LIGHTS){
				return GZ_FAILURE;
			}

			// otherwise, copy this light // INITIALIZE NUMLIGHTS

			// direction
			render->lights[render->numlights].direction[0] = dLight->direction[0];
			render->lights[render->numlights].direction[1] = dLight->direction[1];
			render->lights[render->numlights].direction[2] = dLight->direction[2];

			// color
			render->lights[render->numlights].color[0] = dLight->color[0];
			render->lights[render->numlights].color[1] = dLight->color[1];
			render->lights[render->numlights].color[2] = dLight->color[2];

			//increment number of lights
			render->numlights++;
		}
		// if it's the ambient light
		if (nameList[i] == GZ_AMBIENT_LIGHT){
			GzLight * ambLight = (GzLight*)valueList[i];

			// the ambient light is just a color
			render->ambientlight.color[0] = ambLight->color[0];
			render->ambientlight.color[1] = ambLight->color[1];
			render->ambientlight.color[2] = ambLight->color[2];
		}

		// ambient coefficient
		if (nameList[i] == GZ_AMBIENT_COEFFICIENT){ // INITIALIZE THIS

			//coefficient is a GZColor
			GzColor * ambCoeff = (GzColor*)valueList[i];
			render->Ka[0] = (*ambCoeff)[0];
			render->Ka[1] = (*ambCoeff)[1];
			render->Ka[2] = (*ambCoeff)[2];
		}
		// diffuse coefficient
		if (nameList[i] == GZ_DIFFUSE_COEFFICIENT){
			GzColor * diffCoeff = (GzColor*)valueList[i];
			render->Kd[0] = (*diffCoeff)[0];
			render->Kd[1] = (*diffCoeff)[1];
			render->Kd[2] = (*diffCoeff)[2];
		}
		// specular coefficient
		if (nameList[i] == GZ_SPECULAR_COEFFICIENT){
			GzColor * specCoeff = (GzColor*)valueList[i];
			render->Ks[0] = (*specCoeff)[0];
			render->Ks[1] = (*specCoeff)[1];
			render->Ks[2] = (*specCoeff)[2];
		}
		// distribution coefficient // what is this for???
		if (nameList[i] == GZ_DISTRIBUTION_COEFFICIENT){
			float * distCoeff = (float*)valueList[i];
			render->spec = *distCoeff;
		}

		// texture map
		if (nameList[i] == GZ_TEXTURE_MAP){
			GzTexture tex = (GzTexture)valueList[i];
			render->tex_fun = tex;
		}

		
	}
	
	return GZ_SUCCESS;
}

int GzPutTriangle(GzRender	*render, int numParts, GzToken *nameList, GzPointer	*valueList) // OK
/* numParts : how many names and values */
{
/*  
- pass in a triangle description with tokens and values corresponding to 
      GZ_POSITION:3 vert positions in model space 
- Xform positions of verts using matrix on top of stack 
- Clip - just discard any triangle with any vert(s) behind view plane 
       - optional: test for triangles with all three verts off-screen (trivial frustum cull)
- invoke triangle rasterizer  
*/ 
	// XFORM //

		// vectors for verts and normals
		vector<Vert> verts;
		
		vector<Vert> normals; // temporary store normals here until assigned to verts

		vector<Vert> tex;
		
		if (render == NULL || nameList == NULL || valueList == NULL) {
			return GZ_FAILURE;
		}

		for (int i = 0; i < numParts; i++){ // GO THROUGH VALUE LIST

			
			if (nameList[i] == GZ_NULL_TOKEN){
				// do nothing - no values
			}
			// VERTEX POSITIONS
			else if (nameList[i] == GZ_POSITION){
				GzCoord * triCoords = (GzCoord*)(valueList[i]); // array of three coords

				// sort the verts
				Vert vertex;
				// place the verts in a vector
				for (int i = 0; i < 3; i++){
					vertex.x = (triCoords[i])[0];
					vertex.y = (triCoords[i])[1];
					vertex.z = (triCoords[i])[2];

					// save the screen X and Z 
					vertex.screenX = (triCoords[i])[0];
					vertex.screenZ = (triCoords[i])[2];

					verts.push_back(vertex);
					
				}

				// XFORM VERTICES //
				xFormTris(verts, render->Ximage[render->matlevel - 1]);

				// check if behind camera
				int behind = 0;
				for (int i = 0; i < 3; i++){
					if (verts[i].z < 0) {
						behind = 1;
					}
				}
				if (behind == 1){
					continue;
				}
			}


			// VERTEX NORMALS
			else if (nameList[i] == GZ_NORMAL){
				GzCoord * triNormals = (GzCoord*)(valueList[i]); // get the normals as we did with verts above
				Vert norm;
				// place the norms in a vector
				for (int i = 0; i < 3; i++){
					norm.normX = (triNormals[i])[0];
					norm.normY = (triNormals[i])[1];
					norm.normZ = (triNormals[i])[2];
					normals.push_back(norm);

				}

				// XFORM NORMALS //
				xFormNorms(normals, render->Xnorm[render->matlevel - 1]);

				// NORMALIZE NORMALS //
				normals[0] = normalizeVert(normals[0]);
				normals[1] = normalizeVert(normals[1]);
				normals[2] = normalizeVert(normals[2]);
				
			}

			// TEXTURE COORDS
			else if (nameList[i] == GZ_TEXTURE_INDEX) {
				GzTextureIndex * texCoord = (GzTextureIndex *)(valueList[i]);
				Vert newTex;
				for (int i = 0; i < 3; i++){
					newTex.texU = (texCoord[i])[0];
					newTex.texV = (texCoord[i])[1];
					tex.push_back(newTex);
				}

			}
		}


	

		// set the normals to the vertices xNorm, yNorm, and zNorm, set textures
		for (int i = 0; i < 3; i++){
			verts[i].normX = normals[i].normX;
			verts[i].normY = normals[i].normY;
			verts[i].normZ = normals[i].normZ;
			verts[i].texU = tex[i].texU;
			verts[i].texV = tex[i].texV;

		}

		// calculate vertex color if appropriate
		if (render->interp_mode == GZ_COLOR){
			// loop through all three verts
			for (int i = 0; i < 3; i++){
				calcColor(verts[i], render); // calculate the color
			}
		}


		// transform texture
		//for (int i = 0; i < 3; i++){
			//xFormTex(verts[i]);
		//}

		// sort the vertices by y 
		sort(verts.begin(), verts.end(), ySmaller);

		//vector of Edge DDAs
		vector<DDAEdge> triEdges;

		// flag for the orientation of the triangle
		int orientation;

		// set up DDA's, get orientation
		orientation = DDAinit(verts, triEdges);
		raster(triEdges, orientation, render); 

		return GZ_SUCCESS;
}

/* NOT part of API - just for general assistance */

short	ctoi(float color)		/* convert float color to GzIntensity short */
{
  return(short)((int)(color * ((1 << 12) - 1)));
}


int DDAinit( vector<Vert> & sortedVerts, vector<DDAEdge> &Edges){

	// this is an upright triangle
	if (sortedVerts[1].x == sortedVerts[2].x){
		// first edge
		Edges.push_back(DDAEdge(sortedVerts[0], sortedVerts[1]));
		Edges.push_back(DDAEdge(sortedVerts[1], sortedVerts[2]));
		Edges.push_back(DDAEdge(sortedVerts[0], sortedVerts[2]));

		// set orientation
		return GZ_UPRIGHT_TRI;
	}

	// this is an interted triangle
	else if (sortedVerts[0].x == sortedVerts[1].x){

		// first edge
		Edges.push_back(DDAEdge(sortedVerts[0], sortedVerts[1]));

		// second edge
		Edges.push_back(DDAEdge(sortedVerts[1], sortedVerts[2]));

		// third edge
		Edges.push_back(DDAEdge(sortedVerts[1], sortedVerts[2]));

		// set orientation
		return GZ_INVERT_TRI;
	}

	else {
		// this is either a left or right triangle

		// first edge
		Edges.push_back(DDAEdge(sortedVerts[0], sortedVerts[1]));

		// second edge
		Edges.push_back(DDAEdge(sortedVerts[1], sortedVerts[2]));

		// third edge
		Edges.push_back(DDAEdge(sortedVerts[0], sortedVerts[2]));


		// check slopes
		if (Edges[0].slopeX > Edges[2].slopeX){
			// right
			return GZ_RIGHT_TRI;
		}
		// left
		else return GZ_LEFT_TRI;
	}

}

void raster(vector<DDAEdge> &Edges, int &orientation, GzRender * render){ /* consolidate down to two loops? */

	// advance current positions to top y-scan line for all interpolated variables
	for (int i = 0; i < 3; i++){

		Edges[i].current.y = Edges[i].start.y + ((ceil)(Edges[i].start.y) - (Edges[i].start.y));
		Edges[i].current.x = Edges[i].start.x + Edges[i].slopeX * ((ceil)(Edges[i].start.y) - (Edges[i].start.y));
		Edges[i].current.z = Edges[i].start.z + Edges[i].slopeZ * ((ceil)(Edges[i].start.y) - (Edges[i].start.y));
		Edges[i].current.red = Edges[i].start.red + Edges[i].slopeR * ((ceil)(Edges[i].start.y) - (Edges[i].start.y));
		Edges[i].current.green = Edges[i].start.green + Edges[i].slopeG * ((ceil)(Edges[i].start.y) - (Edges[i].start.y));
		Edges[i].current.blue = Edges[i].start.blue + Edges[i].slopeB * ((ceil)(Edges[i].start.y) - (Edges[i].start.y));
		Edges[i].current.normX = Edges[i].start.normX + Edges[i].slopeNX * ((ceil)(Edges[i].start.y) - (Edges[i].start.y));
		Edges[i].current.normY = Edges[i].start.normY + Edges[i].slopeNY * ((ceil)(Edges[i].start.y) - (Edges[i].start.y));
		Edges[i].current.normZ = Edges[i].start.normZ + Edges[i].slopeNZ * ((ceil)(Edges[i].start.y) - (Edges[i].start.y));

		Edges[i].current.texU = Edges[i].start.texU + Edges[i].slopeTU * ((ceil)(Edges[i].start.y) - (Edges[i].start.y));
		Edges[i].current.texV = Edges[i].start.texV + Edges[i].slopeTV * ((ceil)(Edges[i].start.y) - (Edges[i].start.y));

		Edges[i].current.screenX = Edges[i].start.screenX + Edges[i].slopeScreenX * ((ceil)(Edges[i].start.y) - (Edges[i].start.y));
		Edges[i].current.screenZ = Edges[i].start.screenZ + Edges[i].slopeScreenZ * ((ceil)(Edges[i].start.y) - (Edges[i].start.y));

	}

	// determine which edge DDAs to use based on triangle orientation
	int edge1index = 0;
	int edge2index = 0;
	float end = 0;


	if (orientation == GZ_LEFT_TRI || orientation == GZ_UPRIGHT_TRI){
		edge1index = 0;
		edge2index = 2;
		end = Edges[edge1index].end.y;
	}

	else if (orientation == GZ_RIGHT_TRI ){
		edge1index = 2;
		edge2index = 0;
		end = Edges[edge2index].end.y;
	}

	// top half of triangles
	while ((Edges[edge1index].current.y < end) && (orientation == GZ_LEFT_TRI || orientation == GZ_UPRIGHT_TRI || orientation == GZ_RIGHT_TRI)){

		// set up DDA for span. set span DDA current and end positions to right and left edge values
		DDASpan Span(Edges[edge1index].current, Edges[edge2index].current);

		// advance span current position to left-most covered pixel
		Span.current.x += (ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x); // was start instead of current
		Span.current.z += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeZ;

		// COLORS
		Span.current.red += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeR;
		Span.current.green += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeG;
		Span.current.blue += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeB;

		//NORMALS
		Span.current.normX += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeNX;
		Span.current.normY += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeNY;
		Span.current.normZ += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeNZ;

		//UVS
		Span.current.texU += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeTU;
		Span.current.texV += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeTV;

		//SCREEN X Z 
		Span.current.screenX += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeScreenX;
		Span.current.screenZ += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeScreenZ;

	
		while ((Span.current.x) < (Span.end.x)){

			// get x and y coords
			int xCoord = Span.current.x;
			int yCoord = Span.current.y;
			float zCoord = Span.current.z;

			GzIntensity  red;
			GzIntensity  green;
			GzIntensity  blue;
			GzIntensity  alpha;
			GzDepth  frameBufferZ;

			GzGetDisplay(render->display, xCoord, yCoord, &red, &green, &blue, &alpha, &frameBufferZ);
			// check if z value is < than what's in the frame buffer


			// FLAT
			if (zCoord < frameBufferZ){

		
				// GOURARD
				if (render->interp_mode == GZ_COLOR){

					// get the texture color
					float u, v;
					//reverseTex(Span.current, u, v);

					GzColor texColor;
					if (render->tex_fun != NULL){
						render->tex_fun(Span.current.screenX, Span.current.screenZ, Span.current.texU, Span.current.texV, texColor);
					}

					GzColor finalColor;

					// multiply the interpolated color by Kt 
					finalColor[0] = Span.current.red*texColor[0];
					finalColor[1] = Span.current.green*texColor[1];
					finalColor[2] = Span.current.blue*texColor[2];

					if (render->tex_fun == NULL){
						GzPutDisplay(render->display, xCoord, yCoord, ctoi(Span.current.red), ctoi(Span.current.green), ctoi(Span.current.blue), 0, (GzDepth)zCoord);
					}
					else GzPutDisplay(render->display, xCoord, yCoord, ctoi(finalColor[0]), ctoi(finalColor[1]), ctoi(finalColor[2]), 0, (GzDepth)zCoord);
				}

				// PHONG
				else if (render->interp_mode == GZ_NORMALS){

					// get the texture color
					float u, v;
					//reverseTex(Span.current,u,v);
					GzColor texColor;
					if (render->tex_fun != NULL){
						//render->tex_fun(u, v, texColor);
						render->tex_fun(Span.current.screenX, Span.current.screenZ, Span.current.texU, Span.current.texV, texColor);
					}
					

					// set texture color to Kd Ka
					if (render->tex_fun != NULL){
						render->Kd[0] = texColor[0];
						render->Kd[1] = texColor[1];
						render->Kd[2] = texColor[2];

						render->Ka[0] = texColor[0];
						render->Ka[1] = texColor[1];
						render->Ka[2] = texColor[2];
					}

					calcColor(Span.current, render);
					GzPutDisplay(render->display, xCoord, yCoord, ctoi(Span.current.red), ctoi(Span.current.green), ctoi(Span.current.blue), 0, (GzDepth)zCoord);
				}
				// FLAT
				else {
					GzPutDisplay(render->display, xCoord, yCoord, ctoi(render->flatcolor[0]), ctoi(render->flatcolor[1]), ctoi(render->flatcolor[2]), 0, (GzDepth)zCoord);
				}


			}

			// advance span cursor
			Span.current.x++;
			Span.current.z = Span.current.z + Span.slopeZ;

			// COLORS
			Span.current.red = Span.current.red + Span.slopeR;
			Span.current.green = Span.current.green + Span.slopeG;
			Span.current.blue = Span.current.blue + Span.slopeB;

			// NORMALS
			Span.current.normX = Span.current.normX + Span.slopeNX;
			Span.current.normY = Span.current.normY + Span.slopeNY;
			Span.current.normZ = Span.current.normZ + Span.slopeNZ;

			// UV
			Span.current.texU = Span.current.texU + Span.slopeTU;
			Span.current.texV = Span.current.texV + Span.slopeTV;

			// SCREEN X AND Z
			Span.current.screenX= Span.current.screenX+ Span.slopeScreenX;
			Span.current.screenZ = Span.current.screenZ + Span.slopeScreenZ;

		}
		// advance edge cursors 
		Edges[edge1index].current.y++;
		Edges[edge1index].current.x = Edges[edge1index].current.x + Edges[edge1index].slopeX * 1; //???
		Edges[edge1index].current.z = Edges[edge1index].current.z + Edges[edge1index].slopeZ * 1; //???

		Edges[edge2index].current.y++;
		Edges[edge2index].current.x = Edges[edge2index].current.x + Edges[edge2index].slopeX * 1; //???
		Edges[edge2index].current.z = Edges[edge2index].current.z + Edges[edge2index].slopeZ * 1; //???


		// COLORS
		Edges[edge1index].current.red = Edges[edge1index].current.red + Edges[edge1index].slopeR;
		Edges[edge1index].current.green = Edges[edge1index].current.green + Edges[edge1index].slopeG;
		Edges[edge1index].current.blue = Edges[edge1index].current.blue + Edges[edge1index].slopeB;

		Edges[edge2index].current.red = Edges[edge2index].current.red + Edges[edge2index].slopeR;
		Edges[edge2index].current.green = Edges[edge2index].current.green + Edges[edge2index].slopeG;
		Edges[edge2index].current.blue = Edges[edge2index].current.blue + Edges[edge2index].slopeB;

		// NORMALS
		Edges[edge1index].current.normX = Edges[edge1index].current.normX + Edges[edge1index].slopeNX;
		Edges[edge1index].current.normY = Edges[edge1index].current.normY + Edges[edge1index].slopeNY;
		Edges[edge1index].current.normZ = Edges[edge1index].current.normZ + Edges[edge1index].slopeNZ;

		Edges[edge2index].current.normX = Edges[edge2index].current.normX + Edges[edge2index].slopeNX;
		Edges[edge2index].current.normY = Edges[edge2index].current.normY + Edges[edge2index].slopeNY;
		Edges[edge2index].current.normZ = Edges[edge2index].current.normZ + Edges[edge2index].slopeNZ;

		// UVS
		Edges[edge1index].current.texU = Edges[edge1index].current.texU + Edges[edge1index].slopeTU;
		Edges[edge1index].current.texV = Edges[edge1index].current.texV + Edges[edge1index].slopeTV;

		Edges[edge2index].current.texU = Edges[edge2index].current.texU + Edges[edge2index].slopeTU;
		Edges[edge2index].current.texV = Edges[edge2index].current.texV + Edges[edge2index].slopeTV;


		Edges[edge1index].current.screenX = Edges[edge1index].current.screenX + Edges[edge1index].slopeScreenX;
		Edges[edge1index].current.screenZ = Edges[edge1index].current.screenZ + Edges[edge1index].slopeScreenZ;

		Edges[edge2index].current.screenX = Edges[edge2index].current.screenX + Edges[edge2index].slopeScreenX;
		Edges[edge2index].current.screenZ = Edges[edge2index].current.screenZ + Edges[edge2index].slopeScreenZ;
		
	}

	if (orientation == GZ_LEFT_TRI ){
		edge1index = 1;
		edge2index = 2;
		end = Edges[edge1index].end.y;
	}
	if (orientation == GZ_RIGHT_TRI || orientation == GZ_INVERT_TRI){
		edge1index = 2;
		edge2index = 1;
		end = Edges[edge2index].end.y;
	}

	// bottom half of the triangles
	while ((Edges[edge1index].current.y < end) && (orientation == GZ_LEFT_TRI || GZ_INVERT_TRI || GZ_RIGHT_TRI)){


		// set up DDA for span. set span DDA current and end positions to right and left edge values
		DDASpan Span(Edges[edge1index].current, Edges[edge2index].current);

		// advance span current position to left-most covered pixel
		Span.current.x += (ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x); // was start instead of current
		Span.current.z += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeZ;

		// COLORS
		Span.current.red += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeR;
		Span.current.green += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeG;
		Span.current.blue += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeB;

		//NORMALS
		Span.current.normX += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeNX;
		Span.current.normY += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeNY;
		Span.current.normZ += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeNZ;

		//UVS
		Span.current.texU += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeTU;
		Span.current.texV += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeTV;

		//SCREEN X Z 
		Span.current.screenX += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeScreenX;
		Span.current.screenZ += ((ceil)(Edges[edge1index].current.x) - (Edges[edge1index].current.x))*Span.slopeScreenZ;

		while ((Span.current.x) < (Span.end.x)){

			// get x and y coords
			int xCoord = Span.current.x;
			int yCoord = Span.current.y;
			float zCoord = Span.current.z;

			GzIntensity  red;
			GzIntensity  green;
			GzIntensity  blue;
			GzIntensity  alpha;
			GzDepth  frameBufferZ;

			GzGetDisplay(render->display, xCoord, yCoord, &red, &green, &blue, &alpha, &frameBufferZ);
			// check if z value is < than what's in the frame buffer


			// FLAT
			if (zCoord < frameBufferZ){

				// GOURARD
				if (render->interp_mode == GZ_COLOR){

					// get the texture color
					float u, v;
					//reverseTex(Span.current, u, v);
					GzColor texColor;
					if (render->tex_fun != NULL){
						render->tex_fun(Span.current.screenX, Span.current.screenZ, Span.current.texU, Span.current.texV, texColor);
					}
					

					GzColor finalColor;

					// multiply the interpolated color by Kt 
					finalColor[0] = Span.current.red*texColor[0];
					finalColor[1] = Span.current.green*texColor[1];
					finalColor[2] = Span.current.blue*texColor[2];

					if (render->tex_fun == NULL){
						GzPutDisplay(render->display, xCoord, yCoord, ctoi(Span.current.red), ctoi(Span.current.green), ctoi(Span.current.blue), 0, (GzDepth)zCoord);
					}
					else GzPutDisplay(render->display, xCoord, yCoord, ctoi(finalColor[0]), ctoi(finalColor[1]), ctoi(finalColor[2]), 0, (GzDepth)zCoord);
				}

				// PHONG
				else if (render->interp_mode == GZ_NORMALS){

					// get the texture color
					float u, v;
					//reverseTex(Span.current, u, v);
					GzColor texColor;
					if (render->tex_fun != NULL){
						//render->tex_fun(u, v, texColor);
						render->tex_fun(Span.current.screenX, Span.current.screenZ, Span.current.texU, Span.current.texV, texColor);
					}

					// set texture color to Kd and Ka

					if (render->tex_fun != NULL){
						render->Kd[0] = texColor[0];
						render->Kd[1] = texColor[1];
						render->Kd[2] = texColor[2];

						render->Ka[0] = texColor[0];
						render->Ka[1] = texColor[1];
						render->Ka[2] = texColor[2];
					}

					calcColor(Span.current, render);
					GzPutDisplay(render->display, xCoord, yCoord, ctoi(Span.current.red), ctoi(Span.current.green), ctoi(Span.current.blue), 0, (GzDepth)zCoord);
				}
				else {				// FLAT

					GzPutDisplay(render->display, xCoord, yCoord, ctoi(render->flatcolor[0]), ctoi(render->flatcolor[1]), ctoi(render->flatcolor[2]), 0, (GzDepth)zCoord);
				}


			}

			// advance span cursor
			Span.current.x++;
			Span.current.z = Span.current.z + Span.slopeZ;

			// COLORS
			Span.current.red = Span.current.red + Span.slopeR;
			Span.current.green = Span.current.green + Span.slopeG;
			Span.current.blue = Span.current.blue + Span.slopeB;

			// NORMALS
			Span.current.normX = Span.current.normX + Span.slopeNX;
			Span.current.normY = Span.current.normY + Span.slopeNY;
			Span.current.normZ = Span.current.normZ + Span.slopeNZ;

			// UV
			Span.current.texU = Span.current.texU + Span.slopeTU;
			Span.current.texV = Span.current.texV + Span.slopeTV;

			// SCREEN X AND Z
			Span.current.screenX = Span.current.screenX + Span.slopeScreenX;
			Span.current.screenZ = Span.current.screenZ + Span.slopeScreenZ;

		}
		// advance edge cursors 
		Edges[edge1index].current.y++;
		Edges[edge1index].current.x = Edges[edge1index].current.x + Edges[edge1index].slopeX * 1; //???
		Edges[edge1index].current.z = Edges[edge1index].current.z + Edges[edge1index].slopeZ * 1; //???

		Edges[edge2index].current.y++;
		Edges[edge2index].current.x = Edges[edge2index].current.x + Edges[edge2index].slopeX * 1; //???
		Edges[edge2index].current.z = Edges[edge2index].current.z + Edges[edge2index].slopeZ * 1; //???


		// COLORS
		Edges[edge1index].current.red = Edges[edge1index].current.red + Edges[edge1index].slopeR;
		Edges[edge1index].current.green = Edges[edge1index].current.green + Edges[edge1index].slopeG;
		Edges[edge1index].current.blue = Edges[edge1index].current.blue + Edges[edge1index].slopeB;

		Edges[edge2index].current.red = Edges[edge2index].current.red + Edges[edge2index].slopeR;
		Edges[edge2index].current.green = Edges[edge2index].current.green + Edges[edge2index].slopeG;
		Edges[edge2index].current.blue = Edges[edge2index].current.blue + Edges[edge2index].slopeB;

		// NORMALS
		Edges[edge1index].current.normX = Edges[edge1index].current.normX + Edges[edge1index].slopeNX;
		Edges[edge1index].current.normY = Edges[edge1index].current.normY + Edges[edge1index].slopeNY;
		Edges[edge1index].current.normZ = Edges[edge1index].current.normZ + Edges[edge1index].slopeNZ;

		Edges[edge2index].current.normX = Edges[edge2index].current.normX + Edges[edge2index].slopeNX;
		Edges[edge2index].current.normY = Edges[edge2index].current.normY + Edges[edge2index].slopeNY;
		Edges[edge2index].current.normZ = Edges[edge2index].current.normZ + Edges[edge2index].slopeNZ;

		// UVS
		Edges[edge1index].current.texU = Edges[edge1index].current.texU + Edges[edge1index].slopeTU;
		Edges[edge1index].current.texV = Edges[edge1index].current.texV + Edges[edge1index].slopeTV;

		Edges[edge2index].current.texU = Edges[edge2index].current.texU + Edges[edge2index].slopeTU;
		Edges[edge2index].current.texV = Edges[edge2index].current.texV + Edges[edge2index].slopeTV;

		// SCREEN X AND Z
		Edges[edge1index].current.screenX = Edges[edge1index].current.screenX + Edges[edge1index].slopeScreenX;
		Edges[edge1index].current.screenZ = Edges[edge1index].current.screenZ + Edges[edge1index].slopeScreenZ;

		Edges[edge2index].current.screenX = Edges[edge2index].current.screenX + Edges[edge2index].slopeScreenX;
		Edges[edge2index].current.screenZ = Edges[edge2index].current.screenZ + Edges[edge2index].slopeScreenZ;
	}
}


// custom comparison funcion for c++ sort
bool ySmaller(const Vert &v1, const Vert &v2){
	if (v1.y == v2.y) return v1.x < v2.x; // break a tie by putting the smaller x value first
	else return v1.y < v2.y;
}



void xFormTris(vector<Vert> & verts, GzMatrix xFormMat){

	// put the vertices into a 4 * 3 matrix ( add 1 for homogenious coords )
	float vertsToXForm[4][3];

	for (int j = 0; j < 3; j++){
		vertsToXForm[0][j] = verts[j].x;
		vertsToXForm[1][j] = verts[j].y;
		vertsToXForm[2][j] = verts[j].z;
		vertsToXForm[3][j] = 1;
	}

	// initialize an output matrix
	float xFormedVerts[4][3];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 3; j++){
			xFormedVerts[i][j] = 0;
		}
	}


	// do the transformation
	for (int i = 0; i < 4; i++){ // x, y, z, 1
		for (int j = 0; j < 3; j++) { // each vert
			for (int k = 0; k < 4; k++){
				xFormedVerts[i][j] += xFormMat[i][k] * vertsToXForm[k][j];
			}
		}
	}

	// put back into verts vector ( divide by the last coord to switch back to 3D ) 
	for (int i = 0; i < 3; i++){
		verts[i].x = xFormedVerts[0][i] / xFormedVerts[3][i];
		verts[i].y = xFormedVerts[1][i] / xFormedVerts[3][i];
		verts[i].z = xFormedVerts[2][i] / xFormedVerts[3][i];
	}

}

void xFormNorms(vector<Vert> & normals, GzMatrix xFormMat){
	// this will be the same as the xform func for verts, but use norm data members of Vert class
	// put the vertices into a 4 * 3 matrix ( add 1 for homogenious coords )
	float normsToXForm[4][3];

	for (int j = 0; j < 3; j++){
		normsToXForm[0][j] = normals[j].normX;
		normsToXForm[1][j] = normals[j].normY;
		normsToXForm[2][j] = normals[j].normZ;
		normsToXForm[3][j] = 1;
	}

	// initialize an output matrix
	float xFormedNorms[4][3];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 3; j++){
			xFormedNorms[i][j] = 0;
		}
	}


	// do the transformation
	for (int i = 0; i < 4; i++){ // x, y, z, 1
		for (int j = 0; j < 3; j++) { // each vert
			for (int k = 0; k < 4; k++){
				xFormedNorms[i][j] += xFormMat[i][k] * normsToXForm[k][j];
			}
		}
	}

	// put back into verts vector ( divide by the last coord to switch back to 3D ) 
	for (int i = 0; i < 3; i++){
		normals[i].normX = xFormedNorms[0][i] / xFormedNorms[3][i];
		normals[i].normY = xFormedNorms[1][i] / xFormedNorms[3][i];
		normals[i].normZ = xFormedNorms[2][i] / xFormedNorms[3][i];
	}

}



void buildXsp(GzRender * render ){

	// build the matrix

	// first row
	render->Xsp[0][0] = (render->display->xres) / 2.0;
	render->Xsp[0][1] = 0;
	render->Xsp[0][2] = 0;
	render->Xsp[0][3] = (render->display->xres) / 2.0;
	
	// second row
	render->Xsp[1][0] = 0;
	render->Xsp[1][1] = -(render->display->yres) / 2.0;
	render->Xsp[1][2] = 0;
	render->Xsp[1][3] = (render->display->yres) / 2.0;

	// third row
	render->Xsp[2][0] = 0;
	render->Xsp[2][1] = 0;
	render->Xsp[2][2] = MAXINT;
	render->Xsp[2][3] = 0;

	// fourth row
	render->Xsp[3][0] = 0;
	render->Xsp[3][1] = 0;
	render->Xsp[3][2] = 0;
	render->Xsp[3][3] = 1.0;

}

void buildXpi(GzRender * render){

	float fov = render->camera.FOV; // CONVERT TO RADIAN

	float fovRad = (float)(fov / 180) * 3.1415926;

	float d = 1 / ( tan(fovRad / 2) ) ;

	// first row
	render->camera.Xpi[0][0] = 1;
	render->camera.Xpi[0][1] = 0;
	render->camera.Xpi[0][2] = 0;
	render->camera.Xpi[0][3] = 0;

	// second row
	render->camera.Xpi[1][0] = 0;
	render->camera.Xpi[1][1] = 1;
	render->camera.Xpi[1][2] = 0;
	render->camera.Xpi[1][3] = 0;

	// third row
	render->camera.Xpi[2][0] = 0;
	render->camera.Xpi[2][1] = 0;
	render->camera.Xpi[2][2] = 1 / d;
	render->camera.Xpi[2][3] = 0;

	// fourth row
	render->camera.Xpi[3][0] = 0;
	render->camera.Xpi[3][1] = 0;
	render->camera.Xpi[3][2] = 1 / d;
	render->camera.Xpi[3][3] = 1;
}

void buildXiw(GzRender * render){

	// Z = CL / || CL ||
	// Y = up' / || up' ||
	// X = ( Y X Z )

	// up' = up - (up dot Z)Z

	Vert z;
	Vert y;
	Vert x;

	Vert CL;
	Vert upP;
	Vert up;

	// Z
	// Camera/lookat vector
	CL.x = render->camera.lookat[0] - render->camera.position[0];
	CL.y = render->camera.lookat[1] - render->camera.position[1];
	CL.z = render->camera.lookat[2] - render->camera.position[2];

	// normalize to get Z 
	z = normalizeVert(CL);

	//Y
	// up
	up.x = render->camera.worldup[0];
	up.y = render->camera.worldup[1];
	up.z = render->camera.worldup[2];

	// up'
	float dotP = dotProd(up, z);
	upP.x = up.x - dotP*z.x;
	upP.y = up.y - dotP*z.y;
	upP.z = up.z - dotP*z.z;

	// normalize to get Y
	y = normalizeVert(upP);

	//X
	x.x = y.y*z.z - y.z*z.y;
	x.y = y.z*z.x - y.x*z.z;
	x.z = y.x*z.y - y.y*z.x;


	// build the matrix
	// first row
	render->camera.Xiw[0][0] = x.x;
	render->camera.Xiw[0][1] = x.y;
	render->camera.Xiw[0][2] = x.z;
	render->camera.Xiw[0][3] = -(x.x * render->camera.position[0] + x.y * render->camera.position[1] + x.z * render->camera.position[2]);

	// second row
	render->camera.Xiw[1][0] = y.x;
	render->camera.Xiw[1][1] = y.y;
	render->camera.Xiw[1][2] = y.z;
	render->camera.Xiw[1][3] = -(y.x * render->camera.position[0] + y.y * render->camera.position[1] + y.z * render->camera.position[2]);

	// third row
	render->camera.Xiw[2][0] = z.x;
	render->camera.Xiw[2][1] = z.y;
	render->camera.Xiw[2][2] = z.z;
	render->camera.Xiw[2][3] = -(z.x * render->camera.position[0] + z.y * render->camera.position[1] + z.z * render->camera.position[2]);

	// fourth row
	render->camera.Xiw[3][0] = 0;
	render->camera.Xiw[3][1] = 0;
	render->camera.Xiw[3][2] = 0;
	render->camera.Xiw[3][3] = 1;

}


void calcColor(Vert & vert, GzRender * render){

	GzColor color; // final color we will calculate

	// C = ( Ks* sum of ( light color ( RGB )( R*E)^s ) + (Kd* sum of (light color (N*L)) + Ka(light color)
	// Need L, E, N, and R
	Vert E;  // eye vector
	Vert N;  // normal vector
	Vert L;  // light direction vector
	Vert R;  // reflection vector

	// E is (0,0,-1) in image space
	E.x = 0;
	E.y = 0;
	E.z = -1;

	// We know N
	N.x = vert.normX;
	N.y = vert.normY;
	N.z = vert.normZ;

	// variables to sum the color
	GzColor lightSumDiffuse = { 0, 0, 0 };
	GzColor lightSumSpec = { 0, 0, 0 };
	GzColor lightColor = { 0, 0, 0 };



	// loop over all of the lights to calculate diffuse and specular
	for (int i = 0; i < render->numlights; i++) {

		// light direction vector L, OK 
		L.x = render->lights[i].direction[0];
		L.y = render->lights[i].direction[1];
		L.z = render->lights[i].direction[2];

		L = normalizeVert(L);

		// current light color, OK
		lightColor[0] = render->lights[i].color[0];
		lightColor[1] = render->lights[i].color[1];
		lightColor[2] = render->lights[i].color[2];


		//NL
		float NL = dotProd(N, L);

		// Calculate NE to check for validity
		float NE = dotProd(N, E);

		// If N*E and N*L have opposite signs, skip this
		if ((NE > 0 && NL < 0) || (NE < 0 && NL > 0)){
			continue; // first red goes in here
		}
		// if both are negative, we flip the normals
		if (NE < 0 && NL < 0){
			N.x = -N.x;
			N.y = -N.y;
			N.z = -N.z;

			// negate NL
			NL = -NL;
		}

		// R computed for each light: R = 2(NL)N -L
		R.x = 2 * NL* N.x - L.x;
		R.y = 2 * NL* N.y - L.y;
		R.z = 2 * NL *N.z - L.z;

		// normalize reflect
		R = normalizeVert(R);

		// RE
		float RE = dotProd(R, E);  // need to clamp this value per the slides
		if (RE > 1){
			RE = 1;
		}
		if (RE < 0){
			RE = 0;
		}


		// calculate the specular sum
		lightSumSpec[0] += lightColor[0] * pow(RE, render->spec); 
		lightSumSpec[1] += lightColor[1] * pow(RE, render->spec);
		lightSumSpec[2] += lightColor[2] * pow(RE, render->spec);

		// calculate the diffuse
		lightSumDiffuse[0] += lightColor[0] * NL;
		lightSumDiffuse[1] += lightColor[1] * NL;
		lightSumDiffuse[2] += lightColor[2] * NL;



	}

	
	if (render->interp_mode == GZ_NORMALS || render->tex_fun == NULL){

		// final lighting equation
		// final         // specular					// diffuse							// ambient
		color[0] = render->Ks[0] * lightSumSpec[0] + render->Kd[0] * lightSumDiffuse[0] + render->Ka[0] * render->ambientlight.color[0];
		color[1] = render->Ks[1] * lightSumSpec[1] + render->Kd[1] * lightSumDiffuse[1] + render->Ka[1] * render->ambientlight.color[1];
		color[2] = render->Ks[2] * lightSumSpec[2] + render->Kd[2] * lightSumDiffuse[2] + render->Ka[2] * render->ambientlight.color[2];
	}
	else if (render->interp_mode == GZ_COLOR && render->tex_fun != NULL){
		// factor the K's out of the shading eq
		color[0] = lightSumSpec[0] + lightSumDiffuse[0] + render->ambientlight.color[0];
		color[1] = lightSumSpec[1] + lightSumDiffuse[1] + render->ambientlight.color[1];
		color[2] = lightSumSpec[2] + lightSumDiffuse[2] + render->ambientlight.color[2];
	}
	

	// CLAMP COLOR
	if (color[0] > 1){
		color[0] = 1;
	}
	if (color[0] < 0){
		color[0] = 0;
	}
	if (color[1] > 1){
		color[1] = 1;
	}
	if (color[1] < 0){
		color[1] = 0;
	}
	if (color[2] > 1){
		color[2] = 1;
	}
	if (color[2] < 0){
		color[2] = 0;
	}



	// assign color to the vertex
	vert.red = color[0];
	vert.green = color[1];
	vert.blue = color[2];
}


void xFormTex(Vert & vert){

	// calculate Vz': Z / ZMAX - Z 
	float VzPrime = vert.z / (INT_MAX - vert.z);

	// transform texture parameters into perspective space Ps = P/(Vz' + 1) 
	vert.texU = vert.texU / (VzPrime + 1);
	vert.texV = vert.texV / (VzPrime + 1);
}

void reverseTex(Vert & vert, float & u, float & v){
	
	// again calculate Vz'
	float VzPrime = vert.z / (INT_MAX - vert.z);

	// transform back into affine space P = Ps*(Vz' + 1)
	// DONT copy these back into the vertex
	u = vert.texU*(VzPrime + 1);
	v = vert.texV*(VzPrime + 1);
}
void matrixMultiply(const GzMatrix mat1, const GzMatrix mat2, GzMatrix result){ // OK

	// initialize the result matrix to all zeros
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			result[i][j] = 0;
		}
	}


	// multiply
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++){
			for (int k = 0; k < 4; k++){
				result[i][j] += mat1[i][k] * mat2[k][j];
			}
		}
	}
}

float dotProd(Vert vert1, Vert vert2){
	return vert1.x*vert2.x + vert1.y*vert2.y + vert1.z*vert2.z;
}


// normalize the position and normal fields of custom Vertex class
Vert normalizeVert(Vert & vert){

	Vert normalized;
	normalized.x = vert.x / sqrt(vert.x*vert.x + vert.y*vert.y + vert.z*vert.z);
	normalized.y = vert.y / sqrt(vert.x*vert.x + vert.y*vert.y + vert.z*vert.z);
	normalized.z = vert.z / sqrt(vert.x*vert.x + vert.y*vert.y + vert.z*vert.z);

	normalized.normX = vert.normX / sqrt(vert.normX*vert.normX + vert.normY*vert.normY + vert.normZ*vert.normZ);
	normalized.normY = vert.normY / sqrt(vert.normX*vert.normX + vert.normY*vert.normY + vert.normZ*vert.normZ);
	normalized.normZ = vert.normZ / sqrt(vert.normX*vert.normX + vert.normY*vert.normY + vert.normZ*vert.normZ);

	return normalized;
}

// normalize a GzCoord
void normalizeGzCoord(GzCoord & coord){

	coord[0] = coord[0] / sqrt(coord[0] * coord[0] + coord[1] * coord[1] + coord[2] * coord[2]);
	coord[1] = coord[1] / sqrt(coord[0] * coord[0] + coord[1] * coord[1] + coord[2] * coord[2]);
	coord[2] = coord[2] / sqrt(coord[0] * coord[0] + coord[1] * coord[1] + coord[2] * coord[2]);
}