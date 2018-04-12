package expression.checked;

import expression.AbstractOperation;
import expression.TripleExpression;
import expression.exceptions.DivisionByZeroException;
import expression.exceptions.OverflowException;
import numbers.AbstractNumber;

public class CheckedDivide<T extends AbstractNumber<T>> extends AbstractOperation<T> {

    public CheckedDivide(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    @Override
    protected T calculate(T x0, T x1) throws DivisionByZeroException, OverflowException {
        return x0.divide(x1);
    }
}
