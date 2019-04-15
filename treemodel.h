#pragma once
#include <QAbstractItemModel>
#include <QVariant>

#include "treeitem.h"

/**
 * @brief The TreeModel class
 * provide a model for single column tree view
 */
class TreeModel : public QAbstractItemModel {
  Q_OBJECT
 public:
  explicit TreeModel(QObject *parent = Q_NULLPTR);
  TreeModel(const TreeModel &other) = default;
  TreeModel &operator=(const TreeModel &other) = default;
  ~TreeModel() Q_DECL_OVERRIDE;

  /**
 * @brief The MoveDirection enum
 * enum class using as a paremeter to move items
 */
  enum class MoveDirection { UP, DOWN };

  /**
   * @brief overriden QAbstractItemModel method
   * @param index
   * @param role
   * @return data stored under given 'role' for item refered to by the 'index'
   */
  QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

  /**
   * @brief overriden QAbstractItemModel method
   * @param row
   * @param column
   * @param parent
   * @return index of item in model specified by given parameters
   */
  QModelIndex index(int row, int column,
                    const QModelIndex &parent) const Q_DECL_OVERRIDE;

  /**
   * @brief overriden QAbstractItemModel method
   * @param child
   * @return parent index of item with index 'child'
   */
  QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;

  /**
   * @brief overriden QAbstractItemModel method
   * @param parent
   * @return number of columns for item of given 'parent' (by default return 1)
overriden QAbstractItemModel method   */
  int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

  /**
   * @brief overriden QAbstractItemModel method
   * @param parent
   * @return number of rows under the given 'parent'
overriden QAbstractItemModel method   */
  int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

  /**
   * @brief overriden QAbstractItemModel method
   * @param row
   * @param count
   * @param parent
   * @return result of inserting rows into model
overriden QAbstractItemModel method   */
  bool insertRows(int row, int count,
                  const QModelIndex &parent) Q_DECL_OVERRIDE;

  /**
   * @brief overriden QAbstractItemModel method
   * @param row
   * @param count
   * @param parent
   * @return result of deleting rows from model
overriden QAbstractItemModel method   */
  bool removeRows(int row, int count,
                  const QModelIndex &parent) Q_DECL_OVERRIDE;

  /**
   * @brief overriden QAbstractItemModel method
   * @param section
   * @param orientation
   * @param value
   * @param role
   * @return result of setting data for the given 'role' and 'section' in the
   * header
   * with the specified 'orientation' to the 'value' supplied
   */
  bool setHeaderData(int section, Qt::Orientation orientation,
                     const QVariant &value, int role) Q_DECL_OVERRIDE;

  /**
   * @brief overriden QAbstractItemModel method
   * @param section
   * @param orientation
   * @param role
   * @return data for the given 'role' and 'section' in the header with the
   * specified 'orientation'
   */
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const Q_DECL_OVERRIDE;

  /**
   * @brief overriden QAbstractItemModel method
   * @return actions supported by the data in this mode
   */
  Qt::DropActions supportedDropActions() const Q_DECL_OVERRIDE;

  /**
   * @brief overriden QAbstractItemModel method
   * @param index
   * @return item flags for given 'index'
   */
  Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

  /**
   * @brief moveItem move 'item' one position in a given 'direction'
   * @param item
   * @param direction
   */
  void moveItem(const QModelIndex &item, MoveDirection direction);

  /**
   * @brief overriden QAbstractItemModel method
   * @param column
   * @param order
   */
  void sort(int column,
            Qt::SortOrder order = Qt::AscendingOrder) Q_DECL_OVERRIDE;

 private:
  TreeItem *m_rootItem;
  typedef bool (*compFunc)(const TreeItem *, const TreeItem *);

  /**
   * @brief sortItems - sort list of TreeItems with 'compFunc'
   * @param itemList
   */
  void sortItems(QList<TreeItem *> &itemList, compFunc);
};
