import java.util.*;
public class JNIProvider {
	private static String jniBinaryPath;
	public JNIProvider(){
		loadLibrary();
	}
	private static void loadLibrary(){
		jniBinaryPath=getPath();
		System.loadLibrary(jniBinaryPath);
	}
	private static String getPath(){
		return "sm_access";
	}
	public static void main(String[] args){
		JNIProvider jniProvider = new JNIProvider();
		String[] data = new String[1];
		String[] tokenVal = new String[1];
		data[0]="abhishek";
		System.out.println("Calling JNI function\n.Out="+jniProvider.generateToken(data,tokenVal));
	}
	private native int generateToken(String[] data, String[] tokenVal);
}
