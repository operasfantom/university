package expression.checked;

import expression.TripleExpression;
import expression.exceptions.DivisionByZeroException;
import expression.exceptions.MathException;
import expression.exceptions.OverflowException;

public class CheckedLog10 implements TripleExpression {
    private TripleExpression function;

    public CheckedLog10(TripleExpression function) {
        this.function = function;
    }

    @Override
    public int evaluate(int x, int y, int z) throws OverflowException, DivisionByZeroException, MathException {
        int result = function.evaluate(x, y, z);
        if (result <= 0){
            throw new MathException("log10(" + result + ") is undefined");
        }
        return Integer.toString(result).length() - 1;
    }
}
