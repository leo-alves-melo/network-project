import java.rmi.Naming; 
import java.rmi.RemoteException; 
/* 
Classname: Client 
Comment: The RMI client. 
*/
public class Client { 
	static String message = "blank"; 
	// The Hello object "obj" is the identifier that is 
	   
	// the Hello interface. 
	static Connection server = null; 
	public static void main(String args[]) { 
		try { 
			System.out.println("comecei"); 
			server = (Connection) Naming.lookup("rmi://localhost" + "/Server"); 
			System.out.println("Vou tentar"); 
			message = server.receiveCode(2); 
			System.out.println("Mensagem no servidor RMI de: \"" + message + "\""); 
		} 
		catch (Exception e) { 
			System.out.println("HelloClient exception: " + e.getMessage()); 
			e.printStackTrace(); 
		} 
	} 
}