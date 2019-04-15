#include "customtreeview.h"
#include <QDebug>

CustomTreeView::CustomTreeView(QWidget *parent)
    : QTreeView(parent), m_scalePoint(1.2) {
  this->setAcceptDrops(true);
}

void CustomTreeView::setScaleUnit(float scale) { m_scalePoint = scale; }
float CustomTreeView::getScaleUnit() const { return m_scalePoint; }

void CustomTreeView::wheelEvent(QWheelEvent *event) {
  if (event->modifiers() == Qt::ControlModifier)
    scaleView(event->delta() < 0 ? ZOOM::IN : ZOOM::OUT);
}

void CustomTreeView::scaleView(ZOOM opt) {
  QFont font = this->font();
  if (opt == ZOOM::OUT) {
    if (this->iconSize().width() * m_scalePoint > 500 ||
        this->iconSize().height() * m_scalePoint > 500 ||
        this->font().pointSizeF() * m_scalePoint > 500)
      return;
    this->setIconSize(QSize(this->iconSize().width() * m_scalePoint,
                            this->iconSize().height() * m_scalePoint));
    font.setPointSizeF(this->font().pointSizeF() * m_scalePoint);
    this->setFont(font);
    return;
  } else {
    float p;
    int w, h;
    w = this->iconSize().width() / m_scalePoint;
    h = this->iconSize().height() / m_scalePoint;
    p = this->font().pointSize() / m_scalePoint;
    if (h < 5 || w < 5 || p < 5) return;
    this->setIconSize(QSize(w, h));
    font.setPointSizeF(p);
    this->setFont(font);
  }
}

void CustomTreeView::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasFormat("test"))
    event->accept();
  else
    event->ignore();
}

void CustomTreeView::dragMoveEvent(QDragMoveEvent *event) {
  TreeItem *it =
      static_cast<TreeItem *>(indexAt(event->pos()).internalPointer());

  if (it && event->mimeData()->property("lvl").toInt() > it->getItemLvl())
    event->accept();
  else
    event->ignore();
}

void CustomTreeView::dropEvent(QDropEvent *event) {
  if (event->mimeData()->hasFormat("test")) {
    QByteArray itemData = event->mimeData()->data("test");
    QDataStream stream(&itemData, QIODevice::ReadOnly);
    int row = model()->rowCount(indexAt(event->pos()));
    model()->insertRow(row, indexAt(event->pos()));
    expand(indexAt(event->pos()));
    QModelIndex newIndex = model()->index(row, 0, indexAt(event->pos()));
    TreeItem *item = static_cast<TreeItem *>(newIndex.internalPointer());
    Q_ASSERT(item);
    stream >> *item;
    model()->removeRow(currentIndex().row(), currentIndex().parent());
    event->setDropAction(Qt::MoveAction);
    event->accept();
  } else
    event->ignore();
}

void CustomTreeView::startDrag(Qt::DropActions supportedActions) {
  Q_UNUSED(supportedActions);
  if (!currentIndex().isValid()) return;
  QMimeData *mimeData = new QMimeData;
  TreeItem *item = static_cast<TreeItem *>(currentIndex().internalPointer());
  Q_ASSERT(item);
  QByteArray itemData;
  QDataStream stream(&itemData, QIODevice::WriteOnly);
  stream << *item;
  mimeData->setData("test", itemData);
  mimeData->setProperty("lvl", item->getItemLvl());
  QDrag *drag = new QDrag(this);
  drag->setMimeData(mimeData);
  drag->exec(Qt::MoveAction);
}
