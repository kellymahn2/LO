#include "DatabaseManager.h"
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
namespace LinkedOut {



	DatabaseManager::DatabaseManager(const std::string& url){

		Connect(url);
	}
	DatabaseManager::~DatabaseManager(){
		Disconnect();
	}

	void DatabaseManager::Connect(const std::string& url) {
		Disconnect();

		// Open the database connection
		m_Database = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
		m_Database->setDatabaseName(QString::fromStdString(url));

		if (!m_Database->open()) {
			delete m_Database;
			m_Database = nullptr;
		}

	}

	

	void DatabaseManager::Disconnect() {
		if (m_Database) {
			m_Database->close();
			delete m_Database;
			m_Database = nullptr;
		}
	}

	bool DatabaseManager::Execute(const std::string& statements) {
		if (!Valid())
			return false;
		QSqlQuery query(QString(), *m_Database);
		if (!query.exec(QString::fromStdString(statements))) {
			qDebug(query.lastError().text().toStdString().c_str());
			throw QueryException(query.lastError().text().toStdString());
		}
		return true;
	}
	std::vector<QSqlRecord> DatabaseManager::ExecuteAndReturn(const std::string& statements) {
		std::vector<QSqlRecord> result;

		if (!Valid()) {
			return result;
		}

		QSqlQuery query(QString::fromStdString(statements), *m_Database);

		if (!query.exec()) {
			qDebug(query.lastError().text().toStdString().c_str());
			throw QueryException(query.lastError().text().toStdString());
			return result;
		}

		while (query.next()) {
			result.push_back(query.record());
		}

		return result;
	}
	void DatabaseManager::Commit() {
		if (!Valid())
			return;
		m_Database->commit();
	}
}
