/**
 * ClassName: UserListModel.java
 * Author: qiujy
 * CreateTime: 2009-4-15
 * EMAIL: qjyong@gmail.com
 * Copyright 2009 ++YONG All rights reserved.
 */
package client.model.entity;

import java.util.List;

import javax.swing.AbstractListModel;



/** 在线用户JList的Model */
public class OnlineUserListModel extends AbstractListModel {
	private static final long serialVersionUID = -3903760573171074301L;
	private List<Person> onlineUsers;
	
	public OnlineUserListModel(List<Person> onlineUsers) {
		this.onlineUsers = onlineUsers;
	}
	
	public void addElement(Object object) {
		if (onlineUsers.contains(object)) {
			return;
		}
		int index = onlineUsers.size();
		onlineUsers.add((Person)object);
		fireIntervalAdded(this, index, index);
	}
	
	public boolean removeElement(Object object) {
		int index = onlineUsers.indexOf(object);
		if (index >= 0) {
			fireIntervalRemoved(this, index, index);
		}
		return onlineUsers.remove(object);
	}
	 
    public int getSize() { 
    	return onlineUsers.size(); 
    }
    
    public Object getElementAt(int i) {
    	return onlineUsers.get(i); 
    }
    
    public List<Person> getOnlineUsers() {
    	return onlineUsers;
    }
}