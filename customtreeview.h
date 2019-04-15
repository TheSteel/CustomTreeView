#pragma once

#include <QTreeView>
#include <QFont>
#include <QWheelEvent>
#include <QDrag>
#include <QMimeData>

#include "treemodel.h"

/**
 * @brief The CustomTreeView class
 * provide a custom view with drag'n'drop and scaling
 * for tree model
 *
 */
class CustomTreeView : public QTreeView {
 public:
  explicit CustomTreeView(QWidget *parent = Q_NULLPTR);
  CustomTreeView(const CustomTreeView &other) = default;
  CustomTreeView &operator=(const CustomTreeView &other) = default;
  ~CustomTreeView() Q_DECL_OVERRIDE = default;

  /**
 * @brief The ZOOM enum
 * enum class using as a parameter to scaling view
 */
  enum class ZOOM { IN, OUT };

  /**
   * @brief setScaleView
   * set scale unit to 'scale'
   * @param scale
   */
  void setScaleUnit(float scale);

  /**
   * @brief getScalePoint
   * @return scale unit
   */
  float getScaleUnit() const;

 protected:
  /**
   * @brief overriden QAbstractScrollArea method
   */
  void wheelEvent(QWheelEvent *) Q_DECL_OVERRIDE;

  /**
   * @brief overriden QAbstractItemView method
   * @param event
   */
  void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;

  /**
   * @brief overriden QAbstractItemView method
   * @param event
   */
  void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;

  /**
   * @brief overriden QAbstractItemView method
   * @param event
   */
  void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

  /**
   * @brief overriden QAbstractItemView method
   * @param supportedActions
   */
  void startDrag(Qt::DropActions supportedActions) Q_DECL_OVERRIDE;

 private:
  /**
   * @brief scale the view on scale unit by given parameter 'opt'
   * @param opt
   */
  void scaleView(ZOOM opt);
  float m_scalePoint;
};
