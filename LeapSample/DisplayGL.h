#pragma once
#include <iostream>
#include <vector>
#include <windows.h>

#include <minwinbase.h>

#define GLUT_DISABLE_ATEXIT_HACK
#include "GL/glaux.h" 
#include "GL/glut.h"
#include "CPianoMelody.h"
#define MAX_TEXTURE  3   //define Maximum texture number
#define MAX_KEY_NUM 13   //define Maximum piano key number
using namespace std;
//start is the center of palm£¬end is Fingertip
struct SStick
{
	float start_x;//palm X
	float start_y;//[alm Y
	float start_z;//palm Z
	float end_x;//Fingertip X
	float end_y;//Fingertip Y
	float end_z;//Fingertip Z
};
extern CRITICAL_SECTION g_csPiano;
extern CRITICAL_SECTION g_csStick;
extern vector<SStick> g_StickVector;
extern CPianoMelody	  *g_pianoMelody;
AUX_RGBImageRec *LoadImage(char *Filename);
BOOL	LoadTextureGL(); 
void	glDrawCube(GLfloat width, GLfloat height, GLfloat deep);
void	DrawGLScene(void);
void	ReSizeFunc(int width,int height);
int		Init();
void	KeyBoardFunc(unsigned char key, int x, int y);
bool	SetKeyPress(int musicNum);

