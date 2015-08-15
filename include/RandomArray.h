/**
* created by Pengjie Zhang: zhangpengjieufl@gmail.com
*/
#ifndef _RANDOM_ARRAY_H_
#define _RANDOM_ARRAY_H_

#include "Graph.h"

//This class is used to search minimum spanning tree on a graph, the underlying data structur is array
class RandomArray{
	public:
		RandomArray(){}
		~RandomArray(){}
	public:
		int searchMst(Graph &g);
};
#endif
