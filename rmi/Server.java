import java.rmi.Naming; 
import java.rmi.RemoteException; 
import java.rmi.RMISecurityManager; 
import java.rmi.server.UnicastRemoteObject; 
import java.rmi.*;
import java.rmi.server.*;
import java.util.*;
/* 
Classname: Server 
Purpose: The RMI server. 
*/
public class Server extends UnicastRemoteObject implements Connection { 
	public Server() throws RemoteException { 
		super(); 
	} 
	
	public String receiveCode(int code) { 
		System.out.print("Recebi a mensagem: "); 
		System.out.println(code); 
		return "Hello World from RMI server!"; 
	} 

	public static void main(String args[]) { 
		try { 
			// Creates an object of the HelloServer class. 
			System.out.println("Vou tentar ligar"); 

			//System.setSecurityManager(new RMISecurityManager());
			//System.setProperty("java.rmi.server.hostname","rmi://localhost");

			Server obj = new Server(); 
			System.out.println("opa"); 

			Naming.rebind("Server", obj); 
			System.out.println("Ligado no registro"); 
		} 
		catch (Exception ex) { 
			System.out.println("error: " + ex.getMessage()); 
			ex.printStackTrace(); 
		} 
	}
}