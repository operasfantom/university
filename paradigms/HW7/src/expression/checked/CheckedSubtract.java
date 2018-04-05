package expression.checked;

import expression.AbstractOperation;
import expression.TripleExpression;
import expression.exceptions.DivisionByZeroException;
import expression.exceptions.OverflowException;

public strictfp class CheckedSubtract extends AbstractOperation {

    public CheckedSubtract(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    @Override
    protected int calculate(int x0, int x1) throws OverflowException, DivisionByZeroException {
        int x2 = x0 - x1;
        if (((x0 ^ x1) & (x0 ^ x2)) < 0) {
            throw new OverflowException("integer overflow: " + x1 + " - " + x2);
        } else {
            return x2;
        }
    }

}
