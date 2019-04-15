#pragma once
#include <QString>
#include <QList>
#include <QIcon>
#include <QColor>
#include <QMetaType>

/**
 * @brief The TreeItem class
 * provide data for single column tree model
 */
class TreeItem {
 public:
  explicit TreeItem(TreeItem *parent = Q_NULLPTR);
  explicit TreeItem(const TreeItem &other);
  TreeItem &operator=(const TreeItem &other);
  ~TreeItem();

  /**
   * @brief insertChild
   * insert child item 'child' into list of child items
   * @param child
   */
  void insertChild(TreeItem *child);

  /**
   * @brief childCount
   * @return number of child items in the child item list
   */
  int childCount() const;

  /**
   * @brief removeChild
   * @param pos
   * remove child item at 'pos' position from the child item list
   */
  void removeChild(int pos);

  /**
   * @brief swapChilds
   * swap child item at 'target' position with child item at 'dest' position in
   * child item list
   * @param target
   * @param dest
   */
  void swapChilds(int target, int dest);

  /**
   * @brief getParentItem
   * @return pointer to parent item
   */
  TreeItem *getParentItem() const;

  /**
   * @brief getChildItem
   * @param pos
   * @return pointer to child item at 'pos' position in child item list
   */
  TreeItem *getChildItem(int pos) const;

  /**
   * @brief getChildPos
   * @param child
   * @return position of child item 'child' in child item list
   */
  int getChildPos(TreeItem *child) const;

  /**
   * @brief getItemName
   * @return item name
   */
  QString getItemName() const;

  /**
   * @brief getIcon
   * @return item icon
   */
  QIcon getIcon() const;

  /**
   * @brief getBackColor
   * @return item background color
   */
  QColor getBackColor() const;

  /**
   * @brief getTextColor
   * @return item text color
   */
  QColor getTextColor() const;

  /**
   * @brief getSelfRow
   * @return item position in parent child item list
   */
  int getSelfRow() const;

  /**
   * @brief getChildList
   * @return item's child item list
   */
  QList<TreeItem *> &getChildList();

  /**
   * @brief getChildListRead
   * @return item's child item list for read-only
   */
  const QList<TreeItem *> &getChildListRead() const;

  /**
   * @brief getItemLvl
   * @details by default root item level = 0
   * @return item level, wich is equal to item's depth in a model
   */
  int getItemLvl() const;

  /**
   * @brief deepCopyItem
   * @param child
   * @param parent
   * @return a 'parent' specified new item wich is a deep copy of 'child'
   */
  TreeItem *deepCopyItem(const TreeItem &child, TreeItem *const parent);

  /**
   * @brief setBackColor
   * set item's background color to 'color'
   * @param color
   */
  void setBackColor(const QColor &color);

  /**
   * @brief setTextColor
   * set item's text color to 'color'
   * @param color
   */
  void setTextColor(const QColor &color);

  /**
   * @brief setIcon
   * set item's icon to 'icon'
   * @param icon
   */
  void setIcon(const QIcon &icon);

  /**
   * @brief setName
   * @param name
   * set item's name to 'name'
   */
  void setName(const QString &name);

  /**
   * @brief setParent
   * set item's parent to 'parent'
   * @param parent
   */
  void setParent(TreeItem *parent);

  /**
   * @brief itemCompareAsc
   * @param left
   * @param right
   * @return result of compare two items by name in ascending order
   */
  static bool itemCompareAsc(const TreeItem *left, const TreeItem *right);

  /**
   * @brief itemCompareDes
   * @param left
   * @param right
   * @return result of compare two items by name in descending order
   */
  static bool itemCompareDes(const TreeItem *left, const TreeItem *right);

  friend QDataStream &operator<<(QDataStream &stream, const TreeItem &item);
  friend QDataStream &operator>>(QDataStream &stream, TreeItem &item);

 private:
  /**
   * @brief setItemLvl
   * set item's level to 'lvl'
   * @param lvl
   */
  void setItemLvl(int lvl);
  QList<TreeItem *> m_childItems;
  TreeItem *m_parentItem;
  QString m_name;
  QIcon m_pic;
  QColor m_rowColor;
  QColor m_textColor;
  int m_itemLvl;
};
Q_DECLARE_METATYPE(TreeItem)
