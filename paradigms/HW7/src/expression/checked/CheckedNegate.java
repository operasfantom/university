package expression.checked;

import expression.TripleExpression;
import expression.exceptions.DivisionByZeroException;
import expression.exceptions.OverflowException;

public class CheckedNegate implements TripleExpression {
    private TripleExpression function;

    public CheckedNegate(TripleExpression function) {
        this.function = function;
    }

    @Override
    public int evaluate(int x, int y, int z) throws OverflowException, DivisionByZeroException {
        int result = function.evaluate(x, y, z);
        if (result == Integer.MIN_VALUE) {
            throw new OverflowException("couldn't negate: " + result);
        }
        return -result;
    }
}
