#pragma once
#include "Core/Layer.h"

#include "CustomUI/Division.h"

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
		std::function<void()> Callback;
	};


	struct NavigationMenuSpecification {
		std::unordered_map<std::string, NavigationMenuNavigationButton> Buttons;
		float ButtonMinWidth;
		float ButtonMinHeight;
		float ButtonHeight;
		std::string ButtonOnStyle;
		std::string ButtonOffStyle;
		std::string ProfileOpenStyle;
	};


	class NavigationMenu {
	public:
		NavigationMenu(MainLayer* mainLayer, const NavigationMenuSpecification& specification);
		~NavigationMenu();

		void Show();
		void Hide();

		void SetActiveTab(const std::string& name) { m_ActiveTab = name; ActivateTab(); }

	private:
		void CreateSearchBar();
		void CreateButtons();
		void CreateProfileDropdown();
		void ActivateTab();
	private:
		MainLayer* m_MainLayer;

		NavigationMenuSpecification m_Specification;
		std::unordered_map<std::string, TitledButton*> m_Buttons;

		HDivision m_NavigationDivision;

		SearchBar* m_SearchBar;
		QHBoxLayout* m_ButtonsContainerLayout;
		QWidget* m_ButtonsContainer;

		TitledButton* m_ProfileOpenButton;
		std::string m_ActiveTab;
	};
}