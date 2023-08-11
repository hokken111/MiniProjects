#include <iostream>

using namespace std;

bool exCompare(int &a, int &b) {
	return (a<b);
}

template <typename I>
void merge(I begin, I mid, I end, bool (*compare)(typename I::value_type &x, typename I::value_type &y)) {

	//Get the lengths of the left and the right part.
	int n1 = mid - begin;
	int n2 = end - mid;
	
	typedef typename I::value_type T;
	T* left = new T[n1];
	T* right = new T[n2];
	int i;

	//populate left and right arrays
	i = 0;
	for (I p=begin;i<n1;p++) {
		left[i] = *p;
		i++;
	}
	i = 0;
	for (I p=(mid);i<n2;p++) {
		right[i] = *p;
		i++;
	}
	

	T* result = new T[n1+n2];
	i = 0;
	int leftCtr = 0;
	int rightCtr = 0;

	//Merge the two sorted arrays 
	for (;i<n1+n2;i++) {
		if (leftCtr<n1 && rightCtr<n2) {
			if (compare(left[leftCtr],right[rightCtr])) {
				result[i] = left[leftCtr++];
			}
			else result[i] = right[rightCtr++];
		}
		else if (leftCtr<n1)
			result[i] = left[leftCtr++];
		else
			result[i] = right[rightCtr++];
	}
	i = 0;

	//Copy the sorted array back into the original array.
	for (I p=begin;p!=end;p++) {
		*p = result[i++];
	}
	delete[] result;
	delete[] left;
	delete[] right;
}
			
			

template<typename I>
void mergeSort(I begin, I end, bool (*compare)(typename I::value_type &x, typename I::value_type &y)) {

	int len = end - begin;
	if (len>1) {
		I mid = begin + len/2;
		//Sort both the halves
		mergeSort(begin, mid, compare);
		mergeSort(mid, end, compare);
		merge(begin, mid, end, compare);
	}

	return;
} 

