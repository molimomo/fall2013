import java.security.*;
import java.util.*;
import java.math.BigInteger;
public class bruteMd5{
	public static void main(String args[]){
		//Key = 12 digit hash, value = string
		HashMap<String, String> hashHash = new HashMap<String, String>();
		//HashMap<String, String> stringHash = new HashMap<String, String>();
		//int i = 0;
		
		while(true){
			//Generate new hash
			String c = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
			String tempString = generateString(new Random(),c, 16);
			String tempHash = stripMD5(generateMD5(tempString));
			
			//check hashmap for match 
			if(hashHash.containsKey(tempHash)){
				System.out.println("Match!\n");
				System.out.println("String 1: " + hashHash.get(tempHash) + " hash : " + generateMD5(hashHash.get(tempHash)) + "\n");
				System.out.println("String 2: " + tempString + " hash : " + generateMD5(tempString) + "\n");
				System.exit(0);
			//otherwise add new hash to map
			}else{
				hashHash.put(tempHash, tempString);
				//stringHash.put(i, tempString);
				//i++;
			}
			System.out.println(hashHash.size());
		}
		
	}
	
	public static String generateMD5(String s){
		StringBuffer sb = new StringBuffer();
		try{
			MessageDigest md = MessageDigest.getInstance("MD5");
			md.update(s.getBytes());
 
			byte byteData[] = md.digest();
			for (int i = 0; i < byteData.length; i++) {
				sb.append(Integer.toString((byteData[i] & 0xff) + 0x100, 16).substring(1));
			}
		}catch(Exception e){
			e.printStackTrace();
		}
	return sb.toString();
	}
	
	public static String stripMD5(String s){
		String start, end;
		end = s.substring(26);
		start = s.substring(0,6);
		//end = s.substring(27);
		//start = s.substring(0,5);
		return start + end;
	}

	
	//public static String generateRandomString(){
	//	SecureRandom random = new SecureRandom();
	//	return new BigInteger(130, random).toString(32);
	//}
	
	//Possibly faster
	public static String generateString(Random rng, String characters, int length)
{
    char[] text = new char[length];
    for (int i = 0; i < length; i++)
    {
        text[i] = characters.charAt(rng.nextInt(characters.length()));
    }
    return new String(text);
}
}
