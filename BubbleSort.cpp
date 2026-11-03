
#include "SortingAlgorithms.h"
#include <utility>

void bubbleSort(vector<int>& arr, vector<SortEvent>& events)
{
    int n = static_cast<int>(arr.size());

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            // Record comparison
            events.push_back({
                EventType::Compare,
                j,
                j + 1,
                0
            });

            // Swap if elements are in the wrong order
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);

                events.push_back({
                    EventType::Swap,
                    j,
                    j + 1,
                    0
                });
            }
        }

        // Mark the last element of this pass as sorted
        events.push_back({
            EventType::Sorted,
            n - 1 - i,
            -1,
            0
        });
    }

    // Mark the first element as sorted
    if (n > 0)
    {
        events.push_back({
            EventType::Sorted,
            0,
            -1,
            0
        });
    }
}