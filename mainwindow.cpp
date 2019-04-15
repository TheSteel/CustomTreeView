#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  m_model = new TreeModel(this);
  m_model->setHeaderData(0, Qt::Horizontal, "", Qt::DisplayRole);
  ui->treeView->setModel(m_model);
  ui->treeView->setIconSize(QSize(30, 30));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_addButton_clicked() {
  QModelIndex index = ui->treeView->selectionModel()->currentIndex();
  if (!index.isValid() && index != ui->treeView->rootIndex()) return;
  int row = m_model->rowCount(index.parent());
  if (m_model->insertRow(row, index.parent())) {
    QModelIndex child = m_model->index(row, 0, index.parent());
    TreeItem *item = static_cast<TreeItem *>(child.internalPointer());
    item->setName("123");
    item->setIcon(QIcon(":/img/exit.png"));
    item->setTextColor(QColor(Qt::green));
  }
  ui->treeView->setCurrentIndex(ui->treeView->rootIndex());
}

void MainWindow::on_addSubButton_clicked() {
  QModelIndex index = ui->treeView->selectionModel()->currentIndex();
  if (!index.isValid()) return;
  int row = m_model->rowCount(index);
  if (m_model->insertRow(row, index)) {
    QModelIndex child = m_model->index(row, 0, index);
    TreeItem *item = static_cast<TreeItem *>(child.internalPointer());
    item->setName("222");
    item->setIcon(QIcon(":/img/exit.png"));
    item->setTextColor(QColor(Qt::magenta));
    ui->treeView->expand(index);
  }
  ui->treeView->setCurrentIndex(ui->treeView->rootIndex());
}

void MainWindow::on_deleteButton_clicked() {
  QModelIndex index = ui->treeView->selectionModel()->currentIndex();
  if (!index.isValid()) return;
  m_model->removeRow(index.row(), index.parent());
  ui->treeView->setCurrentIndex(ui->treeView->rootIndex());
}

void MainWindow::on_expandButton_clicked() { ui->treeView->expandAll(); }

void MainWindow::on_collapseButton_clicked() { ui->treeView->collapseAll(); }

void MainWindow::on_moveUpButton_clicked() {
  if (!ui->treeView->selectionModel()->currentIndex().isValid()) return;
  m_model->moveItem(ui->treeView->selectionModel()->currentIndex(),
                    TreeModel::MoveDirection::UP);
  ui->treeView->setCurrentIndex(ui->treeView->rootIndex());
}

void MainWindow::on_moveDownButton_clicked() {
  if (!ui->treeView->selectionModel()->currentIndex().isValid()) return;
  m_model->moveItem(ui->treeView->selectionModel()->currentIndex(),
                    TreeModel::MoveDirection::DOWN);
  ui->treeView->setCurrentIndex(ui->treeView->rootIndex());
}
