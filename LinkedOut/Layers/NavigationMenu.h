#pragma once
#include "Core/Layer.h"
#include "NavPanel.h"
#include "CustomUI/Division.h"
#include "CustomUI/TitledButton.h"

#include <unordered_map>
#include <QIcon>
#include <functional>

class QHBoxLayout;
class SearchBar;
class TitledButton;
struct HDivision;

namespace LinkedOut {
	
	class MainLayer;

	struct NavigationMenuNavigationButton {
		QIcon Icon;
		Ref<NavPanel> Panel;
		bool Hidden = false;

	};

	struct NavigationMenuSpecification {
		std::unordered_map<std::string, NavigationMenuNavigationButton> Buttons;
		float ButtonMinWidth;
		float ButtonMinHeight;
		float ButtonHeight;
		std::string ButtonOnStyle;
		std::string ButtonOffStyle;
		std::string ProfileOpenStyle;

		NavigationMenuNavigationButton ProfileButton;
		Ref<NavPanel> ViewProfilePanel;
	};

	class NavigationMenu : public Layer {
	public:
		NavigationMenu(MainLayer* mainLayer, const NavigationMenuSpecification& specification);
		~NavigationMenu();

		void Show();
		void Hide();

		void SetActiveTab(const std::string& name);

		void Refresh() {
			SetActiveTab(m_ActiveTab);
		}

		void HideButton(const std::string& name) {
			m_Buttons[name]->hide();
			m_ButtonSpecs[name].Hidden = true;
		}
		void ShowButton(const std::string& name) {
			m_ButtonSpecs[name].Hidden = false;
			m_Buttons[name]->show();
		}

	private:
		void CreateSearchBar();
		
		void CreateButtons();
		void CreateProfileDropdown();
		void ActivateTab();
	private:
		static inline const float s_TextPadding = 3;

		MainLayer* m_MainLayer;
		VDivision m_MainDiv;
		VDivision m_PanelDiv;

		NavigationMenuSpecification m_Specification;
		std::unordered_map<std::string, NavigationMenuNavigationButton> m_ButtonSpecs;
		std::unordered_map<std::string, TitledButton*> m_Buttons;

		HDivision m_NavigationDivision;

		SearchBar* m_SearchBar;
		QHBoxLayout* m_ButtonsContainerLayout;
		QWidget* m_ButtonsContainer;

		TitledButton* m_ProfileOpenButton;
		std::string m_ActiveTab;

		// Inherited via Layer
		void* GetMainFrame() const override;
		void OnUpdate() override;
	};
}