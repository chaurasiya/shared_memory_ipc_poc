
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

		System.out.println("Orignal="+data+", Token="+jniProvider.generateToken(data, null));
		System.out.println("Token=MBcDc0Jk"+ "Token="+jniProvider.getOriginal(new String("MBcDc0Jk"), null));
	}
	private native String generateToken(String data, String initKey);
	private native String getOriginal(String token, String initKey);
}
