// all done

function insertionSort(array) {
    const length = array.length;

    for (let i = 1; i < length; i++) {
        const current = array[i];

        let potentialInsertionSpot = i - 1;
        while (potentialInsertionSpot > -1 && (current < array[potentialInsertionSpot])) {
            array[potentialInsertionSpot + 1] = array[potentialInsertionSpot];
            potentialInsertionSpot--;
        }
        array[potentialInsertionSpot + 1] = current;
    }

    return array;
}

module.exports = insertionSort;
