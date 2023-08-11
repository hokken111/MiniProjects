#ifndef _MYMAP_H_
#define _MYMAP_H_

struct server{
	double relevance;
	double xcoord;
	double ycoord;
	std::string name;
};

class mymap{
	private:
		std::string rootname;
		
		//vector of servers that get sorted
		std::vector<server> mapvec;
		
	public:
		//make a new map of minimum size double with root stored in map
		mymap(double, std::string);
		
		//add a new website to the map
		void addtomap(double, std::string, double, double);
		
		//sort the map calling mergesort
		void sort();
		
		//print out the names in the map
		void printOut();

};

#endif
