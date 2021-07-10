import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class SLTGenrator {
    private static char[] allCharArray;
    static int MAX;
    static {
        char[] uppercaseCharArray = IntStream.rangeClosed('A', 'Z')
        .mapToObj(c -> "" + (char) c).collect(Collectors.joining()).toCharArray();
        char[] lowercaseCharArray = IntStream.rangeClosed('a', 'z')
        .mapToObj(c -> "" + (char) c).collect(Collectors.joining()).toCharArray();
        MAX = uppercaseCharArray.length + lowercaseCharArray.length;
        allCharArray = new char[MAX];
        System.arraycopy(uppercaseCharArray, 0, allCharArray, 0, uppercaseCharArray.length);
        System.arraycopy(lowercaseCharArray, 0, allCharArray, uppercaseCharArray.length, lowercaseCharArray.length);
    }
    public static void main(String[] args) throws IOException {
        SLTGenrator sltGenrator  = new SLTGenrator();
        sltGenrator.outputWriter(sltGenrator.generateRandomSTL());
        // for (List<String> rowData : sltGenrator.generateRandomSTL()) {
        //     System.out.println(String.join(",", rowData));
        // }
    }

    public void outputWriter(List<List<String>> rows) throws IOException {
        FileWriter csvWriter = new FileWriter("/home/abhi/Desktop/shared_memory_ipc_poc/src/cpp/smaccess/config/SLT_2.csv");
        csvWriter.append("character");
        csvWriter.append(",");
        csvWriter.append("alphamapping");
        csvWriter.append(",");
        csvWriter.append("numericmapping");
        csvWriter.append("\n");

        for (List<String> rowData : rows) {
            csvWriter.append(String.join(",", rowData));
            csvWriter.append("\n");
        }

        csvWriter.flush();
        csvWriter.close();
    }

    public List<List<String>> generateRandomSTL(){
        List<List<String>> rows = new ArrayList<>();
        for(int i=0;i<allCharArray.length;i++) {
            List<String> mapping = new ArrayList<>();
            mapping.add(allCharArray[i]+"");
            mapping.add(allCharArray[(int) (Math.random() * 100 % MAX)]+"");
            mapping.add((int)(Math.random() * 10 % 10)+"");
            rows.add(mapping);
        }
        return rows;
    }
}
