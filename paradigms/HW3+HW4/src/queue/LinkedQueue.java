package queue;

public class LinkedQueue extends AbstractQueue {
    private Node head, tail;

    protected void enqueueImpl(Object element) {
        if (size == 0) {
            head = tail = new Node(element, null);
        } else {
            tail.next = new Node(element, null);
            tail = tail.next;
        }
    }

    protected Object elementImpl() {
        return head.value;
    }

    protected Object dequeueImpl() {
        Object answer = head.value;
        head = head.next;
        return answer;
    }

    protected void clearImpl() {
        head = tail = null;
    }

    public LinkedQueue makeCopy() {
        LinkedQueue answer = new LinkedQueue();
        for (Node cur = head; cur != null; cur = cur.next) {
            answer.enqueue(cur.value);
        }
        return answer;
    }

    private class Node {
        Object value;
        Node next;

        Node(Object value, Node next) {
            assert value != null : "";
            this.value = value;
            this.next = next;
        }
    }
}
