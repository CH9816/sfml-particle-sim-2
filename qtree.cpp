

#include "qtree.h"




qTree_Fast::Node::isLeaf() {
	return tl == nullptr;
}

qTree_Fast::Node::Node(int id) {
	this->id = id;
}



qTree_Fast::qTree_Fast() {
	rootNode = Node(0);
	
}