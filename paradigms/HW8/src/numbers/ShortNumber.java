package numbers;

public class ShortNumber implements AbstractNumber<ShortNumber> {
    private final short value;

    public ShortNumber(short value) {
        this.value = value;
    }

    public ShortNumber(String s) {
        this.value = Short.parseShort(s);
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
