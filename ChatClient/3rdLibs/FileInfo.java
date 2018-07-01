/**
 * ClassName: File.java
 * Author: qiujy
 * CreateTime: 2009-4-23
 * EMAIL: qjyong@gmail.com
 * Copyright 2009 ++YONG All rights reserved.
 */
package common.model.entity;

import java.io.Serializable;
import java.util.Date;


/** Ҫ���͵��ļ��� */
public class FileInfo implements Serializable {
	private static final long serialVersionUID = -5394575332459969403L;
	/** ��Ϣ������ */
	private Person toUser;
	/** ��Ϣ������ */
	private Person fromUser;
	/** Դ�ļ��� */
	private String srcName;
	/** ����ʱ�� */
	private Date sendTime;
	/** Ŀ���IP */
	private String destIp;  
	/** Ŀ��ض˿� */
	private int destPort;
	/** Ŀ���ļ��� */
	private String destName;
	public Person getToUser() {
		return toUser;
	}
	public void setToUser(Person toUser) {
		this.toUser = toUser;
	}
	public Person getFromUser() {
		return fromUser;
	}
	public void setFromUser(Person fromUser) {
		this.fromUser = fromUser;
	}
	public String getSrcName() {
		return srcName;
	}
	public void setSrcName(String srcName) {
		this.srcName = srcName;
	}
	public Date getSendTime() {
		return sendTime;
	}
	public void setSendTime(Date sendTime) {
		this.sendTime = sendTime;
	}
	public String getDestIp() {
		return destIp;
	}
	public void setDestIp(String destIp) {
		this.destIp = destIp;
	}
	public int getDestPort() {
		return destPort;
	}
	public void setDestPort(int destPort) {
		this.destPort = destPort;
	}
	public String getDestName() {
		return destName;
	}
	public void setDestName(String destName) {
		this.destName = destName;
	}
	
	
}