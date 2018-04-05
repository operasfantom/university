package expression.checked;

import expression.TripleExpression;
import expression.exceptions.DivisionByZeroException;
import expression.exceptions.MathException;
import expression.exceptions.OverflowException;

public class CheckedPow10 implements TripleExpression {
    private TripleExpression function;

    public CheckedPow10(TripleExpression function) {
        this.function = function;
    }

    @Override
    public int evaluate(int x, int y, int z) throws OverflowException, DivisionByZeroException, MathException {
        int result = function.evaluate(x, y, z);
        if (result < 0) {
            throw new MathException("pow10(" + result + ") is undefined");
        }
        if (result > 9) {
            throw new OverflowException("pow(" + result + ") is too large");
        }
        int answer = 1;
        for (int i = 0; i < result; i++) {
            answer *= 10;
        }
        return answer;
    }
}
