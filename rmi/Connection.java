import java.rmi.Remote; 
import java.rmi.RemoteException; 
/* 
Classname: Connection
Comment: The remote interface. 
*/
public interface Connection extends Remote { 
	String receiveCode(int code) throws RemoteException; 
}