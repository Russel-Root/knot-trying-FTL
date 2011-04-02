#include "BvhTree.h"

double BVHNode::deviance = 0.0055; // points_length = 0.25, that's the distance between 2 neighbor points

ofstream fout2("foo.txt");
BVHTree::BVHTree(){
	collisionColorFlag = false;
}

BVHTree::~BVHTree(){}

void BVHTree::SetRope( RopeStruct* struct_rope ){
	rope4tree = struct_rope;
}

// build the rope's bvh tree, one node/knot relates to 2 points
void BVHTree::BuildTree(){
////////////////////////////////////////////////////////////////////////// the leaf nodes
	Vector3f VecPos1;
	VecPos1.setVector(rope4tree->points_vec[0]);

	int i, numNode;
	numNode = rope4tree->num_points - 1; // the number of the knot of the rope, the knot is the leave in the bvh tree
	numLeavesNode = numNode;
	BVHNode** leaves = new BVHNode*[numNode];
	Vector3f VecPos2, center;
	double halfLength, radius;
	for ( i = 0; i < numNode; i++ ){ 
		
		VecPos2.setVector(rope4tree->points_vec[i+1]);

		center = ( VecPos1 + VecPos2 ) / 2;
		halfLength = rope4tree->length_points / 2;
		radius = sqrt( rope4tree->wid * rope4tree->wid + halfLength*halfLength ); // 0.15*0.15 = 0.0225

		leaves[i] = new BVHNode(i, radius,center, NULL, NULL, NULL, NULL, NULL, 0, VecPos1, VecPos2); // level 0 means the leaf node
		VecPos1 = VecPos2;
	}

	// set the neighbor of the knot
	if( numNode != 0 ){
		//leaves[0]->SetLeftNeighbor(NULL);
		leaves[0]->SetRightNeighbor(leaves[1]);

		for( i = 1; i < numNode - 1; i++ ){
			leaves[i]->SetLeftNeighbor(leaves[i-1]);
			leaves[i]->SetRightNeighbor(leaves[i+1]);
		}
		leaves[numNode-1]->SetLeftNeighbor( leaves[numNode-2] );
		//leaves[numNode-1]->SetRightNeighbor(NULL);
	}

	//BVHNode** test = new BVHNode*[1];
	//double ra = 12.0;
	//test[0] = new BVHNode(ra, center, NULL, NULL, NULL, NULL, NULL, 0, VecPos1, VecPos2);
	//
	//BVHNode** target = new BVHNode*[1];
	//memcpy(target, test, sizeof(BVHNode*));
	//cout << target[0]->radius << endl;
	//test[0]->level = 2;
	//cout << target[0]->level << endl;
	//target[0]->level = 3;
	//cout << target[0]->level << endl;

	//float* test ;
	//test = new float[2];
	//test[0] = 1.0;
	//test[1] = 2.0;
	//float* target = new float[2];
	//memcpy(target, test, 2*sizeof(float));
	//cout << target[0] << endl;
	//test[0] += 3.0;
	//cout << target[0] << endl;

////////////////////////////////////////////////////////////////////////// build the hierarchical tree with the background of the leaves
	this->leavesList = new BVHNode*[numNode];
	memcpy( this->leavesList, leaves, numNode * sizeof(BVHNode*) ); // ע�����滹����leaves[]���и�ֵ����Ϊ�����leaves[]�еĸ��ڵ㻹�д�ȷ��
//////////////////////////////////////////////////////////////////////////
	bool isOdd;
	int numUpper; //  number of elements of the upper level, numUpper should plus 1 if numNode is odd
	int curlev = 1;
	
	while (numNode > 1){
		isOdd = numNode % 2 != 0;
		numUpper = numNode / 2; // ���������������ô���һ��node��Ϊuplev��һ��node
		numNode = numUpper; // numNode�Ѿ�������ж�Ҷ�ӽڵ��������ż�����⣬�������һ��numNode�Է���ش����������ż���⡣��������numNode�����ǵ�ǰnode�ĸ�����

		if (isOdd)
			numUpper++;
		
		BVHNode** upLev = new BVHNode*[numUpper];
		// considering the general conditions, odd or even
		for (int i = 0; i < numNode; i++){
			BVHNode* leftChild = leaves[i*2];
			BVHNode* rightChild = leaves[i*2+1];

			radius = leftChild->radius + rightChild->radius;
			center = (leftChild->centerpos + rightChild->centerpos) / 2;
			upLev[i] = new BVHNode(0, radius, center, NULL, leftChild, rightChild, NULL, NULL, curlev, Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, 0.0, 0.0));
			//////////////////////////////////////////////////////////////////////////
			//if ( curlev == 1 ){
			//	level1List.push_back(upLev[i]);
			//}
			//////////////////////////////////////////////////////////////////////////
			leaves[i*2]->SetPar(upLev[i]); // ͬʱ�ı�leavesList
			leaves[i*2+1]->SetPar(upLev[i]);
		}

		if (isOdd){ // �����i�Ѿ��ǵ���numNode�ˣ��������numNode����Ҷ�ӽڵ�ĸ�������2��ע����ż
			upLev[numNode] = leaves[numNode*2]; // ���һ��Ҷ�ӽڵ��index��Ҷ�ӽڵ����-1��
			//if ( curlev == 1 ){
			//	level1List.push_back(upLev[numNode]);
			//}
		}
		
		delete[] leaves; // BVHNode** leaves��BVHNode* leaves[Ҷ�ӽڵ����]
		leaves = upLev;
		curlev++;

		numNode = numUpper;
	}
	root = leaves[0];
	//cout << level1List.size() << endl;

	//cout << leavesList[0]->radius << endl;
	//cout << leavesList[98]->radius << endl;
	//cout << root->radius << endl;
//////////////////////////////////////////////////////////////////////////
//	int numArrayElement = numNode; // ��Ӧ���ӽڵ�������ں����н���Ϊ��һ��ѭ���е��ӽڵ���� 
//	//int numNode; // ������Ҫ����Ľڵ�ĸ���
//	bool numElementIsOdd; // �ж��ӽڵ�����Ƿ����������������odd��Ҫ�����һ���ӽڵ���Ϊ��һ���ڵ�������
//	int level = 1; // �ڵ��level�� Ҷ�ӽڵ��level��0�����ڵ�root��level���
//	// �Ե�����ѭ��
//	while (numArrayElement > 1){
//		numElementIsOdd = (numArrayElement % 2 != 0);
//		numNode = numArrayElement / 2; // here numNode is dependent on the numElementIsOdd, should pay attention to the odd condition
//
//		numArrayElement = numNode; // numArrayElement��ӦΪ��һ��ѭ���е��ӽڵ����������ǰѭ����Uplevel[]
//
//		if ( numElementIsOdd ) // numNode�Ǳ���������numElementIsOdd
//			numArrayElement += 1;
//		
//		BVHNode** upLevel = new BVHNode*[numArrayElement] ; // $$$ ÿ��ѭ���ж�Ҫ�½����Ľڵ������ﶨ�壬����ʼ����������Ҫ���е�forѭ����
//
//		// ���������������forѭ���⿼��
//		for ( int i = 0; i < numNode; i++ ){ 
//			BVHNode* LocalLeftChild = leaves[i*2];
//			BVHNode* LocalRightChild = leaves[i*2+1];
//	
//			//double radius = leaves[i*2]->radius + leaves[i*2+1]->radius;
//			//Vector3f center = (leaves[i*2]->centerpos + leaves[i*2+1]->centerpos) / 2;
//
//			double radius = LocalLeftChild->radius + LocalRightChild->radius;
//			Vector3f center = (LocalLeftChild->centerpos + LocalRightChild->centerpos) / 2;
//			upLevel[i] = new BVHNode(radius, center, NULL, LocalLeftChild, LocalRightChild, NULL, NULL, level, Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, 0.0, 0.0)); // ���ڵ���NULL�� ֻ��Ҷ�ӽڵ���������ھӣ������������ھӶ���NULL
//			// Ϊ������ӽڵ����븸�ڵ���Ϣ��
//			leaves[i*2]->SetPar(upLevel[i]);
//			leaves[i*2+1]->SetPar(upLevel[i]);
//		}
//
//		// ���洦�������������
//		if (numElementIsOdd)
//			upLevel[numArrayElement-1] = leaves[numNode*2];
//////////////////////////////////////////////////////////////////////////// ����leaves������Ϣ
//		delete[] leaves;
//		leaves = upLevel;
////////////////////////////////////////////////////////////////////////////
//		level++;
//	}
////////////////////////////////////////////////////////////////////////////
//	root = leaves[0];
}

// self collision, need further improvement and refinement
void BVHTree::FindCollision(BVHNode* node1, BVHNode* node2){
	//if ( node1->level == 0 && node2->level == 0 ){ // �������������ڵ㶼��Ҷ�ӽڵ� // �������������ڵ���Ҷ�ӽڵ������ھӣ��򷵻�
	//	fout2 << "leaves: " << node1->sn << "\t" << node2->sn << endl;
	//	return;
	//}

	if ( !node1->Collide(node2) ) // �������������ڵ�û����ײ���򷵻�
		return;

	//if ( node1->level == 0 && node2->level == 0 ){ // �������������ڵ㶼��Ҷ�ӽڵ� // �������������ڵ���Ҷ�ӽڵ������ھӣ��򷵻�
		//if ( node1->IsNeighbor(node2) ){ // ��������Ų���ʡȥ�������ܴﵽ�ݹ��Ч��
		//	return;
		//}else{ // ����ͼ�����ڵ��ǲ����ڵ�Ҷ�ӽڵ�
			float dis = (node1->centerpos - node2->centerpos).getLength();

			if ( dis > 0 && dis < ( rope4tree->length_points - BVHNode::deviance ) ){ // length_points = wid * 2;
				collisions.push_back( CollidePair(node1, node2) );
				cout << "collision happened: " << node1->sn << "\t" << node2->sn << endl;
				cout << "distance of centerpos: " << dis << endl;
				fout2 << "find self collision: " <<  node1->sn << "\t" << node2->sn << endl;
				fout2 << "distance of centerpos: " << dis << endl;

				if ( collisionColorFlag ){		
					colPoints.push_back(node1->centerpos);
					colPointsRadius.push_back(node1->radius);
					colPointsColor.push_back(1);

					colPoints.push_back(node2->centerpos);
					colPointsRadius.push_back(node2->radius);
					colPointsColor.push_back(1);
				}
			}
		//}
	//	return;
	//}else{ // ����ͼ�����ڵ㲻��Ҷ�ӽڵ�
	//	if ( node1->level > node2->level ){ // �������
	//		FindCollision( node1->leftchild, node2 );
	//		FindCollision( node1->rightchild, node2 );
	//		//node1->sn = 1;
	//		//fout2 << "node1 after rightchild, sn: " << node1->sn << " level: " << node1->level << endl;
	//	}else{ // �Ҵ����� �������
	//		//FindCollision( node1, node2->leftchild );
	//		//FindCollision( node1, node2->rightchild );
	//		FindCollision( node2->leftchild, node1 );
	//		FindCollision( node2->rightchild, node1 );
	//		//node2->sn = 1;
	//		//fout2 << "node2 after rightchild, sn: " << node2->sn << " level: " << node2->level << endl;
	//	}
	//}
}

void BVHTree::SelfCollision(){
	CollisionClear();
	cout << "self collision start" << endl;
	
	//BVHNode* curfront = level1List.front();
	//for ( int i = 0; i < level1List.size() - 1; i++ ){ // �ⲿѭ�� n = level1List.size() - 1 �Σ���Ϊ���һ��û��Ҫ
	//	for ( int i = 0; i < (level1List.size() - (i+1)) - 1; i++ ){ // �ڲ�ѭ�� level1List.size() - (i + 1) ��
	//		level1List.pop_front();
	//		BVHNode* newfront = level1List.front();
	//		FindCollision( curfront, newfront );
	//	}
	//	curfront = level1List???
	//}	

	//BVHNode* cur;
	//BVHNode* next;

	for ( int i = 0; i < numLeavesNode - 1; i++ ){ // �ⲿѭ�� n-1 ��  ע��leavesList��Ŵ�0��numLeavesNode-1
		for ( int j = i + 2; j < numLeavesNode; j++ ) {// �ڲ�ѭ�� n-1 - (i+1) ��
			FindCollision(leavesList[i], leavesList[j]);
		}
	}
	cout << "selfcollision end, collisions.size(): " << collisions.size() << endl;
	fout2 << "selfcollision end, collisions.size(): " << collisions.size() << endl;
}

void BVHTree::CollisionClear(){
	colPoints.clear();
	colPointsColor.clear();
	colPointsRadius.clear();

	collisions.clear();
}
void BVHTree::UpdateNode(BVHNode* t){
	if ( t->level == 0 ){ // Ҷ�ӽڵ�
		Vector3f pos1 = t->p1;
		Vector3f pos2 = t->p2;

		// ����Ҷ�ӽڵ��Χ��
		Vector3f center = ( pos1 + pos2 ) / 2;
		t->SetCenter( center );

		double r = ( pos2 - pos1 ).getLength() / 2;
		t->SetRadii( r );
	}

	// ����Ľڵ㲻��Ҷ�ӽڵ�
	else{
		Vector3f center = ( t->leftchild->centerpos + t->rightchild->centerpos ) / 2;
		t->SetCenter( center );

		double r = t->leftchild->radius + t->rightchild->radius;
		t->SetRadii( r );
	}
}

void BVHTree::LeafNodeTree(){
	//colPoints.clear();
	//colPointsRadius.clear();
	//colPointsColor.clear();

	list<BVHNode*> nodeQueue; // need delete often, so use list,., not vector<>
	
	nodeQueue.push_back(root);

	while ( ! nodeQueue.empty() ){
		BVHNode* curNode = nodeQueue.front();

		nodeQueue.pop_front();
		if (curNode->leftchild != NULL)
			nodeQueue.push_back(curNode->leftchild);
		if (curNode->rightchild != NULL)
			nodeQueue.push_back(curNode->rightchild);

		if ( curNode->level == 0 ){
			colPoints.push_back(curNode->centerpos);
			colPointsRadius.push_back(curNode->radius);
			colPointsColor.push_back(0);
		}
		//if ( curNode->level == 1 ){
		//	colPoints.push_back(curNode->centerpos);
		//	colPointsRadius.push_back(curNode->radius);
		//	colPointsColor.push_back(1);
		//	//level1List.push_back(curNode);
		//}
		//if ( curNode->level == 2 ){
		//	colPoints.push_back(curNode->centerpos);
		//	colPointsRadius.push_back(curNode->radius);
		//	colPointsColor.push_back(2);
		//}
	}
}

void BVHTree::UpdateTree(){
	BVHNode** leaves;
	leaves = new BVHNode*[numLeavesNode];
	memcpy(leaves, leavesList, numLeavesNode * sizeof(BVHNode*));
	
	//for ( int i = 0; i < numLeavesNode; i++ ){
	//	fout2 << i << "\t" << leavesList[i]->centerpos.x << "\t" << leavesList[i]->centerpos.y << "\t" << leavesList[i]->centerpos.z << endl;
	//}

	bool isOdd;
	int numUpper; //  number of elements of the upper level, numUpper should plus 1 if numNode is odd
	int numNode = numLeavesNode;

	while (numNode > 1){
		for ( int i = 0; i < numNode; i++){
			UpdateNode(leaves[i]);
		}

		isOdd = numNode % 2 != 0;
		numUpper = numNode / 2; // ���������������ô���һ��node��Ϊuplev��һ��node
		numNode = numUpper; // numNode�Ѿ�������ж�Ҷ�ӽڵ��������ż�����⣬�������һ��numNode�Է���ش����������ż���⡣��������numNode�����ǵ�ǰnode�ĸ�����

		if (isOdd)
			numUpper++;

		BVHNode** upLev = new BVHNode*[numUpper];
		// considering the general conditions, odd or even
		for (int i = 0; i < numNode; i++){
			upLev[i] = leaves[i*2]->par ;
		}

		if (isOdd){ // �����i�Ѿ��ǵ���numNode�ˣ��������numNode����Ҷ�ӽڵ�ĸ�������2��ע����ż
			upLev[numNode] = leaves[numNode*2]; // ���һ��Ҷ�ӽڵ��index��Ҷ�ӽڵ����-1��
		}

		delete[] leaves; // BVHNode** leaves��BVHNode* leaves[Ҷ�ӽڵ����]
		leaves = upLev;

		numNode = numUpper;
	}
	UpdateNode(leaves[0]);
}

void BVHTree::setColColor(bool flag){
	collisionColorFlag = flag;
}

void BVHTree::UpdateTreeParticles(){
	for	( int i = 0; i < numLeavesNode; i++ ){
		leavesList[i]->p1.setVector(rope4tree->points_vec[i]);
		leavesList[i]->p2.setVector(rope4tree->points_vec[i+1]);
	}
}

void BVHTree::HandleSeflCollision(){
	//////////////////////////////////////////////////////////////////////////
	vector<CollisionCluster> colCluster;
	vector<CollisionCluster>::iterator itrr;
	int sizeOfcollisions = collisions.size();

	// at the current out-round if find the index already in the result produced in the previous out-round, then should pay attention to the replication, 
	// that's the result produced in the current round is the replication of the result produced in the previous out-round
	// and at the in-round, that's j; should pay attention to replication of collisions[i].anotherNode; 

	for ( int i = 0; i < sizeOfcollisions - 1; i++ ){ // out-round
		int sizeOfcluster = colCluster.size();
		int	indexOutRoundLeftNode = collisions[i].n1->sn;
		int indexOutRoundRightNode = collisions[i].n2->sn;
		//if ( /*i != 0 &&*/ sizeOfcluster != 0 ){ // save the out-round result, be aware of that i != 0;
		//	int* outRoundClusterId = new int[sizeOfcluster];
		//	for ( int k = 0; k < sizeOfcluster; k++ ){
		//		outRoundClusterId[k] = colCluster[k].id;
		//	}
		//}
		vector<CollisionCluster> colClusterInround; // result of the in-round
		for ( int j = i + 1; j < sizeOfcollisions; j++ ){ // in-round
			int sizeClusterInround = colClusterInround.size();
			vector<int> clusterObj;
			bool exitInCluster = false;
			bool alreadyInOutResult = false;
			int indexInRoundLeftNode = collisions[j].n1->sn;
			int indexInRoundRightNode = collisions[j].n2->sn;


			if ( indexOutRoundLeftNode == indexInRoundLeftNode ){ // collisions[i].n1->sn == collisions[j].n1->sn
				// make sure that this in-round result is not the copy of the out-round result
				for ( int k = 0; k < sizeOfcluster; k++ ){
					if ( indexOutRoundLeftNode == colCluster[k].id ){
						alreadyInOutResult = true;
					} // if ()
				} // for ()
				if ( !alreadyInOutResult ){ // traverse the existed in-round result
					for ( int k = 0; k < sizeClusterInround ; k++ ){
						if ( indexOutRoundLeftNode == colClusterInround[k].id ){
							colClusterInround[k].binding.push_back(indexInRoundRightNode);
							exitInCluster = true; 
							break;
						} // if ()
					} // for ()
					// if not exited
					if ( !exitInCluster ){
						clusterObj.push_back(indexOutRoundRightNode);
						clusterObj.push_back(indexInRoundRightNode);
						colClusterInround.push_back(CollisionCluster(indexOutRoundLeftNode, clusterObj));
					} // if ()
				} // if ()
			}else if ( indexOutRoundLeftNode == indexInRoundRightNode ){ // collisions[i].n1->sn == collisions[j].n2->sn
				for ( int k = 0; k < sizeOfcluster; k++ ){
					if ( indexOutRoundLeftNode == colCluster[k].id ){
						alreadyInOutResult = true;
					}
				}

				if ( !alreadyInOutResult ){
					for ( int k = 0; k < sizeClusterInround; k++ ){
						if ( indexOutRoundLeftNode == colClusterInround[k].id ){
							colClusterInround[k].binding.push_back(indexInRoundLeftNode);
							exitInCluster = true;
							break;
						} // if ()
					} // for ()

					if ( !exitInCluster ){
						clusterObj.push_back(indexOutRoundRightNode);
						clusterObj.push_back(indexInRoundLeftNode);
						colClusterInround.push_back(CollisionCluster(indexOutRoundLeftNode, clusterObj));
					} // if ()
				} // if ()
			}else if ( indexOutRoundRightNode == indexInRoundLeftNode ){ // collisions[i].n2->sn == collisions[j].n1->sn
				for ( int k = 0; k < sizeOfcluster; k++ ){
					if ( indexOutRoundRightNode == colCluster[k].id )
						alreadyInOutResult = true;
				} // for ()

				if ( !alreadyInOutResult ){
					for ( int k = 0; k < sizeClusterInround; k++ ){
						if ( indexOutRoundRightNode == colClusterInround[k].id ){
							colClusterInround[k].binding.push_back(indexInRoundRightNode);
							exitInCluster = true;
							break;
						} // if ()
					} // for ()				
					if ( !exitInCluster ){
						clusterObj.push_back(indexOutRoundLeftNode);
						clusterObj.push_back(indexInRoundRightNode);
						colClusterInround.push_back(CollisionCluster(indexOutRoundRightNode, clusterObj));
					} // if ()
				} // if()
			}else if ( indexOutRoundRightNode == indexInRoundRightNode ){ // collisions[i].n2->sn == collisions[j].n2->sn
				for ( int k = 0; k < sizeOfcluster; k++ ){
					if ( indexOutRoundRightNode == colCluster[k].id )
						alreadyInOutResult = true;
				}

				if ( !alreadyInOutResult ){
					for ( int k = 0; k < sizeClusterInround; k++ ){
						if ( indexOutRoundRightNode == colClusterInround[k].id ){
							colClusterInround[k].binding.push_back(indexInRoundLeftNode);
							exitInCluster = true;
							break;
						} // if ()
					} // for ()
					if ( !exitInCluster ){
						clusterObj.push_back(indexOutRoundLeftNode);
						clusterObj.push_back(indexInRoundLeftNode);
						colClusterInround.push_back(CollisionCluster(indexOutRoundRightNode, clusterObj));
					} // if()
				} // if()
			} // else if()
		} // in-round for()
		for ( int k = 0; k < colClusterInround.size(); k++ ){
			colCluster.push_back(colClusterInround[k]);
		}
	} // out-round for()
	fout2 << "cluster " << endl;
	for ( int i = 0; i < colCluster.size(); i++ ){
		if ( colCluster[i].binding.size > 2 ){
			colCluster[i].binding[j]
		}
		fout2 << colCluster[i].id << endl;
		for ( int j = 0; j < colCluster[i].binding.size(); j++ ){
			fout2 << colCluster[i].binding[j] << "\t";
		}
		fout2 << endl;
	}
//////////////////////////////////////////////////////////////////////////
	//vector<pair(int, int)> clusterbuffer; // if 3 leaf nodes are detected colliding with the same leaf node, then these 3 leaf nodes is a cluster
	Vector3f vec;
	float length;
	vector<CollidePair>::iterator itr;
	int farindex;
	int fardist = -1;
	bool isn1 = true;
	int dis2ctrlPt = 0;
	
	cout << "handleselfcollision: " << endl;
	fout2 << "handleselfcollision: " << endl;

	float bigLength = 10.0;
	int bigI = 0;
	for ( int i = 0; i < collisions.size(); i++ ){
		vec = collisions[i].n1->centerpos - collisions[i].n2->centerpos;
		length = vec.getLength();
		if ( length < bigLength ){
			bigLength = length;
			bigI = i;
		}
	}
	if ( abs(collisions[bigI].n1->sn - ctrlPt) < abs(collisions[bigI].n2->sn - ctrlPt) )
		isn1 = false;


 //	for ( itr = collisions.begin(); itr != collisions.end(); itr++, i++ ){
	//	if ( abs((*itr).n1->sn - ctrlPt) > abs((*itr).n2->sn - ctrlPt) ){
	//		//farindex = i; // collisions[i].n1
	//		dis2ctrlPt = abs((*itr).n1->sn - ctrlPt); 
	//	}else{
	//		isn1 = false; // collisions[i].n2
	//		//farindex = i;
	//		dis2ctrlPt = abs((*itr).n2->sn - ctrlPt);
	//	}
	//	//cout << "ctrlPt, dis2ctrlpt & fardist: " << ctrlPt << " VS " << dis2ctrlPt << " & " << fardist << endl;
	//	//fout2 << "ctrlPt, dis2ctrlpt & fardist: " << ctrlPt << " VS " << dis2ctrlPt << " & " << fardist << endl;
	//	if ( dis2ctrlPt > fardist ){
	//		fardist = dis2ctrlPt;
	//		farindex = i;
	//	}
	//	//cout << "after if (dis2ctrlpt > fardist): " << dis2ctrlPt << " & " << fardist << endl;
	//	//fout2 << "after if (dis2ctrlpt > fardist): " << dis2ctrlPt << " & " << fardist << endl;
	//}
	farindex = bigI;
	vec = collisions[farindex].n2->centerpos - collisions[farindex].n1->centerpos;
	length = vec.getLength();
	cout << "collisions distance: " << length << endl;
	fout2 << "collisions distance: " << length << endl;
	vec = vec.Normalize() * ( rope4tree->length_points - length - BVHNode::deviance );
	//if ( abs(vec.getLength()) < BVHNode::deviance / 2.0 ){
	//	vec = vec * 2.0;
	//}
	cout << "handle self collision(leaf node): " << collisions[farindex].n1->sn << " & " << collisions[farindex].n2->sn << "\t" << vec.getLength() << endl;
	fout2 << "handle self collision(leaf node): " << collisions[farindex].n1->sn << " & " << collisions[farindex].n2->sn << "\t" << vec.getLength() << endl;
	if ( isn1 ){
		cout << "MoveConsecutive, collidePairIndex &  nodeIndex: " << farindex << " & " << collisions[farindex].n1->sn << endl;
		fout2 << "MoveConsecutive, collidePairIndex &  nodeIndex: " << farindex << " & " << collisions[farindex].n1->sn << endl;
		rope4tree->MoveConsecutive(collisions[farindex].n1->sn, 2, vec.Negative());
	}else{
		cout << "MoveConsecutive, collidePairIndex &  nodeIndex: " << farindex << " & " << collisions[farindex].n2->sn << endl;
		fout2 << "MoveConsecutive, collidePairIndex &  nodeIndex: " << farindex << " & " << collisions[farindex].n2->sn << endl;
		rope4tree->MoveConsecutive(collisions[farindex].n2->sn, 2, vec);
	}
//////////////////////////////////////////////////////////////////////////
	//	vec = (*itr).n2->centerpos - (*itr).n1->centerpos;
	//	length = vec.getLength();
	//	vec = vec.Normalize() * ( rope4tree->length_points - length );
	//	fout2 << "handle self collision:(leaf node not particle) " << (*itr).n1->sn << " & " << (*itr).n2->sn << "\t" << vec.getLength() << endl;

	//	if ( abs( (*itr).n1->sn - ctrlPt ) < abs( (*itr).n2->sn - ctrlPt ) ){
	//		rope4tree->MoveConsecutive( (*itr).n2->sn, 3, vec );

	//		//rope4tree->Move( vec, (*itr).n2->sn );

	//		//rope4tree->Move( vec, (*itr).n2->sn + 1 );
 //
	//	}else{
	//		rope4tree->MoveConsecutive( (*itr).n1->sn, 3, vec.Negative() );
	//		//rope4tree->Move( vec.Negative(), (*itr).n1->sn );

	//		//rope4tree->Move( vec.Negative(), (*itr).n1->sn + 1 );
	//	}
	//}
}