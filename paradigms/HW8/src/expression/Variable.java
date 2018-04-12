package expression;

import numbers.AbstractNumber;

public  class Variable<T extends AbstractNumber<T>> implements TripleExpression<T> {
    public final String name;

    public Variable(String name) {
        this.name = name;
    }

    public T evaluate(T x, T y, T z) throws Exception {
        if ("x".equals(this.name)) {
            return x;
        } else if ("y".equals(this.name)) {
            return y;
        } else if ("z".equals(this.name)) {
            return z;
        } else {
            throw new Exception("wrong name of variable:" + this.name);
        }
    }
}
