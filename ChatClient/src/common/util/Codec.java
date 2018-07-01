package common.util;

import com.google.protobuf.*;

public class Codec {
	public static byte[] encode(GeneratedMessage msg){
		
		String name = msg.getDescriptorForType().getName();
		byte[] dataName = name.getBytes();
		
		int nameLen = name.length();
		byte[] dataNameLen = Convert.toByteArray(nameLen);
		
		byte[] dataBody =msg.toByteArray();
			
		int tmplen = dataBody.length + dataNameLen.length + dataName.length;
		byte[] dataLen = Convert.toByteArray(tmplen);	
		
		byte[] data = new byte[dataLen.length+ tmplen];
		
		System.arraycopy(dataLen,     0, data, 0,                                                     dataLen.length);
		System.arraycopy(dataNameLen, 0, data, dataLen.length,                                        dataNameLen.length);
		System.arraycopy(dataName,    0, data, dataLen.length + dataNameLen.length,                   dataName.length);
		System.arraycopy(dataBody,    0, data, dataLen.length + dataNameLen.length + dataName.length, dataBody.length);
		
		return data;
	}
}
