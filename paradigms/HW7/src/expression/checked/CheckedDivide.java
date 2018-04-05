package expression.checked;

import expression.AbstractOperation;
import expression.TripleExpression;
import expression.exceptions.DivisionByZeroException;

public strictfp class CheckedDivide extends AbstractOperation {

    public CheckedDivide(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    @Override
    protected int calculate(int x0, int x1) throws DivisionByZeroException {
        if (x1 == 0) {
            throw new DivisionByZeroException("");
        }
        if (x0 == Integer.MIN_VALUE && x1 == -1) {
            throw new DivisionByZeroException("");
        }
        return x0 / x1;
    }

}
