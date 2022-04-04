import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.util.*;
public class DictionaryFile {


    public static void main(String[] args) throws FileNotFoundException {

        File uncut = new File("words.txt");
        Scanner scanner = new Scanner(uncut);
        File CutDictionary = new File("slimmedDictionary.txt");
        PrintStream stream = new PrintStream(CutDictionary);
        System.setOut(stream);
        while (scanner.hasNext()){
            String word = scanner.next();
            if(word.matches("[a-zA-Z]+") && word.length() <6 && word.length() >2){
                System.out.print(word.toLowerCase()+"\n");
            }
        }
    }
}
