package test;

import expression.TripleExpression;
import expression.parser.ExpressionParser;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Main {

    public static void main(String[] args) throws IOException {
        //System.out.println(new File("input.in").getAbsolutePath());

        BufferedReader br = new BufferedReader(new FileReader("input.in"));
        String formula = br.readLine();
        ExpressionParser ep = new ExpressionParser();
        try {
            TripleExpression expression = ep.parse(formula);
            System.out.println(expression.evaluate(0, 0, 1));
        } catch (Exception e){
            System.out.println(e.getMessage());
        }

    }
}
