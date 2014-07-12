#ifndef MULTILIST_H
#define MULTILIST_H

#include <QtGui>
#include <QComboBox>
#include <QStylePainter>

class MultiListWidget : public QComboBox
{
    Q_OBJECT

    Q_PROPERTY(QStringList checkedItems READ checkedItems WRITE setCheckedItems)

public:
    MultiListWidget();
    virtual ~MultiListWidget();

    QStringList checkedItems() const;
    void setCheckedItems(const QStringList &items);

    QString getDisplayText(){return mDisplayText;}

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private:
    QStringList mCheckedItems;

    void collectCheckedItems();

    QString mDisplayText;
    const QRect mDisplayRectDelta;

    void updateDisplayText();

private slots:
    void slotModelRowsInserted(const QModelIndex &parent, int start, int end);
    void slotModelRowsRemoved(const QModelIndex &parent, int start, int end);
    void slotModelItemChanged(QStandardItem *item);

};

#endif // MULTILIST_H
