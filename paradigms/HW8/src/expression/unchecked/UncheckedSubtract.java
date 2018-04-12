package expression.unchecked;

import expression.AbstractOperation;
import expression.TripleExpression;
import expression.exceptions.OverflowException;
import numbers.AbstractNumber;

public class UncheckedSubtract<T extends AbstractNumber<T>> extends AbstractOperation<T> {

    public UncheckedSubtract(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    @Override
    protected T calculate(T x0, T x1) throws OverflowException {
        return x0.subtract(x1);
    }
}
