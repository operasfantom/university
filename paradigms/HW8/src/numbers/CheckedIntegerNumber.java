package numbers;

import expression.exceptions.DivisionByZeroException;
import expression.exceptions.OverflowException;

public class CheckedIntegerNumber implements AbstractNumber<CheckedIntegerNumber> {
    private final int value;

    public CheckedIntegerNumber(int value) {
        this.value = value;
    }

    public CheckedIntegerNumber(String s){
        this.value = Integer.parseInt(s);
    }

    @Override
    public CheckedIntegerNumber add(CheckedIntegerNumber rhs) throws OverflowException {
        int x0 = value, x1 = rhs.value;
        int x2 = value + rhs.value;
        if (((x0 ^ x2) & (x1 ^ x2)) < 0) {
            throw new OverflowException("integer overflow: " + x1 + " + " + x2);
        } else {
            return new CheckedIntegerNumber(x2);
        }
    }

    @Override
    public CheckedIntegerNumber subtract(CheckedIntegerNumber rhs) throws OverflowException {
        int x0 = value, x1 = rhs.value;
        int x2 = x0 - x1;
        if (((x0 ^ x1) & (x0 ^ x2)) < 0) {
            throw new OverflowException("integer overflow: " + x1 + " - " + x2);
        } else {
            return new CheckedIntegerNumber(x2);
        }
    }

    @Override
    public CheckedIntegerNumber multiply(CheckedIntegerNumber rhs) throws OverflowException {
        int x0 = value, x1 = rhs.value;
        if (x0 == 0 || x1 == 0) {
            return new CheckedIntegerNumber(0);
        }
        if (x0 > 0 && x1 > 0 && Integer.MAX_VALUE / x0 < x1) {
            throw new OverflowException(x0 + "*" + x1 + "overflowed");
        }
        if (x0 < 0 && x1 < 0 && Integer.MAX_VALUE / x0 > x1) {
            throw new OverflowException(x0 + "*" + x1 + "overflowed");
        }
        if (x0 > 0 && x1 < 0 && Integer.MIN_VALUE / x0 > x1) {
            throw new OverflowException(x0 + "*" + x1 + "overflowed");
        }
        if (x0 < 0 && x1 > 0 && Integer.MIN_VALUE / x1 > x0) {
            throw new OverflowException(x0 + "*" + x1 + "overflowed");
        }
        return new CheckedIntegerNumber(x0 * x1);
    }

    @Override
    public CheckedIntegerNumber divide(CheckedIntegerNumber rhs) throws DivisionByZeroException, OverflowException {
        int x0 = value, x1 = rhs.value;
        if (x1 == 0) {
            throw new DivisionByZeroException("division by zero:" + x0 + "/" + x1);
        }
        if (x0 == Integer.MIN_VALUE && x1 == -1) {
            throw new OverflowException(x0 + "/" + x1 + " overflowed");
        }
        return new CheckedIntegerNumber(x0 / x1);
    }

    @Override
    public CheckedIntegerNumber negate() throws OverflowException {
        if (value == Integer.MIN_VALUE) {
            throw new OverflowException("couldn't negate: " + value);
        }
        return new CheckedIntegerNumber(-value);
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
    public CheckedIntegerNumber decoder(String s) {
        return new CheckedIntegerNumber(Integer.parseInt(s));
    }

    public Integer getValue(){
        return value;
    }
}
