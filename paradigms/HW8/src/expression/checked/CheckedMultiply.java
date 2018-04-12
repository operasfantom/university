package expression.checked;

import expression.AbstractOperation;
import expression.TripleExpression;
import expression.exceptions.OverflowException;
import numbers.AbstractNumber;

public class CheckedMultiply<T extends AbstractNumber<T>> extends AbstractOperation<T> {
    public CheckedMultiply(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    @Override
    protected T calculate(T x0, T x1) throws OverflowException {
        return x0.multiply(x1);
    }
}
