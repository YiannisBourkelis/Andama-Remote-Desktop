#ifndef TBLLOGSORTFILTERPROXYMODEL_H
#define TBLLOGSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class TblLogSortFilterProxyModel : QSortFilterProxyModel
{
public:
    explicit TblLogSortFilterProxyModel(QObject * parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // TBLLOGSORTFILTERPROXYMODEL_H
