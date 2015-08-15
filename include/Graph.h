/**
* create by Pengjie Zhang, zhangpengjieufl@gmail.com
*/

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>

/**
* This class is used to represent a undirected graph, underlying graph is adjacecy list used by vector<vector<>>
* In fact, we provide other information about this graph. So this class "Graph" is indeed a wraper about vector<vector<..>>
*/
class Graph{
	public :
		//constructor which set corresponding information about the graph
		Graph(){
			numberOfVertices = 0;
			numberOfEdges = 0;
			connected = false;
		}
		//a copy constructor which do a deep copy
		Graph(const Graph &g){
			numberOfVertices = g.numberOfVertices;
			numberOfEdges = g.numberOfEdges;
			connected = g.connected;
			udGraph = g.udGraph;
		}
		~Graph(){}

	public:
		bool isConnected(); //return true if is connected otherwise false
		bool checkConnectedness(); //a function which will delegate its job to "dfs" or "bfs" to get the result whether this graph is connected or not
		bool buildRandomGraph(int vertices, int edges); //a function which is used to build random graph
		bool buildGraphFromFile(char *filePath); //a function which is used to build graph from reading file (the file must have the required format)
		std::vector<std::pair<int,int> > getNeighbors(int index); //return the neighbors of a vertex
	public:
		int getNumOfVertices();
		void setNumOfVertices(int _vertices);
		int getNumOfEdges();
		void setNumOfEdges(int _edges);

	private:
		//this private function is used to decide whether this graph is connected or not, we can write a "bfs(...)" to replace this one
		//decide by the user
		void dfs(int index, std::vector<bool> &con, int &count);

	private:
		//the underlying graph representation
		std::vector<std::vector<std::pair<int, int> > > udGraph;
		//graph can be a connected one or not
		bool connected;
		//the number of vertices hold by this graph
		int numberOfVertices;
		//the number of edges hold by this graph
		int numberOfEdges;
};

#endif
