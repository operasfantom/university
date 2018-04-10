package queue;

// Invariant: queue not is null && queue.size >= 0  && for i in [queue.front, queue.pointerBack) queue.storage[i] != null

// Immutable: for i in range(queue.size) storage[queue.next(queue.front)^(i)] == queue.storage'[queue.next'(queue.front')^i]
// && queue.size == queue.size'

public class ArrayQueueADT {

    final static int INITIAL_SIZE = 1;

    private int size = 0, front = 0, pointerBack = 0;
    private Object[] storage = new Object[INITIAL_SIZE];

    // Pre: Invariant && queue.size > 0 && i in [queue.front, queue.pointerBack)
    // Post: Invariant && Immutable &&
    // R == (i + 1) % queue.storage.legnth
    private static int next(ArrayQueueADT queue, int i) {
        if (i == queue.storage.length - 1) {
            return 0;
        }
        return i + 1;
    }

    // Pre: Invariant && queue.size > 0 && i in [queue.front, queue.pointerBack)
    // Post: Invariant && Immutable &&
    // R == i - 1 ? i' > 0 : queue.storage.length - 1
    private static int prev(ArrayQueueADT queue, int i) {
        if (i == 0) {
            return queue.storage.length - 1;
        }
        return i - 1;
    }

    // Pre: Invariant
    // Post: Invariant && Immutable &&
    // queue.storage.length == max(queue.storage'.length * 2, capacity)
    private static void ensureCapacity(ArrayQueueADT queue, int capacity) {
        if (capacity <= queue.storage.length) {
            return;
        }
        int new_size = Math.max(queue.storage.length * 2, capacity);
        Object[] temp = new Object[new_size];
        int delta = (queue.front < queue.pointerBack ? queue.pointerBack : queue.storage.length) - queue.front;
        System.arraycopy(queue.storage, queue.front, temp, 0, delta);
        System.arraycopy(queue.storage, 0, temp, delta, queue.size - delta);
        queue.storage = temp;
        queue.front = 0;
        queue.pointerBack = queue.size;
    }


    // Pre: Invariant && element != null
    // Post: Invariant && queue.size = queue.size' + 1 &&
    // for i in range(queue.size')
    // queue.storage[queue.next(queue.front)^i]
    // ==
    // queue.storage'[queue.next'(queue.front')^i] &&
    // queue.storage[next(queue.pointerBack)] == element
    //---
    public static void enqueue(ArrayQueueADT queue, Object element) {
        assert (element != null);
        ensureCapacity(queue, queue.size + 1);
        queue.storage[queue.pointerBack] = element;
        queue.pointerBack = next(queue, queue.pointerBack);
        queue.size++;
    }


    // Pre: Invariant && queue.size > 0
    // Post: Invariant && queue.size() == queue.size'() &&
    // for i in range(queue.size)
    // queue.storage[queue.next(queue.front)^i]
    // ==
    // queue.storage'[queue.next'(queue.front')^i] &&
    // R = queue.storage[queue.front]
    //---
    public static Object element(ArrayQueueADT queue) {
        assert (queue.size > 0) : "it's impossible to element() - queue is empty";
        return queue.storage[queue.front];
    }


    // Pre: Invariant && queue.size > 0
    // Post: Invariant && queue.size == queue.size' - 1 &&
    // for i in range(1, queue.size)
    // queue.storage[queue.next(queue.front)^i]
    // ==
    // queue.storage'[queue.next'(queue.front')^i] &&
    // R = queue.element()
    //---
    public static Object dequeue(ArrayQueueADT queue) {
        assert (queue.size > 0) : "it's impossible to dequeue() - queue is empty";
        Object answer = element(queue);
        queue.storage[queue.front] = null;
        queue.front = next(queue, queue.front);
        queue.size--;
        return answer;
    }


    // Pre: Invariant
    // Post: Invariant && && queue.size == queue.size' &&
    // for i in range(queue.size)
    // queue.storage[queue.next(queue.front)^i]
    // ==
    // queue.storage'[queue.next'(queue.front')^i] &&
    // R = queue.size
    //---
    public static int size(ArrayQueueADT queue) {
        return queue.size;
    }


    // Pre: Invariant &&
    // for i in range(queue.size)
    // queue.storage[queue.next(queue.front)^i]
    // ==
    // queue.storage'[queue.next'(queue.front')^i] &&
    // R == (size == 0)
    //---
    public static boolean isEmpty(ArrayQueueADT queue) {
        return queue.size == 0;
    }


    // Pre: Invariant
    // Post: Invariant && queue.size == 0
    //---
    public static void clear(ArrayQueueADT queue) {
        queue.storage = new Object[INITIAL_SIZE];
        queue.size = 0;
        queue.pointerBack = queue.front = 0;
    }


    // Pre: Invariant && element != null
    // Post: Invariant && queue.size == queue.size' + 1 &&
    // for i in range(queue.size)
    // queue.storage[queue.next(queue.front)^(i + 1)]
    // ==
    // queue.storage'[queue.next'(queue.front')^i] &&
    // queue.storage[queue.front] == element
    //---
    public static void push(ArrayQueueADT queue, Object element) { //add element in front of Queue
        assert (element != null);
        ensureCapacity(queue, queue.size + 1);
        queue.front = queue.prev(queue, queue.front);
        queue.storage[queue.front] = element;
        queue.size++;
    }


    // Pre: Invariant && queue.size > 0
    // Post: Invariant && queue.size == queue.size' &&
    // for i in range(queue.size)
    // queue.storage[queue.next(queue.front)^(i)]
    // ==
    // queue.storage'[queue.next'(queue.front')^i]
    public static Object peek(ArrayQueueADT queue) {// return last element of Queue
        assert (queue.size > 0) : "it's impossible to peek() - queue is empty";
        return queue.storage[queue.prev(queue, queue.pointerBack)];
    }


    // Pre: Invariant && queue.size > 0
    // Post: Invariant && queue.size == queue.size' - 1
    // for i in range(queue.size')
    // queue.storage[queue.next(queue.front)^(i)]
    // ==
    // queue.storage'[queue.next'(queue.front')^i]
    // R == queue.storage[queue.pointerBack]
    public static Object remove(ArrayQueueADT queue) { // delete and return last element
        queue.pointerBack = prev(queue, queue.pointerBack);
        queue.size--;
        return queue.storage[queue.pointerBack];
    }
}
