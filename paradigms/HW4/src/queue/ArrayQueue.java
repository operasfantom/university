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
        assert (size > 0) : "it's impossible to element() - queue is empty";
        return storage[pointerFront];
    }


    public Object dequeueImpl() {
        assert (size > 0) : "it's impossible to dequeue() - queue is empty";
        Object answer = element();
        storage[pointerFront] = null;
        pointerFront = next(pointerFront);
        return answer;
    }

    public void clearImpl() {
        storage = new Object[initialSize];
        size = 0;
        pointerBack = pointerFront = 0;
    }


    protected ArrayQueue makeCopy() {
        ArrayQueue answer = new ArrayQueue();
        answer.storage = getArray(size);
        answer.size = size;
        return answer;
    }
}
