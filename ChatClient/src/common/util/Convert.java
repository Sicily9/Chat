package common.util;
import java.nio.ByteBuffer;  
import java.io.ByteArrayInputStream;   
import java.io.ByteArrayOutputStream;   
import java.io.IOException;   
import java.io.ObjectInputStream;   
import java.io.ObjectOutputStream;   
  
public class Convert {
	final protected static char[] hexArray = "0123456789ABCDEF".toCharArray(); 
	
	public static byte[] toByteArray(int num){  
        byte[] result = new byte[4];  
        result[0] = (byte)((num >>> 24) & 0xff);//说明一  
        result[1] = (byte)((num >>> 16)& 0xff );  
        result[2] = (byte)((num >>> 8) & 0xff );  
        result[3] = (byte)((num >>> 0) & 0xff );  
        return result;  
    }  
	

	public static byte[] toByteArray (Object obj) {      
        byte[] bytes = null;      
        ByteArrayOutputStream bos = new ByteArrayOutputStream();      
        try {        
            ObjectOutputStream oos = new ObjectOutputStream(bos);    
            System.out.println("1");
            oos.writeObject(obj);        
            oos.flush();         
            bytes = bos.toByteArray ();      
            oos.close();         
            bos.close();        
        } catch (IOException ex) {        
            ex.printStackTrace();   
        }      
        return bytes;    
	}
	
	public static Object toObject (byte[] bytes) {      
        Object obj = null;      
        try {        
            ByteArrayInputStream bis = new ByteArrayInputStream (bytes);        
            ObjectInputStream ois = new ObjectInputStream (bis);        
            obj = ois.readObject();      
            ois.close();   
            bis.close();   
        } catch (IOException ex) {        
            ex.printStackTrace();   
        } catch (ClassNotFoundException ex) {        
            ex.printStackTrace();   
        }      
        return obj;    
    } 
	
	public static short bytes2Short(byte[] src){  
	        ByteBuffer buffer = ByteBuffer.wrap(src);  
	        return buffer.getShort();  
	}
	
	public static int bytes2Int(byte[] bytes){  
		int result = 0;  
        if(bytes.length == 4){  
            int a = (bytes[0] & 0xff) << 24;//说明二  
            int b = (bytes[1] & 0xff) << 16;  
            int c = (bytes[2] & 0xff) << 8;  
            int d = (bytes[3] & 0xff);  
            result = a | b | c | d;  
        }  
        return result;  
	}
	
	public static long bytes2Long(byte[] src){  
	        ByteBuffer buffer = ByteBuffer.wrap(src);    
	        return buffer.getLong();  
	}  
	
	public static String bytesToHex(byte[] bytes) {  
	    char[] hexChars = new char[bytes.length * 2];  
	    for ( int j = 0; j < bytes.length; j++ ) {  
	        int v = bytes[j] & 0xFF;  
	        hexChars[j * 2] = hexArray[v >>> 4];  
	        hexChars[j * 2 + 1] = hexArray[v & 0x0F];  
	    }  
	    return new String(hexChars);  
	}  
	
}
