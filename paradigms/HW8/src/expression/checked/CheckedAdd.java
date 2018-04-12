package expression.checked;

import expression.AbstractOperation;
import expression.TripleExpression;
import expression.exceptions.OverflowException;
import numbers.AbstractNumber;

public class CheckedAdd<T extends AbstractNumber<T>> extends AbstractOperation<T> {
    public CheckedAdd(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    @Override
    protected T calculate(T x0, T x1) throws OverflowException {
        return x0.add(x1);
    }
}
