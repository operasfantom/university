package expression.checked;

import expression.AbstractOperation;
import expression.TripleExpression;
import expression.exceptions.MathException;
import expression.exceptions.OverflowException;

public class CheckedPower extends AbstractOperation {
    public CheckedPower(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    protected int calculate(int x0, int x1) throws OverflowException, MathException {
        if (x0 == 0 && x1 == 0) {
            throw new MathException("0^0 found");
        }
        if (x1 < 0) {
            throw new MathException("power base is negative:" + x0 + "^" + x1);
        }
        CheckedMultiply cm = new CheckedMultiply(null, null);
        int result = 1;
        for (int i = 0; i < x1; i++) {
            result = cm.calculate(result, x0);
        }
        return result;
    }
}
