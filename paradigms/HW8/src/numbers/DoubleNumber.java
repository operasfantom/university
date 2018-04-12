package numbers;

public class DoubleNumber implements AbstractNumber<DoubleNumber> {
    private final double value;

    public DoubleNumber() {
        this.value = 0.0;
    }

    public DoubleNumber(double value) {
        this.value = value;
    }

    public DoubleNumber(String s) {
        this.value = Double.parseDouble(s);
    }

    @Override
    public DoubleNumber add(DoubleNumber rhs) {
        return new DoubleNumber(value + rhs.value);
    }

    @Override
    public DoubleNumber subtract(DoubleNumber rhs) {
        return new DoubleNumber(value - rhs.value);
    }

    @Override
    public DoubleNumber multiply(DoubleNumber rhs) {
        return new DoubleNumber(value * rhs.value);
    }

    @Override
    public DoubleNumber divide(DoubleNumber rhs) {
        return new DoubleNumber(value / rhs.value);
    }

    @Override
    public DoubleNumber negate() {
        return new DoubleNumber(-value);
    }

    @Override
    public int bitCount() {
        return Long.bitCount(Double.doubleToLongBits(value));
    }

    @Override
    public DoubleNumber min(DoubleNumber rhs) {
        return new DoubleNumber(value < rhs.value ? value : rhs.value);
    }

    @Override
    public DoubleNumber max(DoubleNumber rhs) {
        return new DoubleNumber(value > rhs.value ? value : rhs.value);
    }

    public Double getValue() {
        return value;
    }

    @Override
    public String reader(String s, int i) {
        StringBuilder builder = new StringBuilder();
        while (i < s.length() && Character.isDigit(s.charAt(i))) {
            builder.append(s.charAt(i));
            ++i;
        }
        if (i == s.length() || s.charAt(i) != '.'){
            return builder.toString();
        }
        builder.append('.');
        i += 2;
        while (i < s.length() && Character.isDigit(s.charAt(i))) {
            builder.append(s.charAt(i));
            ++i;
        }
        return builder.toString();
    }

    @Override
    public DoubleNumber decoder(String s) {
        return new DoubleNumber(new Double(s));
    }
}
