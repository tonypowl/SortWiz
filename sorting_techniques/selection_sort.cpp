#include <iostream>
#include <vector>
#include <algorithm>

void selection_sort(std::vector<int>&arr){
    int n = arr.size();
    for(int i=0; i<n-1;i++){
        int min_idx = i;
        for(int j=i+1; j<n;j++){
            if (arr[j]<arr[min_idx]){
                min_idx = j;
            }
        }
        std::swap(arr[i],arr[min_idx]);
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
    std::cout <<"Enter "<<n<<" integers:\n";
    for(int i=0; i<n; i++){
        std::cin >> arr[i];
    }
    std::cout <<"Original Array: ";
    printVector(arr);
    selection_sort(arr);
    std::cout << "Sorted Array (Selection Sort): ";
    printVector(arr);
    return 0;
}
