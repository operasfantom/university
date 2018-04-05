package expression;

import expression.exceptions.DivisionByZeroException;
import expression.exceptions.MathException;
import expression.exceptions.OverflowException;

public class BitCount implements TripleExpression {
    private TripleExpression function;

    public BitCount(TripleExpression function) {
        this.function = function;
    }

    @Override
    public int evaluate(int x, int y, int z) throws OverflowException, DivisionByZeroException, MathException {
        return Integer.bitCount(function.evaluate(x, y, z));
    }
}
