/**
 * ClassName: CharFrame.java
 * Author: qiujy
 * CreateTime: 2009-4-22
 * EMAIL: qjyong@gmail.com
 * Copyright 2009 ++YONG All rights reserved.
 */
package client.ui;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.text.*;
import java.util.Date;
import javax.swing.*;
import client.*;
import client.model.entity.DataBuffer;
import client.model.entity.Person;
import client.model.entity.MyCellRenderer;
import client.model.entity.OnlineUserListModel;
import client.util.*;
import common.model.entity.*;
import common.util.Codec;

/** ���촰�� */
public class ChatFrame extends JFrame{
	private static final long serialVersionUID = -2310785591507878535L;
	/**����Է�����ϢLabel*/
	private JLabel otherInfoLbl;
	/** ��ǰ�û���ϢLbl */
	private JLabel currentUserLbl;
	/**������Ϣ�б�����*/
	public static JTextArea msgListArea;
	/**Ҫ���͵���Ϣ����*/
	public static JTextArea sendArea; 
	/** �����û��б� */
	public static JList onlineList;
	/** �����û���ͳ��Lbl */
	public static JLabel onlineCountLbl;
	/** ׼�����͵��ļ� */
//	public static FileInfo sendFile;
	
	/** ˽�ĸ�ѡ�� */
	public JCheckBox rybqBtn;
	
	public ChatFrame(){
		this.init();
		this.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		this.setVisible(true);
	}
	
	@SuppressWarnings("unchecked")
	public void init(){
		this.setTitle("JQ ChatRoom");
		this.setSize(550, 500);
		this.setResizable(false);
		
		//����Ĭ�ϴ�������Ļ����
		int x = (int)Toolkit.getDefaultToolkit().getScreenSize().getWidth();
		int y = (int)Toolkit.getDefaultToolkit().getScreenSize().getHeight();
		this.setLocation((x - this.getWidth()) / 2, (y-this.getHeight())/ 2);
		
		//��������
		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BorderLayout());
		//�ұ��û����
		JPanel userPanel = new JPanel();
		userPanel.setLayout(new BorderLayout());
		
		// ����һ���ָ�����
		JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
				mainPanel, userPanel);
		splitPane.setDividerLocation(380);
		splitPane.setDividerSize(10);
		splitPane.setOneTouchExpandable(true);
		this.add(splitPane, BorderLayout.CENTER);
		
		//���ϱ���Ϣ��ʾ���
		JPanel infoPanel = new JPanel();
		infoPanel.setLayout(new BorderLayout());
		//������������Ϣ���
		JPanel sendPanel = new JPanel();
		sendPanel.setLayout(new BorderLayout());
		
		// ����һ���ָ�����
		JSplitPane splitPane2 = new JSplitPane(JSplitPane.VERTICAL_SPLIT,
				infoPanel, sendPanel);
		splitPane2.setDividerLocation(300);
		splitPane2.setDividerSize(1);
		mainPanel.add(splitPane2, BorderLayout.CENTER);
		
		otherInfoLbl = new JLabel("��ǰ״̬��Ⱥ����...");
		infoPanel.add(otherInfoLbl, BorderLayout.NORTH);
		
		msgListArea = new JTextArea();
		msgListArea.setLineWrap(true);
		infoPanel.add(new JScrollPane(msgListArea, 
						JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
						JScrollPane.HORIZONTAL_SCROLLBAR_NEVER));
		
		JPanel tempPanel = new JPanel();
		tempPanel.setLayout(new BorderLayout());
		sendPanel.add(tempPanel, BorderLayout.NORTH);
		
		// ���찴ť���
		JPanel btnPanel = new JPanel();
		btnPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
		tempPanel.add(btnPanel, BorderLayout.CENTER);
		
		//���尴ť
		JButton fontBtn = new JButton(new ImageIcon("images/font.png"));
		fontBtn.setMargin(new Insets(0,0,0,0));
		fontBtn.setToolTipText("��������͸�ʽ");
		btnPanel.add(fontBtn);
		
		//���鰴ť
		JButton faceBtn = new JButton(new ImageIcon("images/sendFace.png"));
		faceBtn.setMargin(new Insets(0,0,0,0));
		faceBtn.setToolTipText("ѡ�����");
		btnPanel.add(faceBtn);
		
		//�����ļ���ť
		JButton shakeBtn = new JButton(new ImageIcon("images/shake.png"));
		shakeBtn.setMargin(new Insets(0,0,0,0));
		shakeBtn.setToolTipText("��Է����ʹ�����");
		btnPanel.add(shakeBtn);
		
		//�����ļ���ť
		JButton sendFileBtn = new JButton(new ImageIcon("images/sendPic.png"));
		sendFileBtn.setMargin(new Insets(0,0,0,0));
		sendFileBtn.setToolTipText("��Է������ļ�");
		btnPanel.add(sendFileBtn);
		
		//˽�İ�ť
		rybqBtn = new JCheckBox("˽��");
		tempPanel.add(rybqBtn, BorderLayout.EAST);
		
		//Ҫ���͵���Ϣ������
		sendArea = new JTextArea();
		sendArea.setLineWrap(true);
		sendPanel.add(new JScrollPane(sendArea, 
						JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
						JScrollPane.HORIZONTAL_SCROLLBAR_NEVER));
		
		// ���찴ť���
		JPanel btn2Panel = new JPanel();
		btn2Panel.setLayout(new FlowLayout(FlowLayout.RIGHT));
		this.add(btn2Panel, BorderLayout.SOUTH);
		JButton closeBtn = new JButton("�ر�");
		closeBtn.setToolTipText("�˳���������");
		btn2Panel.add(closeBtn);
		JButton submitBtn = new JButton("����");
		submitBtn.setToolTipText("��Enter��������Ϣ");
		btn2Panel.add(submitBtn);
		sendPanel.add(btn2Panel, BorderLayout.SOUTH);
		
		//�����û��б����
		JPanel onlineListPane = new JPanel();
		onlineListPane.setLayout(new BorderLayout());
		onlineCountLbl = new JLabel("�����û��б�(1)");
		onlineListPane.add(onlineCountLbl, BorderLayout.NORTH);
		
		//��ǰ�û����
		JPanel currentUserPane = new JPanel();
		currentUserPane.setLayout(new BorderLayout());
		currentUserPane.add(new JLabel("��ǰ�û�"), BorderLayout.NORTH);
		
		// �ұ��û��б���һ���ָ�����
		JSplitPane splitPane3 = new JSplitPane(JSplitPane.VERTICAL_SPLIT,
				onlineListPane, currentUserPane);
		splitPane3.setDividerLocation(340);
		splitPane3.setDividerSize(1);
		userPanel.add(splitPane3, BorderLayout.CENTER);
		
		//�����û��б� 
		onlineList = new JList(DataBuffer.onlineUserListModel);
		onlineList.setCellRenderer(new MyCellRenderer());
		//����Ϊ��ѡģʽ
		onlineList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		onlineListPane.add(new JScrollPane(onlineList,
				JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
				JScrollPane.HORIZONTAL_SCROLLBAR_NEVER));
		
		//��ǰ�û���ϢLabel
		currentUserLbl = new JLabel();
		currentUserPane.add(currentUserLbl);
		
		///////////////////////ע���¼�������/////////////////////////
		//�رմ���
		this.addWindowListener(new WindowAdapter(){
			public void windowClosing(WindowEvent e) {
				try {
					logout();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		
		//�رհ�ť���¼�
		closeBtn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				try {
					logout();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		});
		
		//ѡ��ĳ���û�˽��
		rybqBtn.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				if(rybqBtn.isSelected()){
					Person selectedUser = (Person)onlineList.getSelectedValue();
					if(null == selectedUser){
						otherInfoLbl.setText("��ǰ״̬��˽��(ѡ�������û��б���ĳ���û�����˽��)...");
					}else if(DataBuffer.currentUser.getId() == selectedUser.getId()){
						otherInfoLbl.setText("��ǰ״̬������������?...ϵͳ������");
					}else{
						otherInfoLbl.setText("��ǰ״̬���� "+ selectedUser.getNickname()
								+"(" + selectedUser.getId() + ") ˽����...");
					}
				}else{
					otherInfoLbl.setText("��ǰ״̬��Ⱥ��...");
				}
			}
		});
		//ѡ��ĳ���û�
		onlineList.addMouseListener(new MouseAdapter() {
			public void mouseClicked(MouseEvent e) {
				Person selectedUser = (Person)onlineList.getSelectedValue();
				if(rybqBtn.isSelected()){
					if(DataBuffer.currentUser.getId() == selectedUser.getId()){
						otherInfoLbl.setText("��ǰ״̬������������?...ϵͳ������");
					}else{
						otherInfoLbl.setText("��ǰ״̬���� "+ selectedUser.getNickname()
								+"(" + selectedUser.getId() + ") ˽����...");
					}
				}
			}
		});
		
		//�����ı���Ϣ
		sendArea.addKeyListener(new KeyAdapter(){   
			public void keyPressed(KeyEvent e){   
				if(e.getKeyCode() == Event.ENTER){   
					try {
						sendTxtMsg();
					} catch (IOException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
				}
			}   
		});
		
		submitBtn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				try {
					sendTxtMsg();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		});
	
		//������
		shakeBtn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				try {
					sendShakeMsg();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		});
		
		
		//�����ļ�
		sendFileBtn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				sendFile();
			}
		});
		
		this.loadData();  //���س�ʼ����
	}
	
	/**  �������� */
	public void loadData(){
		//���ص�ǰ�û�����
		if(null != DataBuffer.currentUser){ 
			currentUserLbl.setIcon(
					new ImageIcon("images/" + DataBuffer.currentUser.getHead() + ".png"));
			currentUserLbl.setText(DataBuffer.currentUser.getNickname()
					+ "(" + DataBuffer.currentUser.getId() + ")");
		}
		//���������û��б�
		onlineCountLbl.setText("�����û��б�("+ DataBuffer.onlineUserListModel.getSize() +")");
		//����������������Ϣ���߳�
	}
	
	public String getGroupMsg(){
		DateFormat df = new SimpleDateFormat("HH:mm:ss");
		
		String content = sendArea.getText();
		StringBuffer sb = new StringBuffer();
		sb.append(" ").append(df.format(new Date())).append(" ")
		  .append(DataBuffer.currentUser.getNickname())
		  .append("(").append(DataBuffer.currentUser.getId()).append(") ").append("�Դ��˵")
		  .append("\n  ").append(content).append("\n");
		
		return sb.toString();
	}
	
	public String getPrivateMsg(Person selectedUser){
		DateFormat df = new SimpleDateFormat("HH:mm:ss");
		
		String content = sendArea.getText();
		StringBuffer sb = new StringBuffer();
		sb.append(" ").append(df.format(new Date())).append(" ")
		  .append(DataBuffer.currentUser.getNickname())
		  .append("��").append(selectedUser.getNickname())
		  .append("(").append(selectedUser.getId()).append(")")
		  .append("˵").append("\n  ").append(content).append("\n");
		
		return sb.toString();
	}
	
	public String getMsg(Person selectedUser){
		DateFormat df = new SimpleDateFormat("HH:mm:ss");
		
		StringBuffer sb = new StringBuffer();
		sb.append(" ").append(DataBuffer.currentUser.getNickname())
			.append("(").append(DataBuffer.currentUser.getId()).append(") ")
			.append(df.format(new Date()))
			.append("\n  ��").append(selectedUser.getNickname())
			.append("(").append(selectedUser.getId()).append(") ")
			.append("������һ�����ڶ���\n");
		
		return sb.toString();
	}
	
	public String sendShakeMessage(Person selectedUser) throws IOException{
		TcpProto.ShakeMsg.Builder requestBuilder = TcpProto.ShakeMsg.newBuilder();

		requestBuilder.setSrc(DataBuffer.currentUser.getId());
		requestBuilder.setDst(selectedUser.getId());
		
		String data = getMsg(selectedUser);
		requestBuilder.setData(data);
		
		TcpProto.ShakeMsg request = requestBuilder.build();
		byte[] buf = Codec.encode(request);

		DataBuffer.oos.write(buf);
		DataBuffer.oos.flush();
		
		return data;
	}
	
	
	public String sendPrivateMsg(Person selectedUser) throws IOException{
		TcpProto.TextMsgPrivate.Builder requestBuilder = TcpProto.TextMsgPrivate.newBuilder();

		requestBuilder.setSrc(DataBuffer.currentUser.getId());
		requestBuilder.setDst(selectedUser.getId());
		
		String data = getPrivateMsg(selectedUser);
		requestBuilder.setData(data);
		
		TcpProto.TextMsgPrivate request = requestBuilder.build();
		byte[] buf = Codec.encode(request);

		DataBuffer.oos.write(buf);
		DataBuffer.oos.flush();
		
		return data;
	}
	
	public String sendGroupMsg() throws IOException{
		TcpProto.TextMsgGroup.Builder requestBuilder = TcpProto.TextMsgGroup.newBuilder();
		
		requestBuilder.setSrc(DataBuffer.currentUser.getId());
		String data = getGroupMsg();
		requestBuilder.setData(data);
		
		TcpProto.TextMsgGroup request = requestBuilder.build();
		byte[] buf = Codec.encode(request);

		DataBuffer.oos.write(buf);
		DataBuffer.oos.flush();
		
		return data;

	}
	
	/** �����ı���Ϣ  */
	public void sendTxtMsg() throws IOException{
		String content = sendArea.getText();
		String data;
		if ("".equals(content)) { //������
			JOptionPane.showMessageDialog(ChatFrame.this, "���ܷ��Ϳ���Ϣ!",
					"���ܷ���", JOptionPane.ERROR_MESSAGE);
		} else { //����
			Person selectedUser = (Person)onlineList.getSelectedValue();
			if(null != selectedUser && 
					DataBuffer.currentUser.getId() == selectedUser.getId()){
				JOptionPane.showMessageDialog(ChatFrame.this, "���ܸ��Լ�������Ϣ!",
						"���ܷ���", JOptionPane.ERROR_MESSAGE);
				return;
			}
			
			if(rybqBtn.isSelected()){  //˽��
				if(null == selectedUser){
					JOptionPane.showMessageDialog(ChatFrame.this, "û��ѡ��˽�Ķ���!",
							"���ܷ���", JOptionPane.ERROR_MESSAGE);
					return;
				}else{
					data = sendPrivateMsg(selectedUser);
				}
			}else{ //Ⱥ��
				data = sendGroupMsg();
			}
		
			//JTextArea�а���Enter��ʱ��������ݲ��ص�����
			InputMap inputMap = sendArea.getInputMap();
			ActionMap actionMap = sendArea.getActionMap();
			Object transferTextActionKey = "TRANSFER_TEXT";
			inputMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER,0),transferTextActionKey);
			actionMap.put(transferTextActionKey,new AbstractAction() {
				private static final long serialVersionUID = 7041841945830590229L;
				public void actionPerformed(ActionEvent e) {
					sendArea.setText("");
					sendArea.requestFocus();
				}
			});
			sendArea.setText("");
			ClientUtil.appendTxt2MsgListArea(data);
		}
	}
	
	/** ������ */
	public void sendShakeMsg() throws IOException{
		Person selectedUser = (Person)onlineList.getSelectedValue();
		String data;
		if(null != selectedUser){
			if(DataBuffer.currentUser.getId() == selectedUser.getId()){
				JOptionPane.showMessageDialog(ChatFrame.this, "���ܸ��Լ�������!",
					"���ܷ���", JOptionPane.ERROR_MESSAGE);
			}else{
				data = sendShakeMessage(selectedUser);
				ClientUtil.appendTxt2MsgListArea(data);
				new JFrameShaker(ChatFrame.this).startShake();
			}
		}else{
			JOptionPane.showMessageDialog(ChatFrame.this, "����Ⱥ������!",
					"���ܷ���", JOptionPane.ERROR_MESSAGE);
		}
	}
	
	/** �����ļ� */
	private void sendFile() {
		Person selectedUser = (Person)onlineList.getSelectedValue();
		if(null != selectedUser){
			if(DataBuffer.currentUser.getId() == selectedUser.getId()){
				JOptionPane.showMessageDialog(ChatFrame.this, "���ܸ��Լ������ļ�!",
					"���ܷ���", JOptionPane.ERROR_MESSAGE);
			}else{
				JFileChooser jfc = new JFileChooser();
				if (jfc.showOpenDialog(ChatFrame.this) == JFileChooser.APPROVE_OPTION) {
//					File file = jfc.getSelectedFile();
//					sendFile = new FileInfo();
//					sendFile.setFromUser(DataBuffer.currentUser);
//					sendFile.setToUser(selectedUser);
//					try {
//						sendFile.setSrcName(file.getCanonicalPath());
//					} catch (IOException e1) {
//						e1.printStackTrace();
//					}
//					sendFile.setSendTime(new Date());
//					
//					Request request = new Request();
//					request.setAction("toSendFile");
//					request.setAttribute("file", sendFile);
//					try {
//						ClientUtil.sendTextRequest2(request);
//					} catch (IOException e) {
//						e.printStackTrace();
//					}
//					
//					ClientUtil.appendTxt2MsgListArea("���ļ���Ϣ���� "
//								+ selectedUser.getNickname() + "("
//								+ selectedUser.getId() + ") �����ļ� ["
//								+ file.getName() + "]���ȴ��Է�����...\n");
				}
			}
		}else{
			JOptionPane.showMessageDialog(ChatFrame.this, "���ܸ����������û������ļ�!",
					"���ܷ���", JOptionPane.ERROR_MESSAGE);
		}
	}

	/** �رտͻ��� 
	 * @throws IOException */
	private void logout() throws IOException {
		int select = JOptionPane.showConfirmDialog(ChatFrame.this,
				"ȷ���˳���\n\n�˳������ж��������������!", "�˳�������",
				JOptionPane.YES_NO_OPTION);
		if (select == JOptionPane.YES_OPTION) {
			TcpProto.LogoutReq.Builder requestBuilder = TcpProto.LogoutReq.newBuilder();

			requestBuilder.setId(DataBuffer.currentUser.getId());
			TcpProto.LogoutReq request = requestBuilder.build();

			byte[] buf = Codec.encode(request);
			
			DataBuffer.oos.write(buf);
			DataBuffer.oos.flush();
		}else{
			this.setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
		}
	}
}