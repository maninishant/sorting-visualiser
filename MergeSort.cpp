
#include "SortingAlgorithms.h"

void merge(
    vector<int>& arr,
    int left,
    int mid,
    int right,
    vector<SortEvent>& events
)
{
    vector<int> temp;

    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right)
    {
        events.push_back({
            EventType::Compare, i, j, 0
        });

        if (arr[i] <= arr[j])
        {
            temp.push_back(arr[i]);
            i++;
        }
        else
        {
            temp.push_back(arr[j]);
            j++;
        }
    }

    while (i <= mid)
    {
        temp.push_back(arr[i]);
        i++;
    }

    while (j <= right)
    {
        temp.push_back(arr[j]);
        j++;
    }

    for (int k = left; k <= right; k++)
    {
        arr[k] = temp[k - left];

        events.push_back({
            EventType::Overwrite,
            k,
            -1,
            arr[k]
        });
    }
}

void mergeSort(
    vector<int>& arr,
    int left,
    int right,
    vector<SortEvent>& events
)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    mergeSort(arr, left, mid, events);
    mergeSort(arr, mid + 1, right, events);

    merge(arr, left, mid, right, events);

    if (left == 0 && right == static_cast<int>(arr.size()) - 1)
    {
        for (int i = 0; i < static_cast<int>(arr.size()); i++)
        {
            events.push_back({
                EventType::Sorted,
                i,
                -1,
                arr[i]
            });
        }
    }
}
