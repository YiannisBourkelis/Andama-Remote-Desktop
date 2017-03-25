#include "tbllogmodel.h"
#include <QBrush>

tblLogModel::tblLogModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

tblLogModel::tblLogModel(): tblLogModel::tblLogModel(NULL)
{

}

int tblLogModel::rowCount(const QModelIndex & /*parent*/) const
{
   int s = _logData.size();
   return s;
}

int tblLogModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

QVariant tblLogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _logData.size() || index.row() < 0)
        return QVariant();


    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0){
            std::string mdate =  _logData.at(index.row()).getDateTimeString();
            return QString::fromStdString(mdate);
        }
        return QString::fromStdString(_logData.at(index.row()).eventDescription);
    }
    else if (role == Qt::ForegroundRole)
    {
        return QBrush(QColor::fromRgb(255,255,255));
    }
    else if (role == Qt::ToolTipRole)
    {
        return QString::fromStdString(_logData.at(index.row()).eventDescription);
    }
    return QVariant();
}


void tblLogModel::addLogData(const TblLogData &logdata)
{
    int s = _logData.size();
    this->beginInsertRows(QModelIndex(),s, s);
    _logData.insert(_logData.begin(), logdata);
    this->endInsertRows();
}

void tblLogModel::addLogData(QString eventDescription)
{
    addLogData(TblLogData(eventDescription.toStdString()));
}
