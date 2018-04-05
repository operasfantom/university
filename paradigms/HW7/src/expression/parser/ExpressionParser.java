package expression.parser;

import expression.*;
import expression.checked.*;
import expression.exceptions.CheckedParserException;

import java.util.ArrayList;
import java.util.List;
import java.util.function.BinaryOperator;

import static expression.parser.ExpressionTokenizer.Token;
import static expression.parser.ExpressionTokenizer.TokenType.*;

public class ExpressionParser implements Parser {
    private static List<Token> tokens = new ArrayList<>();
    private static int pointer = 0;
    private static int balance = 0;

    private static boolean test(ExpressionTokenizer.TokenType type) {
        if (tokens.get(pointer).getType() == type) {
            pointer++;
            return true;
        }
        return false;
    }

    private static void checkIfNotNull(Object... objects) throws CheckedParserException {
        for (Object o : objects) {
            if (o == null) {
                throw new CheckedParserException("");
            }
        }
    }

    private static TripleExpression checkedEvaluate(TripleExpression expression, TripleExpression rhs, BinaryOperator<TripleExpression> op) throws CheckedParserException {
        checkIfNotNull(expression, rhs);
        return op.apply(expression, rhs);
    }

    private static TripleExpression calcOr() throws CheckedParserException {
        TripleExpression expression = calcXor();

        while (pointer < tokens.size()) {
            if (test(OR)) {
                expression = checkedEvaluate(expression, calcXor(), Or::new);
            } else {
                return expression;
            }
        }
        return expression;
    }

    private static TripleExpression calcXor() throws CheckedParserException {
        TripleExpression expression = calcAnd();

        while (pointer < tokens.size()) {
            if (test(XOR)) {
                expression = checkedEvaluate(expression, calcAnd(), Xor::new);
            } else {
                return expression;
            }
        }
        return expression;
    }

    private static TripleExpression calcAnd() throws CheckedParserException {
        TripleExpression expression = calcPlusMinus();

        while (pointer < tokens.size()) {
            if (test(AND)) {
                expression = checkedEvaluate(expression, calcPlusMinus(), And::new);
            } else {
                return expression;
            }
        }
        return expression;
    }

    private static TripleExpression calcPlusMinus() throws CheckedParserException {
        TripleExpression expression = calcMulDiv();

        while (pointer < tokens.size()) {
            if (test(PLUS)) {
                expression = checkedEvaluate(expression, calcMulDiv(), CheckedAdd::new);
            } else if (test(MINUS)) {
                expression = checkedEvaluate(expression, calcMulDiv(), CheckedSubtract::new);
            } else {
                return expression;
            }
        }
        return expression;
    }

    private static TripleExpression calcMulDiv() throws CheckedParserException {
        TripleExpression expression = calcHighPriority();

        while (pointer < tokens.size()) {
            if (test(TIMES)) {
                expression = checkedEvaluate(expression, calcHighPriority(), CheckedMultiply::new);
            } else if (test(SLASH)) {
                expression = checkedEvaluate(expression, calcHighPriority(), CheckedDivide::new);
            } else {
                return expression;
            }
        }
        return expression;
    }

    private static TripleExpression calcHighPriority() throws CheckedParserException {
        TripleExpression expression = null;
        String word = tokens.get(pointer).getWord();
        if (test(BRACKET_OPEN)) {
            ++balance;
            expression = calcOr();
            if (test(BRACKET_CLOSE)) {
                --balance;
                if (balance < 0) {
                    throw new CheckedParserException("negative balance of parentheses");
                }
            } else {
                throw new CheckedParserException("closed parnthes is absent in position: " + pointer);
            }
        } else if (test(NOT)) {
            expression = new Not(calcHighPriority());
        } else if (test(VARIABLE)) {
            expression = new Variable(word);
        } else if (test(FUNCTION)) {
            if ("count".equals(word)) {
                expression = new BitCount(calcHighPriority());
            } else {
                throw new CheckedParserException("wrong name of function: " + word);
            }
        } else if (test(NUMBER)) {
            expression = new Const(Integer.parseInt(word));
        } else if (test(MINUS)) {
            if (pointer < tokens.size()) {
                /*Token nextToken = tokens.get(pointer++);

                if (nextToken.getType() == ExpressionTokenizer.TokenType.NUMBER) {
                    expression = new Const(Integer.parseInt("-" + nextToken.getWord()));
                } else {
                    pointer--;
                    expression = new CheckedNegate(calcHighPriority());
                }*/
                if (test(NUMBER)) {
                    expression = new Const(Integer.parseInt("-" + tokens.get(pointer - 1).getWord()));
                } else {
                    expression = new CheckedNegate(calcHighPriority());
                }
            }
        } else {
            throw new CheckedParserException();
        }
        return expression;
    }

    public TripleExpression parse(String s) throws CheckedParserException {
        tokens = ExpressionTokenizer.tokenize(s);
        pointer = 0;
        balance = 0;
        TripleExpression result = calcOr();
        if (balance != 0) {
            throw new CheckedParserException("the number of opening and closing parentheses is different");
        }
        if (pointer != tokens.size()) {
            throw new CheckedParserException();
        }
        return result;
    }
}
