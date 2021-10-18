function merge(left, right) {
    let array = []

    while (left.length && right.length) {
        if (left[0] < right[0]) {
            array.push(left.shift())
        } else {
            array.push(right.shift())
        }
    }

    return [...array, ...left, ...right]
}

function mergeSort(array) {
    const length = array.length;
    if (length <= 1) {
        return array
    }


    const halfIndex = Math.floor(length / 2);

    const left = array.slice(0, halfIndex)
    const right = array.slice(halfIndex);

    const leftSorted = mergeSort(left);
    const rightSorted = mergeSort(right);

    return merge(leftSorted, rightSorted);
}

module.exports = mergeSort;
