package expression;

import expression.exceptions.DivisionByZeroException;
import expression.exceptions.MathException;
import expression.exceptions.OverflowException;

public  abstract class AbstractOperation implements TripleExpression {
    private TripleExpression operand0;
    private TripleExpression operand1;

    public AbstractOperation(TripleExpression x, TripleExpression y) {
        operand0 = x;
        operand1 = y;
    }

    @Override
    public int evaluate(int x, int y, int z) throws OverflowException, DivisionByZeroException, MathException {
        int x0 = operand0.evaluate(x, y, z);
        int x1 = operand1.evaluate(x, y, z);
        return calculate(x0, x1);
    }

    protected abstract int calculate(int x0, int x1) throws OverflowException, DivisionByZeroException, MathException;
}
