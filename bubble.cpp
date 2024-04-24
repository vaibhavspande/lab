#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>

using namespace std;

void bubbleSort_s(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void bubbleSort_p(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i)
    {
#pragma omp parallel for
        for (int j = 0; j < n - 1 - i; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void showArray(const vector<int> &arr)
{
    for (int i = 0; i < arr.size(); ++i)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main()
{
    int size;
    cout << "Enter the size of the array: ";
    cin >> size;

    vector<int> arr(size);

    cout << "Enter " << size << " integers for the array: ";
    for (int i = 0; i < size; i++)
    {
        int val;
        arr[i] = rand() % 100 + 1;
    }

    cout << "\nUnsorted Array:\n";
    showArray(arr);

    // Sequential bubble sort
    auto start = chrono::high_resolution_clock::now();
    bubbleSort_s(arr);
    auto stop = chrono::high_resolution_clock::now();
    auto result = stop - start;
    auto duration = chrono::duration_cast<chrono::nanoseconds>(result);

    cout << "\nSequential Bubble sort: " << duration.count() << " ns\n";
    cout << "Sorted Array (Sequential):\n";
    showArray(arr);
    vector<int> arr_p = arr;
    // Parallel bubble sort
    auto start2 = chrono::high_resolution_clock::now();
#pragma omp parallel num_threads(4)
    {
        bubbleSort_p(arr_p);
    }
    auto stop2 = chrono::high_resolution_clock::now();
    auto result2 = stop2 - start2;
    auto duration2 = chrono::duration_cast<chrono::nanoseconds>(result2);

    cout << "\nParallel Bubble sort: " << duration2.count() << " ns\n";
    cout << "Sorted Array (Parallel):\n";
    showArray(arr_p);

    return 0;
}
