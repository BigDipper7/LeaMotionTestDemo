#include "stdafx.h"
#include "DisplayGL.h"
CRITICAL_SECTION g_csPiano;
CRITICAL_SECTION g_csStick;
GLuint			texture[3];   
static int		g_PressWhichKey = -1;
vector<SStick>	g_StickVector;
CPianoMelody	*g_pianoMelody;
AUX_RGBImageRec *LoadImage(char *Filename)  // loading a image   
{   
	FILE *File = NULL;                      // set file handler   
	if(!Filename)                          
	{   
		return NULL;                        
	}   
	File=fopen(Filename,"r");               // open file

	if(File)                               // weather the file exist?   
	{   
		fclose(File);                       // close file handler
		return auxDIBImageLoad(Filename);   // return image
	}   
	return NULL;                                
}   
BOOL LoadTextureGL()                                            // create texture  
{   
	BOOL State=FALSE;                                           // statement label  

	AUX_RGBImageRec *TextureImage[MAX_TEXTURE];                  // get space for texture

	memset(TextureImage,0,sizeof(void *)*MAX_TEXTURE);           //  clear all temp data ,change it to->NULL   
	//laoding source image ...-> return flase while it is not exist      
	if ((TextureImage[0] = LoadImage("Resources/white.bmp"))   
		&& (TextureImage[1] = LoadImage("Resources/black.bmp")) 
		&& (TextureImage[2] = LoadImage("Resources/piano.bmp")))
	{      
		State = TRUE;                                           // change label to -> TRUE   
		glGenTextures(MAX_TEXTURE, &texture[0]);                 

		// generating texture
		for (int loop=0; loop<MAX_TEXTURE; loop++)                // this circle is used to processing n texture
		{   
			glBindTexture(GL_TEXTURE_2D, texture[loop]);   
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[loop]->sizeX, TextureImage[loop]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[loop]->data);   
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   
		}   
	}   
	for (int loop=0; loop<MAX_TEXTURE; loop++)   
	{   
		if (TextureImage[loop])                  
		{   
			if (TextureImage[loop]->data)         
			{   
				free(TextureImage[loop]->data);  // free the used space
			}   
			free(TextureImage[loop]);            
		}   
	}   
	return State;                                // return label   
}   
// draw a cube with texture
void glDrawCube(GLfloat width, GLfloat height, GLfloat deep)   
{   
	glBegin(GL_QUADS);   
	// front   
	glNormal3f( 0.0f, 0.0f, 1.0f);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width/2,-height/2, deep/2);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f( width/2,-height/2, deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f( width/2, height/2, deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width/2, height/2, deep/2);   
	// behind
	glNormal3f( 0.0f, 0.0f,-1.0f);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width/2,-height/2,-deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width/2, height/2,-deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f( width/2, height/2,-deep/2);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f( width/2,-height/2,-deep/2);   
	// top 
	glNormal3f( 0.0f, 1.0f, 0.0f);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width/2, height/2,-deep/2);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width/2, height/2, deep/2);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f( width/2, height/2, deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f( width/2, height/2,-deep/2);   
	// bottom   
	glNormal3f( 0.0f,-1.0f, 0.0f);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width/2,-height/2,-deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f( width/2,-height/2,-deep/2);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f( width/2,-height/2, deep/2);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width/2,-height/2, deep/2);   
	// right 
	glNormal3f( 1.0f, 0.0f, 0.0f);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f( width/2,-height/2,-deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f( width/2, height/2,-deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f( width/2, height/2, deep/2);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f( width/2,-height/2, deep/2);   
	// left  
	glNormal3f(-1.0f, 0.0f, 0.0f);   
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width/2,-height/2,-deep/2);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width/2,-height/2, deep/2);   
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width/2, height/2, deep/2);   
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width/2, height/2,-deep/2);   
	glEnd();   
}   

void DrawGLScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//  clear the cache 
	glLoadIdentity();                                   // reset current matrix
	glTranslatef(0.0f, 0.5f,-2.0f);//set the beginning coordinates system
	glRotatef(30, 1.0f, 0.0f, 0.0f);
	//glRotatef(-20, 0.0f, 1.0f, 0.0f);
	//firstly draw the n fingers  g_StickVector
	SStick *sTemp;
	double factor = 200.0;
	
	GLUquadricObj *qobj = gluNewQuadric();
	EnterCriticalSection(&g_csStick);
	for(int i=0;i<g_StickVector.size();++i)//drawing fingers
	{
		glColor3f(0,1,0);
		sTemp = &g_StickVector[i];

		sTemp->start_x /= factor*1.2;
		sTemp->start_y /= factor*1.5;
		sTemp->start_z /= factor;
		sTemp->end_x /= factor*1.2;
		sTemp->end_y /= factor*1.5;
		sTemp->end_z /= factor;

		sTemp->start_x += 0.1;
		sTemp->end_x += 0.1;

		sTemp->start_z -= 0.2;
		sTemp->end_z -= 0.2;

		sTemp->start_y -= 0.7;
		sTemp->end_y -= 0.7;

		glPushMatrix();//record the current statement
		//initialize
		glLineWidth(3);
		glPointSize(10);

		//draw the lines between points
 		glBegin(GL_LINES);
 		glVertex3f(sTemp->start_x,sTemp->start_y,sTemp->start_z);
 		glVertex3f(sTemp->end_x,sTemp->end_y,sTemp->end_z);
 		glEnd();

		glPopMatrix();//return to the statement

		glColor3f(1,1,1);
		glPushMatrix();//record the current statement
		glTranslatef(sTemp->end_x,sTemp->end_y,sTemp->end_z);
		gluSphere(qobj,0.03,100,100);//draw a sphere to represent the finger
		glPopMatrix();//return to the statement
	}
	LeaveCriticalSection(&g_csStick);

	glColor3f(1,1,1);
	// now drawing the piano key; num: 13
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix();
	glTranslatef(-1.4f,-0.5f,0.0f);//change the  number  and  the beginning coordinate, to make it in the center of screen

	for (int i = 0; i < 13; i++)// use FOR circle to drawing 13 piano keys
	{
		// the i th piano key; color="white"   
		glTranslatef(0.2f, 0.0f, 0.0f);   // make the beginning coordinate system going right; distance =" 0.2f "
		glPushMatrix();   //record temperary current statement,
		if (i == g_PressWhichKey)//set current onTapListener ,bind it on the i th ke
			glTranslatef(0.0f, -0.04f, 0.0f);   //if true , set the key coordinate to make it go down ; distance ="0.04f"
		glDrawCube(0.2f, 0.1f, 0.8f);   // draw the cube 
		glPopMatrix();//return back to the previous statement 
	}
	
	//// 第一个白色按键   
	//glTranslatef(0.2f, 0.0f, 0.0f);   
	//glPushMatrix();   
	//if (0 == g_PressWhichKey)
	//	glTranslatef(0.0f, -0.04f, 0.0f);   
	//glDrawCube(0.2f, 0.1f, 0.8f);   
	//glPopMatrix();   

	//// 第二个白色按键   
	//glTranslatef(0.2f, 0.0f, 0.0f);   
	//glPushMatrix();   
	//if (1 == g_PressWhichKey)
	//{
	// 	glTranslatef(0.0f, -0.04f, 0.0f);
	//}
	//glDrawCube(0.2f, 0.1f, 0.8f);   
	//glPopMatrix();   

	//// 第三个白色按键   
	//glTranslatef(0.2f, 0.0f, 0.0f);   
	//glPushMatrix();   
	//if (2 == g_PressWhichKey)
	// 		glTranslatef(0.0f, -0.04f, 0.0f);   
	//glDrawCube(0.2f, 0.1f, 0.8f);   
	//glPopMatrix();   

	//// 第四个白色按键   
	//glTranslatef(0.2f, 0.0f, 0.0f);   
	//glPushMatrix();   
	//if (3 == g_PressWhichKey)
	// 		glTranslatef(0.0f, -0.04f, 0.0f);   
	//glDrawCube(0.2f, 0.1f, 0.8f);   
	//glPopMatrix();   

	//// 第五个白色按键   
	//glTranslatef(0.2f, 0.0f, 0.0f);   
	//glPushMatrix();   
	//if (4 == g_PressWhichKey)
	// 		glTranslatef(0.0f, -0.04f, 0.0f);   
	//glDrawCube(0.2f, 0.1f, 0.8f);   
	//glPopMatrix();   

	//// 第六个白色按键   
	//glTranslatef(0.2f, 0.0f, 0.0f);   
	//glPushMatrix();   
	//if (5 == g_PressWhichKey)
	// 		glTranslatef(0.0f, -0.04f, 0.0f);   
	//glDrawCube(0.2f, 0.1f, 0.8f);   
	//glPopMatrix();   

	//// 第七个白色按键   
	//glTranslatef(0.2f, 0.0f, 0.0f);   
	//glPushMatrix();   
	//if (6 == g_PressWhichKey)
	// 		glTranslatef(0.0f, -0.04f, 0.0f);   
	//glDrawCube(0.2f, 0.1f, 0.8f);   
	//glPopMatrix();   

	glPopMatrix(); //drawing white keys succeed , return to the statement before drawing

	// using FOR circle to draw black key
	glBindTexture(GL_TEXTURE_2D, texture[1]);   //change the bind texture
	glPushMatrix();   //record the beginning coordinate statement
	glTranslatef(-1.3f, -0.46f, -0.2f);   
	for (int loop=0; loop<12; loop++)   
	{   
		glTranslatef(0.2f, 0.0f, 0.0f);   //make key going right; distance="0.2f"
		glDrawCube(0.1f, 0.1f, 0.4f);   //drawing the black cube
	}   
	glPopMatrix();   //return to the current coordinate

	// drawing the frame of piano
	glBindTexture(GL_TEXTURE_2D, texture[2]);   
	/*glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.8f);//绘制的是顶部正方体
	glDrawCube(1.8f, 0.1f, 1.0f);
	glPopMatrix();*/

	/*glPushMatrix();   
	glTranslatef(0.0f, 0.0f, -0.8f);//顶部下面正方体
	glTranslatef(0.0f, -0.65f, 0.0f);   
	glDrawCube(1.6f, 1.2f, 0.8f);   
	glPopMatrix(); */  

	glPushMatrix();   //drawing the two side of the piano
	glTranslatef(-1.35f, -0.8f, 0.0f);   
	glDrawCube(0.1f, 0.9f, 0.8f);   
	glTranslatef(2.7f, 0.0f, 0.0f);   
	glDrawCube(0.1f,0.9f, 0.8f);   
	glPopMatrix();   

	/*glPushMatrix();   
	glTranslatef(0.0f, -1.3f, -0.4f);   
	glDrawCube(1.8f, 0.1f, 1.7f);   
	glPopMatrix();*/   

	glutSwapBuffers(); 
}
void ReSizeFunc(int width,int height)
{
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));              // reset current window size
	glMatrixMode(GL_PROJECTION);                                        // set model  GL_PROJECTION  
	glLoadIdentity();                                                   // reset the projection matrix
	gluPerspective(45, (float)width/(float)height, 0.1, 1000);           // reset  perspective projection 
	glMatrixMode(GL_MODELVIEW);                                         // set model GL_MODELVIEW
	glLoadIdentity();                                                   // reset the model-view matrix
}

int Init()
{
	InitializeCriticalSection(&g_csPiano);
	InitializeCriticalSection(&g_csStick);

	glClearColor(0,0,0,0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);//  start shade model 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	if (!LoadTextureGL())  // 跳到LoadTextureGL()函数   
	{   
		return FALSE;      // 如果纹理加载错误则返回FALSE   
	}   
	glEnable(GL_TEXTURE_2D);// 开启2D纹理映射   
	//抗锯齿
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
	return 1;
}
void KeyBoardFunc(unsigned char key, int x, int y)
{
//	printf("%x\n",key);
	if(VK_ESCAPE == key)
	{
		exit(0);
	}
	if(('A' == key)||('a' == key))
	{
		g_PressWhichKey = 0;
	}
	else if(('S' == key)||('s' == key))
	{
		g_PressWhichKey = 1;
	}
	else if(('D' == key)||('d' == key))
	{
		g_PressWhichKey = 2;
	}
	else if(('F' == key)||('f' == key))
	{
		g_PressWhichKey = 3;
	}
	else if(('G' == key)||('g' == key))
	{
		g_PressWhichKey = 4;
	}
	else if(('H' == key)||('h' == key))
	{
		g_PressWhichKey = 5;
	}
	else if(('J' == key)||('j' == key))
	{
		g_PressWhichKey = 6;
	}
	else if (('K' == key) || ('k' == key))
	{
		g_PressWhichKey = 7;
	}
	else if (('L' == key) || ('l' == key))
	{
		g_PressWhichKey = 8;
	}
	else if (('Z' == key) || ('z' == key))
	{
		g_PressWhichKey = 9;
	}
	else if (('X' == key) || ('x' == key))
	{
		g_PressWhichKey = 10;
	}
	else if (('C' == key) || ('c' == key))
	{
		g_PressWhichKey = 11;
	}
	else if (('V' == key) || ('v' == key))
	{
		g_PressWhichKey = 12;
	}
	EnterCriticalSection(&g_csPiano);
	g_pianoMelody->Play(g_PressWhichKey);
	LeaveCriticalSection(&g_csPiano);
	DrawGLScene();
}

bool SetKeyPress(int musicNum)
{
	if((musicNum >= MAX_KEY_NUM) || (musicNum < 0))
	{
		return false;
	}
	g_PressWhichKey = musicNum;
	EnterCriticalSection(&g_csPiano);
	g_pianoMelody->Play(musicNum);
	LeaveCriticalSection(&g_csPiano);
	DrawGLScene();
	return true;
}
