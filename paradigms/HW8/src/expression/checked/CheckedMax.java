package expression.checked;

import expression.AbstractOperation;
import expression.TripleExpression;
import numbers.AbstractNumber;

public class CheckedMax<T extends AbstractNumber<T>> extends AbstractOperation<T> {
    public CheckedMax(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    @Override
    protected T calculate(T x0, T x1) {
        return x0.max(x1);
    }
}
