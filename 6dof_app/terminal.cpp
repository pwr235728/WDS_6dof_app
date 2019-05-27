#include "terminal.h"
#include <QScrollBar>


Terminal::Terminal(QWidget *parent)
    : QPlainTextEdit (parent)
{
    document()->setMaximumBlockCount(100);
    QPalette p = QPalette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);
}

void Terminal::putData(const QByteArray &data)
{
    insertPlainText(data);

    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void Terminal::setLocalEchoEnabled(bool set)
{
    localEchoEnabled = set;
}

void Terminal::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Backspace:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        break;
    default:
        if (localEchoEnabled)
            QPlainTextEdit::keyPressEvent(e);
        emit getData(e->text().toLocal8Bit());
    }
}

void Terminal::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void Terminal::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void Terminal::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}
