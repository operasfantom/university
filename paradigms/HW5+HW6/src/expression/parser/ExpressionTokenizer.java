package expression.parser;

import java.util.ArrayList;
import java.util.List;

import static expression.parser.ExpressionTokenizer.TokenType.*;

public class ExpressionTokenizer {
    public static List<Token> tokenize(String s) {
        List<Token> result = new ArrayList<>();
        for (int i = 0; i < s.length(); ) {
            char c = s.charAt(i);
            switch (c) {
                case ' ':
                case '\t':
                    i++;
                    break;
                case '~':
                    i = getToken(s, i, result, NEGATION);
                    break;
                case '&':
                    i = getToken(s, i, result, AND);
                    break;
                case '^':
                    i = getToken(s, i, result, XOR);
                    break;
                case '|':
                    i = getToken(s, i, result, OR);
                    break;
                case '(':
                    i = getToken(s, i, result, BRACKET_OPEN);
                    break;
                case ')':
                    i = getToken(s, i, result, BRACKET_CLOSE);
                    break;
                case '*':
                    i = getToken(s, i, result, TIMES);
                    break;
                case '/':
                    i = getToken(s, i, result, SLASH);
                    break;
                case '+':
                    i = getToken(s, i, result, PLUS);
                    break;
                case '-':
                    i = getToken(s, i, result, MINUS);
                    break;
                default: //numbers and letters
                    Token token = new Token();
                    i = NextToken(s, i, token);
                    result.add(token);
                    break;
            }
        }
        return result;
    }

    private static int getToken(String s, int i, List<Token> result, TokenType type) {
        result.add(new Token(type, s.substring(i, i + 1)));
        i++;
        return i;
    }


    private static int NextToken(String s, int i, Token token) {
        int type = Character.getType(s.charAt(i));
        StringBuilder word = new StringBuilder();
        while (i < s.length() && (Character.getType(s.charAt(i)) == type)) {
            word.append(s.charAt(i));
            i++;
        }
        switch (type) {
            case Character.LOWERCASE_LETTER:
                if (word.length() == 1) {
                    token.type = VARIABLE;
                } else {
                    token.type = FUNCTION;
                }
                break;
            case Character.DECIMAL_DIGIT_NUMBER:
                token.type = NUMBER;
                break;
            default:

        }
        token.word = word.toString();
        return i;
    }

    public enum TokenType {
        AND,           // &
        XOR,           // ^
        OR,            // |

        TIMES,         // *
        SLASH,         // /

        PLUS,          // +
        MINUS,         // -

        NEGATION,      // ~
        VARIABLE,      // x, y, z...
        FUNCTION,      // count...
        NUMBER,        // -1, 1, 999...
        BRACKET_OPEN,  // (
        BRACKET_CLOSE  // )
    }

    public static class Token {
        private TokenType type;
        private String word;

        public Token(TokenType type, String word) {
            this.type = type;
            this.word = word;
        }

        public Token() {

        }

        public TokenType getType() {
            return type;
        }

        public String getWord() {
            return word;
        }
    }
}
