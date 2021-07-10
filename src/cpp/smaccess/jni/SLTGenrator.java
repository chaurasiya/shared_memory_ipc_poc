import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class SLTGenrator {
    private static char[] allCharArray;
    static int MAX;
    private Set<String> lookupPermutation = new HashSet<>();
    List<String> allPossibleString = new ArrayList<>();
    static {
        char[] uppercaseCharArray = IntStream.rangeClosed('A', 'Z')
        .mapToObj(c -> "" + (char) c).collect(Collectors.joining()).toCharArray();
        char[] lowercaseCharArray = IntStream.rangeClosed('a', 'z')
        .mapToObj(c -> "" + (char) c).collect(Collectors.joining()).toCharArray();
        char[] numericCharArray = IntStream.rangeClosed('0', '9')
        .mapToObj(c -> "" + (char) c).collect(Collectors.joining()).toCharArray();
        MAX = uppercaseCharArray.length + lowercaseCharArray.length + numericCharArray.length;
        allCharArray = new char[MAX];
        System.arraycopy(uppercaseCharArray, 0, allCharArray, 0, uppercaseCharArray.length);
        System.arraycopy(lowercaseCharArray, 0, allCharArray, uppercaseCharArray.length, lowercaseCharArray.length);
        System.arraycopy(numericCharArray, 0, allCharArray, uppercaseCharArray.length+lowercaseCharArray.length, numericCharArray.length);
    }
    public static void main(String[] args) throws IOException {
        SLTGenrator sltGenrator  = new SLTGenrator();
        //Generate all possible character string upto size 2...
        sltGenrator.printAllKLength(allCharArray, 2);
        //Write the token mapping into a CSV file
        sltGenrator.outputWriter(sltGenrator.generateRandomSLT());
        // for (List<String> rowData : sltGenrator.generateRandomSTL()) {
        //     System.out.println(String.join(",", rowData));
        // }
    }

    public void outputWriter(List<String> rows) throws IOException {
        FileWriter csvWriter = new FileWriter("/home/abhi/Desktop/shared_memory_ipc_poc/src/cpp/smaccess/config/SLT_2.csv");
        for (String rowData : rows) {
            csvWriter.append(rowData);
            csvWriter.append("\n");
        }

        csvWriter.flush();
        csvWriter.close();
    }

    public List<String> generateRandomSLT(){
        List<String> rows = new ArrayList<>();
        for(String character: allPossibleString) {
            if(!lookupPermutation.contains(character)) {
                String token = getAlphaNumericString(character.length());
                String mapping = character+","+token;
                String reverseMapping = token+","+character;
                rows.add(mapping);
                rows.add(reverseMapping);
                lookupPermutation.add(character);
                lookupPermutation.add(token);
            }
        }
        return rows;
    }

    // function to generate a random string of length n
    private String getAlphaNumericString(int n)
    {
        // create StringBuffer size of AlphaNumericString
        StringBuilder sb = new StringBuilder(n);
  
        for (int i = 0; i < n; i++) {
  
            // generate a random number between
            // 0 to AlphaNumericString variable length
            int index
                = (int)(allCharArray.length
                        * Math.random());
  
            // add Character one by one in end of sb
            sb.append(allCharArray[index]);
        }
  
        return sb.toString();
    }

// The method that prints all
// possible strings of length k.
// It is mainly a wrapper over
// recursive function printAllKLengthRec()
private void printAllKLength(char[] set, int k)
{
    int n = set.length;
    if(k==0){
        System.out.println("Ignoring the empty string...");
        return;
    }
    for(int i=1;i<=k;i++) {
        printAllKLengthRec(set, "", n, i);
    }
}
 
// The main recursive method
// to print all possible
// strings of length k
private void printAllKLengthRec(char[] set,
                               String prefix,
                               int n, int k){
     
    // Base case: k is 0,
    // print prefix
    if (k == 0)
    {
        System.out.println(prefix);
        allPossibleString.add(prefix);
        return;
    }
 
    // One by one add all characters
    // from set and recursively
    // call for k equals to k-1
    for (int i = 0; i < n; ++i)
    {
 
        // Next character of input added
        String newPrefix = prefix + set[i];
         
        // k is decreased, because
        // we have added a new character
        printAllKLengthRec(set, newPrefix,
                                n, k - 1);
    }
}
}
