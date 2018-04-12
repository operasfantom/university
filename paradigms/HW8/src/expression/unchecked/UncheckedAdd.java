package expression.unchecked;

import expression.AbstractOperation;
import expression.TripleExpression;
import expression.exceptions.DivisionByZeroException;
import expression.exceptions.MathException;
import expression.exceptions.OverflowException;
import numbers.AbstractNumber;

public class UncheckedAdd<T extends AbstractNumber<T>> extends AbstractOperation<T> {
    public UncheckedAdd(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    protected T calculate(T x0, T x1) throws OverflowException {
        return x0.add(x1);
    }
}
