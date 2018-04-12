package expression;

import numbers.AbstractNumber;

/**
 * @author Georgiy Korneev (kgeorgiy@kgeorgiy.info)
 */
public interface TripleExpression<T extends AbstractNumber<T>> {
    T evaluate(T x, T y, T z) throws Exception;
}
