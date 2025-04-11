#include <iostream>
#include <time.h>
#include <chrono>
#include <fstream>
using namespace std;

class Arrays
{
private:
    int *Arr;

    void ArrayFilling(int *Array, int Size)
    {
        srand(time(nullptr));
        for (int i = 0; i < Size; i++)
        {
            Array[i] = rand() % Size;
        }
    }

public:
    Arrays(int size)
    {
        Arr = new int[size];
        ArrayFilling(Arr, size);
    }

    ~Arrays()
    {
        delete[] Arr;
    }

    int *getArray() const
    {
        return Arr;
    }

    static void PrintArray(int *Array, int Size)
    {
        for (int i = 0; i < Size; i++)
        {
            cout << Array[i] << " , ";
            if (i % 20 == 0 && i != 0)
                cout << "\n";
        }
    }
};

class QuickSort
{
private:
    static void Swap(int *a, int *b)
    {
        int temp = *a;
        *a = *b;
        *b = temp;
    }

    static int Partition(int *Array, int low, int high)
    {
        int pivot = Array[high];
        int PivotNewPlace = low - 1;
        for (int Pointer = low; Pointer < high; Pointer++)
        {
            if (Array[Pointer] <= pivot)
            {
                PivotNewPlace++;
                Swap(&Array[PivotNewPlace], &Array[Pointer]);
            }
        }
        Swap(&Array[PivotNewPlace + 1], &Array[high]);
        return PivotNewPlace + 1;
    }

public:
    static void Quicksort(int *Array, int low, int high)
    {
        if (low >= high)
            return;
        int pivot = Partition(Array, low, high);
        Quicksort(Array, low, pivot - 1);
        Quicksort(Array, pivot + 1, high);
    }
};

class MergeSort
{
private:
    static void Merge(int *Array, int start, int mid, int end)
    {
        int LowHalfSize = mid - start + 1;
        int HighHalfSize = end - mid;
        int *LowHalf = new int[LowHalfSize];
        int *HighHalf = new int[HighHalfSize];

        for (int i = 0; i < LowHalfSize; i++)
        {
            LowHalf[i] = Array[start + i];
        }
        for (int i = 0; i < HighHalfSize; i++)
        {
            HighHalf[i] = Array[mid + 1 + i];
        }

        int PointerOnLowHalf = 0, PointerOnHighHalf = 0;
        int PointerOnOriginalArray = start;

        while (PointerOnLowHalf < LowHalfSize && PointerOnHighHalf < HighHalfSize)
        {
            if (LowHalf[PointerOnLowHalf] <= HighHalf[PointerOnHighHalf])
            {
                Array[PointerOnOriginalArray] = LowHalf[PointerOnLowHalf];
                PointerOnLowHalf++;
            }
            else
            {
                Array[PointerOnOriginalArray] = HighHalf[PointerOnHighHalf];
                PointerOnHighHalf++;
            }
            PointerOnOriginalArray++;
        }

        while (PointerOnLowHalf < LowHalfSize)
        {
            Array[PointerOnOriginalArray] = LowHalf[PointerOnLowHalf];
            PointerOnLowHalf++;
            PointerOnOriginalArray++;
        }

        while (PointerOnHighHalf < HighHalfSize)
        {
            Array[PointerOnOriginalArray] = HighHalf[PointerOnHighHalf];
            PointerOnHighHalf++;
            PointerOnOriginalArray++;
        }

        delete[] LowHalf;
        delete[] HighHalf;
    }

public:
    static void Mergesort(int *Array, int start, int end)
    {
        if (end <= start)
            return;
        int mid = start + (end - start) / 2;
        Mergesort(Array, start, mid);
        Mergesort(Array, mid + 1, end);
        Merge(Array, start, mid, end);
    }
};

class RadixSort
{
private:
    static int GetMax(int *arr, int n)
    {
        int max = arr[0];
        for (int i = 1; i < n; i++)
            if (arr[i] > max)
                max = arr[i];
        return max;
    }

    static void countSort(int *arr, int n, int exp)
    {
        int *output = new int[n];
        int i, count[10] = {0};

        for (i = 0; i < n; i++)
            count[(arr[i] / exp) % 10]++;

        for (i = 1; i < 10; i++)
            count[i] += count[i - 1];

        for (i = n - 1; i >= 0; i--)
        {
            output[count[(arr[i] / exp) % 10] - 1] = arr[i];
            count[(arr[i] / exp) % 10]--;
        }

        for (i = 0; i < n; i++)
            arr[i] = output[i];

        delete[] output;
    }

public:
    static void Radixsort(int *arr, int n)
    {
        int m = GetMax(arr, n);
        for (int exp = 1; m / exp > 0; exp *= 10)
            countSort(arr, n, exp);
    }
};

class InsertionSort
{
public:
    static void insertionSort(int *arr, int size)
    {
        for (int i = 1; i < size; i++)
        {
            int key = arr[i];
            int j = i - 1; // Fixed the bug here
            while (j >= 0 && arr[j] > key)
            {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }
};

class ProgramInterface
{
private:
    static fstream csvfile;
    static double duration;

    static double CountingMilliSecondsForQuickSort(int *Array, int low, int high)
    {
        auto start = chrono::high_resolution_clock::now();
        QuickSort::Quicksort(Array, low, high);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> Tick = end - start;
        duration = Tick.count();
        return duration;
    }

    static double CountingMilliSecondsForMergeSort(int *Array, int start, int end)
    {
        auto t1 = chrono::high_resolution_clock::now();
        MergeSort::Mergesort(Array, start, end);
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double> Tick = t2 - t1;
        duration = Tick.count();
        return duration;
    }

    static double CountingMilliSecondsForRadixSort(int *Array, int size)
    {
        auto start = chrono::high_resolution_clock::now();
        RadixSort::Radixsort(Array, size);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> Tick = end - start;
        duration = Tick.count();
        return duration;
    }

    static double CountingMilliSecondsForInsertionSort(int *Array, int size)
    {
        auto start = chrono::high_resolution_clock::now();
        InsertionSort::insertionSort(Array, size);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> Tick = end - start;
        duration = Tick.count();
        return duration;
    }

    static void LoadToFile(int NumberOftheRow, double quicksortTime, double mergeSortTime, double radixSortTime, double insertionSortTime)
    {
        csvfile.open("Result.csv", ios::app);
        csvfile << NumberOftheRow << ',' << quicksortTime << ',' << mergeSortTime << ',' << radixSortTime << ',' << insertionSortTime << endl;
        csvfile.close();
    }

    static void CalculateTime()
    {
        for (int i = 10; i <= 100; i += 10)
        {
            Arrays tempArray(i);
            int *originalArray = tempArray.getArray();

            // Create copies
            int *CopyForQuickSort = new int[i];
            int *CopyForMergeSort = new int[i];
            int *CopyForRadixSort = new int[i];
            int *CopyForInsertionSort = new int[i];

            copy(originalArray, originalArray + i, CopyForQuickSort);
            copy(originalArray, originalArray + i, CopyForMergeSort);
            copy(originalArray, originalArray + i, CopyForRadixSort);
            copy(originalArray, originalArray + i, CopyForInsertionSort);

            double QuickSortTime = CountingMilliSecondsForQuickSort(CopyForQuickSort, 0, i - 1) * 1000;
            double MergeSortTime = CountingMilliSecondsForMergeSort(CopyForMergeSort, 0, i - 1) * 1000;
            double RadixSortTime = CountingMilliSecondsForRadixSort(CopyForRadixSort, i) * 1000;
            double InsertionSortTime = CountingMilliSecondsForInsertionSort(CopyForInsertionSort, i) * 1000;

            delete[] CopyForQuickSort;
            delete[] CopyForMergeSort;
            delete[] CopyForRadixSort;
            delete[] CopyForInsertionSort;

            LoadToFile(i, QuickSortTime, MergeSortTime, RadixSortTime, InsertionSortTime);
        }

        for (int i = 200; i <= 1000; i += 100)
        {
            Arrays tempArray(i);
            int *originalArray = tempArray.getArray();

            int *CopyForQuickSort = new int[i];
            int *CopyForMergeSort = new int[i];
            int *CopyForRadixSort = new int[i];
            int *CopyForInsertionSort = new int[i];

            copy(originalArray, originalArray + i, CopyForQuickSort);
            copy(originalArray, originalArray + i, CopyForMergeSort);
            copy(originalArray, originalArray + i, CopyForRadixSort);
            copy(originalArray, originalArray + i, CopyForInsertionSort);

            double QuickSortTime = CountingMilliSecondsForQuickSort(CopyForQuickSort, 0, i - 1) * 1000;
            double MergeSortTime = CountingMilliSecondsForMergeSort(CopyForMergeSort, 0, i - 1) * 1000;
            double RadixSortTime = CountingMilliSecondsForRadixSort(CopyForRadixSort, i) * 1000;
            double InsertionSortTime = CountingMilliSecondsForInsertionSort(CopyForInsertionSort, i) * 1000;

            delete[] CopyForQuickSort;
            delete[] CopyForMergeSort;
            delete[] CopyForRadixSort;
            delete[] CopyForInsertionSort;

            LoadToFile(i, QuickSortTime, MergeSortTime, RadixSortTime, InsertionSortTime);
        }

        for (int i = 2000; i <= 10000; i += 1000)
        {
            Arrays tempArray(i);
            int *originalArray = tempArray.getArray();

            int *CopyForQuickSort = new int[i];
            int *CopyForMergeSort = new int[i];
            int *CopyForRadixSort = new int[i];
            int *CopyForInsertionSort = new int[i];

            copy(originalArray, originalArray + i, CopyForQuickSort);
            copy(originalArray, originalArray + i, CopyForMergeSort);
            copy(originalArray, originalArray + i, CopyForRadixSort);
            copy(originalArray, originalArray + i, CopyForInsertionSort);

            double QuickSortTime = CountingMilliSecondsForQuickSort(CopyForQuickSort, 0, i - 1) * 1000;
            double MergeSortTime = CountingMilliSecondsForMergeSort(CopyForMergeSort, 0, i - 1) * 1000;
            double RadixSortTime = CountingMilliSecondsForRadixSort(CopyForRadixSort, i) * 1000;
            double InsertionSortTime = CountingMilliSecondsForInsertionSort(CopyForInsertionSort, i) * 1000;

            delete[] CopyForQuickSort;
            delete[] CopyForMergeSort;
            delete[] CopyForRadixSort;
            delete[] CopyForInsertionSort;

            LoadToFile(i, QuickSortTime, MergeSortTime, RadixSortTime, InsertionSortTime);
        }

        for (int i = 20000; i <= 100000; i += 10000)
        {
            Arrays tempArray(i);
            int *originalArray = tempArray.getArray();

            int *CopyForQuickSort = new int[i];
            int *CopyForMergeSort = new int[i];
            int *CopyForRadixSort = new int[i];
            int *CopyForInsertionSort = new int[i];

            copy(originalArray, originalArray + i, CopyForQuickSort);
            copy(originalArray, originalArray + i, CopyForMergeSort);
            copy(originalArray, originalArray + i, CopyForRadixSort);
            copy(originalArray, originalArray + i, CopyForInsertionSort);

            double QuickSortTime = CountingMilliSecondsForQuickSort(CopyForQuickSort, 0, i - 1) * 1000;
            double MergeSortTime = CountingMilliSecondsForMergeSort(CopyForMergeSort, 0, i - 1) * 1000;
            double RadixSortTime = CountingMilliSecondsForRadixSort(CopyForRadixSort, i) * 1000;
            double InsertionSortTime = CountingMilliSecondsForInsertionSort(CopyForInsertionSort, i) * 1000;

            delete[] CopyForQuickSort;
            delete[] CopyForMergeSort;
            delete[] CopyForRadixSort;
            delete[] CopyForInsertionSort;

            LoadToFile(i, QuickSortTime, MergeSortTime, RadixSortTime, InsertionSortTime);
        }
    }

public:
    ProgramInterface()
    {
        csvfile.open("Result.csv", ios::out | ios::trunc);
        csvfile << "Range,Quick Sort,Merge Sort,Radix Sort,Insertion Sort\n";
        csvfile.close();
    }

    static void Start()
    {
        CalculateTime();
    }
};

// Initialize static members
fstream ProgramInterface::csvfile;
double ProgramInterface::duration;

int main()
{
    cout << "Please Wait..." << endl;
    ProgramInterface *x = new ProgramInterface();
    x->Start();
    delete x;
    system("pause");
    return 0;
}