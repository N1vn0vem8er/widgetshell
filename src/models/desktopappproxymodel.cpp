#include "desktopappproxymodel.h"
#include "models/desktopappmodel.h"

DesktopAppProxyModel::DesktopAppProxyModel(QObject *parent) : QSortFilterProxyModel{parent}
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterKeyColumn(0);
    setDynamicSortFilter(true);
}

void DesktopAppProxyModel::setSearchText(const QString &text)
{
    if(m_searchText == text)
        return;
    beginFilterChange();
    m_searchText = text;
    endFilterChange();
}

QString DesktopAppProxyModel::searchText() const
{
    return m_searchText;
}

bool DesktopAppProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &) const
{
    if(m_searchText.trimmed().isEmpty())
        return true;

    const auto* model = qobject_cast<const DesktopAppModel*>(sourceModel());

    if(!model)
        return false;

    const auto app = model->appAt(sourceRow);

    if(!app)
        return false;

    const QString query = m_searchText.trimmed();

    auto matches = [&query](const QString& value){
        return value.contains(query, Qt::CaseInsensitive);
    };

    if(matches(app->name) || matches(app->genericName) || matches(app->comment) || matches(app->fileName))
        return true;

    for(const auto& keyword : app->keywords)
    {
        if(matches(keyword))
            return true;
    }
    for(const auto& category : app->categories)
    {
        if(matches(category))
            return true;
    }

    return false;
}
