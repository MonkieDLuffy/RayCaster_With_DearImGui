

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include<cmath>
#define PI 3.1415926535


//-----------------------------MAP----------------------------------------------
#define mapX  8      //map width
#define mapY  8      //map height
#define mapS 64      //map cube size
int map[]=           //the map array. Edit to change level but keep the outer walls
{
 1,1,1,1,1,1,1,1,
 1,0,1,0,0,0,0,1,
 1,0,1,0,0,0,0,1,
 1,0,1,0,0,0,0,1,
 1,0,0,0,0,0,0,1,
 1,0,0,0,0,1,0,1,
 1,0,0,0,0,0,0,1,
 1,1,1,1,1,1,1,1,	
};

void drawMap2D()
{
 int x,y,xo,yo;
 for(y=0;y<mapY;y++)
 {
  for(x=0;x<mapX;x++)
  {
   if(map[y*mapX+x]==1){ glColor3f(1,1,1);} else{ glColor3f(0,0,0);}
   xo=x*mapS; yo=y*mapS;
   glBegin(GL_QUADS); 
   glVertex2i( 0   +xo+1, 0   +yo+1); 
   glVertex2i( 0   +xo+1, mapS+yo-1); 
   glVertex2i( mapS+xo-1, mapS+yo-1);  
   glVertex2i( mapS+xo-1, 0   +yo+1); 
   glEnd();
  } 
 } 
}//-----------------------------------------------------------------------------

//------------------------PLAYER------------------------------------------------

float degToRad(int a) 
{ 
    return a*M_PI/180.0;
}
int FixAng(int a)
{ 
    if(a>359)
    { 
        a-=360;
    } 
    if(a<0)
    { 
        a+=360;
    } 
    return a;
}

float px, py, pdx,pdy,pa; //player position and angles 
bool isAPressed = false; //Represents Key 'A' and 'Left' direction
bool isDPressed = false; //Represents Key 'D' and 'Right' direction
bool isWPressed = false; //Represents Key 'W' and 'Up' direction
bool isSPressed = false; //Represents Key 'S' and 'Down' direction

void drawPlayer2D()
{
    glColor3f(1,1,0);
    glPointSize(10);
    glLineWidth(4);
    glBegin(GL_POINTS);
    glVertex2i(px,py);
    glEnd();

    glBegin(GL_LINES);
    glVertex2i(px,py);
    glVertex2i(px+pdx*20, py+pdy*20);
    glEnd();
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_A)
    {
        if (action == GLFW_PRESS)
        {
            isAPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            isAPressed = false;
        }
    }

    if (key == GLFW_KEY_D)
    {
        if (action == GLFW_PRESS)
        {
            isDPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            isDPressed = false;
        }
    }

    if (key == GLFW_KEY_W)
    {
        if (action == GLFW_PRESS)
        {
            isWPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            isWPressed = false;
        }
    }

    if (key == GLFW_KEY_S)
    {
        if (action == GLFW_PRESS)
        {
            isSPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            isSPressed = false;
        }
    }
}

void updatePlayerPosition()
{
    if (isAPressed)
    {
        pa += 5;
        pa = FixAng(pa);
        pdx = cos(degToRad(pa));
        pdy = -sin(degToRad(pa));
    }
    if (isDPressed)
    {
        pa-=5; 
        pa=FixAng(pa); 
        pdx=cos(degToRad(pa)); 
        pdy=-sin(degToRad(pa));
    }

    if (isWPressed)
    {
        px+=pdx*5; 
        py+=pdy*5;
    }

    if (isSPressed)
    {
        px-=pdx*5; 
        py-=pdy*5;
    }
}


//-----------------------------------------------------------------------------


//---------------------------Draw Rays and Walls--------------------------------

void drawRays2D()
{
 glColor3f(0,1,1); glBegin(GL_QUADS); glVertex2i(526,  0); glVertex2i(1006,  0); glVertex2i(1006,160); glVertex2i(526,160); glEnd();	
 glColor3f(0,0,1); glBegin(GL_QUADS); glVertex2i(526,160); glVertex2i(1006,160); glVertex2i(1006,320); glVertex2i(526,320); glEnd();	 	
	
 int r,mx,my,mp,dof,side; float vx,vy,rx,ry,ra,xo,yo,disV,disH; 
 
 ra=FixAng(pa+30);                                                              //ray set back 30 degrees
 
 for(r=0;r<60;r++)
 {
  //---Vertical--- 
  dof=0; side=0; disV=100000;
  float Tan=tan(degToRad(ra));
       if(cos(degToRad(ra))> 0.001){ rx=(((int)px>>6)<<6)+64;      ry=(px-rx)*Tan+py; xo= 64; yo=-xo*Tan;}//looking left
  else if(cos(degToRad(ra))<-0.001){ rx=(((int)px>>6)<<6) -0.0001; ry=(px-rx)*Tan+py; xo=-64; yo=-xo*Tan;}//looking right
  else { rx=px; ry=py; dof=8;}                                                  //looking up or down. no hit  

  while(dof<8) 
  { 
   mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;                     
   if(mp>0 && mp<mapX*mapY && map[mp]==1){ dof=8; disV=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);}//hit         
   else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
  } 
  vx=rx; vy=ry;

  //---Horizontal---
  dof=0; disH=100000;
  Tan=1.0/Tan; 
       if(sin(degToRad(ra))> 0.001){ ry=(((int)py>>6)<<6) -0.0001; rx=(py-ry)*Tan+px; yo=-64; xo=-yo*Tan;}//looking up 
  else if(sin(degToRad(ra))<-0.001){ ry=(((int)py>>6)<<6)+64;      rx=(py-ry)*Tan+px; yo= 64; xo=-yo*Tan;}//looking down
  else{ rx=px; ry=py; dof=8;}                                                   //looking straight left or right
 
  while(dof<8) 
  { 
   mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;                          
   if(mp>0 && mp<mapX*mapY && map[mp]==1){ dof=8; disH=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);}//hit         
   else{ rx+=xo; ry+=yo; dof+=1;}                                               //check next horizontal
  } 
  
  glColor3f(0,0.8,0);
  if(disV<disH){ rx=vx; ry=vy; disH=disV; glColor3f(0,0.6,0);}                  //horizontal hit first
  glLineWidth(2); glBegin(GL_LINES); glVertex2i(px,py); glVertex2i(rx,ry); glEnd();//draw 2D ray
    
  int ca=FixAng(pa-ra); disH=disH*cos(degToRad(ca));                            //fix fisheye 
  int lineH = (mapS*320)/(disH); if(lineH>320){ lineH=320;}                     //line height and limit
  int lineOff = 160 - (lineH>>1);                                               //line offset
  
  glLineWidth(8);glBegin(GL_LINES);glVertex2i(r*8+530,lineOff);glVertex2i(r*8+530,lineOff+lineH);glEnd();//draw vertical wall  

  ra=FixAng(ra-1);                                                              //go to next ray
 }
}//-----------------------------------------------------------------------------


void init()
{
    glClearColor(0.3,0.3,0.3,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1024, 512,0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    px = 150; 
    py = 400;
    pa = 90;
    pdx=cos(degToRad(pa)); 
    pdy=-sin(degToRad(pa)); 
}

void display(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /*draw Map with Boundry Walls*/
    drawMap2D();

    /*draw Player*/
    drawPlayer2D();

    // Update player position based on press of WASD keys on keyboard
    updatePlayerPosition();
    
    /*draw Rays*/
    drawRays2D();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
}



int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Raycaster Demo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Set the key callback */
    glfwSetKeyCallback(window, keyCallback);

    /*Initialize player coordinates*/
    init();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        display(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}