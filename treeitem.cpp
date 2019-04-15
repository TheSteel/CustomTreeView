#include "treeitem.h"
#include <QDebug>

TreeItem::TreeItem(TreeItem *parent)
    : m_parentItem(parent),
      m_name(""),
      m_rowColor(QColor(Qt::white)),
      m_textColor(QColor(Qt::black)) {
  if (!parent)
    setItemLvl(0);
  else
    setItemLvl(parent->getItemLvl() + 1);
}

TreeItem::TreeItem(const TreeItem &other) {
  m_name = other.getItemName();
  m_pic = other.getIcon();
  m_textColor = other.getTextColor();
  m_rowColor = other.getBackColor();
  m_parentItem = other.getParentItem();
  m_itemLvl = other.getItemLvl();
  const QList<TreeItem *> &list = other.getChildListRead();
  for (auto it = list.begin(); it != list.end(); ++it) {
    TreeItem *item = *it;
    TreeItem *newItem;
    if (item->childCount()) {
      newItem = deepCopyItem(*item, m_parentItem);
    } else {
      newItem = new TreeItem(m_parentItem);
      newItem->setIcon(item->getIcon());
      newItem->setName(item->getItemName());
      newItem->setTextColor(item->getTextColor());
      newItem->setBackColor(item->getBackColor());
      newItem->setItemLvl(item->getItemLvl());
    }
    m_childItems.append(newItem);
  }
}

TreeItem &TreeItem::operator=(const TreeItem &other) {
  if (this == &other) return *this;
  m_name = other.getItemName();
  m_pic = other.getIcon();
  m_textColor = other.getTextColor();
  m_rowColor = other.getBackColor();
  m_parentItem = other.getParentItem();
  m_itemLvl = other.getItemLvl();
  if (m_childItems.count()) qDeleteAll(m_childItems);
  m_childItems = other.getChildListRead();
  return *this;
}

TreeItem::~TreeItem() {
  if (m_childItems.count()) qDeleteAll(m_childItems);
}

void TreeItem::insertChild(TreeItem *child) { m_childItems.append(child); }

int TreeItem::childCount() const { return m_childItems.size(); }

TreeItem *TreeItem::getParentItem() const { return m_parentItem; }

TreeItem *TreeItem::getChildItem(int pos) const {
  if (pos >= 0 && pos < m_childItems.count()) return m_childItems.value(pos);
  return Q_NULLPTR;
}

int TreeItem::getChildPos(TreeItem *child) const {
  return m_childItems.indexOf(child);
}

QString TreeItem::getItemName() const { return m_name; }

QIcon TreeItem::getIcon() const { return m_pic; }

QColor TreeItem::getBackColor() const { return m_rowColor; }

QColor TreeItem::getTextColor() const { return m_textColor; }

void TreeItem::setIcon(const QIcon &icon) { m_pic = icon; }

void TreeItem::setBackColor(const QColor &color) { m_rowColor = color; }

void TreeItem::setTextColor(const QColor &color) { m_textColor = color; }

void TreeItem::setName(const QString &name) { m_name = name; }

void TreeItem::setParent(TreeItem *parent) { m_parentItem = parent; }

void TreeItem::setItemLvl(int lvl) { m_itemLvl = lvl; }

int TreeItem::getSelfRow() const {
  if (m_parentItem != nullptr)
    return m_parentItem->getChildPos(const_cast<TreeItem *>(this));

  return 0;
}

void TreeItem::removeChild(int pos) { delete m_childItems.takeAt(pos); }

void TreeItem::swapChilds(int target, int dest) {
  Q_ASSERT(target >= 0 && target < m_childItems.count());
  Q_ASSERT(dest >= 0 && dest < m_childItems.count());
  qSwap(m_childItems[target], m_childItems[dest]);
}

QList<TreeItem *> &TreeItem::getChildList() { return m_childItems; }

const QList<TreeItem *> &TreeItem::getChildListRead() const {
  return m_childItems;
}

int TreeItem::getItemLvl() const { return m_itemLvl; }

TreeItem *TreeItem::deepCopyItem(const TreeItem &child,
                                 TreeItem *const parent) {
  TreeItem *item = new TreeItem(parent);
  item->setIcon(child.getIcon());
  item->setName(child.getItemName());
  item->setTextColor(child.getTextColor());
  item->setBackColor(child.getBackColor());
  item->setItemLvl(child.getItemLvl());
  for (auto it = child.getChildListRead().begin();
       it != child.getChildListRead().end(); ++it) {
    TreeItem *i = *it;
    TreeItem *newItem;
    if (i->childCount()) {
      newItem = deepCopyItem(*i, item);
    } else {
      newItem = new TreeItem(item);
      newItem->setIcon(i->getIcon());
      newItem->setName(i->getItemName());
      newItem->setTextColor(i->getTextColor());
      newItem->setBackColor(i->getBackColor());
      newItem->setItemLvl(i->getItemLvl());
    }
    item->insertChild(newItem);
  }
  return item;
}

bool TreeItem::itemCompareAsc(const TreeItem *left, const TreeItem *right) {
  Q_ASSERT(left);
  Q_ASSERT(right);
  return left->getItemName() < right->getItemName();
}

bool TreeItem::itemCompareDes(const TreeItem *left, const TreeItem *right) {
  Q_ASSERT(left);
  Q_ASSERT(right);
  return left->getItemName() > right->getItemName();
}

QDataStream &operator>>(QDataStream &stream, TreeItem &item) {
  int childItemCount;
  stream >> item.m_name;
  stream >> item.m_pic;
  stream >> item.m_textColor;
  stream >> item.m_rowColor;
  stream >> item.m_itemLvl;
  stream >> childItemCount;
  Q_ASSERT(childItemCount >= 0 && childItemCount < 100);
  for (auto i = 0; i < childItemCount; ++i) {
    TreeItem *newItem = new TreeItem(&item);
    stream >> *newItem;
    item.m_childItems << newItem;
  }
  return stream;
}

QDataStream &operator<<(QDataStream &stream, const TreeItem &item) {
  stream << item.m_name;
  stream << item.m_pic;
  stream << item.m_textColor;
  stream << item.m_rowColor;
  stream << item.m_itemLvl;
  stream << item.childCount();
  Q_FOREACH (auto *it, item.m_childItems)
    stream << *it;
  return stream;
}
