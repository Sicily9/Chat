/**
 * ClassName: MyCellRenderer.java
 * Author: qiujy
 * CreateTime: 2009-4-15
 * EMAIL: qjyong@gmail.com
 * Copyright 2009 ++YONG All rights reserved.
 */
package client.model.entity;

import java.awt.Component;

import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.ListCellRenderer;





@SuppressWarnings("rawtypes")
public class MyCellRenderer extends JLabel implements ListCellRenderer {

	private static final long serialVersionUID = 3460394416991636990L;

	public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
    	Person user = (Person)value;
        String name = user.getNickname() + "(" + user.getId() + ")";
        setText(name);
        setIcon(user.getHeadIcon());
        if (isSelected) {
        	setBackground(list.getSelectionBackground());
        	setForeground(list.getSelectionForeground());
        } else {
	       setBackground(list.getBackground());
	       setForeground(list.getForeground());
        }
        setEnabled(list.isEnabled());
        setFont(list.getFont());
        setOpaque(true);
        return this;
    }
}