// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif

#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"
#include "math.h"

// L�gg till egna globaler h�r efter behov.

GLfloat kMaxDistance = 100.0f;
GLfloat kAlignmentWeight = 0.03;
GLfloat kCohesionWeight = 0.001;
GLfloat kAvoidanceWeight = 0.3;
GLfloat kDogAvoidance = 1.0;
GLfloat randomness = 20.0f;

SpriteRec *blackSheep;
SpriteRec *dog;

/** 
* Returns the distance between SpriteRec a and SpriteRec b
*/
float distance(SpriteRec *a, SpriteRec *b){
	FPoint aPos = a->position;
	FPoint bPos = b->position;

	return sqrt( pow(aPos.h - bPos.h, 2) + pow(aPos.v - bPos.v, 2) );
}

FPoint calcAvoidance(SpritePtr i, SpritePtr j){
	FPoint a = (FPoint){i->position.h - j->position.h, i->position.v - j->position.v};
	float d = sqrt(pow(a.h, 2.0f) + pow(a.v, 2.0f));

	a.h *= (1.0f-d/100)/d;
	a.v *= (1.0f-d/100)/d;

	return a;
}

void SpriteBehavior() // Din kod!
{
// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
// hastigheter och positioner, eller arbeta fr�n egna globaler.


	GLint count;
	SpritePtr ip = gSpriteRoot;
	SpritePtr jp = gSpriteRoot;

	// Compare i with i->next, maybe compare i with ALL other???

	while (ip != NULL) {


		count = 0;
		ip->speedDiff = (FPoint) {0, 0};
		ip->avaragePosition = (FPoint) {0, 0};
		ip->avoidanceVector = (FPoint) {0, 0};	
		
		jp = gSpriteRoot;
		while(jp != NULL){
			
			if( ip != jp && ip != dog && jp != dog){
				if(distance(ip, jp) < kMaxDistance){
					// Alignment
					ip->speedDiff.h += jp->speed.h - ip->speed.h;
					ip->speedDiff.v += jp->speed.v - ip->speed.v;

					// Cohesion
					ip->avaragePosition.h += jp->position.h;
					ip->avaragePosition.v += jp->position.v;

					// Avoidance
					FPoint avoidance = calcAvoidance(ip, jp);
					ip->avoidanceVector.h += avoidance.h;
					ip->avoidanceVector.v += avoidance.v;

					count++;
				}				
			}

			if(jp == dog && distance(ip, jp) < kMaxDistance ){
				ip->speed.h += 1/(distance(ip, jp) + 0.01) * kDogAvoidance*(ip->position.h - jp->position.h);
				ip->speed.v += 1/(distance(ip, jp) + 0.01) * kDogAvoidance*(ip->position.v - jp->position.v);
			}

			jp = jp->next;
		}

		if(count > 0){

			
			// Divisions
			ip->speedDiff.h /= count;
			ip->speedDiff.v /= count;

			ip->avaragePosition.h /= count;
			ip->avaragePosition.v /= count;

			ip->avoidanceVector.h /= count;
			ip->avoidanceVector.v /= count;

		}

		ip = ip->next;
	} 

	// Add the resulting contributions
	ip = gSpriteRoot;

	while(ip != NULL){

		if( ip == dog ){
			ip = ip->next;
			
		}

		// Cohesion
		ip->speed.h += (ip->avaragePosition.h - ip->position.h) * kCohesionWeight;
		ip->speed.v += (ip->avaragePosition.v - ip->position.v) * kCohesionWeight;
					
		// Seperation
		ip->speed.h += ip->avoidanceVector.h * kAvoidanceWeight;
		ip->speed.v += ip->avoidanceVector.v * kAvoidanceWeight;

		//  Alignment
		ip->speed.h += ip->speedDiff.h * kAlignmentWeight;
		ip->speed.v += ip->speedDiff.v * kAlignmentWeight;

		ip->position.h += ip->speed.h;
		ip->position.v += ip->speed.v; 

		if(ip == blackSheep){
			blackSheep->speed.h += randomness * (rand() / (float)RAND_MAX) - randomness/2.0f;
			blackSheep->speed.v += randomness * (rand() / (float)RAND_MAX) - randomness/2.0f;
		}
		

		ip = ip->next;	
	}



}

// Drawing routine
void Display()
{
	SpritePtr sp;
	
	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	DrawBackground();
	
	SpriteBehavior(); // Din kod!
	
// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do
	{
		HandleSprite(sp); // Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);
	
	glutSwapBuffers();
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Timer(int value)
{
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
  switch (key)
  {
    case '+':
    	kAlignmentWeight += 0.01;
    	printf("kAlignmentWeight = %f\n", kAlignmentWeight);
    	break;
    case '-':
		kAlignmentWeight -= 0.01;
    	printf("kAlignmentWeight = %f\n", kAlignmentWeight);
		break;
	// Dog controls
	case 'a':
		dog->speed = (FPoint) {-3.0, 0.0};
		break;
	case 'd':
		dog->speed = (FPoint) {3.0, 0.0};
		break;
	case 'w':
		dog->speed = (FPoint) {0.0, 3.0};
		break;
	case 's':
		dog->speed = (FPoint) {0.0, -3.0};
		break;
	case 'q':
		dog->speed = (FPoint) {0.0, 0.0};
		break;
    case 0x1b:
      exit(0);
  }
}

void Init()
{
	TextureData *sheepFace, *blackFace, *dogFace, *foodFace;
	
	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund
	
	sheepFace = GetFace("bilder/sheep.tga"); // Ett f�r
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat
	
	NewSprite(sheepFace, 100, 200, 1, 1);
	NewSprite(sheepFace, 200, 100, 1.5, 0);
	NewSprite(sheepFace, 250, 200, -2.0, 1.0);

	NewSprite(sheepFace, 500, 600, 1, 1);
	NewSprite(sheepFace, 700, 500, 1.5, 0);
	NewSprite(sheepFace, 750, 600, 5.0, 1.0);
	
	//blackSheep = NewSprite(blackFace, 450, 300, 2.0, 3.0);
	dog = NewSprite(dogFace, 300, 300, 0.0, 0.0);
	
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SpriteLight demo / Flocking");
	
	glutDisplayFunc(Display);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	
	InitSpriteLight();
	Init();
	
	glutMainLoop();
	return 0;
}
