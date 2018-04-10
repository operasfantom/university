package queue;

// Invariant: this not is null && size >= 0  && for i in [pointerFront, pointerBack) storage[i] != null

// Immutable: for i in range(size) storage[i] == storage'[i] && size == size'

public class ArrayQueueModule {

    final static int INITIAL_SIZE = 1;
    private static int size = 0, pointerFront = 0, pointerBack = 0;
    private static Object[] storage = new Object[INITIAL_SIZE];

    // Pre: Invariant && size > 0 && i in [pointerFront, pointerBack)
    // Post: Immutable &&
    // R == (i + 1) % storage.legnth
    private static int next(int i) {
        if (i == storage.length - 1) {
            return 0;
        }
        return i + 1;
    }

    // Pre: Invariant && size > 0 && i in [pointerFront, pointerBack)
    // Post: Immutable &&
    // R == i - 1 ? i' > 0 : storage.length - 1
    private static int prev(int i) {
        if (i == 0) {
            return storage.length - 1;
        }
        return i - 1;
    }

    // Pre: Invariant
    // Post: Immutable &&
    // storage.length == max(storage'.length * 2, capacity)
    //---
    private static void ensureCapacity(int capacity) {
        if (capacity <= storage.length) {
            return;
        }
        int new_size = Math.max(storage.length * 2, capacity);
        Object[] temp = new Object[new_size];
        int delta = (pointerFront < pointerBack ? pointerBack - pointerFront : storage.length - pointerFront);
        System.arraycopy(storage, pointerFront, temp, 0, delta);
        System.arraycopy(storage, 0, temp, delta, size - delta);
        storage = temp;
        pointerFront = 0;
        pointerBack = size;
    }


    // Pre: Invariant && element != null
    // Post: size = size' + 1 &&
    // for i in range(size') storage[i] == storage'[i] &&
    // storage[size] == element
    //---
    public static void enqueue(Object element) { //add element in back of Queue
        assert (element != null);
        ensureCapacity(size + 1);
        storage[pointerBack] = element;
        pointerBack = next(pointerBack);
        size++;
    }


    // Pre: Invariant && size > 0
    // Post: Immutable &&
    // R == storage[pointerFront]
    //---
    public static Object element() { //return first in Queue
        assert (size > 0) : "it's impossible to element() - queue is empty";
        return storage[pointerFront];
    }


    // Pre: Invariant && size > 0
    // Post: size == size' - 1 &&
    // for i in range(1, size) storage[next(pointerFront)^i] == storage'[i] &&
    // R == element()
    //---
    public static Object dequeue() { //delete and return first element
        assert (size > 0) : "it's impossible to dequeue() - queue is empty";
        Object answer = element();
        storage[pointerFront] = null;
        pointerFront = next(pointerFront);
        size--;
        return answer;
    }


    // Pre: Invariant
    // Post: Invariant && Immutable &&
    // R == size
    //---
    public static int size() { //return size of Queue && Q == Q'
        return size;
    }


    // Pre: Invariant
    // Post: Invariant && Immutable &&
    // R == (size == 0)
    //---
    public static boolean isEmpty() { // return whether Queue is empty && Q == Q'
        return size == 0;
    }


    // Pre: Invariant
    // Post: Invariant && size == 0
    //---
    public static void clear() { //delete all elements
        storage = new Object[INITIAL_SIZE];
        size = 0;
        pointerBack = pointerFront = 0;
    }


    // Pre: Invariant && element != null
    // Post: Invariant && size == size' + 1 &&
    // for i in range(size) storage[next(pointerFront)^(i + 1)] == storage'[i] &&
    // storage[pointerFront] == element
    //---
    public static void push(Object element) { //add element in front of Queue
        assert (element != null) : "It's impossible to push(null)";
        ensureCapacity(size + 1);
        pointerFront = prev(pointerFront);
        storage[pointerFront] = element;
        size++;
    }


    // Pre: Invariant && size > 0
    // Post: Invariant && Immutable &&
    // R == storage[prev(pointerBack)]
    //---
    public static Object peek() {// return last element of Queue
        assert (size > 0) : "it's impossible to peek() - queue is empty";
        return storage[prev(pointerBack)];
    }


    // Pre: Invariant && size > 0
    // Post: Invariant && size == size' - 1
    // for i in range(size - 1) storage[i] == storage'[i]
    // R == storage[pointerBack]
    //---
    public static Object remove() { // delete and return last element
        pointerBack = prev(pointerBack);
        size--;
        return storage[pointerBack];
    }
}
