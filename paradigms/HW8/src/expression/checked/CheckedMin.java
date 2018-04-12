package expression.checked;

import expression.AbstractOperation;
import expression.TripleExpression;
import expression.exceptions.OverflowException;
import numbers.AbstractNumber;

public class CheckedMin<T extends AbstractNumber<T>> extends AbstractOperation<T> {
    public CheckedMin(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    @Override
    protected T calculate(T x0, T x1) {
        return x0.min(x1);
    }
}
