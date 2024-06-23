#include "NavigationMenu.h"
#include "Core/Application.h"
#include "Core/Window.h"
#include "MainLayer.h"
#include "CustomUI/SearchBar.h"
#include "CustomUI/TitledButton.h"
#include "CustomUI/Division.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QBoxLayout>
#include <QLineEdit>


namespace LinkedOut {

	NavigationMenu::NavigationMenu(MainLayer* mainLayer, const NavigationMenuSpecification& specification)
		:m_Specification(specification),m_MainLayer(mainLayer),m_NavigationDivision(m_MainLayer->m_WindowCentralWidget)
	{
        m_NavigationDivision.Layout->setContentsMargins(5, 0, 5, 0);
        m_NavigationDivision.Widget->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum));
        CreateSearchBar();
        CreateButtons();
        CreateProfileDropdown();

        m_MainLayer->m_WindowCentralLayout->addWidget(m_NavigationDivision.Widget);
	}
	NavigationMenu::~NavigationMenu()
	{
        delete m_SearchBar;
	}
    void NavigationMenu::Show()
    {
        m_NavigationDivision.Widget->show();
    }
    void NavigationMenu::Hide()
    {
        m_NavigationDivision.Widget->hide();
    }
    void NavigationMenu::CreateSearchBar()
    {
        m_SearchBar = new SearchBar(m_MainLayer->m_WindowCentralWidget);
        m_NavigationDivision.Layout->addWidget(m_SearchBar,1);
    }

    static QSize CalcTextSize(const QFont& font, const std::string& text) {
        QFontMetrics metrics(font);
        QSize textSize = metrics.size(Qt::TextSingleLine, QString::fromStdString(text));
        return  textSize;
    }

    void NavigationMenu::CreateButtons()
    {
        m_ButtonsContainerLayout = new QHBoxLayout;
        m_ButtonsContainer = new QWidget(m_MainLayer->m_WindowCentralWidget);

        uint32_t buttonCount = m_Specification.Buttons.size();

        const float windowWidth = Window::GetMainWindow()->GetWindowProps().Width;
        const float windowHeight = Window::GetMainWindow()->GetWindowProps().Height;

        const float textPadding = 3;

        float buttonWidth = m_Specification.ButtonMinWidth;
        float buttonHeight = m_Specification.ButtonHeight;


        for (auto& [name, button] : m_Specification.Buttons) {
            auto textSize = CalcTextSize(m_ButtonsContainer->font(), name);
            buttonWidth = std::max(buttonWidth,(float)textSize.width());
        }

        //const float buttonWidth = ;
        //const float buttonHeight = m_Specification.ButtonHeight;

        for (auto& [name, button] : m_Specification.Buttons) {
            float textSize = CalcTextSize(m_ButtonsContainer->font(), name).width();

            auto titledButton = new TitledButton(button.Icon, buttonWidth + 2 * textPadding, buttonHeight, 0, QString::fromStdString(name));
            QObject::connect(titledButton, &TitledButton::clicked, [this,&button,&name](auto&&...) {
                SetActiveTab(name);
                ActivateTab();
                button.Callback();
            });
            titledButton->setMinimumWidth(textSize + 2 * textPadding);
            titledButton->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
            titledButton->setStyleSheet(QString::fromStdString(m_Specification.ButtonOffStyle));

            m_Buttons[name] = titledButton;
            m_ButtonsContainerLayout->addWidget(titledButton);
        }
        m_ButtonsContainer->setLayout(m_ButtonsContainerLayout);
        
        m_NavigationDivision.Layout->addWidget(m_ButtonsContainer,2,Qt::AlignCenter);

    }
    void NavigationMenu::CreateProfileDropdown()
    {
        m_ProfileOpenButton = new TitledButton(QIcon("Resources/NavigationProfileOpenIcon.png"),
                                                    std::max(CalcTextSize(m_NavigationDivision.Widget->font(), "Profile").width(),30), (uint32_t)m_Specification.ButtonHeight,0,
                                                    QString("Profile"), m_MainLayer->m_WindowCentralWidget);
        m_ProfileOpenButton->setStyleSheet(QString::fromStdString(m_Specification.ProfileOpenStyle));
        m_NavigationDivision.Layout->addWidget(m_ProfileOpenButton);
    }
    void NavigationMenu::ActivateTab()
    {
        for (auto& [name, titleButton] : m_Buttons) {
            titleButton->setStyleSheet(QString::fromStdString(m_Specification.ButtonOffStyle));
        }
        m_Buttons[m_ActiveTab]->setStyleSheet(QString::fromStdString(m_Specification.ButtonOnStyle));
    }
}