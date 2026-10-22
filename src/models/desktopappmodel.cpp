#include "desktopappmodel.h"
#include <qicon.h>

DesktopAppModel::DesktopAppModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int DesktopAppModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return m_apps.size();
}

QVariant DesktopAppModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() < 0 || index.row() >= m_apps.size())
        return {};

    const auto& app = m_apps.at(index.row());
    switch(role)
    {
    case Qt::DisplayRole:
        return app.name;
    case Qt::DecorationRole:
        if(!app.icon.isEmpty())
        {
            if(app.icon.startsWith('/'))
                return QIcon(app.icon);
            return QIcon::fromTheme(app.icon);
        }
        return {};
    case AppRole:
        return QVariant::fromValue(app);
    case FileNameRole:
        return app.fileName;
    case GenericNameRole:
        return app.genericName;
    case CommentRole:
        return app.comment;
    case ExecRole:
        return app.exec;
    default:
        return {};
    }
}

QHash<int, QByteArray> DesktopAppModel::roleNames() const
{
    auto roles = QAbstractListModel::roleNames();
    roles[AppRole] = "app";
    roles[FileNameRole] = "fileName";
    roles[GenericNameRole] = "genericName";
    roles[CommentRole] = "comment";
    roles[ExecRole] = "exec";

    return roles;
}

void DesktopAppModel::addApp(const DesktopApp &app)
{
    int row = m_apps.size();
    beginInsertRows(QModelIndex(), row, row);
    m_apps.append(app);
    endInsertRows();
}

void DesktopAppModel::addApps(const QList<DesktopApp> &apps)
{
    if(apps.isEmpty())
        return;

    int first = m_apps.size();
    int last = first + apps.size() - 1;
    beginInsertRows(QModelIndex(), first, last);
    m_apps.append(apps);
    endInsertRows();
}

void DesktopAppModel::removeApp(int row)
{
    if(row < 0 || row >= m_apps.size())
        return;
    beginRemoveRows(QModelIndex(), row, row);
    m_apps.removeAt(row);
    endRemoveRows();
}

void DesktopAppModel::clear()
{
    if(m_apps.isEmpty())
        return;
    beginResetModel();
    m_apps.clear();
    endResetModel();
}

const DesktopApp *DesktopAppModel::appAt(int row) const
{
    if(row < 0 || row >= m_apps.size())
        return nullptr;
    return &m_apps.at(row);
}

QList<DesktopApp> DesktopAppModel::apps() const
{
    return m_apps;
}