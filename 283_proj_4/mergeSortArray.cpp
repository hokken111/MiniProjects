#include <iostream>

using namespace std;

bool exCompare(int a, int b) {
	return (a<b);
}

template <class T>
void merge(T *left, int leftLength, T *right, int rightLength, bool (*compare) (T x, T y), T *result) {

	int totalLength = leftLength + rightLength;
	result = new T[totalLength];

	int leftCtr = 0;
	int rightCtr = 0;
	for (int i=0;i<totalLength;i++)	{
		if (leftCtr<leftLength && rightCtr<rightLength)	{
			if (compare(left[leftCtr],right[rightCtr])==true) {
				result[i] = left[leftCtr++];
				continue;
			}
			else {
				result[i] = right[rightCtr++];
				continue;
			}
		}

		if (leftCtr>=leftLength) {
			result[i] = right[rightCtr++];
		}
		else {
			result[i] = left[leftCtr++];
		}

	}
}

template <class T>
void mergeSort(T  *list, int n, bool (*compare)(T x, T y)) {
	if (n>1) {
		int middle = n/2;

		//Split the array into two halves
		T *left = new T[middle];
		T *right = new T[n-middle];

		//Populate both the halves
		for (int i=0;i< middle;i++) {
			left[i] = list[i];
		}
		for (int i=middle,j = 0;i<n;j++,i++) {
			right[j] = list[i];
		}

		//Sort both the halves		
		mergeSort(left,middle,compare);
		mergeSort(right,n-middle,compare);

		//Merge the sorted halves, left and right and merge them and store it in list
		merge(left,middle,right,n-middle,compare,list);
	}
}


