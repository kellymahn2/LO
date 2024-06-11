#pragma once

#include "Core/Layer.h"

namespace LinkedOut {


	class MainLayer;
	class UserInformationLayer : public Layer{
	public:
		UserInformationLayer(MainLayer* layer);

		void SetContext(const UserData& userData);

		~UserInformationLayer();

		void Show() override;

		void Hide() override;

		void OnUpdate() override;
    private:
        void SetupUserInformation();
        void SetupUserInformationEvents();
        void ValidateInput();

        UserData GetUserInfoFromFields();

	private:
		MainLayer* m_MainLayer;
		//QT

        QFrame* m_MainFrame = nullptr;
        QFrame* m_UserInformationFrame = nullptr;
        QLabel* m_UserProfileLabel = nullptr;
        QFrame* m_FirstNameFrame = nullptr;
        QLineEdit* m_FirstNameTextInput = nullptr;
        QLabel* m_FirstNameLabel = nullptr;
        QFrame* m_LastNameFrame = nullptr;
        QLineEdit* m_LastNameTextInput = nullptr;
        QLabel* m_LastNameLabel = nullptr;
        QFrame* m_BirthDateFrame = nullptr;
        QLineEdit* m_BirthDateTextInput = nullptr;
        QLabel* m_BirthDateLabel = nullptr;
        QFrame* m_PlaceOfEducationFrame = nullptr;
        QLabel* m_PlaceOfEducationLabel = nullptr;
        QLineEdit* m_PlaceOfEducationTextInput = nullptr;
        QPushButton* m_SaveButton = nullptr;
        bool m_IsShowing = false;


		UserData m_UserData;


	};

}