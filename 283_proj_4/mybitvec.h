#ifndef _MYBITVEC_H_
#define _MYBITVEC_H_

class mybitvec{
	private:
		//the bitvec for the root
		std::vector<unsigned char> myvec;
		
		//variable for number of chars/property and propertysize
		int globalsize, propertysize;

		//the root's property in string form
		std::string rootnum;
		
	public:
		//constructor that initializes all the bitvec to the root
		mybitvec(int , std::string * );
		
		//returns how relevant a property is compared to the root
		double returnrelevance(std::string *);
};

#endif
