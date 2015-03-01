

public class ImageReadyListener {
	public void onMessageRecieved(String message){
		System.out.println("Event captured (Java): " + message);
		System.out.println("Fetching image!");
		
		// calling result
		byte[] result = RpiServerMain.getProcessedImage();
		// result is here!
		System.out.println("Result size (Java): " + result.length);

		// prepare a result package with image
		RpiExchangePackage resultPackage = new RpiExchangePackage();
		resultPackage.setMessage("Done!");
		resultPackage.img = result;
		
		// send the image back
		ServerSocket server = ServerSocket.getInstance();
		server.sendMessageToClient(resultPackage);
		
		/*
		InputStream in = new ByteArrayInputStream(result);
		try{
			BufferedImage bi = ImageIO.read(in);
			ImageIO.write(bi, "jpg", new File("java-converted-from-jni.jpg"));
			System.out.println("Success!");
		} catch (IOException ex){
			ex.printStackTrace();
		}
		*/
		// send the result back....
	} 
}
