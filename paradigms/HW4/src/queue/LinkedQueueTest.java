package queue;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.ArrayDeque;
import java.util.function.Function;
import java.util.function.Predicate;

import static org.junit.jupiter.api.Assertions.assertEquals;

class MainTest {
    ArrayDeque<Object> queue = new ArrayDeque<>();
    private LinkedQueue linkedQueue = new LinkedQueue();
    private ArrayQueue arrayQueue = new ArrayQueue();

    @BeforeEach
    void init() {
        queue.clear();
        linkedQueue.clear();
        arrayQueue.clear();
    }

    private void enqueue(Object element) {
        queue.add(element);
        linkedQueue.enqueue(element);
        arrayQueue.enqueue(element);
    }

    private Object dequeue() {
        Object answer = queue.element();
        queue.poll();
        assertEquals(answer, linkedQueue.dequeue());
        assertEquals(answer, arrayQueue.dequeue());
        return answer;
    }

    @Test
    void element() {
        enqueue(0);
        enqueue(1);
        enqueue(2);
        assertEquals(queue.element(), linkedQueue.element());
        dequeue();
        assertEquals(queue.element(), linkedQueue.element());
        for (int i = 0; i < 9; i++) {
            enqueue(i + 3);
            assertEquals(queue.element(), linkedQueue.element());
        }
        for (int i = 0; i < 9; i++) {
            dequeue();
            assertEquals(queue.element(), linkedQueue.element());
        }
    }

    private void checkSize() {
        assertEquals(queue.size(), linkedQueue.size());
        assertEquals(queue.isEmpty(), linkedQueue.isEmpty());
    }

    private void clear() {
        queue.clear();
        linkedQueue.clear();
    }

    void runAndCheck(Runnable operation) {
        operation.run();
        checkSize();
    }

    @Test
    void size() { //also isEmpty() && clear()
        runAndCheck(() -> enqueue(0));
        runAndCheck(() -> enqueue(1));
        runAndCheck(() -> dequeue());
        runAndCheck(() -> clear());
        runAndCheck(() -> enqueue(2));
    }

    void push(Object element) {
        //queue.push(element);
        //linkedQueue.push(element);
    }

    void peek() {
        //assertEquals(queue.getLast(), linkedQueue.peek());
    }

    @Test
    void dequeueTest() {
        runAndCheck(() -> enqueue(0));
        runAndCheck(() -> enqueue(1));
        runAndCheck(() -> push(-1));
        runAndCheck(() -> peek());
    }

    @Test
    void functionTest() {
        runAndCheck(() -> enqueue(0));
        runAndCheck(() -> enqueue(1));
        runAndCheck(() -> enqueue(2));
        runAndCheck(() -> dequeue());

        Predicate<Object> predicate = Predicate.isEqual(1);
        ArrayQueue copyArrayQueue = (ArrayQueue) arrayQueue.filter(predicate);
        assertEquals(1, copyArrayQueue.element());
    }
}