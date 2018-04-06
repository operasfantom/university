package expression.parser;

import expression.exceptions.CheckedParserException;

import java.util.ArrayList;
import java.util.List;

import static expression.parser.ExpressionTokenizer.TokenType.*;

public class ExpressionTokenizer {
    static List<Token> tokenize(String s) throws CheckedParserException {
        List<Token> result = new ArrayList<>();
        for (int i = 0; i < s.length(); ) {
            char c = s.charAt(i);
            switch (c) {
                case ' ':
                case '\t':
                    i++;
                    break;
                case '~':
                    i = getToken(s, i, 1, result, NOT);
                    break;
                case '&':
                    i = getToken(s, i, 1, result, AND);
                    break;
                case '^':
                    i = getToken(s, i, 1, result, XOR);
                    break;
                case '|':
                    i = getToken(s, i, 1, result, OR);
                    break;
                case '(':
                    i = getToken(s, i, 1, result, BRACKET_OPEN);
                    break;
                case ')':
                    i = getToken(s, i, 1, result, BRACKET_CLOSE);
                    break;
                case '*': {
                    int length = readIdentical(s, i, s.charAt(i));
                    if (length == 1) {
                        i = getToken(s, i, length, result, TIMES);
                    } else if (length == 2) {
                        i = getToken(s, i, length, result, POWER);
                    } else {
                        throw new CheckedParserException("there are " + length + "* in a row");
                    }
                    break;
                }
                case '/': {
                    int length = readIdentical(s, i, s.charAt(i));
                    if (length == 1) {
                        i = getToken(s, i, length, result, SLASH);
                    } else if (length == 2) {
                        i = getToken(s, i, length, result, LOGARITHM);
                    } else {
                        throw new CheckedParserException("there are " + length + "/ in a row");
                    }
                    break;
                }
                case '+':
                    i = getToken(s, i, 1, result, PLUS);
                    break;
                case '-':
                    i = getToken(s, i, 1, result, MINUS);
                    break;
                default: //numbers and letters
                    Token token = new Token();
                    i = readIdentifier(s, i, token);
                    result.add(token);
                    break;
            }
        }
        return result;
    }

    private static int getToken(String s, int i, int length, List<Token> result, TokenType type) {
        result.add(new Token(type, s.substring(i, i + length)));
        i += length;
        return i;
    }

    private static int readIdentical(String s, int i, char c) {
        int result = 0;
        while (i < s.length() && s.charAt(i) == c) {
            ++result;
            ++i;
        }
        return result;
    }

    private static int readIdentifier(String s, int i, Token token) throws CheckedParserException {
        StringBuilder word = new StringBuilder();
        while (i < s.length() && (Character.isJavaIdentifierPart(s.charAt(i)))) {
            word.append(s.charAt(i));
            i++;
        }
        token.word = word.toString();
        switch (token.word) {
            case "x":
            case "y":
            case "z":
                token.type = VARIABLE;
                break;
            case "count":
                token.type = FUNCTION;
                break;
            case "log10":
                token.type = LOG10;
                break;
            case "pow10":
                token.type = POW10;
                break;
            default:
                if (token.word.matches("-?\\d+")) {
                    token.type = NUMBER;
                } else {
                    throw new CheckedParserException();
                }
        }
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

        NOT,           // ~
        VARIABLE,      // x, y, z...
        FUNCTION,      // count...
        NUMBER,        // -1, 1, 999...
        BRACKET_OPEN,  // (
        BRACKET_CLOSE, // )
        LOG10,         // logarithm for base 10
        POW10,         // power of 10
        POWER,         // a ^ b
        LOGARITHM      // log a base b
    }

    static class Token {
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
