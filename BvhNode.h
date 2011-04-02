#ifndef __BvhNode_h__
#define __BvhNode_h__

//#include <iostream>
#include <fstream>

class BVHNode{
public:
	BVHNode(){};
	~BVHNode(){};

	//BVHNode(double rad,Vector3f cen,BVHNode* p,BVHNode* l,BVHNode* r,BVHNode* ln,BVHNode* rn,int lev, Vector3f pt1, Vector3f pt2):
	//radius(rad),centerpos(cen),par(p),leftchild(l),rightchild(r),leftneighbor(ln),rightneighbor(rn),level(lev), p1(pt1), p2(pt2){};
	BVHNode(int serialNum,double& rad,Vector3f& cen,BVHNode* p,BVHNode* l,BVHNode* r,BVHNode* ln,BVHNode* rn,int lev, Vector3f pt1, Vector3f pt2):
	sn(serialNum),radius(rad),centerpos(cen),par(p),leftchild(l),rightchild(r),leftneighbor(ln),rightneighbor(rn),level(lev), p1(pt1), p2(pt2){};
	
	void SetCenter(Vector3f center){ centerpos = center; }
	void SetRadii(double r){ radius = r; }
	
	void SetLeftchild(BVHNode* l){ leftchild = l; }
	void SetRightchild(BVHNode* r){ rightchild = r; }
	void SetPar(BVHNode* p){ par  = p; } // set parent node
	
	void SetLeftNeighbor(BVHNode* l){ leftneighbor = l; } // 只有叶子节点才有邻居
	void SetRightNeighbor(BVHNode* r){ rightneighbor = r; }

	bool Collide(BVHNode* nodeRight){ // 如果两个节点的中心距小于两个节点的半径之和，则表明发生碰撞
		double dcenter = (nodeRight->centerpos - this->centerpos).getLength();
		double dradius = (nodeRight->radius + this->radius) - deviance; // $$ deviance

		if (dradius > dcenter){
			return true;
		}
		else{
			return false;
		}
	}

	bool IsNeighbor(BVHNode* node1){ // 判断两叶子节点是否是邻居
		//if ( node1->par)
		//if ( this->par == node1->par || this->par->par == node1->par->par ) // !!! 待纠正 ￥￥￥
		//	return true;
		if ( abs(this->sn - node1->sn) <= 1 )
			return true;
		else 
			return false;
	}

	bool IsLeaf(){}
	
public:
	double radius;
	Vector3f centerpos;

	BVHNode* par; // parent node
	BVHNode* leftchild;
	BVHNode* rightchild;
	BVHNode* leftneighbor; // neighbors only exist in leaf level
	BVHNode* rightneighbor;

	static double deviance; // 在BvhTree中初始化
	int level; // level 0 means leaf,根节点level最高

	Vector3f p1; // 叶子节点的两个质点
	Vector3f p2;

	int sn; // 叶子节点的序号，非叶子节点时作为遍历标志（1），初始化为（0）
};

#endif