#include <errno.h>
#include "algui/index.h"
#include "algui/array_util.h"


//binary search utility function
size_t algui_find_array_element_index_binary_search_util(const char* data, size_t size, size_t element_size, const void* element, ALGUI_COMPARATOR compare) {
    //check the element size
    if (element_size == 0) {
        errno = EINVAL;
        return ALGUI_INVALID_INDEX;
    }

    //check the element
    if (element == NULL) {
        errno = EINVAL;
        return ALGUI_INVALID_INDEX;
    }

    //check the comparator
    if (compare == NULL) {
        errno = EINVAL;
        return ALGUI_INVALID_INDEX;
    }

    //check the data; it is no error to have null data
    if (data == NULL) {
        return ALGUI_INVALID_INDEX;
    }

    //range to check; inclusive on minIndex, exclusive on maxIndex.
    size_t minIndex = 0;
    size_t maxIndex = size;

    //loop until found or range becomes empty.
    while (minIndex < maxIndex) {

        //calculate the mid index.
        const size_t midIndex = (minIndex + maxIndex) / 2;

        //calculate the mid element pointer.
        const char* const arrayElement = data + element_size * midIndex;

        //compare the elements.
        const int comp = compare(arrayElement, element);

        //if comparison is greater, 
        //then the searched element comes before the array element, 
        //therefore continue the search from minIndex to midIndex.
        if (comp > 0) {
            maxIndex = midIndex;
            continue;
        }

        //the searched element is either equal to the array element,
        //or it comes after the array element.
        //therefore continue the search from midIndex + 1 to maxIndex.
        else {
            minIndex = midIndex + 1;
        }
    }

    //if max index is 0, 
    //then the searched element is less than the first array element,
    //therefore the element is not found.
    if (maxIndex == 0) {
        return ALGUI_INVALID_INDEX;
    }

    //the found index is one less than maxIndex.
    const size_t foundIndex = maxIndex - 1;

    //if the element at foundIndex equals the searched element,
    //then the element is found.
    if (compare(data + element_size * foundIndex, element) == 0) {
        return foundIndex;
    }

    //element not found.
    return ALGUI_INVALID_INDEX;
}
