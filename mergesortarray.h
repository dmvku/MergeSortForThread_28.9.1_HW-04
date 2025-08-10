#pragma once

#include <string>
#include <thread>
#include <mutex>

extern const char* outFile;

class Array
{
public:
	Array();
	~Array();

	void initArray();

private:	
	int numberOfElements_{ 0 };
	int maxValueofElement_{ 0 };
	int numberOfThreads_{ 0 };	
	int numberOfCores_ = std::thread::hardware_concurrency();
	int treadsPerCore_{ 0 };
	int totalThreads_{ 0 };
	std::mutex mutex;
	
	void startSorting(int* array);
	void selectValue();
	void createArray(int* array);
	void merge(int* arr, int l, int m, int r);
	void multiThreadMergeSort(int* arr, int l, int r);	
	void printArray(int* array, std::string comment);	
};