
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
		String data="abhishek";
		jniProvider.initConfig();
		System.out.println("Orignal="+data+", Token="+jniProvider.generateToken(data, null));
	}
	private native int generateToken(String[] data, String[] tokenVal);
	private native String generateToken(String data, String initKey);
	private native void initConfig();
}
