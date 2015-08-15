/**
* created by Pengjie Zhang: zhangpengjieufl@gmail.com
*/
#ifndef _RANDOM_FI_HEAP_H_
#define _RANDOM_FI_HEAP_H_

#include "FibonacciHeap.h"
#include "Graph.h"

//This class is build random graph then search minimum spanning tree on that graph
class RandomFiHeap{
	public:
		RandomFiHeap(){}
		~RandomFiHeap(){}
	public:
		int searchMst(Graph &g);
};

#endif
