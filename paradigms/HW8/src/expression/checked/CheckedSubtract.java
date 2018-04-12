package expression.checked;

import expression.AbstractOperation;
import expression.TripleExpression;
import expression.exceptions.OverflowException;
import numbers.AbstractNumber;

public class CheckedSubtract<T extends AbstractNumber<T>> extends AbstractOperation<T> {

    public CheckedSubtract(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    protected T calculate(T x0, T x1) throws OverflowException {
        return x0.subtract(x1);
    }

}
