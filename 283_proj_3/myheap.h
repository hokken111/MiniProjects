using namespace std;

#ifndef _MYHEAP_H_
#define _MYHEAP_H_

struct datapoint{
	string first;
	int second;
	CBTNODE third;
	int added;
	int printint;
};


class myheap{
	public:
		//Construct empty heap
		myheap();
		
		//Destructor
		~myheap();
		
		//Add file with name and relevance to heap
		void addtoheap(string , int );
		
		//Print most relevant element
		int print();
		
		//Clear heap
		void clear();
		
		//Prints history up to last save point
		void printhistory();
		
		//Adds a new save point
		void save();
		
		//Reverts heap to last save (if no save, will be empty heap)
		void revert();

	private:
		bool checkNeedHeapify(CBTNODE nodeA, CBTNODE Aparent);
		void heapifyDown();
		void adddatapointtoheap(datapoint *);
		CompleteBinaryTree< datapoint* > bintree;
		vector< datapoint* > dumpster, dumpster2;
		vector< datapoint* > historicaldoc;
		int numhistoricaldoc, lastsaveat, historicaldocsize;


};

#endif
