
#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

#include <vector>
using namespace std;

enum class EventType
{
    Compare,
    Swap,
    Overwrite,
    Sorted
};

struct SortEvent
{
    EventType type;
    int indexA;
    int indexB;
    int value;
};

void mergeSort(
    vector<int>& arr,
    int left,
    int right,
    vector<SortEvent>& events
);

void bubbleSort(
    vector<int>& arr,
    vector<SortEvent>& events
);

#endif
