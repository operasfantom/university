package numbers;

import java.math.BigInteger;

public class BigIntegerNumber implements AbstractNumber<BigIntegerNumber> {
    private final BigInteger value;

    public BigIntegerNumber() {
        value = BigInteger.valueOf(0);
    }

    public BigIntegerNumber(BigInteger value) {
        this.value = value;
    }

    public BigIntegerNumber(String s) {
        value = new BigInteger(s);
    }

    @Override
    public BigIntegerNumber add(BigIntegerNumber rhs) {
        return new BigIntegerNumber(value.add(rhs.value));
    }

    @Override
    public BigIntegerNumber subtract(BigIntegerNumber rhs) {
        return new BigIntegerNumber(value.subtract(rhs.value));
    }

    @Override
    public BigIntegerNumber multiply(BigIntegerNumber rhs) {
        return new BigIntegerNumber(value.multiply(rhs.value));
    }

    @Override
    public BigIntegerNumber divide(BigIntegerNumber rhs) {
        return new BigIntegerNumber(value.divide(rhs.value));
    }

    @Override
    public BigIntegerNumber negate() {
        return new BigIntegerNumber(value.negate());
    }

    public BigInteger getValue() {
        return value;
    }

    @Override
    public String reader(String s, int i) {
        StringBuilder builder = new StringBuilder();
        while (i < s.length() && Character.isDigit(s.charAt(i))) {
            builder.append(s.charAt(i));
            ++i;
        }
        return builder.toString();
    }

    @Override
    public BigIntegerNumber decoder(String s) {
        return new BigIntegerNumber(new BigInteger(s));
    }
}
