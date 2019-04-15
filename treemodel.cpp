#include "treemodel.h"
#include <QDebug>

TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent) {
  m_rootItem = new TreeItem();
}

TreeModel::~TreeModel() { delete m_rootItem; }

QVariant TreeModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return QVariant();
  TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
  switch (role) {
    case Qt::DisplayRole:
      return item->getItemName();
    case Qt::DecorationRole:
      return item->getIcon();
    case Qt::BackgroundRole:
      return item->getBackColor();
    case Qt::ForegroundRole:
      return item->getTextColor();
    default:
      return QVariant();
  }
}

QModelIndex TreeModel::index(int row, int column,
                             const QModelIndex &parent) const {
  TreeItem *parentItem;
  if (!parent.isValid())
    parentItem = m_rootItem;
  else {
    parentItem = static_cast<TreeItem *>(parent.internalPointer());
    Q_ASSERT(parentItem);
  }
  TreeItem *childItem = parentItem->getChildItem(row);
  if (childItem) return createIndex(row, column, childItem);
  return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &child) const {
  if (!child.isValid()) return QModelIndex();
  TreeItem *childItem = static_cast<TreeItem *>(child.internalPointer());
  if (!childItem) return QModelIndex();
  TreeItem *parentItem = childItem->getParentItem();
  if (parentItem == m_rootItem) {
    return QModelIndex();
  } else
    return createIndex(parentItem->getSelfRow(), 0, parentItem);
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role) {
  if (QAbstractItemModel::setHeaderData(section, orientation, value, role)) {
    emit headerDataChanged(orientation, section, section);
    return true;
  }
  return false;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const {
  Q_UNUSED(section);
  Q_UNUSED(orientation);
  if (role == Qt::DisplayRole)
    return QVariant("");
  else
    return QVariant();
}

int TreeModel::rowCount(const QModelIndex &parent) const {
  TreeItem *parentItem;
  if (!parent.isValid())
    parentItem = m_rootItem;
  else {
    parentItem = static_cast<TreeItem *>(parent.internalPointer());
    Q_ASSERT(parentItem);
  }
  return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 1;
}

bool TreeModel::insertRows(int row, int count, const QModelIndex &parent) {
  TreeItem *parentItem;
  if (parent.isValid()) {
    parentItem = static_cast<TreeItem *>(parent.internalPointer());
    Q_ASSERT(parentItem);
  } else
    parentItem = m_rootItem;
  beginInsertRows(parent, row, row + count - 1);
  parentItem->insertChild(new TreeItem(parentItem));
  endInsertRows();
  return true;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent) {
  TreeItem *parentItem;
  if (!parent.isValid()) {
    parentItem = m_rootItem;
  } else {
    parentItem = static_cast<TreeItem *>(parent.internalPointer());
    Q_ASSERT(parentItem);
  }
  beginRemoveRows(parent, row, row + count - 1);
  parentItem->removeChild(row);
  endRemoveRows();
  return true;
}

void TreeModel::moveItem(const QModelIndex &item, MoveDirection direction) {
  if (!item.isValid()) return;
  TreeItem *node = static_cast<TreeItem *>(item.internalPointer());
  int targetItemPos = node->getSelfRow();
  if (item.parent().isValid())
    node = static_cast<TreeItem *>(item.parent().internalPointer());
  else
    node = m_rootItem;
  if (direction == MoveDirection::UP && targetItemPos) {
    beginMoveRows(item.parent(), targetItemPos, targetItemPos, item.parent(),
                  targetItemPos - 1);
    node->swapChilds(targetItemPos, targetItemPos - 1);
    endMoveRows();
  } else if (direction == MoveDirection::DOWN &&
             targetItemPos < node->childCount() - 1) {
    beginMoveRows(item.parent(), targetItemPos + 1, targetItemPos + 1,
                  item.parent(), targetItemPos);
    node->swapChilds(targetItemPos, targetItemPos + 1);
    endMoveRows();
    return;
  } else {
    return;
  }
}

void TreeModel::sort(int column, Qt::SortOrder order) {
  Q_UNUSED(column);
  emit layoutAboutToBeChanged();
  if (order == Qt::AscendingOrder)
    sortItems(m_rootItem->getChildList(), TreeItem::itemCompareAsc);
  else
    sortItems(m_rootItem->getChildList(), TreeItem::itemCompareDes);
  emit layoutChanged();
}

void TreeModel::sortItems(QList<TreeItem *> &itemList, compFunc func) {
  std::sort(itemList.begin(), itemList.end(), func);
  for (auto it = itemList.begin(); it != itemList.end(); ++it) {
    TreeItem *node = *it;
    if (node->childCount())
      sortItems(node->getChildList(), func);
    else
      continue;
  }
}

Qt::DropActions TreeModel::supportedDropActions() const {
  return Qt::MoveAction;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
  if (index.isValid())
    return (QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled);
  return Qt::ItemIsDropEnabled;
}
