#include <iostream>  
#include <vector>    
#include <algorithm> 

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size(); 
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void printVector(const std::vector<int>& arr) {
    for (int x : arr) { 
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

int main() {
    int n;
    std::cout << "Enter the number of elements: ";
    std::cin >> n;
    std::vector<int> arr(n);
    std::cout << "Enter " << n << " integers:\n";
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i]; 
    }
    std::cout << "Original array: ";
    printVector(arr);
    bubbleSort(arr); 
    std::cout << "Sorted array: ";
    printVector(arr);
    return 0;
}