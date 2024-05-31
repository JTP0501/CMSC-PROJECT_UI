// customeventfilter.h
#ifndef CUSTOMEVENTFILTER_H
#define CUSTOMEVENTFILTER_H

#include <QObject>
#include <QEvent>
#include <QListView>

class CustomEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit CustomEventFilter(QListView* ongoingListView, QListView* waitlistedListView, QObject *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QListView* m_pwOngoing;
    QListView* m_pwWaitlisted;
};

#endif // CUSTOMEVENTFILTER_H
