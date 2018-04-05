package expression.exceptions;

public class CheckedParserException extends Exception {
    public CheckedParserException() {
        super("Incorrect expression");
    }

    public CheckedParserException(String s) {
        super(s);
    }

    public CheckedParserException(String s, Throwable throwable) {
        super(s, throwable);
    }

    public CheckedParserException(Throwable throwable) {
        super(throwable);
    }

    public CheckedParserException(String s, Throwable throwable, boolean b, boolean b1) {
        super(s, throwable, b, b1);
    }
}
