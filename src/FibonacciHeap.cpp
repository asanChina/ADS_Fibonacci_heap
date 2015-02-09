#include "FibonacciHeap.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;

//we perform action as the slide 12 page 6 indicated
void FibonacciHeap::insert(TreeNode* anotherTree)
{
	anotherTree->parent = NULL;
	anotherTree->childCut = false;
	
	if(minimumPointer == NULL) //if the original fibonacci heap is empty
	{
		minimumPointer = anotherTree;
		return;
	}
	
	TreeNode* rightsibling = minimumPointer->rightSibling;
	minimumPointer->rightSibling = anotherTree;
	anotherTree->leftSibling = minimumPointer;
	anotherTree->rightSibling = rightsibling;
	rightsibling->leftSibling = anotherTree;
	

	//now need to reset the minimum pointer if necessary
	if(anotherTree->data < minimumPointer->data)
		minimumPointer = anotherTree;
	
	//even actual time complexity for inserting is O(1), but we still call "pairwiseCombine(...)". we can comment 
	// this statement out and achieve the same time complexity. But I include it here for personal preference
	pairwiseCombine(minimumPointer);
}


void FibonacciHeap::meld(FibonacciHeap& anotherHeap)
{
	if(anotherHeap.minimumPointer == NULL)
		return;
		
	if(minimumPointer == NULL)
	{
		*(this) = anotherHeap;
		return;
	}
		
	//for below 6 statement, please draw correct figures to get the idea why you should cross
	// the pointer to meld two doubly circular linked list
	TreeNode* rightsibling = minimumPointer->rightSibling;
	TreeNode* rightsibling1 = anotherHeap.minimumPointer->rightSibling;
	minimumPointer->rightSibling = rightsibling1;
	anotherHeap.minimumPointer->rightSibling = rightsibling; 
	rightsibling->leftSibling = anotherHeap.minimumPointer;
	rightsibling1->leftSibling = minimumPointer;
	
	if(anotherHeap.minimumPointer->data < minimumPointer->data)
		minimumPointer = anotherHeap.minimumPointer;
	
	pairwiseCombine(minimumPointer); //we can also comment out this sentence
}
		
		
TreeNode* FibonacciHeap::getMin()
{
	return minimumPointer; //caller of this function should be careful with situation when "minimumPointer" is NULL
}


bool FibonacciHeap::removeMin()
{

	if(minimumPointer == NULL)
		return false;
		
	//be careful with situation when there is only one min tree in the fibonacci heap (remember fibonacci heap is collection of min trees)
	TreeNode* next = minimumPointer->rightSibling;
	minimumPointer->rightSibling = NULL;
	if(next->rightSibling == NULL)//means only one min tree in the original fibonacci heap
	{
		minimumPointer = minimumPointer->child;
		delete next;
		next = NULL;
		pairwiseCombine(minimumPointer);
		return true;
	}
	
	//otherwise there are more than one min tree in top level
	TreeNode* child = minimumPointer->child;
	next->leftSibling = minimumPointer->leftSibling;
	minimumPointer->leftSibling->rightSibling = next;

	delete minimumPointer;
	minimumPointer = next;


	//now we need to insert children into the top level
	if(child != NULL)
	{
		//for below 6 statements, please draw correct figures to get the idea
		// why you should cross reset the pointers to meld two doubly circular linked list
		TreeNode* rightsibling = minimumPointer->rightSibling;
		TreeNode* rightsibling1 = child->rightSibling;
		minimumPointer->rightSibling = rightsibling1;
		child->rightSibling = rightsibling;
		rightsibling->leftSibling = child;
		rightsibling1->leftSibling = minimumPointer;
	} 

	//do pairwise combine
	pairwiseCombine(minimumPointer); //this "pairwiseCombine(...)" can not be commented out
	return true;
}

//pairwiseCombine called in this function can not be commented out
bool FibonacciHeap::remove(TreeNode*& nodeInTheHeap)
{
	if(nodeInTheHeap == NULL) 
		return false;

	if(nodeInTheHeap->parent == NULL) //only node in the top level can have NULL parent
	{
		//we don't know whether "nodeInTheHeap" points to "minimum" node or not
		TreeNode* next = nodeInTheHeap->rightSibling;
		nodeInTheHeap->rightSibling = NULL;
		if(next->rightSibling == NULL) //means only one min tree in the top level, and "nodeInTheHeap" is the "minimumPointer"
		{
			minimumPointer = minimumPointer->child;
			delete nodeInTheHeap;
			nodeInTheHeap = NULL;
			pairwiseCombine(minimumPointer);
		}
		else
		{
			TreeNode* child = nodeInTheHeap->child;
			//first delete the "nodeInTheHeap" from the top level doubly circular linked list
			next->leftSibling = nodeInTheHeap->leftSibling;
			nodeInTheHeap->leftSibling->rightSibling = next;
			delete nodeInTheHeap;
			nodeInTheHeap = NULL;

			//be careful when "nodeInTheHeap" is the "minimumPointer"
			// so it's safe to set minimumPointer to an existed node
			minimumPointer = next;

			//now we need insert the child of "nodeInTheHeap" to the top level doubly circular linked list
			if(child != NULL)
			{
				//for below 6 statement, please draw correct figures to get the idea
				// why we need to cross set the pointers
				TreeNode* rightsibling = next->rightSibling;
				TreeNode* rightsibling1 = child->rightSibling;
				next->rightSibling = rightsibling1;
				child->rightSibling = rightsibling;
				rightsibling->leftSibling = child;
				rightsibling1->leftSibling = next;
			}
			pairwiseCombine(minimumPointer);
		}
		return true;
	}
	else //otherwise "nodeInTheHeap" is not a top level node, then we must do cascading cut
	{
		TreeNode* parent = nodeInTheHeap->parent;
		TreeNode* child = nodeInTheHeap->child;

		//first try to put all children into the top level doubly circular linked list
		if(child != NULL)
		{
			TreeNode* rightsibling = minimumPointer->rightSibling;
			TreeNode* rightsibling1 = child->rightSibling;
			minimumPointer->rightSibling = rightsibling1;
			child->rightSibling = rightsibling;
			rightsibling->leftSibling = child;
			rightsibling1->leftSibling = minimumPointer;
		}
		
		//now delete "nodeInTheHeap" from its own doubly circular linked list
		//be careful with when "nodeInTheHeap" is point by its parent
		TreeNode* next = nodeInTheHeap->rightSibling;
		nodeInTheHeap->rightSibling = NULL;
		if(next->rightSibling == NULL) //means "nodeInTheHeap" is the only one node in its own doubly circular linked list
		{
			parent->child = NULL;
			parent->degree--;
			delete nodeInTheHeap;
			nodeInTheHeap = NULL;
		}
		else //in this case, "nodeInTheHeap" can either by pointed by its parent or not
		{
			if(parent->child->rightSibling == NULL) //"nodeInTheHeap" is pointed by its parent
			{
				next->leftSibling = nodeInTheHeap->leftSibling;
				nodeInTheHeap->leftSibling->rightSibling = next;
				parent->child = next;
				parent->degree--;
				delete nodeInTheHeap;
				nodeInTheHeap = NULL;
			}
			else //"nodeInTheHeap" isn't pointed by its parent
			{
				next->leftSibling = nodeInTheHeap->leftSibling;
				nodeInTheHeap->leftSibling->rightSibling = next;
				parent->degree--;
				delete nodeInTheHeap;
				nodeInTheHeap = NULL;
			}
		}
		
		//now do cascading cut
		TreeNode* current = parent; 
		while(current->parent != NULL && current->childCut == true) //current mustn't be node in top level doubly circular linked list
		{
			parent = current->parent;
			TreeNode* next = current->rightSibling;
			//try to pull out "current" from its doubly circular linked list
			current->rightSibling = NULL;
			if(next->rightSibling == NULL) //"current" is the only node in its doubly circular linked list
			{
				parent->child = NULL;
				parent->degree--;
				TreeNode* rightsibling = minimumPointer->rightSibling;
				minimumPointer->rightSibling = current;
				current->leftSibling = minimumPointer;
				current->rightSibling = rightsibling;
				rightsibling->leftSibling = current;
			}
			else //there are more than one nodes in current doubly circular linked list
			{
				//we should watch out whether "current" is pointed by "parent" or not
				if(parent->child->rightSibling == NULL) //pointed by parent
				{
					//first pull current out from its own doubly linked list
					next->leftSibling = current->leftSibling;
					current->leftSibling->rightSibling = next;
					//then reset child pointer and degree
					parent->child = next;
					parent->degree--;
					//then put current into top level doubly circular linked list
					TreeNode* rightsibling = minimumPointer->rightSibling;
					minimumPointer->rightSibling = current;
					current->leftSibling = minimumPointer;
					current->rightSibling = rightsibling;
					rightsibling->leftSibling = current;
				}
				else //not pointed by parent
				{
					//first pull current out of its own doubly circular linked list
					next->leftSibling = current->leftSibling;
					current->leftSibling->rightSibling = next;
					//then decrease parent's degree
					parent->degree--;
					//then put current into top level doubly circular linked list
					TreeNode* rightsibling = minimumPointer->rightSibling;
					minimumPointer->rightSibling = current;
					current->leftSibling = minimumPointer;
					current->rightSibling = rightsibling;
					rightsibling->leftSibling = current;
				}
			}
			current = parent;
		}
		
		if(current->childCut == false && current->parent != NULL)
			current->childCut = true;
		
		pairwiseCombine(minimumPointer);
		return true;
	}
}


bool FibonacciHeap::decreaseKey(TreeNode* nodeInTheHeap, int _newKey)
{
	if(minimumPointer == NULL)
		return false;
	if(nodeInTheHeap == NULL)
		return false;
	if(nodeInTheHeap->data <= _newKey)
		return false;
		
	//now change the key
	nodeInTheHeap->data = _newKey;
	//only when nodeInTheHeap is not in top level doubly circular linked list and its
	// new key is smaller than its parent's, we would do something
	if(nodeInTheHeap->parent != NULL && _newKey < nodeInTheHeap->parent->data)
	{
		TreeNode* current = nodeInTheHeap;
		TreeNode* parent = current->parent;
		//we first need to pull out current from its own doubly circular linked list and 
		// put it into top level doubly circular linked list
		TreeNode* next = current->rightSibling;
		current->rightSibling = NULL;
		if(next->rightSibling == NULL) //there is only one node in current's doubly circular linked list
		{
			//put current into the top level doubly circular linked list
			TreeNode* rightsibling = minimumPointer->rightSibling;
			minimumPointer->rightSibling = current;
			current->leftSibling = minimumPointer;
			current->rightSibling = rightsibling;
			rightsibling->leftSibling = current;
			//change something of the parent node
			parent->child = NULL;
			parent->degree--;
		}
		else //there are more than one nodes in current's doubly circular linked list
		{
			if(parent->child->rightSibling == NULL) //current is pointed by its parent
			{
				//we first pull current out of its own doubly circular linked list
				next->leftSibling = current->leftSibling;
				current->leftSibling->rightSibling = next;
				//then put current into the top level doubly circular linked list
				TreeNode* rightsibling = minimumPointer->rightSibling;
				minimumPointer->rightSibling = current;
				current->leftSibling = minimumPointer;
				current->rightSibling = rightsibling;
				rightsibling->leftSibling = current;
				//then change child pointer and degree of parent
				parent->child = next;
				parent->degree--;
			}
			else //current is not pointed by its parent
			{
				//we first pull current out of its own doubly circular linked list
				next->leftSibling = current->leftSibling;
				current->leftSibling->rightSibling = next;
				//then put current into the top level doubly circular linked list
				TreeNode* rightsibling = minimumPointer->rightSibling;
				minimumPointer->rightSibling = current;
				current->leftSibling = minimumPointer;
				current->rightSibling = rightsibling;
				rightsibling->leftSibling = current;
				//then change degree of parent
				parent->degree--;
			}
		}
		
		//now we may need to do cascading cut
		current = parent; 
		while(current->parent != NULL && current->childCut == true) //current mustn't be node in top level doubly circular linked list
		{
			parent = current->parent;
			TreeNode* next = current->rightSibling;
			//try to pull out "current" from its doubly circular linked list
			current->rightSibling = NULL;
			if(next->rightSibling == NULL) //"current" is the only node in its doubly circular linked list
			{
				parent->child = NULL;
				parent->degree--;
				TreeNode* rightsibling = minimumPointer->rightSibling;
				minimumPointer->rightSibling = current;
				current->leftSibling = minimumPointer;
				rightsibling->leftSibling = current;
				current->rightSibling = rightsibling;
			}
			else //there are more than one nodes in current doubly circular linked list
			{
				//we should watch out whether "current" is pointed by "parent" or not
				if(parent->child->rightSibling == NULL) //pointed by parent
				{
					//first pull current out from its own doubly linked list
					next->leftSibling = current->leftSibling;
					current->leftSibling->rightSibling = next;
					//then reset child pointer and degree
					parent->child = next;
					parent->degree--;
					//then put current into top level doubly circular linked list
					TreeNode* rightsibling = minimumPointer->rightSibling;
					minimumPointer->rightSibling = current;
					current->leftSibling = minimumPointer;
					current->rightSibling = rightsibling;
					rightsibling->leftSibling = current;
				}
				else //not pointed by parent
				{
					//first pull current out of its own doubly circular linked list
					next->leftSibling = current->leftSibling;
					current->leftSibling->rightSibling = next;
					//then decrease parent's degree
					parent->degree--;
					//then put current into top level doubly circular linked list
					TreeNode* rightsibling = minimumPointer->rightSibling;
					minimumPointer->rightSibling = current;
					current->leftSibling = minimumPointer;
					current->rightSibling = rightsibling;
					rightsibling->leftSibling = current;
				}
			}
			current = parent;
		}
		
		if(current->childCut == false && current->parent != NULL)
			current->childCut = true;
		
		pairwiseCombine(minimumPointer);
	}
}



		
void FibonacciHeap::pairwiseCombine(TreeNode *temporaryPointer)
{
	//if the fibonacci heap is already empty, no need to do pairwise combine
	if(temporaryPointer == NULL) 
		return;
		
	vector<TreeNode*> table(10); //we give the table 10 capacity for saving computation time, all entries would set to NULL
	
	//since our table's size is not "maxDegree+1", so we set this "highestDegree" to 
	// help us save time when at last we collect min trees from the table.
	int highestDegree = 0; 
	
	//remember "maxDegree" is the max degree among all the top level min trees.
	TreeNode *current = temporaryPointer->rightSibling;
	temporaryPointer->rightSibling = NULL;
	
	while(current != NULL)
	{
		TreeNode* next = current->rightSibling;

		current->rightSibling = current->leftSibling = current; //we immediately make the current min tree to a self-linked min tree
		current->parent = NULL;
		current->childCut = false;

		highestDegree = highestDegree > (current->degree) ? highestDegree: (current->degree);

		if(current->degree >= table.capacity()) // remember when capacity is "10", then the largest index is "9"
		{
			table.resize(table.capacity()*2); //of course you can times 3, times 4, times 5,..., but I think doubled it is better
			table[current->degree] = current;
		}
		else
		{
			int tempDegree = current->degree;
			if(table[tempDegree] == NULL)//There is no data in this entry
			{
				table[tempDegree] = current;
			}
			else //otherwise we should combine as slide 12 page 18~19 shows, and during one round, we may need to many combine
			{
				int count = 0;
				while(table[tempDegree] != NULL)
				{

					highestDegree = highestDegree > (tempDegree+1) ? highestDegree: (tempDegree+1);

					if(table[tempDegree]->data < current->data)
					{
						if(table[tempDegree]->child == NULL)
						{
							table[tempDegree]->child = current;	
							current->parent = table[tempDegree];
						}
						else
						{
							TreeNode* child = table[tempDegree]->child;
							TreeNode* rightsibling = child->rightSibling;
							child->rightSibling = current;
							current->leftSibling = child;
							current->rightSibling = rightsibling;
							rightsibling->leftSibling = current;
							current->parent = table[tempDegree];
						}
						(table[tempDegree]->degree)++;
						current = table[tempDegree];
						table[tempDegree] = NULL;
						tempDegree++;
						if(tempDegree >= table.capacity())
							table.resize(tempDegree*2); //for safety, we need to check
					}
					else
					{
						if(current->child == NULL)
						{
							current->child = table[tempDegree];
							table[tempDegree]->parent = current;
						}
						else
						{
							TreeNode* child = current->child;
							TreeNode* rightsibling = child->rightSibling;
							child->rightSibling = table[tempDegree];
							table[tempDegree]->leftSibling = child;
							table[tempDegree]->rightSibling = rightsibling;
							rightsibling->leftSibling = table[tempDegree];
							table[tempDegree]->parent = current;
						}
						(current->degree)++;
						table[tempDegree] = NULL;
						tempDegree++;
						if(tempDegree >= table.capacity())
							table.resize(tempDegree*2); //for safety, we need to check
					}
					count++;
				}
				table[tempDegree] = current;
			}
		}
		current = next;
	}
	
	//now we need to collect all entries in the table and reset the "minimumPointer"
	minimumPointer = NULL;
	for(int i = 0; i <= highestDegree; i++)
	{
		if(table[i] != NULL)
		{
			if(minimumPointer == NULL)
			{
				minimumPointer = table[i];
				minimumPointer->rightSibling = minimumPointer->leftSibling = minimumPointer;
			}
			else
			{
				//now insert new min tree to the top level doubly circular linked list
				TreeNode* rightsibling = minimumPointer->rightSibling;
				minimumPointer->rightSibling = table[i];
				table[i]->leftSibling = minimumPointer;
				table[i]->rightSibling = rightsibling;
				rightsibling->leftSibling = table[i];
				if(table[i]->data < minimumPointer->data)
					minimumPointer = table[i];
			}
		}
	}
}












