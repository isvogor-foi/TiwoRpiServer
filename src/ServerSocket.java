/**
 * This class uses KryoNet project: 
 * https://github.com/EsotericSoftware/kryonet
 */


import java.io.IOException;

import com.esotericsoftware.kryonet.Connection;
import com.esotericsoftware.kryonet.Listener;
import com.esotericsoftware.kryonet.Server;

public class ServerSocket {

	private static ServerSocket serverSocketInstance;
	private static Server serverInstance;
	private ImageProcessor imageProcessor;


	private ServerSocket() {
		// set buffer sizes - important, without you get buffer overflow
		//https://code.google.com/p/kryonet/source/browse/trunk/kryonet/test/com/esotericsoftware/kryonet/InputStreamSenderTest.java
		serverInstance = new Server(131072, 131072);
		imageProcessor = new ImageProcessor();
	}

	public static ServerSocket getInstance() {
		if (serverSocketInstance == null)
			serverSocketInstance = new ServerSocket();
		return serverSocketInstance;
	}

	public void start() {
		serverInstance.start();
		try {
			serverInstance.bind(1234);
		} catch (IOException e) {
			e.printStackTrace();
		}
		serverInstance.addListener(serverListener);
		// register communication classes
		serverInstance.getKryo().register(byte[].class);
		serverInstance.getKryo().register(RpiExchangePackage.class);
	}
	
	public void stop(){
		serverInstance.stop();
		try {
			serverInstance.dispose();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void sendMessageToClient(RpiExchangePackage response){
		serverInstance.getConnections()[0].sendTCP(response);
	}

	/**
	 * Handle what happens when server receives a message from a client
	 */
	private Listener serverListener = new Listener() {
		public void received(Connection connection, Object object) {
			if (object instanceof RpiExchangePackage) {
				RpiExchangePackage request = (RpiExchangePackage) object;
				System.out.println("Received: " + request.getMessage() + ", " + request.getFpgaCommand());
				
				// send image for processing...
				try {
					ImageProcessor ipc = new ImageProcessor();
					System.out.println("New image processor (server-java)");
					ipc.processImage(request.img, request.getFpgaCommand());
				} catch (IOException e) {
					e.printStackTrace();
				}
				
			}
		}
	};

}
