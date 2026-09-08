#include "widgetfactory.h"
#include "components/workspaceswitchercomponent.h"

WidgetFactory::WidgetFactory(QObject *parent)
    : QObject{parent}
{
    registerWidget("workspaceswitcher", [](const QJsonObject& obj, QWidget* parent){
        WorkspaceSwitcherComponent* widget = new WorkspaceSwitcherComponent(parent);
        return widget;
    });
}

void WidgetFactory::registerWidget(const QString &name, Creator creator)
{
    m_creators[name] = std::move(creator);
}

QWidget *WidgetFactory::create(const QString &name, const QJsonObject& obj, QWidget *parent) const
{
    auto it = m_creators.find(name);

    if(it == m_creators.end())
        return nullptr;

    return it.value()(obj, parent);
}
