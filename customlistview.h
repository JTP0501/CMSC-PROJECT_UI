#ifndef CUSTOMLISTVIEW_H
#define CUSTOMLISTVIEW_H

#include <QListView>

class CustomListView : public QListView {
    Q_OBJECT

public:
    CustomListView(QWidget *parent = nullptr);

protected:
    void startDrag(Qt::DropActions supportedActions) override;
    void dropEvent(QDropEvent *event) override;
    Qt::DropActions supportedDropActions() const override;
};

#endif // CUSTOMLISTVIEW_H
