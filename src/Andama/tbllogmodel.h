#ifndef TBLLOGMODEL_H
#define TBLLOGMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <vector>
#include "tbllogdata.h"

class tblLogModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    tblLogModel();
    tblLogModel(QObject *parent);


    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addLogData(const TblLogData &logdata);
    void addLogData(QString eventDescription);

private:
    std::vector<TblLogData> _logData;
};
#endif // TBLLOGMODEL_H
