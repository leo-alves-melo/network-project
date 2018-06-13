import java.rmi.Naming; 
import java.rmi.RemoteException; 
import java.rmi.RMISecurityManager; 
import java.rmi.server.UnicastRemoteObject; 
import java.rmi.*;
import java.rmi.server.*;
import java.util.*;
/* 
Classname: HelloServerDemo 
Purpose: The RMI server. 
*/
public class HelloServerDemo extends UnicastRemoteObject implements Hello { 
	public HelloServerDemo() throws RemoteException { 
		super(); 
	} 
	
	public String Hello() { 
		System.out.println("Invocation to Hello was succesful!"); 
		return "Hello World from RMI server!"; 
	} 

	public static void main(String args[]) { 
		try { 
			// Creates an object of the HelloServer class. 
			System.out.println("Vou tentar ligar"); 

			//System.setSecurityManager(new RMISecurityManager());
			//System.setProperty("java.rmi.server.hostname","rmi://localhost");

			HelloServerDemo obj = new HelloServerDemo(); 

			System.out.println("Fiz um"); 
			// Bind this object instance to the name "HelloServer". 
			
			System.out.println("Fiz dois"); 
			//Naming.rebind("rmi://localhost/Hello", obj); 
			Naming.rebind("Hello", obj); 
			System.out.println("Ligado no registro"); 
		} 
		catch (Exception ex) { 
			System.out.println("error: " + ex.getMessage()); 
			ex.printStackTrace(); 
		} 
	}
}