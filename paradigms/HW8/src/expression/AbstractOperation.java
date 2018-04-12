package expression;

import expression.exceptions.DivisionByZeroException;
import expression.exceptions.MathException;
import expression.exceptions.OverflowException;
import numbers.AbstractNumber;

public abstract class AbstractOperation<T extends AbstractNumber<T>> implements TripleExpression<T> {
    private TripleExpression<T> operand0;
    private TripleExpression<T> operand1;

    public AbstractOperation(TripleExpression<T> x, TripleExpression<T> y) {
        operand0 = x;
        operand1 = y;
    }

    public T evaluate(T x, T y, T z) throws Exception {
        T x0 = operand0.evaluate(x, y, z);
        T x1 = operand1.evaluate(x, y, z);
        return calculate(x0, x1);
    }

    protected abstract T calculate(T x0, T x1) throws OverflowException, DivisionByZeroException, MathException;
}
