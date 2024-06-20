#pragma once

#include <QWidget>
#include <QBoxLayout>
struct HDivision {
	QWidget* Widget;
	QHBoxLayout* Layout;

	HDivision(QWidget* widget)
		:Widget(new QWidget(widget))
	{
		Layout = new QHBoxLayout();
		Widget->setLayout(Layout);
	}

};

