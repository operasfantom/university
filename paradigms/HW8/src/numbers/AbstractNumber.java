package numbers;

import expression.exceptions.DivisionByZeroException;
import expression.exceptions.OverflowException;

public interface AbstractNumber<T> {

    T add(T rhs) throws OverflowException;

    T subtract(T rhs) throws OverflowException;

    T multiply(T rhs) throws OverflowException;

    T divide(T rhs) throws DivisionByZeroException, OverflowException;

    T negate() throws OverflowException;

    String reader(String s, int i);

    T decoder(String s);
}
