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
		:m_Specification(specification),m_MainLayer(mainLayer),m_MainDiv(m_MainLayer->m_WindowCentralWidget), Layer("NavigationMenu")
	{
        m_NavigationDivision = HDivision(m_MainDiv.Widget);
        m_PanelDiv = VDivision(m_MainDiv.Widget);
        m_NavigationDivision.Layout->setContentsMargins(5, 0, 5, 0);
        m_NavigationDivision.Widget->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum));
        m_NavigationDivision.Widget->setStyleSheet("background-color:white;");
        m_MainDiv.Layout->addWidget(m_NavigationDivision.Widget);
        m_MainDiv.Layout->addWidget(m_PanelDiv.Widget);
        m_PanelDiv.Layout->setContentsMargins(0, 0, 0, 0);
        CreateSearchBar();
        CreateButtons();
        CreateProfileDropdown();

        m_MainLayer->m_LayersLayout->addWidget(m_MainDiv.Widget);
	}
	NavigationMenu::~NavigationMenu()
	{
        delete m_MainDiv.Widget;
	}
    void NavigationMenu::Show()
    {
        m_NavigationDivision.Widget->show();
    }
    void NavigationMenu::Hide()
    {
        m_NavigationDivision.Widget->hide();
    }
    void NavigationMenu::SetActiveTab(const std::string& name)
    {
        m_Specification.ViewProfilePanel->Clear();
        if (auto ptr = m_ButtonSpecs.find(m_ActiveTab); ptr != m_ButtonSpecs.end()) {
            ptr->second.Panel->Hide();
            ptr->second.Panel->Clear();
        }
        m_ActiveTab = name;
        ActivateTab();
        if (auto ptr = m_ButtonSpecs.find(name); ptr != m_ButtonSpecs.end()) {
            ptr->second.Panel->SetContext(MainLayer::Get().m_CurrentUser);
            ptr->second.Panel->Show();
        }
    }
    void NavigationMenu::CreateSearchBar()
    {
        m_SearchBar = new SearchBar([=](const QString& text) {
            if (text.isEmpty())
                return;
            auto& usernames = MainLayer::Get().GetIDToUsername();
            for (auto& [id, username] : usernames) {
                if (username == text.toStdString()) {
                    m_SearchBar->SetText("");
                    SetActiveTab("");
                    m_Specification.ViewProfilePanel->SetContext(MainLayer::Get().GetPerson(id));
                    break;
                }
            }
        }, m_NavigationDivision.Widget);
        m_NavigationDivision.Layout->addWidget(m_SearchBar);
        m_NavigationDivision.Layout->addStretch();
    }

    static QSize CalcTextSize(const QFont& font, const std::string& text) {
        QFontMetrics metrics(font);
        QSize textSize = metrics.size(Qt::TextSingleLine, QString::fromStdString(text));
        return  textSize;
    }

    void NavigationMenu::CreateButtons()
    {
        m_ButtonsContainerLayout = new QHBoxLayout;
        m_ButtonsContainer = new QWidget(m_NavigationDivision.Widget);

        uint32_t buttonCount = m_Specification.Buttons.size();


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
            if (button.Hidden)
                titledButton->hide();
            QObject::connect(titledButton, &TitledButton::clicked, [this,&button,&name](auto&&...) {
                SetActiveTab(name);
            });
            titledButton->setMinimumWidth(textSize + 2 * textPadding);
            titledButton->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
            titledButton->setStyleSheet(QString::fromStdString(m_Specification.ButtonOffStyle));

            m_Buttons[name] = titledButton;
            m_ButtonsContainerLayout->addWidget(titledButton);

            if (button.Panel) {
                button.Panel->setParent(m_PanelDiv.Widget);
                m_PanelDiv.Layout->addWidget(button.Panel.get());
                button.Panel->Hide();
            }
            m_ButtonSpecs[name] = button;
        }
        m_ButtonsContainer->setLayout(m_ButtonsContainerLayout);
        
        m_NavigationDivision.Layout->addWidget(m_ButtonsContainer);
    }
    void NavigationMenu::CreateProfileDropdown()
    {
        auto& button = m_Specification.ProfileButton;
        m_ProfileOpenButton = new TitledButton(button.Icon,
                                                    std::max(CalcTextSize(m_NavigationDivision.Widget->font(), "Profile").width(),30), (uint32_t)m_Specification.ButtonHeight,0,
                                                    "Profile");
        m_ProfileOpenButton->setStyleSheet(QString::fromStdString(m_Specification.ProfileOpenStyle));
        m_NavigationDivision.Layout->addWidget(m_ProfileOpenButton);

        QObject::connect(m_ProfileOpenButton, &TitledButton::clicked, [this, &button]() {
            SetActiveTab("Profile");
            });
        m_Buttons["Profile"] = m_ProfileOpenButton;
        m_ButtonSpecs["Profile"] = button;

        if (button.Panel) {
            button.Panel->setParent(m_PanelDiv.Widget);
            m_PanelDiv.Layout->addWidget(button.Panel.get());
            button.Panel->Hide();
        }
        m_NavigationDivision.Layout->addStretch();
    }
    void NavigationMenu::ActivateTab()
    {
        for (auto& [name, titleButton] : m_Buttons) {
            titleButton->setStyleSheet(QString::fromStdString(m_Specification.ButtonOffStyle));
        }
        if(auto ptr = m_Buttons.find(m_ActiveTab);ptr != m_Buttons.end())
            ptr->second->setStyleSheet(QString::fromStdString(m_Specification.ButtonOnStyle));
    }
    void* NavigationMenu::GetMainFrame() const
    {
        return nullptr;
    }
    void NavigationMenu::OnUpdate()
    {
    }
}