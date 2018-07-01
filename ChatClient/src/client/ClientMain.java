/**
 * ClassName: ClientMain.java
 * Author: qiujy
 * CreateTime: 2009-4-15
 * EMAIL: qjyong@gmail.com
 * Copyright 2009 ++YONG All rights reserved.
 */
package client;

import java.awt.Color;
import java.awt.Font;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.Enumeration;
import java.util.LinkedList;

import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.plaf.FontUIResource;

import org.jvnet.substance.skin.*;

import client.model.entity.DataBuffer;
import client.model.entity.Global;
import client.model.entity.OnlineUserListModel;
import client.model.entity.Person;
import client.ui.LoginFrame;

/** 客户端入口类 */
public class ClientMain {
	
	 private static void InitGlobalFont(Font font) {
		 UIManager.put("ToolTip.font",font);   
		 UIManager.put("Table.font",font);   
		 UIManager.put("TableHeader.font",font);   
		 UIManager.put("TextField.font",font);   
		 UIManager.put("ComboBox.font",font);   
		 UIManager.put("TextField.font",font);   
		 UIManager.put("PasswordField.font",font);   
		 UIManager.put("TextArea.font",font);   
		 UIManager.put("TextPane.font",font);   
		 UIManager.put("EditorPane.font",font);   
		 UIManager.put("FormattedTextField.font",font);   
		 UIManager.put("Button.font",font);   
		 UIManager.put("CheckBox.font",font);   
		 UIManager.put("RadioButton.font",font);   
		 UIManager.put("ToggleButton.font",font);   
		 UIManager.put("ProgressBar.font",font);   
		 UIManager.put("DesktopIcon.font",font);   
		 UIManager.put("TitledBorder.font",font);   
		 UIManager.put("Label.font",font);   
		 UIManager.put("List.font",font);   
		 UIManager.put("TabbedPane.font",font);   
		 UIManager.put("MenuBar.font",font);   
		 UIManager.put("Menu.font",font);   
		 UIManager.put("MenuItem.font",font);   
		 UIManager.put("PopupMenu.font",font);   
		 UIManager.put("CheckBoxMenuItem.font",font);   
		 UIManager.put("RadioButtonMenuItem.font",font);   
		 UIManager.put("Spinner.font",font);   
		 UIManager.put("Tree.font",font);   
		 UIManager.put("ToolBar.font",font);   
		 UIManager.put("OptionPane.messageFont",font);   
		 UIManager.put("OptionPane.buttonFont",font);  
		 UIManager.put("InternalFrame.titleFont",font);
		 
	 }
	 
	public static void main(String[] args) throws ClassNotFoundException, InstantiationException, IllegalAccessException, UnsupportedLookAndFeelException {
		Font font =  new Font("宋体",Font.PLAIN,12); 
		InitGlobalFont(font);
	
		connection(); //连接到服务器
		new ClientThread().start();
		//设置外观感觉
		JFrame.setDefaultLookAndFeelDecorated(true);
		JDialog.setDefaultLookAndFeelDecorated(true);
		try {
			//UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
			UIManager.setLookAndFeel(new SubstanceOfficeSilver2007LookAndFeel());
			UIManager.setLookAndFeel(new SubstanceBusinessLookAndFeel());
			
		} catch (Exception e) {
			e.printStackTrace();
		}

		Global.loginFrame = new LoginFrame();  //启动登录窗体
		if(DataBuffer.currentUser!=null)
			System.out.println(DataBuffer.currentUser.getId());
	}

	/** 连接到服务器 */
	public static void connection() {
		String ip = DataBuffer.configProp.getProperty("ip");
		int port = Integer.parseInt(DataBuffer.configProp.getProperty("port"));
		try {
			
			DataBuffer.clientSeocket = new Socket(ip, port);
			DataBuffer.oos = DataBuffer.clientSeocket.getOutputStream();
			DataBuffer.ois = DataBuffer.clientSeocket.getInputStream();
			DataBuffer.onlineUsers =  new LinkedList<Person>();
			DataBuffer.onlineUserListModel = new OnlineUserListModel(DataBuffer.onlineUsers);
		} catch (Exception e) {
			JOptionPane.showMessageDialog(new JFrame(), 
					"连接服务器失败,请检查!"+e.getMessage(),"服务器未连上", JOptionPane.ERROR_MESSAGE);//否则连接失败
			System.exit(0);
		}
	}
	

}
