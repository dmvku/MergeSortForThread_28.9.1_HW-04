#include "mergesortarray.h"

#include <iostream>
#include <cstdlib>
#include <thread>
#include <future>

Array::Array()
{
	
}

Array::~Array()
{

}

void Array::initArray()
{
    selectValue();
    int* array = new int[numberOfElements_];
    createArray(array);
    printArray(array, "Unsort array: \n");
    startSorting(array);
    delete[] array;
}

void Array::startSorting(int* array)
{	
    // checking for the maximum number of threads
    totalThreads_ = numberOfThreads_ <= numberOfCores_ * treadsPerCore_
        ? numberOfThreads_ : numberOfCores_ * treadsPerCore_;

    // checking for an even number of threads
    totalThreads_ = numberOfThreads_ % 2 == 0
        ? totalThreads_ : totalThreads_ - 1;

    std::cout << "CPU cores: " << numberOfCores_ << "\n";
    std::cout << "Maximum number of threads per core: "
        << treadsPerCore_ << "\n";    
    std::cout << "Sorting will be done in " << totalThreads_ << " threads\n";    
    
    multiThreadMergeSort(array, 0, numberOfElements_ - 1);
    printArray(array, "Sorted array: \n");	
}

void Array::selectValue()
{
	std::cout << "Select number of elements (> 1): ";
	std::cin >> numberOfElements_;
    if (numberOfElements_ < 2)
    {
        numberOfElements_ = 2 + rand() % abs(maxValueofElement_ + abs(0));
    }

	std::cout << "Select max value of elements: ";
	std::cin >> maxValueofElement_;

	std::cout << "Select number of threads: ";
	std::cin >> numberOfThreads_;	

    std::cout << "Select number of threads per core: ";
    std::cin >> treadsPerCore_;
}

void Array::createArray(int* array)
{
	for (int i{ 0 }; i < numberOfElements_; i++)
		array[i] = 0 + rand() % abs(maxValueofElement_ + abs(0));
}

void Array::merge(int* arr, int l, int m, int r)
{
    int nl = m - l + 1;
    int nr = r - m;
        
    int* left = new int[nl];
    int* right = new int[nr];
        
    for (int i = 0; i < nl; i++)
    {
        left[i] = arr[l + i];
    }

    for (int j = 0; j < nr; j++)
    {
        right[j] = arr[m + 1 + j];
    }

    int i = 0, j = 0;
    int k = l;  

    while (i < nl && j < nr) {
        
        if (left[i] <= right[j])
        {
            arr[k++] = left[i++];
        }
        else
        {
            arr[k++] = right[j++];
        }
    }
        
    while (i < nl)
    {
        arr[k++] = left[i++];
    }
        
    while (j < nr)
    {
        arr[k++] = right[j++];
    }
       
    delete[] left;
    delete[] right;
}

void Array::multiThreadMergeSort(int* array, int l, int r)
{    
    if (l >= r) 
    {
        return;
    }
        
    int m = (l + r - 1) / 2; 
    
    // Locking the mutex to check for free threads and start sorting parts
    mutex.lock();

    // uncomment to view the threads
    // std::cout << "Thread ID: " << std::this_thread::get_id()
    //     << "\tFree threads: " << totalThreads_ << "\n";

    if (totalThreads_ > 0) // sorting in one thread if 0
    {
        totalThreads_--;        
        std::future<void> left = std::async(std::launch::async,
            [this, array, l, m]() { Array::multiThreadMergeSort(array, l, m); }
        );        
        
        totalThreads_--;        
        std::future<void> right = std::async(std::launch::async,
            [this, array, m, r]() { Array::multiThreadMergeSort(array, m + 1, r); }
        );
        mutex.unlock();

        // waiting for the left and right parts to be ready
        left.wait();         
        right.wait();

        // Locking the mutex to add free threads
        mutex.lock();
        totalThreads_ += 2;
        mutex.unlock();
    }
    else
    {               
        mutex.unlock();
        multiThreadMergeSort(array, l, m);
        multiThreadMergeSort(array, m + 1, r);
    }

    merge(array, l, m, r);
}

void Array::printArray(int* array, std::string comment)
{
    std::cout << "\nNumber of elements : " << numberOfElements_ << "\n";
    std::cout << comment;

    for (int i{ 0 }; i < numberOfElements_; i++)
    {
        std::cout << array[i] << "\t";
    }
        
    std::cout << "\n\n";
}
