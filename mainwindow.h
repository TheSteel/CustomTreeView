#pragma once

#include <QMainWindow>

#include "treemodel.h"
#include "customtreeview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = Q_NULLPTR);
  ~MainWindow();

 private slots:
  /**
 * @brief on_deleteButton_clicked
 * remove selected item from CustomTreeView
 * if none item is selected do nothing
 */
  void on_deleteButton_clicked();

  /**
   * @brief on_addButton_clicked
   * add item to CustomTreeView at the same level as selected item
   * if none item is selected add item to root
   */
  void on_addButton_clicked();

  /**
   * @brief on_addSubButton_clicked
   * add item to CustomTreeView as a childitem of selected item
   * if none item is selected do nothing
   */
  void on_addSubButton_clicked();

  /**
   * @brief on_collapseButton_clicked
   * collapse all items in CustomTreeView
   */
  void on_collapseButton_clicked();

  /**
   * @brief on_expandButton_clicked
   * expand all items in CustomTreeView
   */
  void on_expandButton_clicked();

  /**
   * @brief on_moveUpButton_clicked
   * move selected item in CustomTreeView up 1 position
   * cant change selected item level and parent
   * if selected item is in first position in child item list
   * of it's parent,do nothing
   */
  void on_moveUpButton_clicked();

  /**
   * @brief on_moveDownButton_clicked
   * move selected item in CustomTreeView down 1 position
   * cant change selected item level and parent
   * if selected item is in last position in child item list
   * of it's parent,do nothing
   */
  void on_moveDownButton_clicked();

 private:
  Ui::MainWindow *ui;
  TreeModel *m_model;
};
