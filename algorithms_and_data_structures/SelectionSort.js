function selectionSort(array) {
    const length = array.length;

    for (let i = 0; i < length; i++) {

        let minIndex = i;
        for (let j = i + 1; j < length; j++) {
            if (array[j] < array[minIndex]) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            const current = array[i];
            array[i] = array[minIndex];
            array[minIndex] = current;
        }
    }
    return array;
}

module.exports = selectionSort;
