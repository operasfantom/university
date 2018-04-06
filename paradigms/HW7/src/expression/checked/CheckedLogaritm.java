package expression.checked;

import expression.AbstractOperation;
import expression.TripleExpression;
import expression.exceptions.MathException;
import expression.exceptions.OverflowException;

public class CheckedLogaritm extends AbstractOperation {
    public CheckedLogaritm(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    protected int calculate(int x0, int x1) throws OverflowException, MathException {
        if (x0 <= 0 || x1 <= 0 || x1 == 1) {
            throw new MathException("log(" + x0 + ") base " + x1 + "is undefined");
        }
        int answer = 0;
        while (x0 > 0){
            x0 /= x1;
            ++answer;
        }
        return answer - 1;
    }
}
