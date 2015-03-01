
//
// run as: java -Djava.library.path=/home/ivan/Dev/java/workspace/TiwoRpiServer/src/cimgs/ -jar jartest.jar

public class RpiServerMain {
	// declare native functions
	public native static void sendImageForProcessing(byte[] image, String commands);
	public native static void setImageReadyListener(ImageReadyListener listener);
	public native static byte[] getProcessedImage();

	//load library
	static {
		System.loadLibrary("SenderImpl");
	}

	// begin main
	public static void main(String[] args) {
		ServerSocket server = ServerSocket.getInstance();
		server.start();
	}
	

	
}
