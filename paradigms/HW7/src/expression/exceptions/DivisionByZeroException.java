package expression.exceptions;

public class DivisionByZeroException extends Exception {
    public DivisionByZeroException() {
    }

    public DivisionByZeroException(String s) {
        super(s);
    }

    public DivisionByZeroException(String s, Throwable throwable) {
        super(s, throwable);
    }

    public DivisionByZeroException(Throwable throwable) {
        super(throwable);
    }

    public DivisionByZeroException(String s, Throwable throwable, boolean b, boolean b1) {
        super(s, throwable, b, b1);
    }
}
