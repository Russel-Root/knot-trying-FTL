#include "RopeStruct.h"

ofstream fout1("foo.txt");

RopeStruct::RopeStruct(){
	num_points	= 0;
	points_vec	= NULL;
	points_vec_copy = NULL;

	//redFlag = 1;
	//red[0] = 0.5;
	//red[1] = 0.2;
	//red[2] = 0.3, 1.0};
	//gray = {0.75f, 0.75f, 0.75f, 1.0f};
	//GLfloat gray[] = { 0.75f, 0.75f, 0.75f, 1.0f };
	//GLfloat red[4] = {0.5, 0.2, 0.3, 1.0};

	//matr44 = new Vector4f[4];
	//matr44[0].setVector( 0,       1,       0,          0 ); // the first row第一行
	//matr44[1].setVector( -tau,    0,       tau,        0 );
	//matr44[2].setVector( 2 * tau, tau - 3, 3 - 2 * tau, -tau );
	//matr44[3].setVector( -tau,    2 - tau, tau - 2,    tau  );
}

RopeStruct::~RopeStruct(){
	if ( num_points ){ 
		delete points_vec;
		delete points_vec_copy;
		//delete points_vec_tmp;
		//delete points_vec_tmp_result;
	}
}

bool RopeStruct::CreateMeshFromFile(char* _file_name, float rank){
	// 得到文件后缀
	string type =  strrchr( _file_name, '.' );
	// 如果文件格式不是洗下四种，返回空
	if ( !(type == ".3dsa" || type == ".vtka" || type == ".plya" || type == ".bod" || type == ".bd" || type == ".roda") )
		return false;

	ifstream readin( _file_name, ios::in );

	if ( type == ".roda" ){
		if( !readin ){
			cout << "Cannot Open File" << _file_name << endl;
			exit(0);
		}

		int n, i, k; 
		readin >> n;
		num_points = n;
		points_vec = new Vector3f[n];
		points_vec_copy = new Vector3f[n];

		for ( i = 0; i < n; i++ ){
			readin >> k >> points_vec[i].x >> points_vec[i].y >> points_vec[i].z;
			points_vec[i] *= rank;
			//fout1 << i << "\t" << points_vec[i].x << "\t" << points_vec[i].y << "\t" << points_vec[i].z << endl;
		}

		//Vector3f temp1_vec(points[0], points[1], points[2]);
		//Vector3f temp2_vec(points[0+3], points[1+3], points[2+3]);
		//length_points = (temp1_vec - temp2_vec).getLength();

		length_points = (points_vec[0] - points_vec[1]).getLength();
		wid = length_points / 2.0;
		
		////////////////////////////////////////////////////////////////////////// for the 1st and last node(1 node = 2 particle) drawing
		points_vec_1 = points_vec[0] * 2 - points_vec[1];
		points_vec_tail = points_vec[num_points - 1] * 2 - points_vec[num_points - 2];
	}

	//tree->BuildTree();

	cout << "length of the points: " << length_points << endl;
	//glLightfv(GL_LIGHT0, GL_AMBIENT, red);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Catmull-Rom 
/* t = 0.0 ~ 1.0
   t2 = t * t; 
   t3 = t2 * t;
   P(s) = [1, t, t2, t3] * 0.5 * [ 0,  2,  0,  0] * [p0]
                                 [-1,  0,  1,  0]   [p1]
								 [ 2, -5,  4, -1]   [p2]
								 [-1,  3, -3,  1]   [p3]
  if t == 0.0, p(s) = p1;
  if t == 1.0, p(s) = p2;
*/
// 在p1点与p2点之间插值n个点 Catmull-Rom Spline Interpolation
void RopeStruct::PointCatmullRom( int n, Vector3f p0, Vector3f p1, Vector3f p2, Vector3f p3 ){
	Vector3f* vv;
	vv = new Vector3f[n+1];

	float t, t2, t3;
	float temp1, temp2, temp3, temp4;
	
	vv[0] = p1;
	vv[n] = p2;

	int i;
	float step = 1.0 / n;
	for ( i = 1; i < n; i ++ ){
		t = step * i;
		t2 = t * t; 
		t3 = t * t2;

		temp1 = -t + 2 * t2 - t3;
		temp2 =  2 - 5 * t2 + 3 * t3;
		temp3 = t + 4 * t2 - 3 * t3;
		temp4 = -t2 + t3;

		vv[i].x = 0.5 * ( temp1 * p0.x + temp2 * p1.x + temp3 * p2.x + temp4 * p3.x );
		vv[i].y = 0.5 * ( temp1 * p0.y + temp2 * p1.y + temp3 * p2.y + temp4 * p3.y );
		vv[i].z = 0.5 * ( temp1 * p0.z + temp2 * p1.z + temp3 * p2.z + temp4 * p3.z );

		SphereDraw( vv[i], wid );

		//CylinderDraw( vv[i-1], vv[i], length_points / 2.0 );
	}
	//CylinderDraw( vv[i-1], vv[n], length_points / 2.0 );
	delete[] vv;
}

void RopeStruct::SphereDraw(Vector3f p, float rad){
	glPushMatrix();
	glTranslatef(p.x, p.y, p.z);
	glutSolidSphere(rad, 15, 15);
	glPopMatrix();
}

void RopeStruct::CylinderDraw(Vector3f p1, Vector3f p2, float rad){
	float h, delta;
	Vector3f v;
	Vector3f Z(0.0, 0.0, 1.0);

	v = p2 - p1;
	h = v.getLength();
	//fout1 << "length of cylinders " << h << endl;
	delta = acos( v.z / h ) * 180.0 / M_PI;
	v = Z.Cross(v);

	glPushMatrix();
	
	glTranslatef(p1.x, p1.y, p1.z);
	glRotatef(delta, v.x, v.y, v.z);
	GLUquadric* qua = gluNewQuadric();
	gluQuadricDrawStyle( qua, GLU_SMOOTH );
	gluCylinder( qua, rad, rad, h, 15, 15 );
	
	glPopMatrix();
}

void RopeStruct::Draw(){
	int i;
	//for ( i = 1; i < num_points - 2; i++ ){
	//	//glPushMatrix();
	//	//glTranslatef(points_vec[i].x, points_vec[i].y, points_vec[i].z);
	//	//glutSolidSphere(length_points/3, 15, 15);
	//	//glPopMatrix();

	//	PointCatmullRom(points_vec[i-1], points_vec[i], points_vec[i+1], points_vec[i+2]);
	//}

	////////////////////////////////////////////////////////////////////////// draw control_points
	//for ( i = 0; i < num_points; i++ ){
	//	SphereDraw(points_vec[i], length_points / 2.0);
	//}

	// draw the interpolation points
	int num_interpolation = 50;
	// draw points_vec[1] to points_vec[num_point-2], where are the last and the 1st point?
	for ( i = 1; i < num_points - 2; i++ ){
		if ( i % 2 == 0 )
			glLightfv(GL_LIGHT0, GL_AMBIENT, green);
		else
			glLightfv(GL_LIGHT0, GL_AMBIENT, red);

		PointCatmullRom(num_interpolation, points_vec[i-1], points_vec[i], points_vec[i+1], points_vec[i+2]);
		// draw points between the 2 points: points_vec[i], points_vec[i+1]
	}
	glLightfv(GL_LIGHT0, GL_AMBIENT, green);
	//cout << "1st point" << endl;
	//cout << points_vec[0].x << "\t" << points_vec[0].y << "\t" << points_vec[0].z << endl;
	//cout << points_vec_1.x << "\t" << points_vec_1.y << "\t" << points_vec_1.z << endl;
	//
	//cout << "tail point" << endl;
	//cout << points_vec[num_points - 1].x << "\t" << points_vec[num_points - 1].y << points_vec[num_points - 1].z << endl;
	//cout << points_vec_tail.x << "\t" << points_vec_tail.y << "\t" << points_vec_tail.z << endl;

	PointCatmullRom(num_interpolation, points_vec_1, points_vec[0], points_vec[1], points_vec[2]);
	PointCatmullRom(num_interpolation, points_vec_tail, points_vec[num_points - 1], points_vec[num_points - 2], points_vec[num_points - 3]);

	//glPushMatrix();
	//glTranslatef(points_vec[num_points - 1].x, points_vec[num_points - 1].y, points_vec[num_points - 1].z);
	//glutWireSphere(length_points * 0.75, 15, 15);
	//glPopMatrix();

	//glPushMatrix();
	//glTranslatef(points_vec[0].x, points_vec[0].y, points_vec[0].z);
	//glutWireSphere(length_points * 0.75, 15, 15);
	//glPopMatrix();

	//// i = num_points - 2
	//glPushMatrix();
	//glTranslatef(points_vec[i].x, points_vec[i].y, points_vec[i].z);
	//glutSolidSphere(length_points/2, 15, 15);
	//glPopMatrix();

	//i++;
	//// i = num_points - 1
	//glPushMatrix();
	//glTranslatef(points_vec[i].x, points_vec[i].y, points_vec[i].z);
	//glutSolidSphere(length_points/2, 15, 15);
	//glPopMatrix();
}
//void RopeStruct::Draw(){
//	Vector3f p1, p2, v;
//	float h, delta;
//	Vector3f z(0.0, 0.0, 1.0);
//	GLfloat gray[] = { 0.75f, 0.75f, 0.75f, 1.0f };
//	GLfloat red[4] = {0.5, 0.2, 0.3, 1.0};
//
//	for ( int i = 0; i < num_points-1; i++ ){
//		if ( i != 50 )/*if ( i % 2 == 0 )*/
//			glLightfv(GL_LIGHT0, GL_AMBIENT, gray);
//		else
//			glLightfv(GL_LIGHT0, GL_AMBIENT, red);
//
//		p2.setVector(points_vec[i]);
//		p1.setVector(points_vec[i+1]);
//
//		v = p1 - p2;
//		h = v.getLength();
//		delta = acos( v.z / h ) * 180.0 / M_PI;
//		v = z.Cross(v);
//
//		glPushMatrix();
//		glTranslatef(p2.x, p2.y, p2.z);
//		//glutSolidSphere(0.5, 12, 12);
//		glRotatef(delta, v.x, v.y, v.z);
//		GLUquadric* quadric = gluNewQuadric();
//		gluQuadricDrawStyle(quadric, GLU_SMOOTH);
//		gluCylinder(quadric, h/2, h/2, h, 15, 15); // radius is 0.15
//		glPopMatrix();
//
//		//fout1 << i << "   " << points[i*3] << "   " << points[i*3+1] << "   " << points[i*3+2] << endl; 
//	}
//	//fout1 << 99 << "   " << points[99*3] << "   " << points[99*3+1] << "   " << points[99*3+2] << endl; 
//}

void RopeStruct::drawAboutTailHead(int target, Vector3f dist_vec){
	////////////////////////////////////////////////////////////////////////// 为了头尾两个节点的圆滑曲线catmull-rom插值
	if ( target == 0 ){
		points_vec_1 = points_vec[0] + dist_vec * 2;
	}else{
		points_vec_1 = points_vec[0];
	}

	if ( target == num_points - 1 ){
		points_vec_tail = points_vec[num_points - 1] + dist_vec * 2;
	}else{
		points_vec_tail = points_vec[num_points - 1];
	}
	//////////////////////////////////////////////////////////////////////////
}

void RopeStruct::CopyPoints(){
	for ( int i = 0; i < num_points; i++ ){
		points_vec_copy[i] = points_vec[i];
	}
}
//////////////////////////////////////////////////////////////////////////
// 外部施加作用于序号为target的控制点，该控制点在外力的作用下移动的距离为dist_vec
void RopeStruct::Move(Vector3f dist_vec, int target){
	CopyPoints();
	drawAboutTailHead(target, dist_vec);
	//////////////////////////////////////////////////////////////////////////// 为了头尾两个节点的圆滑曲线catmull-rom插值
	//if ( target == 0 ){
	//	points_vec_1 = points_vec[0] + dist_vec * 2;
	//}else{
	//	points_vec_1 = points_vec[0];
	//}

	//if ( target == num_points - 1 ){
	//	points_vec_tail = points_vec[num_points - 1] + dist_vec * 2;
	//}else{
	//	points_vec_tail = points_vec[num_points - 1];
	//}
	////////////////////////////////////////////////////////////////////////////
	Vector3f points_orientation;
	int target_temp;
	target_temp = target;
	points_vec[target_temp]	+= dist_vec; // update the control point

	// deal with points_vet[target_temp - 1], tail to head, so target_temp != 0;
	while ( target_temp < num_points && target_temp > 0){
		points_orientation = points_vec[target_temp] - points_vec[target_temp-1];
		points_vec[target_temp-1] = points_vec[target_temp] - points_orientation.Normalize() * length_points;
		target_temp--; // 向下循环
	}

	target_temp = target;
	// deal with points_vec[target_temp + 1], head to tail, target_temp != num_points - 1;
	while ( target_temp > -1 && target_temp < num_points - 1 ){
		points_orientation = points_vec[target_temp] - points_vec[target_temp+1];
		points_vec[target_temp+1] = points_vec[target_temp] - points_orientation.Normalize() * length_points; 
		target_temp++; // 向上循环
	}
}

void RopeStruct::MoveConsecutive(int targetHead, int n, Vector3f dis_vec){ // consecutive n points, that's [target], [target+1],..., [target+n-1]
	if ( n < 2 || n > 3 ) // n = 2, 3, so { [target] & [target+1] } or { [target] & [target+1] & [target+2] }
		return;
	//////////////////////////////////////////////////////////////////////////
	//if ( targetHead == 0 ){
	//	drawAboutTailHead(targetHead, dis_vec);
	//}
	if ( targetHead == (num_points - 1) - (n - 1) ){
		drawAboutTailHead(targetHead, dis_vec);
	}
	else{
		drawAboutTailHead(targetHead, dis_vec);
	}
	//////////////////////////////////////////////////////////////////////////
	if ( n == 3 ){
		points_vec[targetHead + 1] += dis_vec;
		points_vec[targetHead] += dis_vec; // here the length between these 3 control points is not const length_points!!!
		points_vec[targetHead - 1] += dis_vec;
	}
	
	if ( n == 2 ){
		points_vec[targetHead] += dis_vec;
		points_vec[targetHead + 1] += dis_vec;
	}

	littleEndian( (targetHead + 1), dis_vec );
	notLittleEndian( targetHead, dis_vec );
}

void RopeStruct::notLittleEndian(int target, Vector3f move3f){ // from tail to head, the particle index ranges from [num_points - 1] to [0], actually, it's from [cur_point] to [0]
	int cur_point;
	Vector3f points_orientation;
	while ( target > 0 && target < num_points ){ // cur_point = target -1, so target should >= 1;
		cur_point = target - 1;
		points_orientation = ( points_vec[target] - points_vec[cur_point] ).Normalize(); // here we'll move points_vec[target - 1]
		points_vec[cur_point] = points_vec[target] - points_orientation * length_points;
		target--;
	}
}

void RopeStruct::littleEndian(int target, Vector3f move3f){ // from head to tail, the particle index ranges from [0] to [num_points - 1], actually, it's from [cur_point] to [num_points - 1]
	int cur_point;
	Vector3f points_orientation;
	while ( target < num_points - 1 && target >= 0 ){ // cur_point = target + 1, so target should <= num_points - 2
		cur_point = target + 1;
		points_orientation = (points_vec[target] - points_vec[cur_point]).Normalize();
		points_vec[cur_point] = points_vec[target] - points_orientation * length_points;
		target++;
	}
}

void RopeStruct::MoveAll(Vector3f dist_vec){
	for ( int i = 0; i < num_points; i++ ){
		points_vec[i] += dist_vec;
	}
	points_vec_1 += dist_vec;
	points_vec_tail += dist_vec;
}

void RopeStruct::Update(){}

//void RopeStruct::ActionBuffer(Vector3f direc, int particleIndex){
//	vector<ParticlesBuffer>::iterator itr;
//	for ( itr = bufferSelfcolHandling.begin(); itr != bufferSelfcolHandling.end(); itr++ ){ 
//		// if bufferSelfcolHanding.size == 0, we can infer to bufferSelfcolHanding.begin() == bufferSelfcolHandling.end();
//		if ( (*itr).index == particleIndex ){
//			(*itr).direction += direc;
//			//break;
//			return;
//		}
//	}
//	////ParticlesBuffer* buffer;
//	////buffer = new ParticlesBuffer(particleIndex, direc);
//	//ParticlesBuffer* buffer = new ParticlesBuffer(particleIndex, direc);
//	//bufferSelfcolHandling.push_back(*buffer);
//	bufferSelfcolHandling.push_back( ParticlesBuffer(particleIndex, direc) );
//}
//
//void RopeStruct::MoveAction(){
//	vector<ParticlesBuffer>::iterator itr;
//	for ( itr = bufferSelfcolHandling.begin(); itr != bufferSelfcolHandling.end(); itr++ ){
//		points_vec[ (*itr).index ] += (*itr).direction; // note that the points_length
//
//		//fout1 << "bufferSelfcolHandling" << endl;
//		//fout1 << (*itr).index << "\t" << (*itr).direction.x << "\t" << (*itr).direction.y << "\t" << (*itr).direction.z << endl;	
//	}
//}

void RopeStruct::Axis(){ 
	glColor3f(1.0, 1.0, 1.0);
	GLUquadricObj* pObj;

	// Measurements
	float fAxisRadius = 0.025f;
	float fAxisHeight = 1.0f;
	float fArrowRadius = 0.06f;
	float fArrowHeight = 0.1f;

	glPushMatrix();
	glScalef(2.5, 2.5, 2.5);

	// Setup the quadric object
	pObj = gluNewQuadric();
	gluQuadricDrawStyle(pObj, GLU_FILL);
	gluQuadricNormals(pObj, GLU_SMOOTH);
	gluQuadricOrientation(pObj, GLU_OUTSIDE);
	gluQuadricTexture(pObj, GLU_FALSE);

	///////////////////////////////////////////////////////
	// Draw the blue Z axis first, with arrowed head
	glColor3f(0.0f, 0.0f, 1.0f);
	gluCylinder(pObj, fAxisRadius, fAxisRadius, fAxisHeight, 10, 1);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluCylinder(pObj, fArrowRadius, 0.0f, fArrowHeight, 10, 1);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(pObj, fAxisRadius, fArrowRadius, 10, 1);
	glPopMatrix();

	///////////////////////////////////////////////////////
	// Draw the Red X axis 2nd, with arrowed head
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(pObj, fAxisRadius, fAxisRadius, fAxisHeight, 10, 1);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluCylinder(pObj, fArrowRadius, 0.0f, fArrowHeight, 10, 1);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	gluDisk(pObj, fAxisRadius, fArrowRadius, 10, 1);
	glPopMatrix();
	glPopMatrix();

	///////////////////////////////////////////////////////
	// Draw the Green Y axis 3rd, with arrowed head
	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(pObj, fAxisRadius, fAxisRadius, fAxisHeight, 10, 1);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluCylinder(pObj, fArrowRadius, 0.0f, fArrowHeight, 10, 1);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(pObj, fAxisRadius, fArrowRadius, 10, 1);
	glPopMatrix();
	glPopMatrix();

	////////////////////////////////////////////////////////
	// White Sphere at origin
	glColor3f(1.0f, 1.0f, 1.0f);
	gluSphere(pObj, 0.05f, 15, 15);

	glPopMatrix();

	// Delete the quadric
	gluDeleteQuadric(pObj);
}