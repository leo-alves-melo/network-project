import java.rmi.Remote; 
import java.rmi.RemoteException; 
/* 
Classname: Hello 
Comment: The remote interface. 
*/
public interface Connection extends Remote { 
	String SendCode(int code) throws RemoteException; 
}