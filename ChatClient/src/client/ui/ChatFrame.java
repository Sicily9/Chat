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

/** 聊天窗体 */
public class ChatFrame extends JFrame{
	private static final long serialVersionUID = -2310785591507878535L;
	/**聊天对方的信息Label*/
	private JLabel otherInfoLbl;
	/** 当前用户信息Lbl */
	private JLabel currentUserLbl;
	/**聊天信息列表区域*/
	public static JTextArea msgListArea;
	/**要发送的信息区域*/
	public static JTextArea sendArea; 
	/** 在线用户列表 */
	public static JList onlineList;
	/** 在线用户数统计Lbl */
	public static JLabel onlineCountLbl;
	/** 准备发送的文件 */
//	public static FileInfo sendFile;
	
	/** 私聊复选框 */
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
		
		//设置默认窗体在屏幕中央
		int x = (int)Toolkit.getDefaultToolkit().getScreenSize().getWidth();
		int y = (int)Toolkit.getDefaultToolkit().getScreenSize().getHeight();
		this.setLocation((x - this.getWidth()) / 2, (y-this.getHeight())/ 2);
		
		//左边主面板
		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BorderLayout());
		//右边用户面板
		JPanel userPanel = new JPanel();
		userPanel.setLayout(new BorderLayout());
		
		// 创建一个分隔窗格
		JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
				mainPanel, userPanel);
		splitPane.setDividerLocation(380);
		splitPane.setDividerSize(10);
		splitPane.setOneTouchExpandable(true);
		this.add(splitPane, BorderLayout.CENTER);
		
		//左上边信息显示面板
		JPanel infoPanel = new JPanel();
		infoPanel.setLayout(new BorderLayout());
		//右下连发送消息面板
		JPanel sendPanel = new JPanel();
		sendPanel.setLayout(new BorderLayout());
		
		// 创建一个分隔窗格
		JSplitPane splitPane2 = new JSplitPane(JSplitPane.VERTICAL_SPLIT,
				infoPanel, sendPanel);
		splitPane2.setDividerLocation(300);
		splitPane2.setDividerSize(1);
		mainPanel.add(splitPane2, BorderLayout.CENTER);
		
		otherInfoLbl = new JLabel("当前状态：群聊中...");
		infoPanel.add(otherInfoLbl, BorderLayout.NORTH);
		
		msgListArea = new JTextArea();
		msgListArea.setLineWrap(true);
		infoPanel.add(new JScrollPane(msgListArea, 
						JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
						JScrollPane.HORIZONTAL_SCROLLBAR_NEVER));
		
		JPanel tempPanel = new JPanel();
		tempPanel.setLayout(new BorderLayout());
		sendPanel.add(tempPanel, BorderLayout.NORTH);
		
		// 聊天按钮面板
		JPanel btnPanel = new JPanel();
		btnPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
		tempPanel.add(btnPanel, BorderLayout.CENTER);
		
		//字体按钮
		JButton fontBtn = new JButton(new ImageIcon("images/font.png"));
		fontBtn.setMargin(new Insets(0,0,0,0));
		fontBtn.setToolTipText("设置字体和格式");
		btnPanel.add(fontBtn);
		
		//表情按钮
		JButton faceBtn = new JButton(new ImageIcon("images/sendFace.png"));
		faceBtn.setMargin(new Insets(0,0,0,0));
		faceBtn.setToolTipText("选择表情");
		btnPanel.add(faceBtn);
		
		//发送文件按钮
		JButton shakeBtn = new JButton(new ImageIcon("images/shake.png"));
		shakeBtn.setMargin(new Insets(0,0,0,0));
		shakeBtn.setToolTipText("向对方发送窗口振动");
		btnPanel.add(shakeBtn);
		
		//发送文件按钮
		JButton sendFileBtn = new JButton(new ImageIcon("images/sendPic.png"));
		sendFileBtn.setMargin(new Insets(0,0,0,0));
		sendFileBtn.setToolTipText("向对方发送文件");
		btnPanel.add(sendFileBtn);
		
		//私聊按钮
		rybqBtn = new JCheckBox("私聊");
		tempPanel.add(rybqBtn, BorderLayout.EAST);
		
		//要发送的信息的区域
		sendArea = new JTextArea();
		sendArea.setLineWrap(true);
		sendPanel.add(new JScrollPane(sendArea, 
						JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
						JScrollPane.HORIZONTAL_SCROLLBAR_NEVER));
		
		// 聊天按钮面板
		JPanel btn2Panel = new JPanel();
		btn2Panel.setLayout(new FlowLayout(FlowLayout.RIGHT));
		this.add(btn2Panel, BorderLayout.SOUTH);
		JButton closeBtn = new JButton("关闭");
		closeBtn.setToolTipText("退出整个程序");
		btn2Panel.add(closeBtn);
		JButton submitBtn = new JButton("发送");
		submitBtn.setToolTipText("按Enter键发送消息");
		btn2Panel.add(submitBtn);
		sendPanel.add(btn2Panel, BorderLayout.SOUTH);
		
		//在线用户列表面板
		JPanel onlineListPane = new JPanel();
		onlineListPane.setLayout(new BorderLayout());
		onlineCountLbl = new JLabel("在线用户列表(1)");
		onlineListPane.add(onlineCountLbl, BorderLayout.NORTH);
		
		//当前用户面板
		JPanel currentUserPane = new JPanel();
		currentUserPane.setLayout(new BorderLayout());
		currentUserPane.add(new JLabel("当前用户"), BorderLayout.NORTH);
		
		// 右边用户列表创建一个分隔窗格
		JSplitPane splitPane3 = new JSplitPane(JSplitPane.VERTICAL_SPLIT,
				onlineListPane, currentUserPane);
		splitPane3.setDividerLocation(340);
		splitPane3.setDividerSize(1);
		userPanel.add(splitPane3, BorderLayout.CENTER);
		
		//在线用户列表 
		onlineList = new JList(DataBuffer.onlineUserListModel);
		onlineList.setCellRenderer(new MyCellRenderer());
		//设置为单选模式
		onlineList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		onlineListPane.add(new JScrollPane(onlineList,
				JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
				JScrollPane.HORIZONTAL_SCROLLBAR_NEVER));
		
		//当前用户信息Label
		currentUserLbl = new JLabel();
		currentUserPane.add(currentUserLbl);
		
		///////////////////////注册事件监听器/////////////////////////
		//关闭窗口
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
		
		//关闭按钮的事件
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
		
		//选择某个用户私聊
		rybqBtn.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				if(rybqBtn.isSelected()){
					Person selectedUser = (Person)onlineList.getSelectedValue();
					if(null == selectedUser){
						otherInfoLbl.setText("当前状态：私聊(选中在线用户列表中某个用户进行私聊)...");
					}else if(DataBuffer.currentUser.getId() == selectedUser.getId()){
						otherInfoLbl.setText("当前状态：想自言自语?...系统不允许");
					}else{
						otherInfoLbl.setText("当前状态：与 "+ selectedUser.getNickname()
								+"(" + selectedUser.getId() + ") 私聊中...");
					}
				}else{
					otherInfoLbl.setText("当前状态：群聊...");
				}
			}
		});
		//选择某个用户
		onlineList.addMouseListener(new MouseAdapter() {
			public void mouseClicked(MouseEvent e) {
				Person selectedUser = (Person)onlineList.getSelectedValue();
				if(rybqBtn.isSelected()){
					if(DataBuffer.currentUser.getId() == selectedUser.getId()){
						otherInfoLbl.setText("当前状态：想自言自语?...系统不允许");
					}else{
						otherInfoLbl.setText("当前状态：与 "+ selectedUser.getNickname()
								+"(" + selectedUser.getId() + ") 私聊中...");
					}
				}
			}
		});
		
		//发送文本消息
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
	
		//发送振动
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
		
		
		//发送文件
		sendFileBtn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				sendFile();
			}
		});
		
		this.loadData();  //加载初始数据
	}
	
	/**  加载数据 */
	public void loadData(){
		//加载当前用户数据
		if(null != DataBuffer.currentUser){ 
			currentUserLbl.setIcon(
					new ImageIcon("images/" + DataBuffer.currentUser.getHead() + ".png"));
			currentUserLbl.setText(DataBuffer.currentUser.getNickname()
					+ "(" + DataBuffer.currentUser.getId() + ")");
		}
		//设置在线用户列表
		onlineCountLbl.setText("在线用户列表("+ DataBuffer.onlineUserListModel.getSize() +")");
		//启动监听服务器消息的线程
	}
	
	public String getGroupMsg(){
		DateFormat df = new SimpleDateFormat("HH:mm:ss");
		
		String content = sendArea.getText();
		StringBuffer sb = new StringBuffer();
		sb.append(" ").append(df.format(new Date())).append(" ")
		  .append(DataBuffer.currentUser.getNickname())
		  .append("(").append(DataBuffer.currentUser.getId()).append(") ").append("对大家说")
		  .append("\n  ").append(content).append("\n");
		
		return sb.toString();
	}
	
	public String getPrivateMsg(Person selectedUser){
		DateFormat df = new SimpleDateFormat("HH:mm:ss");
		
		String content = sendArea.getText();
		StringBuffer sb = new StringBuffer();
		sb.append(" ").append(df.format(new Date())).append(" ")
		  .append(DataBuffer.currentUser.getNickname())
		  .append("对").append(selectedUser.getNickname())
		  .append("(").append(selectedUser.getId()).append(")")
		  .append("说").append("\n  ").append(content).append("\n");
		
		return sb.toString();
	}
	
	public String getMsg(Person selectedUser){
		DateFormat df = new SimpleDateFormat("HH:mm:ss");
		
		StringBuffer sb = new StringBuffer();
		sb.append(" ").append(DataBuffer.currentUser.getNickname())
			.append("(").append(DataBuffer.currentUser.getId()).append(") ")
			.append(df.format(new Date()))
			.append("\n  给").append(selectedUser.getNickname())
			.append("(").append(selectedUser.getId()).append(") ")
			.append("发送了一个窗口抖动\n");
		
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
	
	/** 发送文本消息  */
	public void sendTxtMsg() throws IOException{
		String content = sendArea.getText();
		String data;
		if ("".equals(content)) { //无内容
			JOptionPane.showMessageDialog(ChatFrame.this, "不能发送空消息!",
					"不能发送", JOptionPane.ERROR_MESSAGE);
		} else { //发送
			Person selectedUser = (Person)onlineList.getSelectedValue();
			if(null != selectedUser && 
					DataBuffer.currentUser.getId() == selectedUser.getId()){
				JOptionPane.showMessageDialog(ChatFrame.this, "不能给自己发送消息!",
						"不能发送", JOptionPane.ERROR_MESSAGE);
				return;
			}
			
			if(rybqBtn.isSelected()){  //私聊
				if(null == selectedUser){
					JOptionPane.showMessageDialog(ChatFrame.this, "没有选择私聊对象!",
							"不能发送", JOptionPane.ERROR_MESSAGE);
					return;
				}else{
					data = sendPrivateMsg(selectedUser);
				}
			}else{ //群聊
				data = sendGroupMsg();
			}
		
			//JTextArea中按“Enter”时，清空内容并回到首行
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
	
	/** 发送振动 */
	public void sendShakeMsg() throws IOException{
		Person selectedUser = (Person)onlineList.getSelectedValue();
		String data;
		if(null != selectedUser){
			if(DataBuffer.currentUser.getId() == selectedUser.getId()){
				JOptionPane.showMessageDialog(ChatFrame.this, "不能给自己发送振动!",
					"不能发送", JOptionPane.ERROR_MESSAGE);
			}else{
				data = sendShakeMessage(selectedUser);
				ClientUtil.appendTxt2MsgListArea(data);
				new JFrameShaker(ChatFrame.this).startShake();
			}
		}else{
			JOptionPane.showMessageDialog(ChatFrame.this, "不能群发送振动!",
					"不能发送", JOptionPane.ERROR_MESSAGE);
		}
	}
	
	/** 发送文件 */
	private void sendFile() {
		Person selectedUser = (Person)onlineList.getSelectedValue();
		if(null != selectedUser){
			if(DataBuffer.currentUser.getId() == selectedUser.getId()){
				JOptionPane.showMessageDialog(ChatFrame.this, "不能给自己发送文件!",
					"不能发送", JOptionPane.ERROR_MESSAGE);
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
//					ClientUtil.appendTxt2MsgListArea("【文件消息】向 "
//								+ selectedUser.getNickname() + "("
//								+ selectedUser.getId() + ") 发送文件 ["
//								+ file.getName() + "]，等待对方接收...\n");
				}
			}
		}else{
			JOptionPane.showMessageDialog(ChatFrame.this, "不能给所有在线用户发送文件!",
					"不能发送", JOptionPane.ERROR_MESSAGE);
		}
	}

	/** 关闭客户端 
	 * @throws IOException */
	private void logout() throws IOException {
		int select = JOptionPane.showConfirmDialog(ChatFrame.this,
				"确定退出吗？\n\n退出程序将中断与服务器的连接!", "退出聊天室",
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