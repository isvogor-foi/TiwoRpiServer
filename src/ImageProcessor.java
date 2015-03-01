import java.io.IOException;


public class ImageProcessor {
	
	public void processImage(byte[] image, String commands) throws IOException{
		System.out.println("Image size (Java): " + image.length);
		// prepare listener for when it's done
		RpiServerMain.setImageReadyListener(new ImageReadyListener());
		// go to FPGA
		RpiServerMain.sendImageForProcessing(image, commands); 	
	}

}
