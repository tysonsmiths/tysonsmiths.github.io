
// Big-O
// Best: n
// Average: n^2
// Worst: n^2

function bubbleSort(array) {
    const length = array.length;
    let didSwap = true;
    while (didSwap) {
        didSwap = false;

        for (let i = 0; i < length; i++) {
            const current = array[i];
            const next = array[i + 1];

            if (current > next) {
                array[i] = next;
                array[i + 1] = current;
                didSwap = true;
            }
        }
    }
    return array;
};

module.exports = bubbleSort;
