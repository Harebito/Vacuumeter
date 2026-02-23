#include "database_manager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::init() {
    // 1. Set up the SQLite driver
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    // 2. Define path (saves to standard AppData location, or just "log.db" for local)
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    if (!dir.exists()) dir.mkpath("."); // Ensure folder exists

    QString dbPath = path + "/usb_Vacuumeter_log.db";
    m_db.setDatabaseName(dbPath);

    qDebug() << "Database path:" << dbPath;

    if (!m_db.open()) {
        qCritical() << "Error: Connection failed" << m_db.lastError();
        return false;
    }

    // 3. Create table if it doesn't exist
    QSqlQuery query;
    // Simple schema: ID, Timestamp, Data
    QString createTable = R"(
        CREATE TABLE IF NOT EXISTS sensor_log (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
            payload BLOB
        )
    )";

    if (!query.exec(createTable)) {
        qCritical() << "Error: Creating table failed" << query.lastError();
        return false;
    }

    return true;
}

void DatabaseManager::save_data(const QByteArray &raw_data) {
    QSqlQuery query;

    // Use "PREPARED STATEMENTS" (using :placeholder)
    // This is safer and handles special characters/binary data correctly
    query.prepare("INSERT INTO sensor_log (timestamp, payload) VALUES (:time, :data)");

    query.bindValue(":time", QDateTime::currentDateTime());
    query.bindValue(":data", raw_data);

    if (!query.exec()) {
        qWarning() << "Error: Insert failed" << query.lastError();
    }
}