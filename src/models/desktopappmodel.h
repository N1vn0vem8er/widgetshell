#ifndef DESKTOPAPPMODEL_H
#define DESKTOPAPPMODEL_H

#include "services/desktopappsservice.h"
#include <QAbstractListModel>

class DesktopAppModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Role {
        AppRole = Qt::UserRole + 1,
        FileNameRole,
        GenericNameRole,
        CommentRole,
        ExecRole
    };

    explicit DesktopAppModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void addApp(const DesktopApp& app);
    void addApps(const QList<DesktopApp> &apps);
    void removeApp(int row);
    void clear();
    const DesktopApp* appAt(int row) const;
    QList<DesktopApp> apps() const;

private:
    QList<DesktopApp> m_apps;
};

#endif // DESKTOPAPPMODEL_H
