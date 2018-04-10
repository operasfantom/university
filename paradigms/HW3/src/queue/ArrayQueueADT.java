package queue;

// Invariant: queue.size >= 0  && for i in [queue.pointerFront, queue.pointerBack) queue.storage[i] != null && queue not is null

// Immutable: for i in range(queue.size) storage[queue.next(queue.pointerFront)^(i)] == queue.storage'[queue.next'(queue.pointerFront')^i]
// && queue.size == queue.size'

public class ArrayQueueADT {

    final static int initialSize = 1;
    private int size = 0, pointerFront = 0, pointerBack = 0;
    private Object[] storage = new Object[initialSize];

    // queue.size > 0 && i in [queue.pointerFront, queue.pointerBack)
    // Post: Immutable &&
    // R == (i + 1) % queue.storage.legnth
    private static int next(ArrayQueueADT queue, int i) {
        if (i == queue.storage.length - 1) {
            return 0;
        }
        return i + 1;
    }

    // Pre: Invariant && queue.size > 0 && i in [queue.pointerFront, queue.pointerBack)
    // Post: Immutable &&
    // R == i - 1 ? i' > 0 : queue.storage.length - 1
    private static int prev(ArrayQueueADT queue, int i) {
        if (i == 0) {
            return queue.storage.length - 1;
        }
        return i - 1;
    }

    // Pre: Invariant
    // Post: Immutable &&
    // queue.storage.length == max(queue.storage'.length * 2, capacity)
    private static void ensureCapacity(ArrayQueueADT queue, int capacity) {
        if (capacity <= queue.storage.length) {
            return;
        }
        int new_size = Math.max(queue.storage.length * 2, capacity);
        Object[] temp = new Object[new_size];
        int delta = (queue.pointerFront < queue.pointerBack ?
                queue.pointerBack - queue.pointerFront :
                queue.storage.length - queue.pointerFront);
        System.arraycopy(queue.storage, queue.pointerFront, temp, 0, delta);
        System.arraycopy(queue.storage, 0, temp, delta, queue.size - delta);
        queue.storage = temp;
        queue.pointerFront = 0;
        queue.pointerBack = queue.size;
    }


    // Pre: element != null
    // Post: queue.size = queue.size' + 1 &&
    // for i in range(queue.size')
    // queue.storage[queue.next(queue.pointerFront)^i]
    // ==
    // queue.storage'[queue.next'(queue.pointerFront')^i] &&
    // queue.storage[next(queue.pointerBack)] == element
    //---
    public static void enqueue(ArrayQueueADT queue, Object element) {
        assert (element != null);
        ensureCapacity(queue, queue.size + 1);
        queue.storage[queue.pointerBack] = element;
        queue.pointerBack = next(queue, queue.pointerBack);
        queue.size++;
    }


    // Pre: queue.size > 0
    // Post: queue.size() == queue.size'() &&
    // for i in range(queue.size)
    // queue.storage[queue.next(queue.pointerFront)^i]
    // ==
    // queue.storage'[queue.next'(queue.pointerFront')^i] &&
    // R = queue.storage[queue.pointerFront]
    //---
    public static Object element(ArrayQueueADT queue) {
        assert (queue.size > 0) : "it's impossible to element() - queue is empty";
        return queue.storage[queue.pointerFront];
    }


    // Pre: queue.size > 0
    // Post: queue.size == queue.size' - 1 &&
    // for i in range(1, queue.size)
    // queue.storage[queue.next(queue.pointerFront)^i]
    // ==
    // queue.storage'[queue.next'(queue.pointerFront')^i] &&
    // R = queue.element()
    //---
    public static Object dequeue(ArrayQueueADT queue) {
        assert (queue.size > 0) : "it's impossible to dequeue() - queue is empty";
        Object answer = element(queue);
        queue.storage[queue.pointerFront] = null;
        queue.pointerFront = next(queue, queue.pointerFront);
        queue.size--;
        return answer;
    }


    // Pre: True
    // Post: && queue.size == queue.size' &&
    // for i in range(queue.size)
    // queue.storage[queue.next(queue.pointerFront)^i]
    // ==
    // queue.storage'[queue.next'(queue.pointerFront')^i] &&
    // R = queue.size
    //---
    public static int size(ArrayQueueADT queue) {
        return queue.size;
    }


    // Pre: True
    // for i in range(queue.size)
    // queue.storage[queue.next(queue.pointerFront)^i]
    // ==
    // queue.storage'[queue.next'(queue.pointerFront')^i] &&
    // R == (size == 0)
    //---
    public static boolean isEmpty(ArrayQueueADT queue) {
        return queue.size == 0;
    }


    // Pre: True
    // Post: queue.size == 0
    //---
    public static void clear(ArrayQueueADT queue) {
        queue.storage = new Object[initialSize];
        queue.size = 0;
        queue.pointerBack = queue.pointerFront = 0;
    }


    // Pre: element != null
    // Post: queue.size == queue.size' + 1 &&
    // for i in range(queue.size)
    // queue.storage[queue.next(queue.pointerFront)^(i + 1)]
    // ==
    // queue.storage'[queue.next'(queue.pointerFront')^i] &&
    // queue.storage[queue.pointerFront] == element
    //---
    public static void push(ArrayQueueADT queue, Object element) { //add element in front of Queue
        assert (element != null);
        ensureCapacity(queue, queue.size + 1);
        queue.pointerFront = queue.prev(queue, queue.pointerFront);
        queue.storage[queue.pointerFront] = element;
        queue.size++;
    }


    // Pre: queue.size > 0
    // Post: queue.size == queue.size' &&
    // for i in range(queue.size)
    // queue.storage[queue.next(queue.pointerFront)^(i)]
    // ==
    // queue.storage'[queue.next'(queue.pointerFront')^i]
    public static Object peek(ArrayQueueADT queue) {// return last element of Queue
        assert (queue.size > 0) : "it's impossible to peek() - queue is empty";
        return queue.storage[queue.prev(queue, queue.pointerBack)];
    }


    // Pre: queue.size > 0
    // Post : queue.size == queue.size' - 1
    // for i in range(queue.size')
    // queue.storage[queue.next(queue.pointerFront)^(i)]
    // ==
    // queue.storage'[queue.next'(queue.pointerFront')^i]
    // R == queue.storage[queue.pointerBack]
    public static Object remove(ArrayQueueADT queue) { // delete and return last element
        queue.pointerBack = prev(queue, queue.pointerBack);
        queue.size--;
        return queue.storage[queue.pointerBack];
    }
}
