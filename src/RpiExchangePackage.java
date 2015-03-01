


public class RpiExchangePackage {
	private String message;
	public byte[] img;
	private String fpgaCommand;
	// add time measurement stuff
	
	public String getMessage() {
		return message;
	}
	public void setMessage(String message) {
		this.message = message;
	}
	public String getFpgaCommand() {
		return fpgaCommand;
	}
	public void setFpgaCommand(String fpgaCommand) {
		this.fpgaCommand = fpgaCommand;
	} 
	
	
	
}
