package expression;

import numbers.AbstractNumber;

public class BitCount<T extends AbstractNumber<T>> implements TripleExpression<T> {
    private final TripleExpression<T> function;

    public BitCount(TripleExpression<T> function) {
        this.function = function;
    }

    @Override
    public T evaluate(T x, T y, T z) throws Exception {
        T result = function.evaluate(x, y, z);
        return result.decoder(Integer.toString(result.bitCount()));
    }
}
