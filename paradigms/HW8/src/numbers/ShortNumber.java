package numbers;

public class ShortNumber implements AbstractNumber<ShortNumber> {
    private final short value;

    public ShortNumber(short value) {
        this.value = value;
    }

    public ShortNumber(String s) {
        this.value = (short) Integer.parseInt(s);
    }

    @Override
    public ShortNumber add(ShortNumber rhs) {
        return new ShortNumber((short) (value + rhs.value));
    }

    @Override
    public ShortNumber subtract(ShortNumber rhs) {
        return new ShortNumber((short) (value - rhs.value));
    }

    @Override
    public ShortNumber multiply(ShortNumber rhs) {
        return new ShortNumber((short) (value * rhs.value));
    }

    @Override
    public ShortNumber divide(ShortNumber rhs) {
        return new ShortNumber((short) (value / rhs.value));
    }

    @Override
    public ShortNumber negate() {
        return new ShortNumber((short) -value);
    }

    @Override
    public int bitCount() {
        if (value >= 0) {
            return Integer.bitCount(value);
        } else{
            return Integer.bitCount(value) - 16;
        }
    }

    @Override
    public ShortNumber min(ShortNumber rhs) {
        return new ShortNumber(value < rhs.value ? value : rhs.value);
    }

    @Override
    public ShortNumber max(ShortNumber rhs) {
        return new ShortNumber(value > rhs.value ? value : rhs.value);
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
    public ShortNumber decoder(String s) {
        return new ShortNumber(new Short(s));
    }

    public Short getValue() {
        return value;
    }
}
