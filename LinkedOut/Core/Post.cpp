#include "Post.h"
#include "CustomUI/Division.h"
#include "CustomUI/ClickableLabel.h"
#include "CustomUI/Division.h"
#include "CustomUI/Separator.h"
#include "CustomUI/TitledButton.h"	
#include "CustomUI/PopupWindow.h"


#include <QWidget>
#include <QLayout>
#include <QLabel>


#include <QFrame>
#include <functional>
namespace LinkedOut {

	static QString ShortenText(const QString& text, int maxWidth, const QFont& font, const QString& seeMoreText = " ...see more") {
		QFontMetrics metrics(font);
		int seeMoreWidth = metrics.horizontalAdvance(seeMoreText);

		if (metrics.horizontalAdvance(text) <= maxWidth) {
			return text;
		}

		QString shortenedText;
		for (int i = 0; i < text.length(); ++i) {
			shortenedText.append(text[i]);
			if (metrics.horizontalAdvance(shortenedText) + seeMoreWidth > maxWidth) {
				shortenedText.chop(1);
				break;
			}
		}

		return shortenedText + seeMoreText;
	}

	void Post::Serialize(QWidget* parent)
	{
		auto mainLayout = parent->layout();

		const uint32_t m_ButtonSize = 30;

		HDivision* m_HeaderDiv = new HDivision(parent);
		VDivision* m_UsernameDiv = new VDivision(parent);
		QLabel* m_UsernameLabel = new QLabel("Username", parent);
		QLabel* m_TimestampLabel = new QLabel("Timestamp", parent);
		Separator* m_TopSeparator = new Separator(parent);

		VDivision* m_ContentDiv = new VDivision(parent);
		ClickableLabel* m_ContentLabel = new ClickableLabel(parent);

		m_UsernameDiv->Layout->addWidget(m_UsernameLabel);
		m_UsernameDiv->Layout->addWidget(m_TimestampLabel);
		m_UsernameDiv->Layout->setContentsMargins(0, 0, 0, 0);

		m_HeaderDiv->Layout->addWidget(m_UsernameDiv->Widget);

		m_HeaderDiv->Layout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(m_HeaderDiv->Widget);
		m_TopSeparator->setStyleSheet("background-color:rgb(100, 165, 155);");
		mainLayout->addWidget(m_TopSeparator);


		QString postText = ShortenText(QString::fromStdString(GetContentText()), 200, parent->font());

		m_ContentLabel->setText(postText);

		if (postText == QString::fromStdString(GetContentText())) {
			//m_ContentLabel->setCursor(Qt::ArrowCursor);
		}
		else {
			QObject::connect(m_ContentLabel, &ClickableLabel::clicked, [=]() {
				if (m_ContentLabel->text() == postText)
					m_ContentLabel->setText(QString::fromStdString(GetContentText()));
				else
					m_ContentLabel->setText(postText);
				});
		}

		if (!GetContentPicture().empty()) {
			QPixmap image(GetContentPicture().c_str());
			if (!image.isNull()) {
				QLabel* imageLabel = new QLabel(m_ContentDiv->Widget);
				imageLabel->setPixmap(image.scaled(QSize(100,100),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
				m_ContentDiv->Layout->addWidget(imageLabel);
			}
		}

		m_ContentDiv->Layout->addWidget(m_ContentLabel);

		mainLayout->addWidget(m_ContentDiv->Widget);

		m_UsernameLabel->setText(QString::fromStdString(GetPosterName()));
		{
			std::string timestamp;
			const Time& time = GetTimeSent();

			timestamp += time.GetYear() + "/";
			timestamp += time.GetMonth() + "/";
			timestamp += time.GetDay() + " ";
			timestamp += time.GetHour() + ":";
			timestamp += time.GetMinute();
			m_TimestampLabel->setText(QString::fromStdString(timestamp));
		}

		//Styles
		m_UsernameLabel->setStyleSheet("color:black;font-weight:bold;");
		m_ContentLabel->setStyleSheet("color:black;font-weight:bold;");
		m_TimestampLabel->setStyleSheet("color:rgb(210,215,210);font-weight:bold;");

	}
}

