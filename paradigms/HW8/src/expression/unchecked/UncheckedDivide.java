package expression.unchecked;

import expression.AbstractOperation;
import expression.TripleExpression;
import expression.exceptions.DivisionByZeroException;
import expression.exceptions.MathException;
import expression.exceptions.OverflowException;
import numbers.AbstractNumber;

public class UncheckedDivide<T extends AbstractNumber<T>> extends AbstractOperation<T> {

    public UncheckedDivide(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    protected T calculate(T x0, T x1) throws OverflowException, DivisionByZeroException {
        return x0.divide(x1);
    }

}
