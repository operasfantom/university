package expression.checked;

import expression.AbstractOperation;
import expression.TripleExpression;
import expression.exceptions.OverflowException;

public class CheckedMultiply extends AbstractOperation {
    public CheckedMultiply(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    protected int calculate(int x0, int x1) throws OverflowException {
        if (x0 == 0 || x1 == 0) {
            return 0;
        }
        if (x0 > 0 && x1 > 0 && Integer.MAX_VALUE / x0 < x1) {
            throw new OverflowException(x0 + "*" + x1 + "overflowed");
        }
        if (x0 < 0 && x1 < 0 && Integer.MAX_VALUE / x0 > x1) {
            throw new OverflowException(x0 + "*" + x1 + "overflowed");
        }
        if (x0 > 0 && x1 < 0 && Integer.MIN_VALUE / x0 > x1) {
            throw new OverflowException(x0 + "*" + x1 + "overflowed");
        }
        if (x0 < 0 && x1 > 0 && Integer.MIN_VALUE / x1 > x0) {
            throw new OverflowException(x0 + "*" + x1 + "overflowed");
        }
        return x0 * x1;
    }
}
