#pragma once
#include <string>
#include <vector>
#include "Base.h"

#include <QVariantList>
#include <QVariant>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>



class QSqlDatabase;



namespace LinkedOut {

	class QueryException:public std::exception {
	public:
		QueryException(const std::string& str)
			:m_Error(str)
		{}


		virtual const char* what()const override {
			return m_Error.c_str();
		}
	private:
		std::string m_Error;;
	};


	class DatabaseManager {
	public:
		DatabaseManager() = default;
		DatabaseManager(const std::string& url);
		~DatabaseManager();


		void Connect(const std::string& url);
		void Disconnect();
		bool Valid()const { return m_Database != nullptr; }

		bool Execute(const std::string& statements);

		std::vector<QSqlRecord> ExecuteAndReturn(const std::string& statements);

		void Commit();

	private:
		QSqlDatabase* m_Database = nullptr;
	};

}

