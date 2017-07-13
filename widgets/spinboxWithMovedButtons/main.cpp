#include <QtWidgets>

class DoubleSpinStyle : public QProxyStyle
{
public:
    DoubleSpinStyle(QStyle *baseStyle = 0) : QProxyStyle(baseStyle) {}
    QRect subControlRect(ComplexControl control, const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget = 0) const
    {
        QRect r = QProxyStyle::subControlRect(control, option, subControl, widget);
        if (control == CC_SpinBox && (subControl == QStyle::SC_SpinBoxDown || subControl == QStyle::SC_SpinBoxUp)) {
            // Adjust the buttons to appear where over the lineedit
            r.moveTop(r.top() - 3);
            r.moveLeft(r.left() - 15);
        } else if (control == CC_SpinBox && subControl == QStyle::SC_SpinBoxEditField) {
            // Adjust the size of the lineedit to fill the space
            r.setWidth(r.width() + 15);
        }
        return r;
    }
    SubControl hitTestComplexControl(ComplexControl control, const QStyleOptionComplex *option, const QPoint &position, const QWidget *widget = 0) const
    {
        // Need to cover the fact we have changed the positions of the arrow buttons
        if (control == CC_SpinBox) {
            QRect r = subControlRect(control, option, SC_SpinBoxDown, widget);
            if (r.contains(position))
                return SC_SpinBoxDown;
            r = subControlRect(control, option, SC_SpinBoxUp, widget);
            if (r.contains(position))
                return SC_SpinBoxUp;
        }
        return QProxyStyle::hitTestComplexControl(control, option, position, widget);
    }
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = 0) const
    {
        // As we draw on top of the lineedit, we need to make sure the buttons are only drawn once
        QStyleOptionComplex *opt = (QStyleOptionComplex *)option;
        if (control == CC_SpinBox) {
            if (qobject_cast<const QDoubleSpinBox *>(widget)) {
                opt->subControls = QStyle::SC_SpinBoxFrame | QStyle::SC_SpinBoxEditField;
                QProxyStyle::drawComplexControl(control, opt, painter, widget);
            } else {
                opt->subControls = QStyle::SC_SpinBoxUp | QStyle::SC_SpinBoxDown;
                QProxyStyle::drawComplexControl(control, opt, painter, widget);
            }
            return;
        }
        QProxyStyle::drawComplexControl(control, opt, painter, widget);
    }
};

class MyLineEdit : public QLineEdit
{
public:
    MyLineEdit(QWidget *parent = 0) : QLineEdit(parent) {}
protected:
    void paintEvent(QPaintEvent *e);
};

class MyDoubleSpin : public QDoubleSpinBox
{
    Q_OBJECT
public:
    MyDoubleSpin(QWidget *parent = 0) : QDoubleSpinBox(parent)
    {
        setLineEdit(new MyLineEdit(this));
        lineEdit()->installEventFilter(this);
    }
    void initStyleOption(QStyleOptionSpinBox *option) { QDoubleSpinBox::initStyleOption(option); }
protected:
    bool eventFilter(QObject *object, QEvent *ev)
    {
        // Since the buttons are now over the lineedit, we need to intercept the events meant for them
        // otherwise they go to the lineedit first and the up/down do not have an effect
        if (object == lineEdit()) {
            if (ev->type() == QEvent::MouseButtonPress || ev->type() == QEvent::MouseButtonRelease ||
                ev->type() == QEvent::MouseMove || ev->type() == QEvent::MouseButtonDblClick) {
                QStyleOptionSpinBox option;
                initStyleOption(&option);
                QMouseEvent *me = (QMouseEvent *)ev;
                QStyle::SubControl sc = style()->hitTestComplexControl(QStyle::CC_SpinBox, &option, me->pos());
                if (sc == QStyle::SC_SpinBoxDown || sc == QStyle::SC_SpinBoxUp) {
                    event(me);
                    return true;
                }
                return false;
            }
        }
        return QDoubleSpinBox::eventFilter(object, ev);
    }
};

// To draw the buttons on top, we have to paint them directly, so we use the code for
// QAbstractSpinBox to do this with the help of our style
void MyLineEdit::paintEvent(QPaintEvent *e)
{
    QLineEdit::paintEvent(e);
    MyDoubleSpin *ds = qobject_cast<MyDoubleSpin *>(parentWidget());
    QStyleOptionSpinBox opt;
    ds->initStyleOption(&opt);
    QStylePainter p(this);
    p.drawComplexControl(QStyle::CC_SpinBox, opt);
}

#include "main.moc"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    a.setStyle(new DoubleSpinStyle(a.style()));
    QWidget w;
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(new MyDoubleSpin);
    w.setLayout(vbox);
    w.show();
    return a.exec();
}
