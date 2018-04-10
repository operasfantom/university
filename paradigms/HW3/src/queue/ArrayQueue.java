package queue;

// Invariant: this.size >= 0  && for i in [this.pointerFront, this.pointerBack) this.storage[i] != null && this not is null

// Immutable: for i in range(this.size) this.storage[this.next(this.pointerFront)^(i)] == this.storage'[this.next'(this.pointerFront')^i] &&
// this.size == this.size'

public class ArrayQueue {

    final int initialSize = 1;
    private int size = 0, pointerFront = 0, pointerBack = 0;
    private Object[] storage = new Object[initialSize];

    // this.size > 0 && i in [this.pointerFront, this.pointerBack)
    // Post: Immutable &&
    // R == (i + 1) % this.storage.legnth
    private int next(int i) {
        if (i == storage.length - 1) {
            return 0;
        }
        return i + 1;
    }

    // Pre: Invariant && this.size > 0 && i in [this.pointerFront, this.pointerBack)
    // Post: Immutable &&
    // R == i - 1 ? i' > 0 : this.storage.length - 1
    private int prev(int i) {
        if (i == 0) {
            return storage.length - 1;
        }
        return i - 1;
    }

    // Pre: Invariant
    // Post: Immutable &&
    // this.storage.length == max(this.storage'.length * 2, capacity)
    private void ensureCapacity(int capacity) {
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

    // Pre: element != null
    // Post: this.size = this.size' + 1 &&
    // for i in range(this.size')
    // this.storage[this.next(this.pointerFront)^i]
    // ==
    // this.storage'[this.next'(this.pointerFront')^i] &&
    // this.storage[next(this.pointerBack)] == element
    //---
    public void enqueue(Object element) {
        assert (element != null);
        ensureCapacity(size + 1);
        storage[pointerBack] = element;
        pointerBack = next(pointerBack);
        size++;
    }

    // Pre: this.size > 0
    // Post: this.size() == this.size'() &&
    // for i in range(this.size)
    // this.storage[this.next(this.pointerFront)^i]
    // ==
    // this.storage'[this.next'(this.pointerFront')^i] &&
    // R = this.storage[this.pointerFront]
    //---
    public Object element() {
        assert (size > 0) : "it's impossible to element() - queue is empty";
        return storage[pointerFront];
    }


    // Pre: this.size > 0
    // Post: this.size == this.size' - 1 &&
    // for i in range(1, this.size)
    // this.storage[this.next(this.pointerFront)^i]
    // ==
    // this.storage'[this.next'(this.pointerFront')^i] &&
    // R = this.element()
    //---
    public Object dequeue() {
        assert (size > 0) : "it's impossible to dequeue() - queue is empty";
        Object answer = element();
        storage[pointerFront] = null;
        pointerFront = next(pointerFront);
        size--;
        return answer;
    }


    // Pre: True
    // Post: && queue.size == this.size' &&
    // for i in range(this.size)
    // this.storage[this.next(this.pointerFront)^i]
    // ==
    // this.storage'[this.next'(this.pointerFront')^i] &&
    // R = this.size
    //---
    public int size() {
        return size;
    }


    // Pre: True
    // for i in range(this.size)
    // this.storage[this.next(this.pointerFront)^i]
    // ==
    // this.storage'[this.next'(this.pointerFront')^i] &&
    // R == (size == 0)
    //---
    public boolean isEmpty() {
        return size == 0;
    }


    // Pre: True
    // Post: this.size == 0
    //---
    public void clear() {
        storage = new Object[initialSize];
        size = 0;
        pointerBack = pointerFront = 0;
    }


    // Pre: element != null
    // Post: this.size == this.size' + 1 &&
    // for i in range(this.size)
    // this.storage[this.next(this.pointerFront)^(i + 1)]
    // ==
    // this.storage'[this.next'(this.pointerFront')^i] &&
    // this.storage[this.pointerFront] == element
    //---
    public void push(Object element) { //add element in front of Queue
        assert (element != null);
        ensureCapacity(size + 1);
        pointerFront = prev(pointerFront);
        storage[pointerFront] = element;
        size++;
    }


    // Pre: this.size > 0
    // Post: this.size == this.size' &&
    // for i in range(this.size)
    // this.storage[this.next(this.pointerFront)^(i)]
    // ==
    // this.storage'[this.next'(this.pointerFront')^i]
    public Object peek() {// return last element of Queue
        assert (size > 0) : "it's impossible to peek() - queue is empty";
        return storage[prev(pointerBack)];
    }


    // Pre: this.size > 0
    // Post : this.size == this.size' - 1
    // for i in range(this.size')
    // this.storage[this.next(this.pointerFront)^(i)]
    // ==
    // this.storage'[this.next'(this.pointerFront')^i]
    // R == this.storage[this.pointerBack]
    public Object remove() { // delete and return last element
        pointerBack = prev(pointerBack);
        size--;
        return storage[pointerBack];
    }
}
