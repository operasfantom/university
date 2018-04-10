package expression.checked;

import expression.AbstractOperation;
import expression.TripleExpression;
import expression.exceptions.DivisionByZeroException;
import expression.exceptions.OverflowException;

public  class CheckedDivide extends AbstractOperation {

    public CheckedDivide(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    @Override
    protected int calculate(int x0, int x1) throws DivisionByZeroException, OverflowException {
        if (x1 == 0) {
            throw new DivisionByZeroException("division by zero:" + x0 + "/" + x1);
        }
        if (x0 == Integer.MIN_VALUE && x1 == -1) {
            throw new OverflowException(x0 + "/" + x1 + " overflowed");
        }
        return x0 / x1;
    }

}
