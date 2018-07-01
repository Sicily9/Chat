
package client.model.entity;

import java.awt.Dimension;
import java.awt.Toolkit;
import java.io.*;
import java.net.*;
import java.util.*;



public class DataBuffer {
	/** ��ǰ�ͻ��˵��û���Ϣ */
	public static Person currentUser;
	/** �����û��б� */
	public static List<Person> onlineUsers;
	/** ��ǰ�ͻ������ӵ����������׽��� */
	public static Socket clientSeocket;
	/** ��ǰ�ͻ������ӵ�������������� */
	public static OutputStream oos;
	/** ��ǰ�ͻ������ӵ��������������� */
	public static InputStream ois;
	/** ���������ò������Լ� */
	public static Properties configProp;
	/** ��ǰ�ͻ��˵���Ļ�ߴ� */
	public static Dimension screenSize;
	/** ���ͻ��˵�IP��ַ */
	public static String ip ;
	/** ���������ļ��Ķ˿� */
	public static final int RECEIVE_FILE_PORT = 6666;
	/** �����û�JList��Model */
	public static OnlineUserListModel onlineUserListModel;

	static{
		screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		//���ط����������ļ�
		configProp = new Properties();
		try {
			ip = InetAddress.getLocalHost().getHostAddress();
			configProp.load(Thread.currentThread()
									.getContextClassLoader()
									.getResourceAsStream("data.properties"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	private DataBuffer(){}
};
