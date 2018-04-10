package queue;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.ArrayDeque;

import static org.junit.jupiter.api.Assertions.assertEquals;

class MainTest {
    ArrayDeque<Object> queue = new ArrayDeque<>();
    private ArrayQueueADT arrayQueueADT = new ArrayQueueADT();
    private ArrayQueue arrayQueue = new ArrayQueue();

    @BeforeEach
    void init() {
        queue.clear();
        ArrayQueueModule.clear();
        ArrayQueueADT.clear(arrayQueueADT);
        arrayQueue.clear();
    }

    private void enqueue(Object element) {
        queue.add(element);
        ArrayQueueModule.enqueue(element);
        ArrayQueueADT.enqueue(arrayQueueADT, element);
        arrayQueue.enqueue(element);
    }

    private Object dequeue() {
        Object answer = queue.element();
        queue.poll();
        assertEquals(answer, ArrayQueueModule.dequeue());
        assertEquals(answer, ArrayQueueADT.dequeue(arrayQueueADT));
        assertEquals(answer, arrayQueue.dequeue());
        return answer;
    }

    @Test
    void element() {
        enqueue(0);
        enqueue(1);
        enqueue(2);
        assertEquals(queue.element(), ArrayQueueModule.element());
        dequeue();
        assertEquals(queue.element(), ArrayQueueModule.element());
        for (int i = 0; i < 9; i++) {
            enqueue(i + 3);
            assertEquals(queue.element(), ArrayQueueModule.element());
        }
        for (int i = 0; i < 9; i++) {
            dequeue();
            assertEquals(queue.element(), ArrayQueueModule.element());
        }
    }

    private void checkSize() {
        assertEquals(queue.size(), ArrayQueueModule.size());
        assertEquals(queue.isEmpty(), ArrayQueueModule.isEmpty());

        assertEquals(queue.size(), ArrayQueueADT.size(arrayQueueADT));
        assertEquals(queue.isEmpty(), ArrayQueueADT.isEmpty(arrayQueueADT));

        assertEquals(queue.size(), arrayQueue.size());
        assertEquals(queue.isEmpty(), arrayQueue.isEmpty());
    }

    private void clear() {
        queue.clear();
        ArrayQueueModule.clear();
        ArrayQueueADT.clear(arrayQueueADT);
        arrayQueue.clear();
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
        queue.push(element);
        ArrayQueueModule.push(element);
        ArrayQueueADT.push(arrayQueueADT, element);
        arrayQueue.push(element);
    }

    void peek() {
        assertEquals(queue.getLast(), ArrayQueueModule.peek());
        assertEquals(queue.getLast(), ArrayQueueADT.peek(arrayQueueADT));
        assertEquals(queue.getLast(), arrayQueue.peek());
    }

    @Test
    void dequeueTest() {
        runAndCheck(() -> enqueue(0));
        runAndCheck(() -> enqueue(1));
        runAndCheck(() -> push(-1));
        runAndCheck(() -> peek());
    }
}