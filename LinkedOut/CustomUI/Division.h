#pragma once

#include <QWidget>
#include <QBoxLayout>
struct HDivision {
	QWidget* Widget;
	QHBoxLayout* Layout;

	HDivision() = default;

	HDivision(QWidget* widget)
		:Widget(new QWidget(widget))
	{
		Layout = new QHBoxLayout();
		Widget->setLayout(Layout);
	}


};

struct VDivision {
	QWidget* Widget;
	QVBoxLayout* Layout;

	VDivision() = default;

	VDivision(QWidget* widget)
		:Widget(new QWidget(widget))
	{
		Layout = new QVBoxLayout();
		Widget->setLayout(Layout);
	}
};

