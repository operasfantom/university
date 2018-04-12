package numbers;

import expression.exceptions.DivisionByZeroException;
import expression.exceptions.OverflowException;

public class LongNumber implements AbstractNumber<LongNumber> {
    private final long value;

    public LongNumber(long value) {
        this.value = value;
    }

    public LongNumber(String s){
        this.value = Long.parseLong(s);
    }

    @Override
    public LongNumber add(LongNumber rhs) {
        return new LongNumber(value + rhs.value);
    }

    @Override
    public LongNumber subtract(LongNumber rhs) {
        return new LongNumber(value - rhs.value);
    }

    @Override
    public LongNumber multiply(LongNumber rhs) {
        return new LongNumber(value * rhs.value);
    }

    @Override
    public LongNumber divide(LongNumber rhs) {
        return new LongNumber(value / rhs.value);
    }

    @Override
    public LongNumber negate() {
        return new LongNumber(-value);
    }

    @Override
    public String reader(String s, int i) {
        StringBuilder sb = new StringBuilder();
        while (i < s.length() && Character.isDigit(s.charAt(i))) {
            sb.append(s.charAt(i));
            ++i;
        }
        return sb.toString();
    }

    @Override
    public LongNumber decoder(String s) {
        return new LongNumber(new Long(s));
    }

    public Long getValue(){
        return value;
    }
}
