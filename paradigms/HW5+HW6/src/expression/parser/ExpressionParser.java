package expression.parser;

import expression.*;

import java.util.ArrayList;
import java.util.List;

import static expression.parser.ExpressionTokenizer.Token;
import static expression.parser.ExpressionTokenizer.TokenType.*;

public class ExpressionParser implements Parser {
    private static List<Token> tokens = new ArrayList<>();
    private static int pointer = 0;

    private static CommonExpression calcOr() {
        CommonExpression expression = calcXor();

        while (pointer < tokens.size()) {
            Token token = tokens.get(pointer++);
            switch (token.getType()) {
                case OR:
                    expression = new Or(expression, calcXor());
                    break;
                default:
                    pointer--;
                    return expression;
            }
        }
        return expression;
    }

    private static CommonExpression calcXor() {
        CommonExpression expression = calcAnd();

        while (pointer < tokens.size()) {
            Token token = tokens.get(pointer++);
            switch (token.getType()) {
                case XOR:
                    expression = new Xor(expression, calcAnd());
                    break;
                default:
                    pointer--;
                    return expression;
            }
        }
        return expression;
    }

    private static CommonExpression calcAnd() {
        CommonExpression expression = calcPlusMinus();

        while (pointer < tokens.size()) {
            Token token = tokens.get(pointer++);
            switch (token.getType()) {
                case AND:
                    expression = new And(expression, calcPlusMinus());
                    break;
                default:
                    pointer--;
                    return expression;
            }
        }
        return expression;
    }

    private static CommonExpression calcPlusMinus() {
        CommonExpression expression = calcMulDiv();

        while (pointer < tokens.size()) {
            if (test(PLUS)) {
                expression = new Add(expression, calcMulDiv());
            } else if (test(MINUS)) {
                expression = new Subtract(expression, calcMulDiv());
            } else {
                return expression;
            }
        }

        return expression;
    }

    private static boolean test(ExpressionTokenizer.TokenType type) {
        if (tokens.get(pointer).getType() == type) {
            pointer++;
            return true;
        }
        return false;
    }

    private static CommonExpression calcMulDiv() {
        CommonExpression expression = calcHighPriority();

        while (pointer < tokens.size()) {
            Token token = tokens.get(pointer++);
            switch (token.getType()) {
                case TIMES:
                    expression = new Multiply(expression, calcHighPriority());
                    break;
                case SLASH:
                    expression = new Divide(expression, calcHighPriority());
                    break;
                default:
                    pointer--;
                    return expression;
            }
        }
        return expression;
    }

    private static CommonExpression calcHighPriority() {
        CommonExpression expression = null;
        Token token = tokens.get(pointer++);
        switch (token.getType()) {
            case BRACKET_OPEN:
                expression = calcOr();
                pointer++;
                break;
            case BRACKET_CLOSE:
                break;
            case NEGATION:
                expression = new Negate(calcHighPriority());
                break;
            case VARIABLE:
                expression = new Variable(token.getWord());
                break;
            case FUNCTION:
                if ("count".equals(token.getWord())) {
                    expression = new BitCount(calcHighPriority());
                } else{
                    //null
                }
                break;
            case NUMBER:
                expression = new Const(Integer.parseInt(token.getWord()));
                break;
            case MINUS:
                if (pointer < tokens.size()) {
                    Token nextToken = tokens.get(pointer++);
                    if (nextToken.getType() == ExpressionTokenizer.TokenType.NUMBER) {
                        expression = new Const(Integer.parseInt("-" + nextToken.getWord()));
                    } else {
                        pointer--;
                        expression = new Unary(calcHighPriority());
                    }
                }
                break;
            default:
                pointer--;
                return expression;
        }
        return expression;
    }

    public CommonExpression parse(String s) {
        tokens = ExpressionTokenizer.tokenize(s);
        pointer = 0;
        return calcOr();
    }
}
