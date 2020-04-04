#include "tbllogsortfilterproxymodel.h"

TblLogSortFilterProxyModel::TblLogSortFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}

/*
QVariant TblLogSortFilterProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   if(orientation == Qt::Horizontal)
   {
       if(role == Qt::DisplayRole)
           switch(section)
           {
           case 0: return "Date/time";            break;
           case 1: return "Event";   break;
           default:
               return QString("Column %1").arg(section + 1);
               break;
           }
   }

   return QVariant();
}
*/

