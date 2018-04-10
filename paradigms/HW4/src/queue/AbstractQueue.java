package queue;

import java.util.function.Function;
import java.util.function.Predicate;

public abstract class AbstractQueue implements Queue {
    protected int size;

    public void enqueue(Object element) {
        assert element != null : "";

        enqueueImpl(element);
        size++;
    }

    protected abstract void enqueueImpl(Object element);

    public Object element() {
        assert size > 0 : "";

        return elementImpl();
    }

    protected abstract Object elementImpl();

    public Object dequeue() {
        assert size > 0 : "";

        Object answer = dequeueImpl();
        size--;
        return answer;
    }

    protected abstract Object dequeueImpl();

    public int size() {
        return size;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public void clear() {
        clearImpl();
        size = 0;
    }

    protected abstract void clearImpl();

    protected abstract AbstractQueue makeCopy();

    public AbstractQueue filter(Predicate<Object> predicate) {
        assert predicate != null : "predicate is null";
        AbstractQueue answer = makeCopy();
        for (int i = 0; i < size; i++) {
            Object temp = answer.dequeue();
            if (predicate.test(temp)) {
                answer.enqueue(temp);
            }
        }
        return answer;
    }

    public AbstractQueue map(Function<Object, Object> function) {
        assert function != null : "function is null";
        AbstractQueue answer = makeCopy();
        for (int i = 0; i < size; i++) {
            Object temp = answer.dequeue();
            answer.enqueue(function.apply(temp));
        }
        return answer;
    }
}
