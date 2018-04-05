package expression.exceptions;

public class MathException extends Exception {
    public MathException() {
    }

    public MathException(String s) {
        super(s);
    }

    public MathException(String s, Throwable throwable) {
        super(s, throwable);
    }

    public MathException(Throwable throwable) {
        super(throwable);
    }

    public MathException(String s, Throwable throwable, boolean b, boolean b1) {
        super(s, throwable, b, b1);
    }
}
