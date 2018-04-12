package expression.exceptions;

public class CheckedParserException extends Exception{

    public CheckedParserException(String s) {
        super(s);
    }

    public CheckedParserException(String s, int position){
        super(s + " in position " + position);
    }
}
