#include "RopeSimulator.h"

ofstream fout3("foo.txt");
RopeSimulator::RopeSimulator(){
	// geometry model
	static const float rank = 1.0;
	this->geoModel = new RopeStruct;
	this->geoModel->CreateMeshFromFile("../Data/Mesh/rd4Experiment.roda", rank);
	
	// bvh tree
	this->tree	 = new BVHTree;
	this->tree->SetRope(geoModel);
	this->tree->BuildTree(); 
}

RopeSimulator::~RopeSimulator(){}

void RopeSimulator::controlMovInput(Vector3f move3f, int ctrlPointIndex, int ctlNum){
	tree->ctrlPt = ctrlPointIndex;
	if ( ctlNum == 1 ){
		geoModel->Move(move3f, ctrlPointIndex);
	}else if ( ctlNum > 1 && ctlNum < 4 ){
		geoModel->MoveConsecutive(ctrlPointIndex, ctlNum, move3f);
	}
	else {
		return;
	}

	cout << "ropesimulator::controlMoveInput: " << endl;

	tree->UpdateTreeParticles();
	tree->UpdateTree();
	// update tree 待改进 !!!
	tree->SelfCollision();
	int count = 0;
	while( tree->collisions.size() != 0 ){
		if ( count > 1 )
			break;
		tree->HandleSeflCollision();
		count ++;
		tree->UpdateTreeParticles();
		tree->UpdateTree();
		// update tree 待改进 !!!
		tree->SelfCollision();
	}

	cout << "ropesimulator::controlMoveInput close:  " << endl;
	cout << endl;
	fout3 << "ropesimulator::controlMoveInput close:  " << endl;
	fout3 << endl;
	//  tree->LeafNodeTree();
	//if ( tree->collisions.size() != 0 )
}

void RopeSimulator::controlMovInputAll(Vector3f move3f){
	geoModel->MoveAll(move3f);
}

void RopeSimulator::setCollisonColor(bool flag){
	tree->setColColor(flag);
}