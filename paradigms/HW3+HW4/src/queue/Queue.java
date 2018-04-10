package queue;

import java.util.function.Function;
import java.util.function.Predicate;

// Invariant: size >= 0  && for i in range(size) queue[next(head)^(i)] != null && queue != null

// Immutable: for i in range(size) queue[next(head)^(i)] == queue'[next'(head')^i] && size == size'

public interface Queue {

    // Pre: Invariant && element != null
    // Post: size = size' + 1 &&
    // for i in range(size') queue[next(head)^i] == queue'[next'(head')^i] &&
    // queue[next'(head')^(size' + 1)] == element
    //---
    void enqueue(Object element);

    // Pre: Invariant && size > 0
    // Post: Immutable &&
    // R == queue[head]
    //---
    Object element();

    // Pre: Invariant && size > 0
    // Post: size == size' - 1 &&
    // for i in range(1, size) queue[next(head)^i] == queue'[next'(head')^i] &&
    // R == element()
    //---
    Object dequeue();

    // Pre: Invariant
    // Post: Invariant && Immutable &&
    // R == size
    //---
    int size();

    // Pre: Invariant
    // Post: Invariant && Immutable &&
    // R == (size == 0)
    //---
    boolean isEmpty();

    // Pre: Invariant
    // Post: Invariant && size == 0
    //---
    void clear();

    // Pre: Invariant
    // Post: Invariant && answer = Queue(x : predicate.test(x)) &&
    // elements in new_queue is subsequence of elements queue'
    //---
    Queue filter(Predicate<Object> predicate);

    // Pre: Invariant
    // Post: Invariant && answer.size == this.size &&
    // for i in range(size) answer[i] == function.apply(this[i])
    //---
    Queue map(Function<Object, Object> function);

    // Pre: queue not is null
    // Post: new_queue is copy of queue'
    //---
    Queue makeCopy();
}
