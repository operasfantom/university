package expression.parser;

import expression.TripleExpression;
import expression.exceptions.CheckedParserException;
import numbers.AbstractNumber;

/**
 * @author Georgiy Korneev (kgeorgiy@kgeorgiy.info)
 */
public interface Parser {
    <T extends AbstractNumber<T>> TripleExpression<T> parse(String expression, T number) throws CheckedParserException;
}
