package queue;

public class ArrayQueue extends AbstractQueue {

    final int initialSize = 1;
    private int pointerFront = 0, pointerBack = 0;
    private Object[] storage = new Object[initialSize];

    private int next(int i) {
        if (i == storage.length - 1) {
            return 0;
        }
        return i + 1;
    }

    private int prev(int i) {
        if (i == 0) {
            return storage.length - 1;
        }
        return i - 1;
    }

    private void ensureCapacity(int capacity) {
        if (capacity <= storage.length) {
            return;
        }
        int new_size = Math.max(storage.length * 2, capacity);
        storage = getArray(new_size);
        pointerFront = 0;
        pointerBack = size;
    }

    private Object[] getArray(int new_size) {
        Object[] temp = new Object[new_size];
        if (size == 0) {
            return temp;
        }
        int delta = (pointerFront < pointerBack ? pointerBack - pointerFront : storage.length - pointerFront);
        System.arraycopy(storage, pointerFront, temp, 0, delta);
        System.arraycopy(storage, 0, temp, delta, size - delta);
        return temp;
    }

    public void enqueueImpl(Object element) {
        assert (element != null);
        ensureCapacity(size + 1);
        storage[pointerBack] = element;
        pointerBack = next(pointerBack);
    }

    public Object elementImpl() {
        return storage[pointerFront];
    }


    public Object dequeueImpl() {
        Object answer = element();
        storage[pointerFront] = null;
        pointerFront = next(pointerFront);
        return answer;
    }

    public void clearImpl() {
        storage = new Object[initialSize];
        pointerBack = pointerFront = 0;
    }

    // Pre: element != null
    // Post: this.size == this.size' + 1 &&
    // for i in range(this.size)
    // this.storage[this.next(this.pointerFront)^(i + 1)]
    // ==
    // this.storage'[i] &&
    // this.storage[this.pointerFront] == element
    //---
    public void push(Object element) {
        assert (element != null);
        ensureCapacity(size + 1);
        pointerFront = prev(pointerFront);
        storage[pointerFront] = element;
        size++;
    }

    // Pre: this.size > 0
    // Post: this.size == this.size' &&
    // for i in range(this.size)
    // this.storage[i]
    // ==
    // this.storage'[i]
    //---
    public Object peek() {
        return storage[prev(pointerBack)];
    }

    // Pre: this.size > 0
    // Post : this.size == this.size' - 1
    // for i in range(this.size')
    // this.storage[i]
    // ==
    // this.storage'[i]
    // R == this.storage[this.pointerBack]
    //---
    public Object remove() {
        size--;
        pointerBack = prev(pointerBack);
        return storage[pointerBack];
    }


    public ArrayQueue makeCopy() {
        ArrayQueue answer = new ArrayQueue();
        answer.storage = getArray(size);
        answer.size = size;
        return answer;
    }
}
