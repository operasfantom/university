package numbers;

public class IntegerNumber implements AbstractNumber<IntegerNumber> {
    private final int value;

    public IntegerNumber() {
        this.value = 0;
    }

    public IntegerNumber(int value) {
        this.value = value;
    }

    public IntegerNumber(String s) {
        this.value = Integer.parseInt(s);
    }

    @Override
    public IntegerNumber add(IntegerNumber rhs) {
        return new IntegerNumber(this.value + rhs.value);
    }

    @Override
    public IntegerNumber subtract(IntegerNumber rhs) {
        return new IntegerNumber(this.value - rhs.value);
    }

    @Override
    public IntegerNumber multiply(IntegerNumber rhs) {
        return new IntegerNumber(this.value * rhs.value);
    }

    @Override
    public IntegerNumber divide(IntegerNumber rhs) {
        return new IntegerNumber(this.value / rhs.value);
    }

    @Override
    public IntegerNumber negate() {
        return new IntegerNumber(-value);
    }

    @Override
    public int bitCount() {
        return Integer.bitCount(value);
    }

    @Override
    public IntegerNumber min(IntegerNumber rhs) {
        return new IntegerNumber(value < rhs.value ? value : rhs.value);
    }

    @Override
    public IntegerNumber max(IntegerNumber rhs) {
        return new IntegerNumber(value > rhs.value ? value : rhs.value);
    }

    public Integer getValue() {
        return value;
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
    public IntegerNumber decoder(String s) {
        return new IntegerNumber(new Integer(s));
    }
}
