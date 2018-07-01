/**
 * ClassName: ClientThread.java
 * Author: qiujy
 * CreateTime: 2009-4-22
 * EMAIL: qjyong@gmail.com
 * Copyright 2009 ++YONG All rights reserved.
 */
package client;

import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.LinkedList;

import javax.swing.*;

import client.model.entity.DataBuffer;
import client.model.entity.Global;
import client.model.entity.Person;
import client.ui.ChatFrame;
import client.ui.RegisterFrame;
import client.ui.LoginFrame;
import client.util.*;
import common.model.entity.TcpProto;

import common.util.*;

/** 客户端线程，不断监听服务器发送过来的信息 */
public class ClientThread extends Thread {
	
	public void run() {
		try {
			while (DataBuffer.clientSeocket.isConnected()) {
				int errno = 0;
				
				byte[] lenBytes = new byte[4];
				errno = DataBuffer.ois.read(lenBytes);
				byte[] nameLenBytes = new byte[4];
				errno = DataBuffer.ois.read(nameLenBytes);
				
				int len = Convert.bytes2Int(lenBytes);
				int nameLen = Convert.bytes2Int(nameLenBytes);
				byte[] name = new byte[nameLen];
				errno = DataBuffer.ois.read(name);
				
				if(errno == -1){
					System.out.println("fuck"+ DataBuffer.currentUser.getId());
					DataBuffer.clientSeocket.shutdownOutput();
					return;
				}
				
				int dataLength = len - 4 - nameLen;
				byte[] data = new byte[dataLength];
				DataBuffer.ois.read(data);
				
				String className = new String(name);
				if(className.equals("RegistRes")){
					
					TcpProto.RegistRes msg = TcpProto.RegistRes.parseFrom(data);
					JOptionPane.showMessageDialog(Global.registerFrame, 
							"恭喜您，您的Q号:"+ msg.getId() + ",请牢记!!!",
							"注册成功",JOptionPane.INFORMATION_MESSAGE);
					
				}else if(className.equals("LoginRes")){
					
					TcpProto.LoginRes msg = TcpProto.LoginRes.parseFrom(data);
					TcpProto.Person user = msg.getPerson(0);
					int size = msg.getPersonCount();
															
					Person newUser = new Person(msg.getPerson(0).getPassword(), msg.getPerson(0).getNickname(),
												msg.getPerson(0).getSex(), msg.getPerson(0).getHead());
					newUser.setId(msg.getPerson(0).getId());
					DataBuffer.onlineUserListModel.addElement(newUser);
					
					if(msg.getPerson(0).getId() == DataBuffer.currentUser.getId()){
						
						for(int i = 1; i< size; i++){
							Person OnlineUser = new Person(msg.getPerson(i).getPassword(), msg.getPerson(i).getNickname(),
									msg.getPerson(i).getSex(), msg.getPerson(i).getHead());
							OnlineUser.setId(msg.getPerson(i).getId());
							DataBuffer.onlineUserListModel.addElement(OnlineUser);
						}
						
						if(newUser!= null){ //登录成功
							DataBuffer.currentUser = newUser;
							
							Global.loginFrame.dispose();
							Global.chatFrame = new ChatFrame();  //打开聊天窗体
						}else{ 
							//登录失败
							String str = "";
							JOptionPane.showMessageDialog(Global.loginFrame, 
										str,
										"ERROR",JOptionPane.ERROR_MESSAGE);
						}
					} 
					ChatFrame.onlineCountLbl.setText(
							"在线用户列表("+ DataBuffer.onlineUsers.size() +")");
					ClientUtil.appendTxt2MsgListArea("【系统消息】用户"+newUser.getNickname() + "(" + newUser.getId() + ")" + "上线了！\n"); 
					
				}else if(className.equals("LogoutRes")){
					
					TcpProto.LogoutRes msg = TcpProto.LogoutRes.parseFrom(data);
					Person newUser = new Person(msg.getPerson().getPassword(), msg.getPerson().getNickname(), 
												msg.getPerson().getSex(), msg.getPerson().getHead());
					newUser.setId(msg.getPerson().getId());
					
					if(msg.getPerson().getId() != DataBuffer.currentUser.getId()){
						System.out.println(DataBuffer.currentUser.getId());
						DataBuffer.onlineUserListModel.removeElement(newUser);
						ChatFrame.onlineCountLbl.setText(
								"在线用户列表("+ DataBuffer.onlineUserListModel.getSize() +")");
						ClientUtil.appendTxt2MsgListArea("【系统消息】用户" + newUser.getNickname() + "(" + newUser.getId() + ")" + "下线了！\n");
					}
				}else if(className.equals("TextMsgPrivate")){
					
					TcpProto.TextMsgPrivate msg = TcpProto.TextMsgPrivate.parseFrom(data);
					ClientUtil.appendTxt2MsgListArea(msg.getData());
					
				}else if(className.equals("TextMsgGroup")){
					
					TcpProto.TextMsgGroup msg = TcpProto.TextMsgGroup.parseFrom(data);
					ClientUtil.appendTxt2MsgListArea(msg.getData());
					
				}else if(className.equals("ShakeMsg")){
					
					TcpProto.ShakeMsg msg = TcpProto.ShakeMsg.parseFrom(data);
					ClientUtil.appendTxt2MsgListArea(msg.getData());
					new JFrameShaker(Global.chatFrame).startShake();
					
				}else if(className.equals("TcpHeartBeatRes")){
					
					TcpProto.TcpHeartBeatRes msg = TcpProto.TcpHeartBeatRes.parseFrom(data);
					
				}
				
//				System.out.println("获取了响应内容：" + type);
//				if(type == ResponseType.CHAT){ //聊天
//					Message msg = (Message)response.getData("txtMsg");
//					ClientUtil.appendTxt2MsgListArea(msg.getMessage());
//				}else if(type == ResponseType.SHAKE){ //振动
//					Message msg = (Message)response.getData("ShakeMsg");
//					ClientUtil.appendTxt2MsgListArea(msg.getMessage());
//					new JFrameShaker(this.currentFrame).startShake();
//				}else if(type == ResponseType.TOSENDFILE){ //准备发送文件
//					toSendFile(response);
//				}else if(type == ResponseType.AGREERECEIVEFILE){ //对方同意接收文件
//					sendFile(response);
//				}else if(type == ResponseType.REFUSERECEIVEFILE){ //对方拒绝接收文件
//					ClientUtil.appendTxt2MsgListArea("【文件消息】对方拒绝接收，文件发送失败！\n");
//				}else if(type == ResponseType.RECEIVEFILE){ //开始接收文件
//					receiveFile(response);
//				}
			}
		} catch (IOException e) {
			e.printStackTrace();
			//System.out.println(e.getMessage());
		} 
	}

//	/** 发送文件 */
//	private void sendFile(Response response) {
//		final FileInfo sendFile = (FileInfo)response.getData("sendFile");
//		
//		BufferedInputStream bis = null;
//		BufferedOutputStream bos = null;
//		Socket socket = null;
//		try {
//			socket = new Socket(sendFile.getDestIp(),sendFile.getDestPort());//套接字连接
//			bis = new BufferedInputStream(new FileInputStream(sendFile.getSrcName()));//文件读入
//			bos = new BufferedOutputStream(socket.getOutputStream());//文件写出
//			
//			byte[] buffer = new byte[1024];
//			int n = -1;
//			while ((n = bis.read(buffer)) != -1){
//				bos.write(buffer, 0, n);
//			}
//			bos.flush();
//			synchronized (this) {
//				ClientUtil.appendTxt2MsgListArea("【文件消息】文件发送完毕!\n");
//			}
//		} catch (IOException e) {
//			e.printStackTrace();
//		}finally{
//			IOUtil.close(bis,bos);
//			SocketUtil.close(socket);
//		}
//	}
//	
//	/** 接收文件 */
//	private void receiveFile(Response response) {
//		final FileInfo sendFile = (FileInfo)response.getData("sendFile");
//		
//		BufferedInputStream bis = null;
//		BufferedOutputStream bos = null;
//		ServerSocket serverSocket = null;
//		Socket socket = null;
//		try {
//			serverSocket = new ServerSocket(sendFile.getDestPort());
//			socket = serverSocket.accept(); //接收
//			bis = new BufferedInputStream(socket.getInputStream());//缓冲读
//			bos = new BufferedOutputStream(new FileOutputStream(sendFile.getDestName()));//缓冲写出
//			
//			byte[] buffer = new byte[1024];
//			int n = -1;
//			while ((n = bis.read(buffer)) != -1){
//				bos.write(buffer, 0, n);
//			}
//			bos.flush();
//			synchronized (this) {
//				ClientUtil.appendTxt2MsgListArea("【文件消息】文件接收完毕!存放在["
//						+ sendFile.getDestName()+"]\n");
//			}
//			
//		} catch (IOException e) {
//			e.printStackTrace();
//		}finally{
//			IOUtil.close(bis,bos);
//			SocketUtil.close(socket);
//			SocketUtil.close(serverSocket);
//		}
//	}
//
//	/** 准备发送文件	 */
//	private void toSendFile(Response response) {
//		FileInfo sendFile = (FileInfo)response.getData("sendFile");
//		
//		String fromName = sendFile.getFromUser().getNickname() 
//				+ "(" + sendFile.getFromUser().getId() + ")";
//		String fileName = sendFile.getSrcName()
//			.substring(sendFile.getSrcName().lastIndexOf(File.separator)+1);
//		
//		int select = JOptionPane.showConfirmDialog(this.currentFrame, 
//				fromName + " 向您发送文件 [" + fileName+ "]!\n同意接收吗?",
//				"接收文件", JOptionPane.YES_NO_OPTION);
//		try {
//			Request request = new Request();
//			request.setAttribute("sendFile", sendFile);
//			
//			if (select == JOptionPane.YES_OPTION) {
//				JFileChooser jfc = new JFileChooser();
//				jfc.setSelectedFile(new File(fileName));
//				int result = jfc.showSaveDialog(this.currentFrame);
//				
//				if (result == JFileChooser.APPROVE_OPTION){
//					//设置目的地文件名
//					sendFile.setDestName(jfc.getSelectedFile().getCanonicalPath());
//					//设置目标地的IP和接收文件的端口
//					sendFile.setDestIp(DataBuffer.ip);
//					sendFile.setDestPort(DataBuffer.RECEIVE_FILE_PORT);
//					
//					request.setAction("agreeReceiveFile");
//					ClientUtil.appendTxt2MsgListArea("【文件消息】您已同意接收来自 "
//							+ fromName +" 的文件，正在接收文件 ...\n");
//				} else {
//					request.setAction("refuseReceiveFile");
//					ClientUtil.appendTxt2MsgListArea("【文件消息】您已拒绝接收来自 "
//							+ fromName +" 的文件!\n");
//				}	
//			} else {
//				request.setAction("refuseReceiveFile");
//				ClientUtil.appendTxt2MsgListArea("【文件消息】您已拒绝接收来自 "
//						+ fromName +" 的文件!\n");
//			}
//			
//			ClientUtil.sendTextRequest2(request);
//		} catch (IOException e) {
//			e.printStackTrace();
//		}
//	}
}
