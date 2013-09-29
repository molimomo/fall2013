/*Write a program that reads 4 unsigned ints sent in host byte order 
from hitchens.cs.colorado.edu por 1234 adds them up, and sends them back
to that port. (This is a little-endian machine. Recall that "network 
order" is big-endian.)When you successfully accomplish this task, you 
will get a username and password as output.*/
import java.net.Socket;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.DataInputStream;
import java.io.BufferedReader;


public class socket{
	public static void main(String args[]){
		int[] uInts = new int[4]; //array to hold 4 unsigned integers
		try{
			Socket s = new Socket("128.138.201.119", 1234);
			DataInputStream input = new DataInputStream(s.getInputStream());
			//BufferedReader input = new BufferedReader(new InputStreamReader(s.getInputStream()));
			Long answer = input.readLong();
			System.out.println(answer);
		}catch(Exception e){
			System.out.println("Socket connection error!\n");
			e.printStackTrace();
		}
		
		
	}
}
