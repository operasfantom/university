package expression;

import numbers.AbstractNumber;

public class Const<T extends AbstractNumber<T>> implements TripleExpression<T> {
    private T value;

    public Const(T value) {
        this.value = value;
    }

    @Override
    public T evaluate(T x, T y, T z) {
        return value;
    }
}
