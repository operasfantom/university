import java.io.*;
import java.security.*;
import java.nio.file.*;

public class CalcSHA256 {
    public static void main(String[] args) {
        if (args.length == 0) {
            System.err.println("Not enough launch arguments");
            return;
        }
        try (BufferedReader in = new BufferedReader(new FileReader(args[0]))) {
            for (String name = in.readLine(); name != null; name = in.readLine()) {
                try {
                    MessageDigest md = MessageDigest.getInstance("SHA-256");
                    byte[] bytes = md.digest(Files.readAllBytes(Paths.get(name)));
                    for (byte b : bytes) {
                        System.out.printf("%02X", b);
                    }
                    System.out.println();
                } catch (NoSuchAlgorithmException e) {
                    System.err.println("Cryptographic algorithm MD5 is not available");
                } catch (IOException e) {
                    System.err.println("The " + name + " doesn't exist");
                }
            }
        } catch (IOException e) {
            System.err.println("No file with name(s) of file(s)");
        }
   }
}