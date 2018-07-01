/**
 * ClassName: LoginFrame.java
 * Author: qiujy
 * CreateTime: 2009-4-22
 * EMAIL: qjyong@gmail.com
 * Copyright 2009 ++YONG All rights reserved.
 */
package client.ui;

import java.awt.*;
import java.awt.event.*;
import java.io.IOException;
import java.util.List;
import javax.swing.*;
import javax.swing.border.*;

import client.model.entity.DataBuffer;
import client.model.entity.Global;
import client.model.entity.Person;
import client.util.ClientUtil;
import common.model.entity.*;
import common.util.Codec;
import common.util.Convert;
import common.util.SocketUtil;


/** ��¼���� */
public class LoginFrame extends JFrame {
	private static final long serialVersionUID = -3426717670093483287L;
	private JTextField idTxt;
	private JPasswordField pwdFld;
	
	public LoginFrame(){
		this.init();
		setVisible(true);
	}
	
	public void init(){
		this.setTitle("Chat Login");
		this.setSize(330, 230);
		//����Ĭ�ϴ�������Ļ����
		int x = (int)Toolkit.getDefaultToolkit().getScreenSize().getWidth();
		int y = (int)Toolkit.getDefaultToolkit().getScreenSize().getHeight();
		this.setLocation((x - this.getWidth()) / 2, (y-this.getHeight())/ 2);
		this.setResizable(false);
		
		//��Logo���õ�JFrame�ı���
		Icon icon = new ImageIcon("images/logo.png");
		JLabel label = new JLabel(icon);
		this.add(label, BorderLayout.NORTH);

		//��¼��Ϣ���
		JPanel mainPanel = new JPanel();
		Border border = BorderFactory.createEtchedBorder(EtchedBorder.LOWERED);
		mainPanel.setBorder(BorderFactory.createTitledBorder(border, "�����¼��Ϣ", TitledBorder.CENTER,TitledBorder.TOP));
		this.add(mainPanel, BorderLayout.CENTER);
		mainPanel.setLayout(null);
		
		JLabel nameLbl = new JLabel("�˺�:");
		nameLbl.setBounds(50, 30, 40, 22);
		mainPanel.add(nameLbl);
		idTxt = new JTextField();
		idTxt.setBounds(95, 30, 150, 22);
		idTxt.requestFocusInWindow();//�û�����ý���
		mainPanel.add(idTxt);
		
		JLabel pwdLbl = new JLabel("����:");
		pwdLbl.setBounds(50, 60, 40, 22);
		mainPanel.add(pwdLbl);
		pwdFld = new JPasswordField();
		pwdFld.setBounds(95, 60, 150, 22);
		mainPanel.add(pwdFld);
		
		//��ť��������JFrame���ϱ�
		JPanel btnPanel = new JPanel();
		this.add(btnPanel, BorderLayout.SOUTH);
		btnPanel.setLayout(new BorderLayout());
		btnPanel.setBorder(new EmptyBorder(2, 8, 4, 8)); 

		JButton registeBtn = new JButton("ע��");
		btnPanel.add(registeBtn, BorderLayout.WEST);
		JButton submitBtn = new JButton("��¼");
		btnPanel.add(submitBtn, BorderLayout.EAST);
		
		//�رմ���
		this.addWindowListener(new WindowAdapter(){
			public void windowClosing(WindowEvent e) {
				SocketUtil.close(DataBuffer.clientSeocket);
			}
		});
		
		//ע��
		registeBtn.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				Global.registerFrame = new RegisterFrame();  //��ע�ᴰ��
			}
		});
		
		//"��¼"
		submitBtn.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				try {
					login();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
	}

	/** ��¼ 
	 * @throws IOException */
	@SuppressWarnings("unchecked")
	private void login() throws IOException {
		if (idTxt.getText().length() == 0 
				|| pwdFld.getPassword().length == 0) {
			JOptionPane.showMessageDialog(LoginFrame.this, 
					"�˺ź������Ǳ����",
					"ERROR",JOptionPane.ERROR_MESSAGE);
			idTxt.requestFocusInWindow();
			return;
		}
		
		if(!idTxt.getText().matches("^\\d*$")){
			JOptionPane.showMessageDialog(LoginFrame.this, 
					"�˺ű���������",
					"ERROR",JOptionPane.ERROR_MESSAGE);
			idTxt.requestFocusInWindow();
			return;
		}
		
		int id = Integer.parseInt(idTxt.getText());
		String password = String.valueOf(pwdFld.getPassword());
		TcpProto.LoginReq.Builder requestBuilder = TcpProto.LoginReq.newBuilder();

		requestBuilder.setId(id);
		requestBuilder.setPassword(password);

		TcpProto.LoginReq request = requestBuilder.build();

		byte[] buf = Codec.encode(request);
			
		DataBuffer.oos.write(buf);
		
		DataBuffer.currentUser = new Person(id, password);
		DataBuffer.oos.flush();

	}
}