#include <QApplication>
#include <QMainWindow>
#include <QtGui>

#include "barchart.h"
class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *  = NULL) ;
private:
    BarChart * d_chart;
};

MainWindow::MainWindow(QWidget *panrent) : QMainWindow(panrent)
{
    d_chart = new BarChart(this);
    setCentralWidget(d_chart);//设置windows的中心控件

    QToolBar *toolBar = new QToolBar(this);

    QComboBox * orientationBox = new QComboBox(toolBar);
    orientationBox->addItem("Vertical");
    orientationBox->addItem("Horizontal");
    orientationBox->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );//设置ComboBox的大小方案

    QToolButton * btnButton = new QToolButton(toolBar);
    btnButton->setText("Export");
    btnButton->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    toolBar->addWidget(orientationBox);
    toolBar->addWidget(btnButton);
    addToolBar(toolBar);


//    d_chart->setOrienttation(orientationBox->currentIndex());
//    connect(orientationBox,)
}


int main(int argc,char *argv[])
{
    QApplication a(argc,argv);

    MainWindow mw;
    mw.show();

    return a.exec();
}

