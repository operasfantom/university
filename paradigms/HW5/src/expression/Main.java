package expression;

public class Main {
    public static void main(String[] args) throws ArithmeticException {
        int answer;
        answer =
                new Add(
                        new Subtract(
                                new Multiply(
                                        new Variable("x"),
                                        new Variable("x")
                                ),
                                new Multiply(
                                        new Const(2),
                                        new Variable("x")
                                )
                        ),
                        new Const(1)
                ).evaluate(Integer.parseInt(args[0]));

        System.out.println(answer);

        answer =
                new Add(
                        new Variable("x"),
                        new Const(2)
                ).evaluate(0);

        System.out.println(answer);
    }
}
