import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.IntStream;


public class SLTGenrator {
    private static char[] allCharArray;
    static int MAX;
    private Set<String> lookupPermutation = new HashSet<>();
    static List<String> allPossibleString = new ArrayList<>();
    Map<String,String> SLT_1_3_MAP = new HashMap<>();

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
        sltGenrator.printAllKLength(allCharArray, 3);
        //Write the token mapping into a CSV file
        List<String> allPermutationOfK = new ArrayList<>();
        allPermutationOfK.addAll(allPossibleString);
        sltGenrator.randomize(allPossibleString);
        System.out.println("Total number of permutations of size 3 with all alphanumeric characters="+allPermutationOfK.size());
        //sltGenrator.outputWriter(allPermutationOfK,allPossibleString);
        
        sltGenrator.prepareHashMap(allPermutationOfK,allPossibleString);
        String data = "abhishek";
        String protectedData = sltGenrator.tokenize(data);
        String orignalData = sltGenrator.detokenize(protectedData);
        System.out.println("Data="+data+", Token="+protectedData);
        System.out.println("TokenizedData="+protectedData+", Original="+orignalData);
        // for (List<String> rowData : sltGenrator.generateRandomSTL()) {
        //     System.out.println(String.join(",", rowData));
        // }
    }

    private String detokenize(String protectedData) {
        return null;
    }

    private String tokenize(String data) {
        String tokenValue="";
        for(int i=0;i<3;i++) {
            tokenValue=oneRoundOfSubstitution(data);
            data=tokenValue;
        }

        return tokenValue;
    }
    private String oneRoundOfSubstitution(String input) {
        String token1 = chainingLeftToRight(input);
        String token2 = chainingLeftToRight(reverseString(token1));
        return reverseString(token2);
    }
    private String chainingLeftToRight(String input) {
        if(input.length()==3) {
            return SLT_1_3_MAP.get(input);
        }
        int l=0;
        int r=2;
        String carry="";
        String token1="";
        boolean firstSub=true;
        System.out.println("Doing some chaining operation=>>");
        while(l<=r && r<=input.length() && l<input.length()) {
            int rightIndex=r<input.length() ? r+1 : r;
            if(r==input.length()-1){
                token1 += SLT_1_3_MAP.get(carry+input.substring(l,rightIndex));
            } else if(l < input.length()) {
                token1 += SLT_1_3_MAP.get(carry+input.substring(l,rightIndex)).substring(0, 2);
            } else{
                token1 += SLT_1_3_MAP.get(carry+input.substring(l,rightIndex));
            }
            if(firstSub) {
                l+=3;
                r+=2;
                firstSub=false;
            } else {
                l+=2;
                r+=2;
            }
            if(l==input.length()-1){
                carry = token1.substring(token1.length()-2);
            } else {
                carry = token1.charAt(token1.length()-1)+"";
            }
        }
        return token1;
    }
    private String reverseString(String str) {
        StringBuilder input1 = new StringBuilder();
 
        // append a string into StringBuilder input1
        input1.append(str);
 
        // reverse StringBuilder input1
        return input1.reverse().toString();
    }
    private void prepareHashMap(List<String> keys, List<String> values) {
        for(int i=0;i<keys.size();i++) {
            this.SLT_1_3_MAP.put(keys.get(i), values.get(i));
        }

    }

    private void outputWriter(List<String> keys, List<String> values) throws IOException {
        FileWriter csvWriter = new FileWriter("/home/abhi/Desktop/shared_memory_ipc_poc/src/cpp/smaccess/config/SLT_1_3.csv");
        for(int i=0;i<keys.size();i++) {
            csvWriter.append(keys.get(i)+","+values.get(i));
            csvWriter.append("\n");
        }
        csvWriter.flush();
        csvWriter.close();
    }
    /**
     * fisher-yates-shuffle-algorithm
     * @param arr
     * @param n
     */
    public void randomize( List<String> list)
    {
        // Creating a object for Random class
        Random r = new Random();
         
        // Start from the last element and swap one by one. We don't
        // need to run for the first element that's why i > 0
        int n = list.size();
        for (int i = n-1; i > 0; i--) {
             
            // Pick a random index from 0 to i
            int j = r.nextInt(i+1);
             
            // Swap arr[i] with the element at random index
            String temp = list.get(i);
            list.set(i, list.get(j));
            list.set(j, temp);
        }
        // Prints the random array
        //System.out.println(Arrays.toString(arr));
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
    printAllKLengthRec(set, "", n, k);
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
        //System.out.println(prefix);
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
