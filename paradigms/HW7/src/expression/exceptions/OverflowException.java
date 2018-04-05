package expression.exceptions;

public class OverflowException extends Exception {

    public OverflowException(String s) {
        super(s);
    }

    public OverflowException(String s, Throwable throwable) {
        super(s, throwable);
    }

    public OverflowException(Throwable throwable) {
        super(throwable);
    }

    public OverflowException(String s, Throwable throwable, boolean b, boolean b1) {
        super(s, throwable, b, b1);
    }
}
