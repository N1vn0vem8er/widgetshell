#ifndef DESKTOPAPPPROXYMODEL_H
#define DESKTOPAPPPROXYMODEL_H

#include <QSortFilterProxyModel>

class DesktopAppProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit DesktopAppProxyModel(QObject *parent = nullptr);
    void setSearchText(const QString& text);
    QString searchText() const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex&) const override;

private:
    QString m_searchText;
};

#endif // DESKTOPAPPPROXYMODEL_H
