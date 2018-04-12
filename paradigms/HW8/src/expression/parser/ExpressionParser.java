package expression.parser;

import expression.BitCount;
import expression.Const;
import expression.TripleExpression;
import expression.Variable;
import expression.checked.*;
import expression.exceptions.CheckedParserException;
import numbers.AbstractNumber;

import java.util.ArrayList;
import java.util.List;
import java.util.function.BinaryOperator;

import static expression.parser.ExpressionTokenizer.Token;
import static expression.parser.ExpressionTokenizer.TokenType.*;

public class ExpressionParser implements Parser {
    private static List<Token> tokens = new ArrayList<>();
    private static int pointer = 0;
    private static int balance = 0;

    private static boolean test(ExpressionTokenizer.TokenType type) throws CheckedParserException {
        if (pointer < tokens.size()) {
            if (tokens.get(pointer).getType() == type) {
                pointer++;
                return true;
            }
            return false;
        } else {
            throw new CheckedParserException("required missing elements in the end");
        }
    }

    private static String currentWord() throws CheckedParserException {
        if (pointer < tokens.size()) {
            return tokens.get(pointer).getWord();
        } else {
            throw new CheckedParserException("required missing elements in the end");
        }
    }

    private static void checkIfNotNull(Object... objects) throws CheckedParserException {
        for (Object o : objects) {
            if (o == null) {
                throw new CheckedParserException("incorrect expression");
            }
        }
    }

    private static <T extends AbstractNumber<T>> TripleExpression<T> checkedEvaluate(TripleExpression<T> expression, TripleExpression<T> rhs, BinaryOperator<TripleExpression<T>> op) throws CheckedParserException {
        checkIfNotNull(expression, rhs);
        return op.apply(expression, rhs);
    }

    private static <T extends AbstractNumber<T>> TripleExpression<T> calcLowPriority(T objectT) throws CheckedParserException {
        return calcMinMax(objectT);
    }

    private static <T extends AbstractNumber<T>> TripleExpression<T> calcMinMax(T objectT) throws CheckedParserException {
        TripleExpression<T> expression = calcPlusMinus(objectT);

        while (pointer < tokens.size()) {
            if (test(MIN)) {
                expression = checkedEvaluate(expression, calcPlusMinus(objectT), CheckedMin<T>::new);
            } else if (test(MAX)) {
                expression = checkedEvaluate(expression, calcPlusMinus(objectT), CheckedMax<T>::new);
            } else {
                return expression;
            }
        }
        return expression;
    }

    private static <T extends AbstractNumber<T>> TripleExpression<T> calcPlusMinus(T objectT) throws CheckedParserException {
        TripleExpression<T> expression = calcMulDiv(objectT);

        while (pointer < tokens.size()) {
            if (test(PLUS)) {
                expression = checkedEvaluate(expression, calcMulDiv(objectT), CheckedAdd<T>::new);
            } else if (test(MINUS)) {
                expression = checkedEvaluate(expression, calcMulDiv(objectT), CheckedSubtract<T>::new);
            } else {
                return expression;
            }
        }
        return expression;
    }

    private static <T extends AbstractNumber<T>> TripleExpression<T> calcMulDiv(T objectT) throws CheckedParserException {
        TripleExpression<T> expression = calcHighPriority(objectT);

        while (pointer < tokens.size()) {
            if (test(TIMES)) {
                expression = checkedEvaluate(expression, calcHighPriority(objectT), CheckedMultiply<T>::new);
            } else if (test(SLASH)) {
                expression = checkedEvaluate(expression, calcHighPriority(objectT), CheckedDivide<T>::new);
            } else {
                return expression;
            }
        }
        return expression;
    }

    private static <T extends AbstractNumber<T>> TripleExpression<T> calcHighPriority(T objectT) throws CheckedParserException {
        TripleExpression<T> expression;
        String word = currentWord();
        if (test(BRACKET_OPEN)) {
            ++balance;
            expression = calcLowPriority(objectT);
            if (test(BRACKET_CLOSE)) {
                --balance;
                if (balance < 0) {
                    throw new CheckedParserException("negative balance of parentheses");
                }
            } else {
                throw new CheckedParserException("closed parentheses is absent", pointer);
            }
        } else if (test(VARIABLE)) {
            expression = new Variable<>(word);
        } else if (test(FUNCTION)) {
            if ("count".equals(word)) {
                expression = new BitCount<>(calcHighPriority(objectT));
            } else {
                throw new CheckedParserException("wrong name of function: " + word);
            }
        } else if (test(NUMBER)) {
            try {
                expression = new Const<>(objectT.decoder(word));
            } catch (NumberFormatException e) {
                throw new CheckedParserException("\"" + word + "\"" + " is incorrect number", pointer);
            }
        } else if (test(MINUS)) {
            if (pointer < tokens.size()) {
                word = currentWord();
                if (test(NUMBER)) {
                    try {
                        expression = new Const<>(objectT.decoder("-" + word));
                    } catch (NumberFormatException e) {
                        throw new CheckedParserException("\"" + word + "\"" + " is incorrect number", pointer);
                    }
                } else {
                    expression = new CheckedNegate<>(calcHighPriority(objectT));
                }
            } else {
                throw new CheckedParserException("required missing elements in the end after sign minus");
            }
        } else {
            throw new CheckedParserException("\"" + word + "\"" + " is unsuitable token", pointer);
        }
        return expression;
    }

    public <T extends AbstractNumber<T>> TripleExpression<T> parse(String s, T objectT) throws CheckedParserException {
        ExpressionTokenizer tokenizer = new ExpressionTokenizer();
        tokens = tokenizer.tokenize(s, objectT);
        pointer = 0;
        balance = 0;
        TripleExpression<T> result = calcLowPriority(objectT);
        if (balance != 0) {
            throw new CheckedParserException("the quantity of opening and closing parentheses is different");
        }
        if (pointer != tokens.size()) {
            throw new CheckedParserException("couldn't parse expression after position" + pointer);
        }
        return result;
    }
}
