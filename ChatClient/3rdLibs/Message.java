/**
 * ClassName: TextMessage.java
 * Author: qiujy
 * CreateTime: 2009-4-23
 * EMAIL: qjyong@gmail.com
 * Copyright 2009 ++YONG All rights reserved.
 */
package common.model.entity;

import java.io.Serializable;
import java.util.Date;


/** 消息 */
public class Message implements Serializable {
	private static final long serialVersionUID = 1820192075144114657L;
	/** 消息接收者 */
	private Person toUser;
	/** 消息发送者 */
	private Person fromUser;
	/** 消息内容 */
	private String message;
	/** 发送时间 */
	private Date sendTime;
	
	
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
	public String getMessage() {
		return message;
	}
	public void setMessage(String message) {
		this.message = message;
	}
	
	public Date getSendTime() {
		return sendTime;
	}
	public void setSendTime(Date sendTime) {
		this.sendTime = sendTime;
	}
}
