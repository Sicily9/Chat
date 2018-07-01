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

/** �ͻ����̣߳����ϼ������������͹�������Ϣ */
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
							"��ϲ��������Q��:"+ msg.getId() + ",���μ�!!!",
							"ע��ɹ�",JOptionPane.INFORMATION_MESSAGE);
					
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
						
						if(newUser!= null){ //��¼�ɹ�
							DataBuffer.currentUser = newUser;
							
							Global.loginFrame.dispose();
							Global.chatFrame = new ChatFrame();  //�����촰��
						}else{ 
							//��¼ʧ��
							String str = "";
							JOptionPane.showMessageDialog(Global.loginFrame, 
										str,
										"ERROR",JOptionPane.ERROR_MESSAGE);
						}
					} 
					ChatFrame.onlineCountLbl.setText(
							"�����û��б�("+ DataBuffer.onlineUsers.size() +")");
					ClientUtil.appendTxt2MsgListArea("��ϵͳ��Ϣ���û�"+newUser.getNickname() + "(" + newUser.getId() + ")" + "�����ˣ�\n"); 
					
				}else if(className.equals("LogoutRes")){
					
					TcpProto.LogoutRes msg = TcpProto.LogoutRes.parseFrom(data);
					Person newUser = new Person(msg.getPerson().getPassword(), msg.getPerson().getNickname(), 
												msg.getPerson().getSex(), msg.getPerson().getHead());
					newUser.setId(msg.getPerson().getId());
					
					if(msg.getPerson().getId() != DataBuffer.currentUser.getId()){
						System.out.println(DataBuffer.currentUser.getId());
						DataBuffer.onlineUserListModel.removeElement(newUser);
						ChatFrame.onlineCountLbl.setText(
								"�����û��б�("+ DataBuffer.onlineUserListModel.getSize() +")");
						ClientUtil.appendTxt2MsgListArea("��ϵͳ��Ϣ���û�" + newUser.getNickname() + "(" + newUser.getId() + ")" + "�����ˣ�\n");
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
				
//				System.out.println("��ȡ����Ӧ���ݣ�" + type);
//				if(type == ResponseType.CHAT){ //����
//					Message msg = (Message)response.getData("txtMsg");
//					ClientUtil.appendTxt2MsgListArea(msg.getMessage());
//				}else if(type == ResponseType.SHAKE){ //��
//					Message msg = (Message)response.getData("ShakeMsg");
//					ClientUtil.appendTxt2MsgListArea(msg.getMessage());
//					new JFrameShaker(this.currentFrame).startShake();
//				}else if(type == ResponseType.TOSENDFILE){ //׼�������ļ�
//					toSendFile(response);
//				}else if(type == ResponseType.AGREERECEIVEFILE){ //�Է�ͬ������ļ�
//					sendFile(response);
//				}else if(type == ResponseType.REFUSERECEIVEFILE){ //�Է��ܾ������ļ�
//					ClientUtil.appendTxt2MsgListArea("���ļ���Ϣ���Է��ܾ����գ��ļ�����ʧ�ܣ�\n");
//				}else if(type == ResponseType.RECEIVEFILE){ //��ʼ�����ļ�
//					receiveFile(response);
//				}
			}
		} catch (IOException e) {
			e.printStackTrace();
			//System.out.println(e.getMessage());
		} 
	}

//	/** �����ļ� */
//	private void sendFile(Response response) {
//		final FileInfo sendFile = (FileInfo)response.getData("sendFile");
//		
//		BufferedInputStream bis = null;
//		BufferedOutputStream bos = null;
//		Socket socket = null;
//		try {
//			socket = new Socket(sendFile.getDestIp(),sendFile.getDestPort());//�׽�������
//			bis = new BufferedInputStream(new FileInputStream(sendFile.getSrcName()));//�ļ�����
//			bos = new BufferedOutputStream(socket.getOutputStream());//�ļ�д��
//			
//			byte[] buffer = new byte[1024];
//			int n = -1;
//			while ((n = bis.read(buffer)) != -1){
//				bos.write(buffer, 0, n);
//			}
//			bos.flush();
//			synchronized (this) {
//				ClientUtil.appendTxt2MsgListArea("���ļ���Ϣ���ļ��������!\n");
//			}
//		} catch (IOException e) {
//			e.printStackTrace();
//		}finally{
//			IOUtil.close(bis,bos);
//			SocketUtil.close(socket);
//		}
//	}
//	
//	/** �����ļ� */
//	private void receiveFile(Response response) {
//		final FileInfo sendFile = (FileInfo)response.getData("sendFile");
//		
//		BufferedInputStream bis = null;
//		BufferedOutputStream bos = null;
//		ServerSocket serverSocket = null;
//		Socket socket = null;
//		try {
//			serverSocket = new ServerSocket(sendFile.getDestPort());
//			socket = serverSocket.accept(); //����
//			bis = new BufferedInputStream(socket.getInputStream());//�����
//			bos = new BufferedOutputStream(new FileOutputStream(sendFile.getDestName()));//����д��
//			
//			byte[] buffer = new byte[1024];
//			int n = -1;
//			while ((n = bis.read(buffer)) != -1){
//				bos.write(buffer, 0, n);
//			}
//			bos.flush();
//			synchronized (this) {
//				ClientUtil.appendTxt2MsgListArea("���ļ���Ϣ���ļ��������!�����["
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
//	/** ׼�������ļ�	 */
//	private void toSendFile(Response response) {
//		FileInfo sendFile = (FileInfo)response.getData("sendFile");
//		
//		String fromName = sendFile.getFromUser().getNickname() 
//				+ "(" + sendFile.getFromUser().getId() + ")";
//		String fileName = sendFile.getSrcName()
//			.substring(sendFile.getSrcName().lastIndexOf(File.separator)+1);
//		
//		int select = JOptionPane.showConfirmDialog(this.currentFrame, 
//				fromName + " ���������ļ� [" + fileName+ "]!\nͬ�������?",
//				"�����ļ�", JOptionPane.YES_NO_OPTION);
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
//					//����Ŀ�ĵ��ļ���
//					sendFile.setDestName(jfc.getSelectedFile().getCanonicalPath());
//					//����Ŀ��ص�IP�ͽ����ļ��Ķ˿�
//					sendFile.setDestIp(DataBuffer.ip);
//					sendFile.setDestPort(DataBuffer.RECEIVE_FILE_PORT);
//					
//					request.setAction("agreeReceiveFile");
//					ClientUtil.appendTxt2MsgListArea("���ļ���Ϣ������ͬ��������� "
//							+ fromName +" ���ļ������ڽ����ļ� ...\n");
//				} else {
//					request.setAction("refuseReceiveFile");
//					ClientUtil.appendTxt2MsgListArea("���ļ���Ϣ�����Ѿܾ��������� "
//							+ fromName +" ���ļ�!\n");
//				}	
//			} else {
//				request.setAction("refuseReceiveFile");
//				ClientUtil.appendTxt2MsgListArea("���ļ���Ϣ�����Ѿܾ��������� "
//						+ fromName +" ���ļ�!\n");
//			}
//			
//			ClientUtil.sendTextRequest2(request);
//		} catch (IOException e) {
//			e.printStackTrace();
//		}
//	}
}
