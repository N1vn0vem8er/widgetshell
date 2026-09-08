#ifndef WIDGETFACTORY_H
#define WIDGETFACTORY_H

#include <QHash>
#include <QObject>

class WidgetFactory : public QObject
{
    Q_OBJECT
public:
    using Creator = std::function<QWidget*(const QJsonObject&, QWidget*)>;
    explicit WidgetFactory(QObject *parent = nullptr);
    void registerWidget(const QString& name, Creator creator);
    QWidget* create(const QString& name, const QJsonObject& obj, QWidget* parent = nullptr) const;

private:
    QHash<QString, Creator> m_creators;
};

#endif // WIDGETFACTORY_H
