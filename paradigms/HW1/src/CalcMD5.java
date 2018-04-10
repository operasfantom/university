import java.io.*;
import java.security.*;
import java.nio.file.*;

public class CalcMD5 {
    public static void main(String[] args) {
        if (args.length == 0){
            System.err.println("Not enough launch parametres");
            return;
        }
        try {
            BufferedReader in = new BufferedReader(new InputStreamReader(new FileInputStream(args[0])));
            for (String name = in.readLine(); name != null; name = in.readLine()) {
                MessageDigest md = null;
                try {
                    md = MessageDigest.getInstance("MD5");
                    byte[] bytes = null;
                    try {
                        bytes = md.digest(Files.readAllBytes(Paths.get(name)));
                    } catch (IOException e) {
                        System.err.println("The " + name + " doesn't exist");
                    }
                    for (byte b : bytes) {
                        System.out.printf("%02X", b);
                    }
                    System.out.println();
                } catch (NoSuchAlgorithmException e) {
                    System.err.println("Cryptographic algorithm MD5 is not available");
                }
            }
        } catch (IOException e) {
            System.err.println("No file with name(s) of file(s)");
        }
    }
}
