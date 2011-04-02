#ifndef __BvhTree_h__
#define __BvhTree_h__

#include "RopeStruct.h"
#include "BvhNode.h"

#include <list>
#include <set>
#include <vector>
#include <iterator>

using namespace std;

typedef pair<int, int> INT_PAIR;

struct CollisionCluster{
public:
	CollisionCluster(int idi, vector<int> bind){
		id		= idi;
		binding = bind;
	};

	int			id;
	vector<int> binding;
};

// 发生碰撞的两个BVHNode*节点
struct CollidePair{
public:
	BVHNode* n1;
	BVHNode* n2;
	//Vector3f VecDist;
	//float dist;

	//CollidePair(BVHNode* nd1, BVHNode* nd2, Vector3f VecDis, float dis){
	CollidePair(BVHNode* nd1, BVHNode* nd2){
		n1 = nd1; 
		n2 = nd2;
		//VecDist = VecDis;
		//dist = dis;
	}
	//bool Replication(CollidePair r){
	//	if ( this->n1->sn == r.n1->sn || this->n2->sn == r.n2->sn )
	//		return true;
	//	else if ( this->n1->sn == r.n2->sn || this->n2->sn == r.n1->sn )
	//		return true;
	//	else 
	//		return false;
	//}
};

class BVHTree{
public:
	BVHTree();
	~BVHTree();

	void SetRope( RopeStruct* struct_rope );
	void BuildTree();
	void UpdateNode(BVHNode* t);
	void UpdateTree();
	void FindCollision(BVHNode* node1, BVHNode* node2);
	void SelfCollision();
	void HandleCollisionWithMesh();

	void LeafNodeTree(); // to draw the leaf nodes
	void CollisionClear(); // clear vector<CollidePair> collisions, vector<Vector3f> colPoints and etc. 

	void setColColor(bool flag);
	void UpdateTreeParticles();
	void HandleSeflCollision();

public:
	RopeStruct* rope4tree;
	Vector3f ropeConnectionVel; // rope connection velocity
	//BVHTree* collisionDetector;
	
	int numLeavesNode;
	BVHNode** leavesList;
	//vector<BVHNode*>  level1List;

	BVHNode* root;

	vector<CollidePair> collisions; // 发生碰撞的node，不是质点

	vector<Vector3f> colPoints; // leaf nodes' Vector3f center position
	vector<float> colPointsRadius;
	vector<int> colPointsColor;

	bool collisionColorFlag;

	int	ctrlPt;
};

#endif