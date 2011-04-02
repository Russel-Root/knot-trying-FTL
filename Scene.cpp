#include <fstream>
#include <time.h>
//#include <conio.h>
//#include <windows.h>
//#include <gl/glut.h>
//#include <math.h>
#include "MyTimer.h"
#include "Vector3f.h"
#include "Camera.h"
#include "RenderImpGL.h"
#include "RopeStruct.h"
//#include "BasicStruct.h"
//#include "AabbTree.h"
#include "BvhTree.h"

#include "RopeSimulator.h"

using namespace std;

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "glut32.lib")

const int milliseconds = 10;
//#define M_PI 3.14159265358
MyTimer* timer;
MyTimer* timer4Frame;
clock_t start;
clock_t finish;

Camera* camera;
//ofstream fout1("foo.txt");


// mouse control variables
int mouse_old_x, mouse_old_y;
int mouse_buttons = 0;
GLfloat tx = 0.0, ty = 0.0, tz = 0.0; // 移动端点
GLfloat ttx = 0.0, tty = 0.0, ttz = 0.0; // 移动整体
GLfloat rotate_y = 30;
GLfloat height_y = 0;

int control_int = 0;
int control_num = 1;

// light
GLfloat lightAmbient[4] = {0.5, 0.2, 0.3, 1.0};
GLfloat lightDiffuse[4] = {0.671, 0.255, 0.243, 1.0};
GLfloat lightSpecular[4] = {0.0, 0.0, 1.0, 1.0};
GLfloat lightPosition[4] = {1.0, 1.0, 1.0, 0.0};
GLfloat lightShininess[4] = {150.0};

RenderImpGL* renderImpGL;

RopeSimulator* ropeSimulator;

float length2points = 0.0;

//RopeStruct* rope;
//BVHTree* ropeTree;

bool draw_fill_mode = false;
Vector3f movement_vec(0.0, 0.0, 0.0);
//void buildTreeFunc();

void init( void ){
	//glClearColor( 1.0, 1.0, 0.0, 0.0 );
	glShadeModel( GL_SMOOTH );
	glEnable( GL_DEPTH_TEST );

	timer = new MyTimer();
	timer4Frame = new MyTimer();
	//timer4Frame->Start();

	ropeSimulator = new RopeSimulator();
	length2points = ropeSimulator->geoModel->length_points;

	//float rank = 1.0; // the distance between the two neighbors
	//rope = new RopeStruct();
	//rope->CreateMeshFromFile("rd4Experiment.roda", rank);
	////rope->CreateMeshFromFile("rd4Exp_50.roda", rank);//("rd3.roda");
	//
	//// build the bvh tree for the tissue
	//ropeTree = new BVHTree();
	//ropeTree -> SetRope( rope ); // rope collides against the tissue ( RopeStruct* rope, BasicStruct* object )
	//ropeTree -> BuildTree();
	
	camera = new Camera();
	camera->setCamera(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 5.0f, 0.0f, 1.0f, 0.0f);

	//////////////////////////////////////////////////////////////////////////
	renderImpGL = new RenderImpGL();
	renderImpGL->init();
	//////////////////////////////////////////////////////////////////////////

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_SHININESS, lightShininess);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

    cout << "L: left, R: right, U: lift, D: lower, I: in, O: out" << endl;

//	rope->BuildTree();
	
	//tree = new AabbTree(rope);
	/*rope->Move(0.0, 0.0, 0.0);*/

	//object = new BasicStruct();
	//object->CreateMeshFromFile("cut_approximation_1.3dsa", 0.15f, 0.0f, 0.0f, 0.0f);

	//object_mirror = new BasicStruct();
	//object_mirror->CreateMeshFromFile("cut_approximation_1_mirror.3dsa", 0.15f, 0.0f, 0.0f, 0.0f);
}

//void buildTreeFunc(){
//	// build the bvh tree
//	scene_tree = new BVHTree();
//	scene_tree -> SetRope( rope );
//	scene_tree -> BuildTree();
//}

void display( void ){
	//timer4Frame->Reset();
	//timer4Frame->Start();
	//Sleep( 1000 );
	//timer4Frame->End();
	//timer4Frame->showDuration();
	//timer4Frame->Reset();
	//start = clock();
	//Sleep(1000);
	//finish = clock();
	//cout << double(finish - start) / CLOCKS_PER_SEC << endl;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
		
	camera->Look();
	
	renderImpGL->renderManager.setMat(0);
	ropeSimulator->geoModel->Draw();

	//选择线框模式
	//if(draw_fill_mode)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	////选择实体模式
	//else
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	////if (draw_fill_mode){
	//	object->DrawMesh();//(FILL_MODE);
	//	object_mirror->DrawMesh();//(FILL_MODE);
	////}
	////else{
	//	object->DrawMesh();//(LINE_MODE);
	//	object_mirror->DrawMesh();//(LINE_MODE);
	//}

	//rope->Axis();
	///*rope->Update();*/

	//glPushMatrix();
	//glTranslatef(rope->points_vec[control_int].x, rope->points_vec[control_int].y, rope->points_vec[control_int].z);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, blue);
	//glutSolidCube( rope->length_points * 2.0 );
	////glutSolidSphere(0.25/2, 15, 15);
	//glPopMatrix();

	//glPushMatrix();
	//glTranslatef(ropeTree->root->centerpos.x, ropeTree->root->centerpos.y, ropeTree->root->centerpos.z);
	//glutWireSphere(ropeTree->root->radius, 25, 25);
	//glPopMatrix();
	//cout << rope->points_vec[0].x << '\t' << rope->points_vec[0].y << '\t' << rope->points_vec[0].z << endl;
	//cout << rope->points_vec[99].x << '\t' << rope->points_vec[99].y << '\t' << rope->points_vec[99].z << endl;
	//cout << ropeTree->root->centerpos.x << '\t' << ropeTree->root->centerpos.y << '\t' << ropeTree->root->centerpos.z << '\t' << ropeTree->root->radius << endl;

	//GLfloat mat_ambient[] = {0.0, 0.0, 1.0, 0.5};
	//GLfloat mat_diffuse[] = {0.0, 0.0, 1.0, 0.5};
	//GLfloat mat_specular[] = {0.0, 0.0, 1.0, 0.5};
	//GLfloat mat_shininess[] = {30.0};

	//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
 
	vector<Vector3f>::iterator itr = ropeSimulator->tree->colPoints.begin();
	vector<int>::iterator itrColor = ropeSimulator->tree->colPointsColor.begin();
	vector<float>::iterator itrRadius = ropeSimulator->tree->colPointsRadius.begin();

	for ( ; itr != ropeSimulator->tree->colPoints.end(); itr++, itrColor++, itrRadius++ ){
		//////////////////////////////////////////////////////////////////////////
		float p[3];
		int color = (*itrColor);
		renderImpGL->renderManager.setMat(color); // 设置材质色彩

		(*itr).ToFloat(p);
		float rad = (*itrRadius);
		//////////////////////////////////////////////////////////////////////////

		::glPushMatrix();
		glDisable(GL_COLOR_MATERIAL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		//glColor4f(1.0, 1.0, 1.0, 0.3);
		glTranslatef(p[0], p[1], p[2]);
		glutSolidSphere(rad, 10, 10);
		//glTranslatef((*itr).x, (*itr).y, (*itr).z );
		//glutSolidSphere( (*itrRadius), 10, 10 );

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		::glPopMatrix();
	}

	glutSwapBuffers();
	glutPostRedisplay();
	//timer4Frame->End();
	//finish = clock();
	//cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;
	//timer4Frame->showDuration();
	//cout << "Frame: " << (double)1.0 / timer4Frame->costTime << endl;
	//timer4Frame->Reset();
	//timer4Frame->Start();
}

void mouse( int button, int state, int x, int y ){
	if ( state == GLUT_DOWN )
		mouse_buttons |= 1 << button;
	else if ( state == GLUT_UP )
		mouse_buttons = 0;

	mouse_old_x = x;
	mouse_old_y = y;

	glutPostRedisplay();
}

void motion( int x, int y ){
	GLfloat dx, dy;

	dx = x - mouse_old_x;
	dy = y - mouse_old_y;

	// drag the left button
	if ( mouse_buttons & 1 )
	{
		camera->RotateCamera_X( dx * 0.5 );
		camera->RotateCamera_Y( dy * 0.5 );
	}

	// drag the right button
	else if ( mouse_buttons & 4 )
	{
		camera->MoveCamera( dx / 20 );
		//rotate_y -= dx / 20;
		//height_y -= dy / 50;
	}

	mouse_old_x = x;
	mouse_old_y = y;
}

void specialKeyboard( int key, int x, int y ){
	switch ( key )
	{
	case GLUT_KEY_RIGHT:
		movement_vec.x = length2points;
		break;

	case GLUT_KEY_LEFT:
		movement_vec.x = -length2points;
		break;

	case GLUT_KEY_UP:
		movement_vec.y = length2points;
		break;

	case GLUT_KEY_DOWN:
		movement_vec.y = -length2points ;
		break;

	default:
		break;
	}

	ropeSimulator->controlMovInputAll(movement_vec);
	movement_vec.Clear();

	glutPostRedisplay();
}

void keyboard( unsigned char key, int x, int y ){
	switch ( key )
	{
	case 'C': // assign the index of the control point
		cout << "control node index:(0-99)" << endl;
		cin >> control_int;
		cout << "consecutive particles number: (2 or 3) " << endl;
		cin >> control_num;
		break;
	case 'L': // move the control point to the left
		movement_vec.x = -length2points;
		break;
	case 'R': // move the control point to the right
		movement_vec.x = length2points;
		break;
	case 'U': // lift the control point
		movement_vec.y = length2points;
		break;
	case 'D': // lower the control point
		movement_vec.y = -length2points;
		break;
	case 'I': // in, go deeper
		movement_vec.z = -length2points;
		break;
	case 'O': // out, go out
		movement_vec.z = length2points;
		break;
	case '7':
		ropeSimulator->setCollisonColor(false);
		break;
	case '6':
		ropeSimulator->setCollisonColor(true);
		break;
	case 'Q': // quit
		exit(0);
		//break;
	default:
		break;
	}

	timer->Reset();
	timer->Start();
	//start = clock();
	ropeSimulator->controlMovInput(movement_vec, control_int, control_num);
	timer->End();
	timer->showDuration();
	//finish = clock();
	//double duration = double(finish - start) / CLOCKS_PER_SEC;
	//system("cls");
	//cout << "duration of movement process:(s) " << duration << endl;

	//cout << "movement_vec\t" << movement_vec.x << ", " << movement_vec.y << ", " << movement_vec.z << endl;
	movement_vec.Clear();
	//cout << "movement_vec after Clear\t" << movement_vec.x << ", " << movement_vec.y << ", " << movement_vec.z << endl;
	
	//ropeSimulator->tree->UpdateTree();

	glutPostRedisplay();
}

void IdleFunc(){}

void TimerFunc(int p){
	glutTimerFunc(milliseconds, TimerFunc, 1);
}

void reshape( int w, int h ){
	glViewport( 0, 0, (GLsizei)w, (GLsizei)h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 120.0f, (GLfloat)w / (GLfloat)h, 0.2f, 600.0f );
}

int main( int argc, char** argv ){	
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 1024, 768 );
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow("Suture Simulation");

	init();
	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutMouseFunc( mouse );
	glutMotionFunc( motion );
	glutKeyboardFunc( keyboard );
	glutSpecialFunc( specialKeyboard );

	glutIdleFunc(IdleFunc);
	glutTimerFunc(milliseconds, TimerFunc, 1);
	glutMainLoop();
}