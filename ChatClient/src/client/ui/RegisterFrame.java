/**
 * ClassName: RegisterFrame.java
 * Author: qiujy
 * CreateTime: 2009-4-22
 * EMAIL: qjyong@gmail.com
 * Copyright 2009 ++YONG All rights reserved.
 */
package client.ui;

import java.awt.event.*;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import javax.swing.*;

import client.model.entity.DataBuffer;
import client.model.entity.Person;
import client.util.ClientUtil;
import common.model.entity.*;
import common.util.Convert;
import common.util.Codec;

/* ע�ᴰ�� */
public class RegisterFrame extends JFrame {
	private static final long serialVersionUID = -768631070458723803L;
	private JPasswordField pwdFld;
	private JPasswordField pwd2Fld;
	private JTextField nickname;
	private JComboBox head;
	private JRadioButton sex0;
	private JRadioButton sex1;
	private JButton ok;
	private JButton reset;
	private JButton cancel;
	
	public RegisterFrame(){
		this.init();
		setVisible(true);
	}
	
	public void init(){
		this.setTitle("Register");//���ñ���
		setBounds((DataBuffer.screenSize.width - 387)/2, 
				(DataBuffer.screenSize.height - 267)/2, 
				387, 267);
		getContentPane().setLayout(null);
		setResizable(false);

		JLabel label = new JLabel("�ǳ�:"); //label��ʾ
		label.setBounds(24, 36, 59, 17);
		getContentPane().add(label);
		
		nickname = new JTextField(); //�ǳ�
		nickname.setBounds(90, 34, 110, 22);
		getContentPane().add(nickname);
		
		JLabel label5 = new JLabel("����:*");
		label5.setBounds(24, 72, 50, 17);
		getContentPane().add(label5);
		
		JLabel label3 = new JLabel("ȷ������:*");
		label3.setBounds(24, 107, 65, 17);
		getContentPane().add(label3);
		
		pwdFld = new JPasswordField();//�����
		pwdFld.setBounds(90, 70, 110, 22);
		getContentPane().add(pwdFld);

		pwd2Fld = new JPasswordField();
		pwd2Fld.setBounds(90, 105, 110, 22);
		getContentPane().add(pwd2Fld);

		JLabel label4 = new JLabel("�Ա�:");
		label4.setBounds(230, 36, 31, 17);
		getContentPane().add(label4);
		
		sex1 = new JRadioButton("��",true);
		sex1.setBounds (268, 31,44, 25);
		getContentPane().add(sex1);
		sex0 = new JRadioButton("Ů");
		sex0.setBounds(310, 31, 44, 25);
		getContentPane().add(sex0);
		ButtonGroup buttonGroup = new ButtonGroup();//��ѡ��ť��
		buttonGroup.add(sex0);
		buttonGroup.add(sex1);
		
		JLabel label6 = new JLabel("ͷ��:");
		label6.setBounds(230, 72, 31, 17);
		getContentPane().add(label6);
		
		head = new JComboBox();//�����б�ͼ��
		head.setBounds(278, 70, 65, 45);
		head.setMaximumRowCount(5);
		for (int i = 0; i < 11; i++) {
			head.addItem(new ImageIcon("images/" + i + ".png"));
			//ͨ��ѭ�����ͼƬ��ע��ͼƬ����Ҫȡ��1,2,3,4,5,��	
		}
		head.setSelectedIndex(0);
		getContentPane().add(head);
		
		//��ť
		ok = new JButton("ȷ��");
		ok.setBounds(27, 176, 60, 28);
		getContentPane().add(ok);

		reset = new JButton("����");
		reset.setBounds(123, 176, 60, 28);
		getContentPane().add(reset);

		cancel = new JButton("ȡ��");
		cancel.setBounds(268, 176, 60, 28);
		getContentPane().add(cancel);
		
		//////////////////////ע���¼�������////////////////////////
		//ȡ����ť�����¼�����
		cancel.addActionListener(new ActionListener() {
			public void actionPerformed(final ActionEvent event) {
				RegisterFrame.this.dispose();
			}
		});
		//�رմ���
		this.addWindowListener(new WindowAdapter(){
			public void windowClosing(WindowEvent e) {
				RegisterFrame.this.dispose();
			}
		});
		
		// ���ð�ť�����¼�����
		reset.addActionListener(new ActionListener() {
			public void actionPerformed(final ActionEvent e) {
				nickname.setText("");
				pwdFld.setText("");
				pwd2Fld.setText("");
				nickname.requestFocusInWindow();//�û�����ý���
			}
		});
		
		//ȷ�ϰ�ť�����¼�����
		ok.addActionListener(new ActionListener() {
			public void actionPerformed(final ActionEvent e) {
				if (pwdFld.getPassword().length==0 || pwd2Fld.getPassword().length==0) {
					JOptionPane.showMessageDialog(RegisterFrame.this, "�� �� * �� Ϊ��������!");
				//�ж��û����������Ƿ�Ϊ��
				} else if (!new String(pwdFld.getPassword()).equals(new String(pwd2Fld.getPassword()))) {
					JOptionPane.showMessageDialog(RegisterFrame.this, "�����������벻һ��!");
					pwdFld.setText("");
					pwd2Fld.setText("");
					pwdFld.requestFocusInWindow();
				//�ж����������Ƿ�һ��
				} else {
					Person user = new Person(new String(pwdFld.getPassword()), 
							nickname.getText(), 
							sex0.isSelected() ? "woman" : "man", 
							head.getSelectedIndex());
					try {
						RegisterFrame.this.registe(user);
					} catch (IOException e1) {
						e1.printStackTrace();
					} catch (ClassNotFoundException ex) {
						ex.printStackTrace();
					}
				}
			}
		});
	}
	//ע�᷽��
	private void registe(Person user) throws IOException, ClassNotFoundException{
		TcpProto.RegistReq.Builder requestBuilder = TcpProto.RegistReq.newBuilder();

		requestBuilder.setNickname(user.getNickname());
		requestBuilder.setPassword(user.getPassword());
		requestBuilder.setSex(user.getSex());
		requestBuilder.setHead(user.getHead());
		
		TcpProto.RegistReq request = requestBuilder.build();
		//��ȡ��Ӧ
		byte[] buf = Codec.encode(request);

		DataBuffer.oos.write(buf);
		DataBuffer.oos.flush();

	}
}