/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

#define PI 3.14159

short ctoi(float color);
float AreaOfTriangle(GzCoord v0, GzCoord v1, GzCoord v2);
int newMatrixPush(GzRender *render, GzMatrix matrix);

int GzRotXMat(float degree, GzMatrix mat)
{
	// Create rotate matrix : rotate along x axis
	// Pass back the matrix using mat value
	if (!mat)
		return GZ_FAILURE;

	float radian = degree * (PI / 180.0);

	mat[0][0] = 1;
	mat[0][1] = 0;
	mat[0][2] = 0;
	mat[0][3] = 0;

	mat[1][0] = 0;
	mat[1][1] = cos(radian);
	mat[1][2] = -sin(radian);
	mat[1][3] = 0;

	mat[2][0] = 0;
	mat[2][1] = sin(radian);
	mat[2][2] = cos(radian);
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
	if (!mat)
		return GZ_FAILURE;

	float radian = degree * (PI / 180.0);

	mat[0][0] = cos(radian);
	mat[0][1] = 0;
	mat[0][2] = sin(radian);
	mat[0][3] = 0;

	mat[1][0] = 0;
	mat[1][1] = 1;
	mat[1][2] = 0;
	mat[1][3] = 0;

	mat[2][0] = -sin(radian);
	mat[2][1] = 0;
	mat[2][2] = cos(radian);
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
	if (!mat)
		return GZ_FAILURE;

	float radian = degree * (PI / 180.0);

	mat[0][0] = cos(radian);
	mat[0][1] = -sin(radian);
	mat[0][2] = 0;
	mat[0][3] = 0;

	mat[1][0] = sin(radian);
	mat[1][1] = cos(radian);
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
	if (!mat)
		return GZ_FAILURE;

	mat[0][0] = 1;
	mat[0][1] = 0;
	mat[0][2] = 0;
	mat[0][3] = translate[X];

	mat[1][0] = 0;
	mat[1][1] = 1;
	mat[1][3] = translate[Y];
	mat[1][2] = 0;

	mat[2][0] = 0;
	mat[2][1] = 0;
	mat[2][2] = 1;
	mat[2][3] = translate[Z];

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
	if (!mat)
		return GZ_FAILURE;

	mat[0][0] = scale[X];
	mat[1][1] = scale[Y];
	mat[2][2] = scale[Z];

	return GZ_SUCCESS;
}

int normalizeVector(GzCoord vec) {
	if (vec == NULL) {
		return GZ_FAILURE;
	}

	float norm = sqrt((vec[X] * vec[X]) + (vec[Y] * vec[Y]) + (vec[Z] * vec[Z]));

	vec[X] = vec[X] / norm;
	vec[Y] = vec[Y] / norm;
	vec[Z] = vec[Z] / norm;

	return GZ_SUCCESS;
}

float dotProduct(GzCoord v1, GzCoord v2){
	return v1[X] * v2[X] + v1[Y] * v2[Y] + v1[Z] * v2[Z];
}

int shadingEquation(GzRender *render, GzColor color, GzCoord norm){
	normalizeVector(norm);

	GzCoord eye;
	eye[X] = 0;
	eye[Y] = 0;
	eye[Z] = -1;
	normalizeVector(eye);

	//int lightIdx;
	GzCoord* R = new GzCoord[render->numlights];
	float NdotE = dotProduct(norm, eye);
	float NdotL;
	int* lightType = new int[render->numlights];
	for (int lightIdx = 0; lightIdx < render->numlights; ++lightIdx) {
		NdotL = dotProduct(norm, render->lights[lightIdx].direction);

		if (NdotL >= 0 && NdotE >= 0) {
			lightType[lightIdx] = 1;
			R[lightIdx][X] = 2 * NdotL*norm[X] - render->lights[lightIdx].direction[X];
			R[lightIdx][Y] = 2 * NdotL*norm[Y] - render->lights[lightIdx].direction[Y];
			R[lightIdx][Z] = 2 * NdotL*norm[Z] - render->lights[lightIdx].direction[Z];
			normalizeVector(R[lightIdx]);
		}
		else if (NdotL < 0 && NdotE < 0) {
			lightType[lightIdx] = -1;
			R[lightIdx][X] = 2 * NdotL*(-norm[X]) - render->lights[lightIdx].direction[X];
			R[lightIdx][Y] = 2 * NdotL*(-norm[Y]) - render->lights[lightIdx].direction[Y];
			R[lightIdx][Z] = 2 * NdotL*(-norm[Z]) - render->lights[lightIdx].direction[Z];
			normalizeVector(R[lightIdx]);
		}
		else{
			lightType[lightIdx] = 0;
			continue;
		}
	}

	GzColor specComponent, specSum = { 0, 0, 0 };
	for (int lightIdx = 0; lightIdx < render->numlights; ++lightIdx) {
		if (lightType[lightIdx] == 0) continue;
		float RdotE = dotProduct(R[lightIdx], eye);
		//Bound RdotE values
		if (RdotE < 0)
			RdotE = 0;
		if (RdotE > 1)
			RdotE = 1;

		specSum[0] += render->lights[lightIdx].color[0] * pow(RdotE, render->spec);
		specSum[1] += render->lights[lightIdx].color[1] * pow(RdotE, render->spec);
		specSum[2] += render->lights[lightIdx].color[2] * pow(RdotE, render->spec);
	}
	//Calculated final spectral component
	specComponent[RED] = render->Ks[0] * specSum[0];
	specComponent[GREEN] = render->Ks[1] * specSum[1];
	specComponent[BLUE] = render->Ks[2] * specSum[2];

	GzColor diffuseComponent, diffuseSum = { 0, 0, 0 };
	for (int lightIdx = 0; lightIdx < render->numlights; ++lightIdx) {
		//float NdotL = dotProduct(norm, render->lights[lightIdx].direction);

		if (lightType[lightIdx] == 0)
			continue;

		if (lightType[lightIdx] == 1) {
			//if (NdotL >= 0 && NdotE >= 0){
			diffuseSum[0] += render->lights[lightIdx].color[0] * dotProduct(norm, render->lights[lightIdx].direction);
			diffuseSum[1] += render->lights[lightIdx].color[1] * dotProduct(norm, render->lights[lightIdx].direction);
			diffuseSum[2] += render->lights[lightIdx].color[2] * dotProduct(norm, render->lights[lightIdx].direction);
		}
		else if (lightType[lightIdx] == -1){
			//else if (NdotL < 0 && NdotE < 0){
			GzCoord negNorm = { -norm[X], -norm[Y], -norm[Z] };  //Flip normals
			diffuseSum[0] += render->lights[lightIdx].color[0] * dotProduct(negNorm, render->lights[lightIdx].direction);
			diffuseSum[1] += render->lights[lightIdx].color[1] * dotProduct(negNorm, render->lights[lightIdx].direction);
			diffuseSum[2] += render->lights[lightIdx].color[2] * dotProduct(negNorm, render->lights[lightIdx].direction);
		}
	}
	//Calculated final diffused component
	diffuseComponent[RED] = render->Kd[0] * diffuseSum[0];
	diffuseComponent[GREEN] = render->Kd[1] * diffuseSum[1];
	diffuseComponent[BLUE] = render->Kd[2] * diffuseSum[2];

	//Ambient Component
	GzColor ambComponent;
	ambComponent[RED] = render->Ka[0] * render->ambientlight.color[0];
	ambComponent[GREEN] = render->Ka[1] * render->ambientlight.color[1];
	ambComponent[BLUE] = render->Ka[2] * render->ambientlight.color[2];

	//Implementing shading equation
	color[RED] = specComponent[RED] + diffuseComponent[RED] + ambComponent[RED];
	color[GREEN] = specComponent[GREEN] + diffuseComponent[GREEN] + ambComponent[GREEN];
	color[BLUE] = specComponent[BLUE] + diffuseComponent[BLUE] + ambComponent[BLUE];

	delete[] R;
	delete[] lightType;
	return GZ_SUCCESS;
}

int gouraudTextureShadingEquation(GzRender *render, GzColor color, GzCoord norm){
	normalizeVector(norm);

	GzCoord eye;
	eye[X] = 0;
	eye[Y] = 0;
	eye[Z] = -1;
	normalizeVector(eye);

	//int lightIdx;
	GzCoord* R = new GzCoord[render->numlights];
	float NdotE = dotProduct(norm, eye);
	float NdotL;
	int* lightType = new int[render->numlights];
	for (int lightIdx = 0; lightIdx < render->numlights; ++lightIdx) {
		NdotL = dotProduct(norm, render->lights[lightIdx].direction);

		if (NdotL >= 0 && NdotE >= 0) {
			lightType[lightIdx] = 1;
			R[lightIdx][X] = 2 * NdotL*norm[X] - render->lights[lightIdx].direction[X];
			R[lightIdx][Y] = 2 * NdotL*norm[Y] - render->lights[lightIdx].direction[Y];
			R[lightIdx][Z] = 2 * NdotL*norm[Z] - render->lights[lightIdx].direction[Z];
			normalizeVector(R[lightIdx]);
		}
		else if (NdotL < 0 && NdotE < 0) {
			lightType[lightIdx] = -1;
			R[lightIdx][X] = 2 * NdotL*(-norm[X]) - render->lights[lightIdx].direction[X];
			R[lightIdx][Y] = 2 * NdotL*(-norm[Y]) - render->lights[lightIdx].direction[Y];
			R[lightIdx][Z] = 2 * NdotL*(-norm[Z]) - render->lights[lightIdx].direction[Z];
			normalizeVector(R[lightIdx]);
		}
		else{
			lightType[lightIdx] = 0;
			continue;
		}
	}

	GzColor specSum = { 0, 0, 0 };
	for (int lightIdx = 0; lightIdx < render->numlights; ++lightIdx) {
		if (lightType[lightIdx] == 0) continue;
		float RdotE = dotProduct(R[lightIdx], eye);
		//Bound RdotE values
		if (RdotE < 0)
			RdotE = 0;
		if (RdotE > 1)
			RdotE = 1;

		specSum[0] += render->lights[lightIdx].color[0] * pow(RdotE, render->spec);
		specSum[1] += render->lights[lightIdx].color[1] * pow(RdotE, render->spec);
		specSum[2] += render->lights[lightIdx].color[2] * pow(RdotE, render->spec);
	}
	//Calculated final spectral component
	/*specComponent[RED] = render->Ks[0] * specSum[0];
	specComponent[GREEN] = render->Ks[1] * specSum[1];
	specComponent[BLUE] = render->Ks[2] * specSum[2];*/

	GzColor diffuseSum = { 0, 0, 0 };
	for (int lightIdx = 0; lightIdx < render->numlights; ++lightIdx) {
		//float NdotL = dotProduct(norm, render->lights[lightIdx].direction);

		if (lightType[lightIdx] == 0)
			continue;

		if (lightType[lightIdx] == 1) {
			//if (NdotL >= 0 && NdotE >= 0){
			diffuseSum[0] += render->lights[lightIdx].color[0] * dotProduct(norm, render->lights[lightIdx].direction);
			diffuseSum[1] += render->lights[lightIdx].color[1] * dotProduct(norm, render->lights[lightIdx].direction);
			diffuseSum[2] += render->lights[lightIdx].color[2] * dotProduct(norm, render->lights[lightIdx].direction);
		}
		else if (lightType[lightIdx] == -1){
			//else if (NdotL < 0 && NdotE < 0){
			GzCoord negNorm = { -norm[X], -norm[Y], -norm[Z] };  //Flip normals
			diffuseSum[0] += render->lights[lightIdx].color[0] * dotProduct(negNorm, render->lights[lightIdx].direction);
			diffuseSum[1] += render->lights[lightIdx].color[1] * dotProduct(negNorm, render->lights[lightIdx].direction);
			diffuseSum[2] += render->lights[lightIdx].color[2] * dotProduct(negNorm, render->lights[lightIdx].direction);
		}
	}
	//Calculated final diffused component
	/*diffuseComponent[RED] = render->Kd[0] * diffuseSum[0];
	diffuseComponent[GREEN] = render->Kd[1] * diffuseSum[1];
	diffuseComponent[BLUE] = render->Kd[2] * diffuseSum[2];*/

	//Ambient Component
	/*GzColor ambComponent;
	ambComponent[RED] = render->Ka[0] * render->ambientlight.color[0];
	ambComponent[GREEN] = render->Ka[1] * render->ambientlight.color[1];
	ambComponent[BLUE] = render->Ka[2] * render->ambientlight.color[2];*/

	//Implementing shading equation
	color[RED] = specSum[RED] + diffuseSum[RED] + render->ambientlight.color[RED];
	color[GREEN] = specSum[GREEN] + diffuseSum[GREEN] + render->ambientlight.color[GREEN];
	color[BLUE] = specSum[BLUE] + diffuseSum[BLUE] + render->ambientlight.color[BLUE];

	delete[]R;
	delete[]lightType;
	return GZ_SUCCESS;
}

int newMatrixPush(GzRender *render, GzMatrix matrix) {
	float factor = 1 / sqrt(matrix[0][0] * matrix[0][0] + matrix[0][1] * matrix[0][1] + matrix[0][2] * matrix[0][2] + matrix[0][3] * matrix[0][3]);

	for (int row = 0; row < 3; row++){
		for (int col = 0; col < 3; col++){

			matrix[row][col] *= factor;
		}
	}

	memcpy(render->Xnorm[render->matlevel], matrix, sizeof(GzMatrix));
	return GZ_SUCCESS;
}

//----------------------------------------------------------
// Begin main functions

int GzNewRender(GzRender **render, GzDisplay	*display)
{
	/*
	- malloc a renderer struct
	- setup Xsp and anything only done once
	- save the pointer to display
	- init default camera
	*/
	// new render
	*render = new GzRender();
	// check render
	if (*render == NULL)
		return GZ_FAILURE;

	if (!display)
		return GZ_FAILURE;

	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		//(*render)->Xsp[i][j] = 0;
	//		(*render)->camera.Xiw[i][j] = 0;
	//		(*render)->camera.Xpi[i][j] = 0;
	//	}
	//}

	(*render)->display = display;
	(*render)->matlevel = -1;

	for (int i = 0; i < AAKERNEL_SIZE; i++){
		GzNewDisplay(&(*render)->newAAdisplay[i], 256, 256);
		GzInitDisplay((*render)->newAAdisplay[i]);
	}

	// Xsp setup
	(*render)->Xsp[0][0] = (*render)->display->xres / 2;
	(*render)->Xsp[0][1] = 0.0;
	(*render)->Xsp[0][2] = 0.0;
	(*render)->Xsp[0][3] = (*render)->display->xres / 2;
	(*render)->Xsp[1][0] = 0.0;
	(*render)->Xsp[1][1] = -((*render)->display->yres / 2);
	(*render)->Xsp[1][2] = 0.0;
	(*render)->Xsp[1][3] = (*render)->display->yres / 2;
	(*render)->Xsp[2][0] = 0.0;
	(*render)->Xsp[2][1] = 0.0;
	//(*render)->Xsp[2][2] = INT_MAX * tan(((*render)->camera.FOV / 2.0) * (PI / 180.0)); // bounded z buffer range : [0,MAXINT] 
	(*render)->Xsp[2][3] = 0.0;
	(*render)->Xsp[3][0] = 0.0;
	(*render)->Xsp[3][1] = 0.0;
	(*render)->Xsp[3][2] = 0.0;
	(*render)->Xsp[3][3] = 1.0;

	// Setting up camera defaults
	(*render)->camera.position[X] = DEFAULT_IM_X;
	(*render)->camera.position[Y] = DEFAULT_IM_Y;
	(*render)->camera.position[Z] = DEFAULT_IM_Z;

	(*render)->camera.lookat[X] = 0.0;
	(*render)->camera.lookat[Y] = 0.0;
	(*render)->camera.lookat[Z] = 0.0;

	(*render)->camera.worldup[X] = 0.0;
	(*render)->camera.worldup[Y] = 1.0;
	(*render)->camera.worldup[Z] = 0.0;

	(*render)->camera.FOV = DEFAULT_FOV;
	(*render)->numlights = 0;
	(*render)->Xsp[2][2] = INT_MAX * tan(((*render)->camera.FOV / 2.0) * (PI / 180.0));
	return GZ_SUCCESS;

}


int GzFreeRender(GzRender *render)
{
	/*
	-free all renderer resources
	*/
	if (!render)
		return GZ_FAILURE;
	GzFreeDisplay(render->display);
	for (int i = 0; i < 6; ++i) {
		GzFreeDisplay(render->newAAdisplay[i]);
	}

	/*GzFreeDisplay(render->display);
	delete render->Ximage;
	delete render->Xsp;
	delete render->camera.Xpi;
	delete render->camera.lookat;
	delete render->camera.position;
	delete render->camera.worldup;
	delete render->camera.Xiw;*/
	//render->display = NULL; 
	delete render;
	return GZ_SUCCESS;
}


int GzBeginRender(GzRender *render)
{
	/*
	- setup for start of each frame - init frame buffer color,alpha,z
	- compute Xiw and projection xform Xpi from camera definition
	- init Ximage - put Xsp at base of stack, push on Xpi and Xiw
	- now stack contains Xsw and app can push model Xforms when needed
	*/
	if (!render)
		GZ_FAILURE;

	GzInitDisplay(render->display);

	if (render->display == NULL)
		return GZ_FAILURE;

	GzMatrix identityMatrix =
	{ 1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0 };

	//Compute Xiw
	GzCoord camZ; //Camera Z axis
	camZ[X] = render->camera.lookat[X] - render->camera.position[X];
	camZ[Y] = render->camera.lookat[Y] - render->camera.position[Y];
	camZ[Z] = render->camera.lookat[Z] - render->camera.position[Z];
	normalizeVector(camZ);

	GzCoord camY; //Camera Y axis
	float updotZ = render->camera.worldup[X] * camZ[X] + render->camera.worldup[Y] * camZ[Y] + render->camera.worldup[Z] * camZ[Z];
	camY[X] = render->camera.worldup[X] - updotZ * camZ[X];
	camY[Y] = render->camera.worldup[Y] - updotZ * camZ[Y];
	camY[Z] = render->camera.worldup[Z] - updotZ * camZ[Z];
	normalizeVector(camY);

	GzCoord camX;//Camera X axis
	camX[X] = camY[Y] * camZ[Z] - camY[Z] * camZ[Y];
	camX[Y] = camY[Z] * camZ[X] - camY[X] * camZ[Z];
	camX[Z] = camY[X] * camZ[Y] - camY[Y] * camZ[X];
	normalizeVector(camX);

	//Build Xiw
	render->camera.Xiw[0][0] = camX[X];
	render->camera.Xiw[0][1] = camX[Y];
	render->camera.Xiw[0][2] = camX[Z];
	render->camera.Xiw[0][3] = -(camX[X] * render->camera.position[X] + camX[Y] * render->camera.position[Y] + camX[Z] * render->camera.position[Z]);

	render->camera.Xiw[1][0] = camY[X];
	render->camera.Xiw[1][1] = camY[Y];
	render->camera.Xiw[1][2] = camY[Z];
	render->camera.Xiw[1][3] = -(camY[X] * render->camera.position[X] + camY[Y] * render->camera.position[Y] + camY[Z] * render->camera.position[Z]);

	render->camera.Xiw[2][0] = camZ[X];
	render->camera.Xiw[2][1] = camZ[Y];
	render->camera.Xiw[2][2] = camZ[Z];
	render->camera.Xiw[2][3] = -(camZ[X] * render->camera.position[X] + camZ[Y] * render->camera.position[Y] + camZ[Z] * render->camera.position[Z]);

	render->camera.Xiw[3][0] = 0.0;
	render->camera.Xiw[3][1] = 0.0;
	render->camera.Xiw[3][2] = 0.0;
	render->camera.Xiw[3][3] = 1.0;

	//Build Xpi
	float radian = (render->camera.FOV / 2.0) * (PI / 180.0);

	render->camera.Xpi[0][0] = 1.0;
	render->camera.Xpi[0][1] = 0.0;
	render->camera.Xpi[0][2] = 0.0;
	render->camera.Xpi[0][3] = 0.0;

	render->camera.Xpi[1][0] = 0.0;
	render->camera.Xpi[1][1] = 1.0;
	render->camera.Xpi[1][2] = 0.0;
	render->camera.Xpi[1][3] = 0.0;

	render->camera.Xpi[2][0] = 0.0;
	render->camera.Xpi[2][1] = 0.0;
	render->camera.Xpi[2][2] = 1.0; //This wasted 1.5 hours! :(
	render->camera.Xpi[2][3] = 0.0;

	render->camera.Xpi[3][0] = 0.0;
	render->camera.Xpi[3][1] = 0.0;
	render->camera.Xpi[3][2] = tan(radian);
	render->camera.Xpi[3][3] = 1.0;

	render->matlevel = -1;
	GzPushMatrix(render, render->Xsp);
	newMatrixPush(render, identityMatrix);
	GzPushMatrix(render, render->camera.Xpi);
	newMatrixPush(render, identityMatrix);
	GzPushMatrix(render, render->camera.Xiw);

	return GZ_SUCCESS;
}

int GzPutCamera(GzRender *render, GzCamera *camera)
{
	/*
	- overwrite renderer camera structure with new camera definition
	*/
	if (!render || !camera)
		return GZ_FAILURE;
	render->camera.FOV = camera->FOV;
	render->camera.lookat[X] = camera->lookat[X];
	render->camera.lookat[Y] = camera->lookat[Y];
	render->camera.lookat[Z] = camera->lookat[Z];
	render->camera.position[X] = camera->position[X];
	render->camera.position[Y] = camera->position[Y];
	render->camera.position[Z] = camera->position[Z];
	render->camera.worldup[X] = camera->worldup[X];
	render->camera.worldup[Y] = camera->worldup[Y];
	render->camera.worldup[Z] = camera->worldup[Z];
	normalizeVector(render->camera.worldup);

	render->Xsp[2][2] = INT_MAX * tan((render->camera.FOV / 2.0) * (PI / 180.0));
	return GZ_SUCCESS;
}

int GzPushMatrix(GzRender *render, GzMatrix	matrix)
{
	/*
	- push a matrix onto the Ximage stack
	- check for stack overflow
	*/
	if (!render || !matrix)
		return GZ_FAILURE;

	render->matlevel = render->matlevel + 1;
	if (render->matlevel >= MATLEVELS) {
		return GZ_FAILURE;
	}
	/*
	if (render->matlevel == -1)
	{
	memcpy(render->Ximage[render->matlevel+1], matrix, sizeof(GzMatrix));
	}
	else{
	GzMatrix MatrixProduct;
	matrixmul(render->Ximage[render->matlevel], matrix, MatrixProduct);
	memcpy(render->Ximage[render->matlevel + 1], MatrixProduct, sizeof(GzMatrix));
	}
	render->matlevel++;*/

	memcpy(render->Ximage[render->matlevel], matrix, sizeof(GzMatrix));
	newMatrixPush(render, matrix);
	return GZ_SUCCESS;
}

int GzPopMatrix(GzRender *render)
{
	/*
	- pop a matrix off the Ximage stack
	- check for stack underflow
	*/
	// underflow, empty stack!
	if (render->Ximage[0] == NULL) {
		return GZ_FAILURE;
	}
	if (render->matlevel < 0) {
		return GZ_FAILURE;
	}

	render->matlevel--;
	return GZ_SUCCESS;
}


int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList,
	GzPointer *valueList) /* void** valuelist */
{
	/*
	- set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
	- later set shaders, interpolaters, texture maps, and lights
	*/

	if (!render || !nameList || !valueList)
		return GZ_FAILURE;

	for (int i = 0; i < numAttributes; ++i)
	{
		if (nameList[i] == GZ_RGB_COLOR){

			GzColor* clr_ptr = (GzColor*)valueList[i];
			render->flatcolor[RED] = (*clr_ptr)[RED];
			render->flatcolor[GREEN] = (*clr_ptr)[GREEN];
			render->flatcolor[BLUE] = (*clr_ptr)[BLUE];
		}
		else if (nameList[i] == GZ_DIRECTIONAL_LIGHT){
			GzLight* directionalLight = (GzLight*)valueList[i];
			render->lights[render->numlights] = *directionalLight;
			normalizeVector(render->lights[render->numlights].direction);
			render->numlights += 1;
		}
		else if (nameList[i] == GZ_AMBIENT_LIGHT) {
			GzLight* ambLight = (GzLight*)valueList[i];
			render->ambientlight = *ambLight;
		}
		else if (nameList[i] == GZ_DIFFUSE_COEFFICIENT) {
			GzColor* diffuseColor = (GzColor*)valueList[i];
			render->Kd[RED] = (*diffuseColor)[0];
			render->Kd[GREEN] = (*diffuseColor)[1];
			render->Kd[BLUE] = (*diffuseColor)[2];
		}
		else if (nameList[i] == GZ_INTERPOLATE) {
			int* intrpltMode = (int*)valueList[i];
			render->interp_mode = *intrpltMode;
		}
		else if (nameList[i] == GZ_AMBIENT_COEFFICIENT) {
			GzColor* ambColor = (GzColor*)valueList[i];
			render->Ka[RED] = (*ambColor)[0];
			render->Ka[GREEN] = (*ambColor)[1];
			render->Ka[BLUE] = (*ambColor)[2];
		}
		else if (nameList[i] == GZ_SPECULAR_COEFFICIENT) {
			GzColor* specColor = (GzColor*)valueList[i];
			render->Ks[RED] = (*specColor)[0];
			render->Ks[GREEN] = (*specColor)[1];
			render->Ks[BLUE] = (*specColor)[2];
		}
		else if (nameList[i] == GZ_DISTRIBUTION_COEFFICIENT) {
			float* specPower = (float*)valueList[i];
			render->spec = *specPower;
		}
		else if (nameList[i] == GZ_TEXTURE_MAP) {
			render->tex_fun = (GzTexture)valueList[i];
		}
		else if (nameList[i] == GZ_AASHIFTX) {

			float* table = (float*)valueList[i];
			int tIdx = 0;
			for (int j = 0; j < 6; ++j) {
				render->filterGrid[j][X] = table[tIdx++];
				render->filterGrid[j][Y] = table[tIdx++];
				render->filterGrid[j][Z] = table[tIdx++];
			}
		}
	}

	return GZ_SUCCESS;
}

int GzPutTriangle(GzRender	*render, int numParts, GzToken *nameList,
	GzPointer	*valueList)
	/* numParts : how many names and values */
{
	/*
	- pass in a triangle description with tokens and values corresponding to
	GZ_POSITION:3 vert positions in model space
	- Xform positions of verts
	- Clip - just discard any triangle with verts behind view plane // ALL OF THEM? or any one of them?
	- test for triangles with all three verts off-screen
	- invoke triangle rasterizer
	*/

	if (!render || !nameList || !valueList)
		return GZ_FAILURE;

	// get Xsm
	if (render->matlevel > 0)
	{
		GzMatrix Xsm, temp, XNormalMatrix;
		memcpy(Xsm, render->Ximage[0], sizeof(GzMatrix)); // copy first one 
		//copyMatrix(Xsm, render->Ximage[0]);
		memcpy(XNormalMatrix, render->Xnorm[0], sizeof(GzMatrix));

		for (int i = 1; i <= render->matlevel; ++i)
		{
			memcpy(temp, Xsm, sizeof(GzMatrix));
			//copyMatrix(temp, Xsm);
			Xsm[0][0] = temp[0][0] * render->Ximage[i][0][0] + temp[0][1] * render->Ximage[i][1][0]
				+ temp[0][2] * render->Ximage[i][2][0] + temp[0][3] * render->Ximage[i][3][0];
			Xsm[0][1] = temp[0][0] * render->Ximage[i][0][1] + temp[0][1] * render->Ximage[i][1][1]
				+ temp[0][2] * render->Ximage[i][2][1] + temp[0][3] * render->Ximage[i][3][1];
			Xsm[0][2] = temp[0][0] * render->Ximage[i][0][2] + temp[0][1] * render->Ximage[i][1][2]
				+ temp[0][2] * render->Ximage[i][2][2] + temp[0][3] * render->Ximage[i][3][2];
			Xsm[0][3] = temp[0][0] * render->Ximage[i][0][3] + temp[0][1] * render->Ximage[i][1][3]
				+ temp[0][2] * render->Ximage[i][2][3] + temp[0][3] * render->Ximage[i][3][3];

			Xsm[1][0] = temp[1][0] * render->Ximage[i][0][0] + temp[1][1] * render->Ximage[i][1][0]
				+ temp[1][2] * render->Ximage[i][2][0] + temp[1][3] * render->Ximage[i][3][0];
			Xsm[1][1] = temp[1][0] * render->Ximage[i][0][1] + temp[1][1] * render->Ximage[i][1][1]
				+ temp[1][2] * render->Ximage[i][2][1] + temp[1][3] * render->Ximage[i][3][1];
			Xsm[1][2] = temp[1][0] * render->Ximage[i][0][2] + temp[1][1] * render->Ximage[i][1][2]
				+ temp[1][2] * render->Ximage[i][2][2] + temp[1][3] * render->Ximage[i][3][2];
			Xsm[1][3] = temp[1][0] * render->Ximage[i][0][3] + temp[1][1] * render->Ximage[i][1][3]
				+ temp[1][2] * render->Ximage[i][2][3] + temp[1][3] * render->Ximage[i][3][3];

			Xsm[2][0] = temp[2][0] * render->Ximage[i][0][0] + temp[2][1] * render->Ximage[i][1][0]
				+ temp[2][2] * render->Ximage[i][2][0] + temp[2][3] * render->Ximage[i][3][0];
			Xsm[2][1] = temp[2][0] * render->Ximage[i][0][1] + temp[2][1] * render->Ximage[i][1][1]
				+ temp[2][2] * render->Ximage[i][2][1] + temp[2][3] * render->Ximage[i][3][1];
			Xsm[2][2] = temp[2][0] * render->Ximage[i][0][2] + temp[2][1] * render->Ximage[i][1][2]
				+ temp[2][2] * render->Ximage[i][2][2] + temp[2][3] * render->Ximage[i][3][2];
			Xsm[2][3] = temp[2][0] * render->Ximage[i][0][3] + temp[2][1] * render->Ximage[i][1][3]
				+ temp[2][2] * render->Ximage[i][2][3] + temp[2][3] * render->Ximage[i][3][3];

			Xsm[3][0] = temp[3][0] * render->Ximage[i][0][0] + temp[3][1] * render->Ximage[i][1][0]
				+ temp[3][2] * render->Ximage[i][2][0] + temp[3][3] * render->Ximage[i][3][0];
			Xsm[3][1] = temp[3][0] * render->Ximage[i][0][1] + temp[3][1] * render->Ximage[i][1][1]
				+ temp[3][2] * render->Ximage[i][2][1] + temp[3][3] * render->Ximage[i][3][1];
			Xsm[3][2] = temp[3][0] * render->Ximage[i][0][2] + temp[3][1] * render->Ximage[i][1][2]
				+ temp[3][2] * render->Ximage[i][2][2] + temp[3][3] * render->Ximage[i][3][2];
			Xsm[3][3] = temp[3][0] * render->Ximage[i][0][3] + temp[3][1] * render->Ximage[i][1][3]
				+ temp[3][2] * render->Ximage[i][2][3] + temp[3][3] * render->Ximage[i][3][3];



			/*for (int x = 1; x <= render->matlevel; ++x){

			for (int i = 0; i <= 3; i++){
			for (int j = 0; j <= 3; j++){


			Xsm[i][j] = temp[i][0] * render->Ximage[x][0][j]
			+ temp[i][1] * render->Ximage[x][1][j]
			+ temp[i][2] * render->Ximage[x][2][j]
			+ temp[i][3] * render->Ximage[x][3][j];

			memcpy(temp, XNormalMatrix, sizeof(GzMatrix));

			XNormalMatrix[i][j] = temp[i][0] * render->Xnorm[x][0][j]
			+ temp[i][1] * render->Xnorm[x][1][j]
			+ temp[i][2] * render->Xnorm[x][2][j]
			+ temp[i][3] * render->Xnorm[x][3][j];
			}
			}
			}

			*/
			memcpy(temp, XNormalMatrix, sizeof(GzMatrix));
			XNormalMatrix[0][0] = temp[0][0] * render->Xnorm[i][0][0] + temp[0][1] * render->Xnorm[i][1][0]
				+ temp[0][2] * render->Xnorm[i][2][0] + temp[0][3] * render->Xnorm[i][3][0];
			XNormalMatrix[0][1] = temp[0][0] * render->Xnorm[i][0][1] + temp[0][1] * render->Xnorm[i][1][1]
				+ temp[0][2] * render->Xnorm[i][2][1] + temp[0][3] * render->Xnorm[i][3][1];
			XNormalMatrix[0][2] = temp[0][0] * render->Xnorm[i][0][2] + temp[0][1] * render->Xnorm[i][1][2]
				+ temp[0][2] * render->Xnorm[i][2][2] + temp[0][3] * render->Xnorm[i][3][2];
			XNormalMatrix[0][3] = temp[0][0] * render->Xnorm[i][0][3] + temp[0][1] * render->Xnorm[i][1][3]
				+ temp[0][2] * render->Xnorm[i][2][3] + temp[0][3] * render->Xnorm[i][3][3];

			XNormalMatrix[1][0] = temp[1][0] * render->Xnorm[i][0][0] + temp[1][1] * render->Xnorm[i][1][0]
				+ temp[1][2] * render->Xnorm[i][2][0] + temp[1][3] * render->Xnorm[i][3][0];
			XNormalMatrix[1][1] = temp[1][0] * render->Xnorm[i][0][1] + temp[1][1] * render->Xnorm[i][1][1]
				+ temp[1][2] * render->Xnorm[i][2][1] + temp[1][3] * render->Xnorm[i][3][1];
			XNormalMatrix[1][2] = temp[1][0] * render->Xnorm[i][0][2] + temp[1][1] * render->Xnorm[i][1][2]
				+ temp[1][2] * render->Xnorm[i][2][2] + temp[1][3] * render->Xnorm[i][3][2];
			XNormalMatrix[1][3] = temp[1][0] * render->Xnorm[i][0][3] + temp[1][1] * render->Xnorm[i][1][3]
				+ temp[1][2] * render->Xnorm[i][2][3] + temp[1][3] * render->Xnorm[i][3][3];

			XNormalMatrix[2][0] = temp[2][0] * render->Xnorm[i][0][0] + temp[2][1] * render->Xnorm[i][1][0]
				+ temp[2][2] * render->Xnorm[i][2][0] + temp[2][3] * render->Xnorm[i][3][0];
			XNormalMatrix[2][1] = temp[2][0] * render->Xnorm[i][0][1] + temp[2][1] * render->Xnorm[i][1][1]
				+ temp[2][2] * render->Xnorm[i][2][1] + temp[2][3] * render->Xnorm[i][3][1];
			XNormalMatrix[2][2] = temp[2][0] * render->Xnorm[i][0][2] + temp[2][1] * render->Xnorm[i][1][2]
				+ temp[2][2] * render->Xnorm[i][2][2] + temp[2][3] * render->Xnorm[i][3][2];
			XNormalMatrix[2][3] = temp[2][0] * render->Xnorm[i][0][3] + temp[2][1] * render->Xnorm[i][1][3]
				+ temp[2][2] * render->Xnorm[i][2][3] + temp[2][3] * render->Xnorm[i][3][3];

			XNormalMatrix[3][0] = temp[3][0] * render->Xnorm[i][0][0] + temp[3][1] * render->Xnorm[i][1][0]
				+ temp[3][2] * render->Xnorm[i][2][0] + temp[3][3] * render->Xnorm[i][3][0];
			XNormalMatrix[3][1] = temp[3][0] * render->Xnorm[i][0][1] + temp[3][1] * render->Xnorm[i][1][1]
				+ temp[3][2] * render->Xnorm[i][2][1] + temp[3][3] * render->Xnorm[i][3][1];
			XNormalMatrix[3][2] = temp[3][0] * render->Xnorm[i][0][2] + temp[3][1] * render->Xnorm[i][1][2]
				+ temp[3][2] * render->Xnorm[i][2][2] + temp[3][3] * render->Xnorm[i][3][2];
			XNormalMatrix[3][3] = temp[3][0] * render->Xnorm[i][0][3] + temp[3][1] * render->Xnorm[i][1][3]
				+ temp[3][2] * render->Xnorm[i][2][3] + temp[3][3] * render->Xnorm[i][3][3];

		}

		// POP EVERYTHING
		for (int i = render->matlevel; i >= 0; --i)
		{
			GzPopMatrix(render);
		}
		// push on Xsm
		GzPushMatrix(render, Xsm);
		newMatrixPush(render, XNormalMatrix);
	}


	GzTextureIndex* textCoord = new GzTextureIndex[3];
	GzCoord* xformTri = new GzCoord[3];
	GzCoord* xformNormals = new GzCoord[3];
	GzMatrix MatrixTop, MatrixTopN;
	bool visible = true;
	int top = render->matlevel;
	memcpy(MatrixTop, render->Ximage[top], sizeof(GzMatrix));
	memcpy(MatrixTopN, render->Xnorm[top], sizeof(GzMatrix));
	float W;
	for (int i = 0; i < numParts; ++i)
	{

		if (nameList[i] == GZ_NULL_TOKEN)
		{
			continue;
		}
		if (nameList[i] == GZ_POSITION)
		{
			GzCoord* verts = (GzCoord*)valueList[i];
			for (int j = 0; j < 3; ++j)
			{
				// xform verticies

				xformTri[j][X] = MatrixTop[0][0] * verts[j][X] + MatrixTop[0][1] * verts[j][Y] + MatrixTop[0][2] * verts[j][Z]
					+ MatrixTop[0][3] * 1.0;
				xformTri[j][Y] = MatrixTop[1][0] * verts[j][X] + MatrixTop[1][1] * verts[j][Y] + MatrixTop[1][2] * verts[j][Z]
					+ MatrixTop[1][3] * 1.0;
				xformTri[j][Z] = MatrixTop[2][0] * verts[j][X] + MatrixTop[2][1] * verts[j][Y] + MatrixTop[2][2] * verts[j][Z]
					+ MatrixTop[2][3] * 1.0;
				W = MatrixTop[3][0] * verts[j][X] + MatrixTop[3][1] * verts[j][Y] + MatrixTop[3][2] * verts[j][Z]
					+ MatrixTop[3][3] * 1;
				xformTri[j][X] /= W;
				xformTri[j][Y] /= W;
				xformTri[j][Z] /= W;

				// check if any vertices are behind view plane
				if (xformTri[j][Z] < render->camera.position[Z])
				{
					visible = false;
					//break;
				}
			}
		}
		if (nameList[i] == GZ_NORMAL){
			GzCoord* Normals = (GzCoord*)valueList[i];
			normalizeVector(Normals[0]);
			normalizeVector(Normals[1]);
			normalizeVector(Normals[2]);

			for (int j = 0; j < 3; j++) {
				xformNormals[j][X] = MatrixTopN[0][0] * Normals[j][X] + MatrixTopN[0][1] * Normals[j][Y] + MatrixTopN[0][2] * Normals[j][Z];
				xformNormals[j][Y] = MatrixTopN[1][0] * Normals[j][X] + MatrixTopN[1][1] * Normals[j][Y] + MatrixTopN[1][2] * Normals[j][Z];
				xformNormals[j][Z] = MatrixTopN[2][0] * Normals[j][X] + MatrixTopN[2][1] * Normals[j][Y] + MatrixTopN[2][2] * Normals[j][Z];
			}
			normalizeVector(xformNormals[0]);
			normalizeVector(xformNormals[1]);
			normalizeVector(xformNormals[2]);
		}

		//get texture u,v coordinates
		if (nameList[i] == GZ_TEXTURE_INDEX) {
			GzTextureIndex* uv = (GzTextureIndex*)valueList[i];
			for (int k = 0; k < 3; ++k) {							//prespective correction
				float Vz = xformTri[k][Z] / (INT_MAX - xformTri[k][Z]);
				textCoord[k][0] = uv[k][0] / (Vz + 1);
				textCoord[k][1] = uv[k][1] / (Vz + 1);
			}
		}
	}

	int leftX = 0, rightX = 0;
	int topY = 0, bottomY = 0;

	// Rasterization
	
	if (visible == true)
	{

		// sort verts by Y
		int minY = 0;
		for (int i = 0; i < 2; ++i)
		{
			minY = i;
			for (int j = i + 1; j < 3; ++j)
			{
				if (xformTri[minY][Y] > xformTri[j][Y])
				{
					minY = j;
				}
			}
			// swapping
			if (minY != i)
			{
				float tempU, tempV;
				float tempX = xformTri[i][X];
				float tempY = xformTri[i][Y];
				float tempZ = xformTri[i][Z];
				float tempNX = xformNormals[i][X];
				float tempNY = xformNormals[i][Y];
				float tempNZ = xformNormals[i][Z];

				if (render->tex_fun != NULL) {
					tempU = textCoord[i][0];
					tempV = textCoord[i][1];
				}

				xformTri[i][X] = xformTri[minY][X];
				xformTri[i][Y] = xformTri[minY][Y];
				xformTri[i][Z] = xformTri[minY][Z];
				xformNormals[i][X] = xformNormals[minY][X];
				xformNormals[i][Y] = xformNormals[minY][Y];
				xformNormals[i][Z] = xformNormals[minY][Z];

				if (render->tex_fun != NULL) {
					textCoord[i][0] = textCoord[minY][0];
					textCoord[i][1] = textCoord[minY][1];
				}
				xformTri[minY][X] = tempX;
				xformTri[minY][Y] = tempY;
				xformTri[minY][Z] = tempZ;
				xformNormals[minY][X] = tempNX;
				xformNormals[minY][Y] = tempNY;
				xformNormals[minY][Z] = tempNZ;

				if (render->tex_fun != NULL) {
					textCoord[minY][0] = tempU;
					textCoord[minY][1] = tempV;
				}
			}
		}

		// Offset here!
		//int k = 0;
		for (int k = 0; k < 6; ++k) {
			GzCoord offsetVerts[3];
			float offsetX = render->filterGrid[k][X];
			float offsetY = render->filterGrid[k][Y];
			offsetVerts[0][X] = xformTri[0][X] - offsetX;
			offsetVerts[1][X] = xformTri[1][X] - offsetX;
			offsetVerts[2][X] = xformTri[2][X] - offsetX;
			offsetVerts[0][Y] = xformTri[0][Y] - offsetY;
			offsetVerts[1][Y] = xformTri[1][Y] - offsetY;
			offsetVerts[2][Y] = xformTri[2][Y] - offsetY;
			offsetVerts[0][Z] = xformTri[0][Z];
			offsetVerts[1][Z] = xformTri[1][Z];
			offsetVerts[2][Z] = xformTri[2][Z];

			// Get bounding box
			
			topY = floor(offsetVerts[0][Y]);
			bottomY = ceil(offsetVerts[2][Y]);

			// 0 comes before 1
			if (offsetVerts[0][X] < offsetVerts[1][X]) {
				// 201
				if (offsetVerts[2][X] < offsetVerts[0][X]) {
					leftX = floor(offsetVerts[2][X]);
					rightX = ceil(offsetVerts[1][X]);
				}
				else { //0
					leftX = floor(offsetVerts[0][X]);
					// 021
					if (offsetVerts[2][X] < offsetVerts[1][X]) {
						rightX = ceil(offsetVerts[1][X]);
					}
					else { // 012
						rightX = ceil(offsetVerts[2][X]);
					}
				}
			}
			else { // 1 comes before 0
				//210
				if (offsetVerts[2][X] < offsetVerts[1][X]) {
					leftX = floor(offsetVerts[2][X]);
					rightX = ceil(offsetVerts[0][X]);
				}
				else { //1
					leftX = floor(offsetVerts[1][X]);
					// 120
					if (offsetVerts[2][X] < offsetVerts[0][X]) {
						rightX = ceil(offsetVerts[0][X]);
					}
					else { // 102
						rightX = ceil(offsetVerts[2][X]);
					}
				}
			}
			// For interpolating Z
			// Ax + By + Cz + D = 0;
			// xformTri[0] x xformTri[1] = (A, B, C) (For finding normal to the vector)
			//GzCoord edge01;
			//edge01[X] = xformTri[1][X] - xformTri[0][X];
			//edge01[Y] = xformTri[1][Y] - xformTri[0][Y];
			//edge01[Z] = xformTri[1][Z] - xformTri[0][Z];
			//GzCoord edge12;
			//edge12[X] = xformTri[2][X] - xformTri[1][X];
			//edge12[Y] = xformTri[2][Y] - xformTri[1][Y];
			//edge12[Z] = xformTri[2][Z] - xformTri[1][Z];

			//float A = edge01[Y] * edge12[Z] - edge01[Z] * edge12[Y];
			//float B = edge01[Z] * edge12[X] - edge01[X] * edge12[Z];
			//float C = edge01[X] * edge12[Y] - edge01[Y] * edge12[X];
			//// get D
			//float D = -(A*xformTri[0][X]) - (B*xformTri[0][Y]) - (C*xformTri[0][Z]);
			
			
			//GOURAUD SHADING-----------------------------------
			GzColor colorR, colorG, colorB;
			if (render->interp_mode == GZ_COLOR) {
				if (render->tex_fun == NULL) {
					shadingEquation(render, colorR, xformNormals[0]);
					shadingEquation(render, colorG, xformNormals[1]);
					shadingEquation(render, colorB, xformNormals[2]);
				}
				else {
					gouraudTextureShadingEquation(render, colorR, xformNormals[0]);
					gouraudTextureShadingEquation(render, colorG, xformNormals[1]);
					gouraudTextureShadingEquation(render, colorB, xformNormals[2]);
				}
			}

			//Total area of triangle for interpolation
			float fullTri = AreaOfTriangle(offsetVerts[0], offsetVerts[1], offsetVerts[2]);
			

			// for pixels in this bounding box
			float intrpltZ;
			for (int i = leftX; i < rightX; ++i) //bounds check
			{

				if (i < 0 || i > render->display->xres)
				{
					continue;
				}
				for (int j = topY; j < bottomY; ++j)
				{

					if (j < 0 || j > render->display->yres)
					{
						continue;
					}

					// Compute LEE rasterization coordinates
					// E(x, y) = dY(x-X) - dX(y-Y)
					// EDGE 0-1
					float e01 = (offsetVerts[1][Y] - offsetVerts[0][Y])*((float)i - offsetVerts[0][X])
						- (offsetVerts[1][X] - offsetVerts[0][X])*((float)j - offsetVerts[0][Y]);
					// EDGE 1-2
					float e12 = (offsetVerts[2][Y] - offsetVerts[1][Y])*((float)i - offsetVerts[1][X])
						- (offsetVerts[2][X] - offsetVerts[1][X])*((float)j - offsetVerts[1][Y]);
					// EDGE 2-0
					float e20 = (offsetVerts[0][Y] - offsetVerts[2][Y])*((float)i - offsetVerts[2][X])
						- (offsetVerts[0][X] - offsetVerts[2][X])*((float)j - offsetVerts[2][Y]);

					if (e01 == 0 || e12 == 0 || e20 == 0 ||
						((e01 > 0) && (e12 > 0) && (e20 > 0)) ||
						((e01 < 0) && (e12 < 0) && (e20 < 0))) {
						// Interpolate Z value
						//intrpltZ = (-(A*i) - (B*j) - D) / C;

						// get current z at this pixel
						GzCoord currentZ = { i, j, 1 };
						GzIntensity r, g, b, a;
						GzDepth z = 0;

						//Calculate areas of inner triangles
						float A0 = AreaOfTriangle(offsetVerts[1], currentZ, offsetVerts[2]);
						float A1 = AreaOfTriangle(offsetVerts[0], currentZ, offsetVerts[2]);
						float A2 = AreaOfTriangle(offsetVerts[0], currentZ, offsetVerts[1]);

						// Interpolate Z for this point
						intrpltZ = (A0*offsetVerts[0][Z] + A1*offsetVerts[1][Z] + A2*offsetVerts[2][Z]) / fullTri;

						// get current values at this point
						GzGetDisplay(render->newAAdisplay[k], i, j, &r, &g, &b, &a, &z);
						//GzGetDisplay(render->display, i, j, &r, &g, &b, &a, &z);
						// compare, if intrpltZ less than draw over
						if (intrpltZ < z) {

							//Interpolate UV coordinates
							GzTextureIndex UV, uv;
							GzColor textureColor;
							if (render->tex_fun != NULL) {
							UV[0] = (A0*textCoord[0][0] + A1*textCoord[1][0] + A2*textCoord[2][0]) / fullTri;
							UV[1] = (A0*textCoord[0][1] + A1*textCoord[1][1] + A2*textCoord[2][1]) / fullTri;

							float Vz = intrpltZ / (INT_MAX - intrpltZ);
							uv[0] = UV[0] * (Vz + 1);
							uv[1] = UV[1] * (Vz + 1);

							//lookup texture color at this pixel
					
							render->tex_fun(uv[0], uv[1], textureColor);
							}

						//GOURAUD SHADING
						if (render->interp_mode == GZ_COLOR) {
							float r1 = (A0*colorR[0] + A1*colorG[0] + A2*colorB[0]) / fullTri;
							float g1 = (A0*colorR[1] + A1*colorG[1] + A2*colorB[1]) / fullTri;
							float b1 = (A0*colorR[2] + A1*colorG[2] + A2*colorB[2]) / fullTri;
							if (render->tex_fun != NULL) {
								r1 *= textureColor[RED];
								g1 *= textureColor[GREEN];
								b1 *= textureColor[BLUE];
							}

							if (r1 > 1.0)
								r1 = 1.0;
							if (g1 > 1.0)
								g1 = 1.0;
							if (b1 > 1.0)
								b1 = 1.0;

							r = (GzIntensity)ctoi(r1);
							g = (GzIntensity)ctoi(g1);
							b = (GzIntensity)ctoi(b1);
							z = intrpltZ;
						}
						//PHONG SHADING
							else if (render->interp_mode == GZ_NORMALS) {

								// interpolate Normal of this point
								GzCoord pointN;
								pointN[X] = (A0*xformNormals[0][X] + A1*xformNormals[1][X] + A2*xformNormals[2][X]) / fullTri;
								pointN[Y] = (A0*xformNormals[0][Y] + A1*xformNormals[1][Y] + A2*xformNormals[2][Y]) / fullTri;
								pointN[Z] = (A0*xformNormals[0][Z] + A1*xformNormals[1][Z] + A2*xformNormals[2][Z]) / fullTri;
								normalizeVector(pointN);

								//To apply texture
								if (render->tex_fun != NULL) {
									render->Ka[RED] = render->Kd[RED] = textureColor[RED];
									render->Ka[GREEN] = render->Kd[GREEN] = textureColor[GREEN];
									render->Ka[BLUE] = render->Kd[BLUE] = textureColor[BLUE];
								}

								GzColor teapotColor;
								shadingEquation(render, teapotColor, pointN);
								if (teapotColor[RED] > 1.0) teapotColor[RED] = 1.0;
								if (teapotColor[GREEN] > 1.0) teapotColor[GREEN] = 1.0;
								if (teapotColor[BLUE] > 1.0) teapotColor[BLUE] = 1.0;

								r = (GzIntensity)ctoi(teapotColor[0]);
								g = (GzIntensity)ctoi(teapotColor[1]);
								b = (GzIntensity)ctoi(teapotColor[2]);
								z = intrpltZ;
							}
							//FLAT SHADING
							else {
								r = (GzIntensity)ctoi((float)render->flatcolor[0]);
								g = (GzIntensity)ctoi((float)render->flatcolor[1]);
								b = (GzIntensity)ctoi((float)render->flatcolor[2]);
								z = intrpltZ;
							}
							// anti aliasing
							if (render->filterGrid != NULL) {
							}
							// write to buffer
							GzPutDisplay(render->newAAdisplay[k], i, j, r, g, b, a, z);
							//GzPutDisplay(render->display, i, j, r, g, b, a, z);
						}
					}

				}
			}
		}
		// Combine displays here
		for (int i = leftX; i < rightX; ++i) {
			// bounds check
			if (i < 0 || i > render->display->xres) {
				continue;
			}
			for (int j = topY; j < bottomY; ++j) {
				//// bounds check
				if (j < 0 || j > render->display->yres) {
					continue;
				}
				GzIntensity red[6], green[6], blue[6], alpha[6];
				GzDepth zdepth[6];
				GzIntensity r, g, b, a;
				GzDepth z = 0;
				for (int k = 0; k < 6; ++k)
					GzGetDisplay(render->newAAdisplay[k], i, j, &red[k], &green[k], &blue[k], &alpha[k], &zdepth[k]);
				//GzGetDisplay(render->display, i, j,  &r, &g, &b, &a, &z);
				r = render->filterGrid[0][2] * red[0] + render->filterGrid[1][2] * red[1] +
					render->filterGrid[2][2] * red[2] + render->filterGrid[3][2] * red[3] +
					render->filterGrid[4][2] * red[4] + render->filterGrid[5][2] * red[5];
				g = render->filterGrid[0][2] * green[0] + render->filterGrid[1][2] * green[1] +
					render->filterGrid[2][2] * green[2] + render->filterGrid[3][2] * green[3] +
					render->filterGrid[4][2] * green[4] + render->filterGrid[5][2] * green[5];;
				b = render->filterGrid[0][2] * blue[0] + render->filterGrid[1][2] * blue[1] +
					render->filterGrid[2][2] * blue[2] + render->filterGrid[3][2] * blue[3] +
					render->filterGrid[4][2] * blue[4] + render->filterGrid[5][2] * blue[5];;
				a = render->filterGrid[0][2] * alpha[0] + render->filterGrid[1][2] * alpha[1] +
					render->filterGrid[2][2] * alpha[2] + render->filterGrid[3][2] * alpha[3] +
					render->filterGrid[4][2] * alpha[4] + render->filterGrid[5][2] * alpha[5];;
				GzPutDisplay(render->display, i, j, r, g, b, a, z);
			}
		}
	}

	return GZ_SUCCESS;
}

float AreaOfTriangle(GzCoord v0, GzCoord v1, GzCoord v2) {
	return abs(0.5 * (v0[X] * v1[Y] + v0[Y] * v2[X] + v1[X] * v2[Y] - v1[Y] * v2[X] - v0[Y] * v1[X] - v0[X] * v2[Y]));
}

/* NOT part of API - just for general assistance */

short ctoi(float color)		/* convert float color to gzintensity short */
{
	return(short)((int)(color * ((1 << 12) - 1)));
}

