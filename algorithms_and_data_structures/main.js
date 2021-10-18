const bubbleSort = require('./BubbleSort');
const insertionSort = require('./InsertionSort');
const mergeSort = require('./MergeSort');
const selectionSort = require('./SelectionSort');
const quickSort = require('./QuickSort');





const array = Array.from({length: 40000}, () => Math.floor(Math.random() * 40));


function main() {
    console.time('bubbleSort');
    bubbleSort([...array]);
    console.timeEnd('bubbleSort');

    console.time('insertionSort');
    insertionSort([...array]);
    console.timeEnd('insertionSort');

    console.time('mergeSort');
    mergeSort([...array]);
    console.timeEnd('mergeSort');

    console.time('selectionSort');
    selectionSort([...array]);
    console.timeEnd('selectionSort');

    console.time('quickSort');
    quickSort([...array]);
    console.timeEnd('quickSort');
}

main();
