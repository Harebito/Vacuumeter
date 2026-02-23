#ifndef USBMONITOR_DATABASE_MANAGER_H
#define USBMONITOR_DATABASE_MANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QDateTime>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    // Call this once at startup
    bool init();

public slots:
    // Slot to capture data.
    // You can change QByteArray to QString or specific values depending on what to save.
    void save_data(const QByteArray &raw_data);

private:
    QSqlDatabase m_db;
};

#endif //USBMONITOR_DATABASE_MANAGER_H