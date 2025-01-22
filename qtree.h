#pragma once
#include "defines.hpp"


class qTree_Fast {

	struct Node {

		int id;

		// either all these are null or none are
		Node
			* tl = nullptr, * tr = nullptr,
			* bl = nullptr	, * br = nullptr
			;

		bool isLeaf();
		Node(int id);

	};


	Node* rootNode = nullptr;

public:

	// each leaf node has id != -1, this specifies location within this vector
	// which stores list of particle ids (locations within object array) of particles
	// if specific leaf
	vectorList<vectorList<int>> particleIdListList;
	
	qTree_Fast();





};


