import java.rmi.Naming; 
import java.rmi.RemoteException; 
/* 
Classname: HelloClient 
Comment: The RMI client. 
*/
public class HelloClient { 
	static String message = "blank"; 
	// The Hello object "obj" is the identifier that is 
	   
	// the Hello interface. 
	static Hello obj = null; 
	public static void main(String args[]) { 
		try { 
			System.out.println("comecei"); 
			obj = (Hello)Naming.lookup("rmi://localhost" + "/Hello"); 
			System.out.println("Vou tentar"); 
			message = obj.Hello(); 
			System.out.println("Mensagem no servidor RMI de: \"" + message + "\""); 
		} 
		catch (Exception e) { 
			System.out.println("HelloClient exception: " + e.getMessage()); 
			e.printStackTrace(); 
		} 
	} 
}