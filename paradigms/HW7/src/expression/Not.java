package expression;

import expression.exceptions.DivisionByZeroException;
import expression.exceptions.MathException;
import expression.exceptions.OverflowException;

public class Not implements TripleExpression {
    private TripleExpression function;

    public Not(TripleExpression function) {
        this.function = function;
    }

    @Override
    public int evaluate(int x, int y, int z) throws OverflowException, DivisionByZeroException, MathException {
        return ~(function.evaluate(x, y, z));
    }
}
