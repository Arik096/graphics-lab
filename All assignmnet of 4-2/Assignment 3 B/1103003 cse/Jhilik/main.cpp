#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
float posx=350;
float posy=350;
float posz=0;
float lx=-1/sqrt(2);
float ly=-1/sqrt(2);
float lz=0;
float rx=-1/sqrt(2);
float ry=1/sqrt(2);
float rz=0;
float ux=0;
float uy=0;
float uz=1;

struct point
{
	double x,y,z;
};


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}



void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);

				glVertex3f(-90,0,i*10);
				glVertex3f(90,0,i*10);
			}
		}glEnd();
	}
}


void drawSquare(double a,double b){

glBegin(GL_QUADS);{

		glVertex3f( a, b,2);
		glVertex3f( a,-b,2);
		glVertex3f(-a,-b,2);
		glVertex3f(-a, b,2);

	}glEnd();

}





void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    struct point points1[100];

    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
        points[i].z=radius*sin(((double)i/(double)segments)*2*pi);

        points1[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points1[i].y=radius*sin(((double)i/(double)segments)*2*pi);
        points1[i].z=radius*sin(((double)i/(double)segments)*2*pi);



    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x,points[i].y,10);
			glVertex3f(points[i+1].x,points[i+1].y,10);

			glVertex3f(points[i].x,points[i].y,-10);
			glVertex3f(points[i+1].x,points[i+1].y,-10);

        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}



void drawCube(double a)
{
    glBegin(GL_QUADS);{
		glVertex3f( a, a, a);
		glVertex3f( a,-a, a);
		glVertex3f(-a,-a, a);
		glVertex3f(-a, a, a);

		glVertex3f( a, a,-a);
		glVertex3f( a,-a,-a);
		glVertex3f(-a,-a,-a);
		glVertex3f(-a, a,-a);

		glVertex3f( a, a, a);
		glVertex3f( a, a,-a);
		glVertex3f( a,-a,-a);
		glVertex3f( a,-a, a);

		glVertex3f(-a, a, a);
		glVertex3f(-a, a,-a);
		glVertex3f(-a,-a,-a);
		glVertex3f(-a,-a, a);

		glVertex3f( a, a, a);
		glVertex3f( a, a,-a);
		glVertex3f(-a, a,-a);
		glVertex3f(-a, a, a);

		glVertex3f( a,-a, a);
		glVertex3f( a,-a,-a);
		glVertex3f(-a,-a,-a);
		glVertex3f(-a,-a, a);
	}glEnd();
}

//draws a cylinder of given radius and height
void drawCylinder(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw quads using generated points
    for(i=0;i<segments;i++)
    {

        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x,points[i].y,height/2);
			glVertex3f(points[i+1].x,points[i+1].y,height/2);
			glVertex3f(points[i].x,points[i].y,-height/2);
			glVertex3f(points[i+1].x,points[i+1].y,-height/2);
        }
        glEnd();
    }
}







void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}
double tx=0;
double t1x=0;
double t2x=0;
double t3x=0;
double t4x=0;
double t5x=0;
double b1= 0;

void drawSS()
{

     glPushMatrix();
    {

        glTranslated(0,0,b1);

    glPushMatrix();
    {
    glColor3f(0,1,0);
    drawCube(15);
    }glPopMatrix();



    glPushMatrix();
    {
    glTranslatef(0,-20,-18);
    glColor3f(0,0,0);
    drawCylinder(10,5,5);
    }glPopMatrix();


    glPushMatrix();
    {
    glTranslatef(0,-20,18);
    glColor3f(0,0,0);
    drawCylinder(10,5,5);
    }
    glPopMatrix();

    }glPopMatrix();



}

void road(){

    glPushMatrix();
    {
    glColor3f(0,0,1);
    glTranslated(-100,-40,0);
    glScaled(150,1,15);
    drawCube(5);
    }
    glPopMatrix();

    glPushMatrix();
    {
    glTranslated(tx,0,0);
    glColor3f(2,0,0);
    glTranslated(60,-18,50);
    glScaled(3,3,5);
    drawCube(5);
    }
    glPopMatrix();

    glPushMatrix();
    {
    glTranslated(t1x,0,0);
    glColor3f(2,0,0);
    glTranslated(20,-18,-50);
    glScaled(3,3,5);
    drawCube(5);
    }
    glPopMatrix();

    glPushMatrix();
    {
    glTranslated(t2x,0,0);
    glColor3f(2,0,0);
    glTranslated(-80,-18,50);
    glScaled(3,3,5);
    drawCube(5);
    }
    glPopMatrix();

    glPushMatrix();
    {glTranslated(t3x,0,0);
    glColor3f(2,0,0);
    glTranslated(-120,-18,-50);
    glScaled(3,3,5);
    drawCube(5);
    }
    glPopMatrix();

     glPushMatrix();
    {glTranslated(t4x,0,0);
    glColor3f(2,0,0);
    glTranslated(-250,-18,-50);
    glScaled(3,3,5);
    drawCube(5);
    }
    glPopMatrix();

      glPushMatrix();
    {glTranslated(t5x,0,0);
    glColor3f(2,0,0);
    glTranslated(-200,-18,50);
    glScaled(3,3,5);
    drawCube(5);
    }
    glPopMatrix();





}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;
        case 'a':
			b1+=30;
			break;
         case 's':
			b1-=30;
			break;


    case 'f':
        posx+=lx;
        posy+=ly;
        posz+=lz;
        break;

    case 'b':
        posx-=lx;
        posy-=ly;
        posz-=lz;
        break;

    case 'd':
        posx+=rx;
        posy+=ry;
        posz+=rz;
        break;

    case 'u':
        posx-=rx;
        posy-=ry;
        posz-=rz;
        break;

    case 'l':
        posx+=ux;
        posy+=uy;
        posz+=uz;
        break;

    case 'r':
        posx-=ux;
        posy-=uy;
        posz-=uz;
        break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;

		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;


		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(posx,posy,posz, posx+lx, posy+ly, posz+lz,	0,1,0);
	//gluLookAt(200,200,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/

	//add objects

	drawAxes();
	//drawGrid();

    glColor3f(1,1,1);
   // drawSquare(10);
	//drawBus(50,20,10);
    drawSS();
    road();







	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}




void animate(){
	angle-=(2*3.1416*.1);
	tx+=(((2*pi*10)/360)*.5);
    if(tx>250){
        tx=-700;
    }

    t1x+=(((2*pi*10)/360)*.5);
    if(t1x>280){
        t1x=-700;
    }
    t2x+=(((2*pi*10)/360)*.5);
    if(t2x>380){
        t2x=-700;
    }
   t3x+=(((2*pi*10)/360)*.5);
    if(t3x>430){
        t3x=-700;
    }

    t4x+=(((2*pi*10)/360)*.5);
    if(t4x>540){
        t4x=-700;
    }

     t5x+=(((2*pi*10)/360)*.5);
    if(t5x>500){
        t5x=-700;
    }

    //b+=(((2*pi*10)/360)*.1);

     if(b1>60 ){
        b1=60;
    }
    if(b1<-60){

        b1=-60;
    }


	//codes for any changes in Models, Camera
	glutPostRedisplay();
}




void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0 ,0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
