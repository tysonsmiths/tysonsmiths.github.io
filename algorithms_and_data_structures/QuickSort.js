
// Big-O
// Best: nlog(n)
// Average: nlog(n)
// Worst: n^2

function partition(array, start, end) {
    const pivotValue = array[end];
    let pivotIndex = start;
    for (let i = start; i < end; i++) {
        const current = array[i];
        if (current < pivotValue) {
            array[i] = array[pivotIndex];
            array[pivotIndex] = current
            pivotIndex++;
        }
    }

    const newEndValue = array[end];
    array[end] = array[pivotIndex];
    array[pivotIndex] = newEndValue;
    return pivotIndex;
};

function quickSortRecursive(array, start, end) {
    if (start >= end) {
        return;
    }

    const index = partition(array, start, end);

    quickSortRecursive(array, start, index - 1);
    quickSortRecursive(array, index + 1, end);
}

function quickSort(array) {
    quickSortRecursive(array, 0, array.length - 1);
    return array;
}

module.exports = quickSort;
