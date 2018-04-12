package expression.checked;

import expression.TripleExpression;
import numbers.AbstractNumber;

public class CheckedNegate<T extends AbstractNumber<T>>  implements TripleExpression<T> {
    private TripleExpression<T> function;

    public CheckedNegate(TripleExpression<T> function) {
        this.function = function;
    }

    @Override
    public T evaluate(T x, T y, T z) throws Exception {
        T result = function.evaluate(x, y, z);
        return result.negate();
    }
}
