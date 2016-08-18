#include "barchart.h"

class DistroScaleDraw: public QwtScaleDraw
{
public:
    DistroScaleDraw(Qt::Orientation orientation,const QStringList &labels) :d_labels(labels)
    {
        setTickLength(QwtScaleDiv::MinorTick,0);
        setTickLength(QwtScaleDiv::MediumTick,0);
        setTickLength(QwtScaleDiv::MajorTick,2); //设置各个间隔标签的长度

        enableComponent(QwtScaleDraw::Backbone,false);//component 组件

        if (orientation == Qt::Vertical) {
            setLabelRotation(-60.0);
        } else {
            setLabelRotation(-20.0);
        }

        setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);//设置标签的排列方式
    }

    virtual QwtText label(double v) const
    {
        QwtText lab;

        const int index = qRound(v);
        if(index >= 0 && index < d_labels.size()){
            lab = d_labels[index];
        }
//        qDebug() << lab.text();
        return lab;//坐标轴上显示的刻度标签就是返回的ｌａｂ设定的
    }

private:
    const QStringList d_labels;
};

class DistroChartItem:public QwtPlotBarChart
        /**
         *我猜，画柱形图是只需要给出一个值，就是柱形图的高度，
         *然后QwtPlotBarChart就会以xBottom的一个刻度值为柱形的中心点来画图，
         *接着自己，实现一些虚函数，设置一下颜色，风格，标题之类的属性就OK了**/


        //QwtPlotBarChart 源码的注释
        /**
         *Each bar might be customized individually by implementing a specialSymbol(). Otherwise it is rendered using a default symbol.
         * Depending on its orientation() the bars are displayed horizontally or vertically.
        */
{
public:
    DistroChartItem():QwtPlotBarChart("Page Hits")
    {
        setLegendMode(QwtPlotBarChart::LegendBarTitles);//显示图表的标题  一个显示默认的标签，一个显示的是个人设定的标签
        setLegendIconSize(QSize(10,14));
        setLayoutPolicy(AutoAdjustSamples);// 设置柱形图的排列风格

        setLayoutHint(10);//提示engine如何计算bar的宽度
        setSpacing(10);
        //The spacing is the distance between 2 samples ( bars for QwtPlotBarChart or a group of bars for QwtPlotMultiBarChart ) in paint device coordinates.
        //设置两个bar之间的距离在坐标轴上
    }

    void addDistro(const QString & distro,const QColor & color)
    {
        d_colors += color;
        d_distors += distro;
        itemChanged();
    }

    virtual QwtColumnSymbol * specialSymbol(int sampleIndex, const QPointF &) const//这个函数会在画图的过程中被多次调用 调用24次,而且当图表发生变化时，该函数会被调用
    {
        QwtColumnSymbol * symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);//NoStyle 就是什么也不做，默认属性，所以使用是要设置属性
        symbol->setLineWidth(2);//画symbol时使用的线宽
        symbol->setFrameStyle(QwtColumnSymbol::Raised);

        QColor c(Qt::white);
        if( sampleIndex >=0 && sampleIndex < d_colors.size() )
            c = d_colors[sampleIndex];
        symbol->setPalette(c);

//        qDebug() << c.name()<<"-------------";
        static int i = 0;
        i++;
        qDebug() << i << "----------";
        return symbol;
    }

    virtual QwtText barTitle(int sampleIndex) const//同上，不知道这个函数为什么调用了16次
    {
        QwtText title;
        if ( sampleIndex >= 0 && sampleIndex < d_distors.size() )
            title = d_distors[sampleIndex];
//        qDebug() << title.text()<<"++++++++++++";
        static int i = 0;
        i++;
        qDebug() << i<<"++++++++++";
        return title;
    }

private:
    QList<QColor> d_colors;
    QList<QString> d_distors;
};


BarChart::BarChart(QWidget *parent) :
    QwtPlot(parent)
{
    //-----------数据
    const struct
    {
        const char * distro;
        const int hits;
        QColor color;
    } pageHint[] = {
        { "Arch", 1114, QColor( "DodgerBlue" ) },
        { "Debian", 1373, QColor( "#d70751" ) },
        { "Fedora", 1638, QColor( "SteelBlue" ) },
        { "Mageia", 1395, QColor( "Indigo" ) },
        { "Mint", 3874, QColor( 183, 255, 183 ) },
        { "openSuSE", 1532, QColor( 115, 186, 37 ) },
        { "Puppy", 1059, QColor( "LightSkyBlue" ) },
        { "Ubuntu", 2391, QColor( "FireBrick" ) }
    };//设置颜色时，如果设置的颜色格式非法，则最终出来的颜色为黑色  目标数据

    setAutoFillBackground(true);
    setPalette(QColor("Linen"));//使用调色板

    //----------调色板
    QwtPlotCanvas * canvas = new QwtPlotCanvas();//画布
//    canvas->setLineWidth(2);
    canvas->setFrameStyle(QFrame::WinPanel | QFrame::StyledPanel);
    canvas->setBorderRadius(10);  //设置画布边角的圆角半径为10 这个挺好玩的

    QPalette canvasPaltte(QColor("Plum"));//使用调色板的好处是，可以动态的调整控件的颜色，单纯的使用颜色配置就会导致硬编码的存在。
    canvasPaltte.setColor(QPalette::Foreground,QColor("Indigo"));
    canvas->setPalette(canvasPaltte);
    setCanvas(canvas);
    setTitle("1*2*3*4*5*6*7*8*9*");

    //----------显示图表
    d_barChartItem = new DistroChartItem();

    QVector<double> samples;

    for (int i = 0; i < (int)(sizeof(pageHint) / sizeof(pageHint[ 0 ])) ; ++i) {//循环，将数据从结构体取出
        d_distros += pageHint[i].distro;
        samples += pageHint[i].hits;

        d_barChartItem->addDistro(pageHint[i].distro,pageHint[i].color);
    }
    d_barChartItem->setSamples(samples);//最终数据的显示方式
    d_barChartItem->attach(this);

    insertLegend(new QwtLegend());//图例

    setOrienttation(0);//设置图表的垂直方向以及设置坐标轴的一些属性
    setAutoReplot(false);
}

void BarChart::setOrienttation(int o)
{
    const Qt::Orientation orienation = (o == 0) ? Qt::Vertical : Qt::Horizontal;

    int xBottom = QwtPlot::xBottom;
    int yLeft = QwtPlot::yLeft;

    d_barChartItem->setOrientation(orienation);

    setAxisTitle(xBottom,"Distor");
    setAxisMaxMinor(xBottom,3);//暂时没有看到效果,效果为：可以控制小间隔的出现
    setAxisScaleDraw(xBottom,new DistroScaleDraw(orienation,d_distros));

    setAxisTitle(yLeft,"Hits per day (HPD)");
    setAxisMaxMinor(yLeft,3);//设置小间隔刻度是否出现，但不影响中间刻度
    QwtScaleDraw * scaleDraw = new QwtScaleDraw();
    scaleDraw->setTickLength(QwtScaleDiv::MediumTick,4);//设置中间刻度（MediumTick）的长度为4
    scaleDraw->setTickLength(QwtScaleDiv::MajorTick,8);//设置大刻度（MajorTick）的长度为8
    setAxisScaleDraw(QwtPlot::yLeft,scaleDraw);

    plotLayout()->setCanvasMargin(-1);//通过plot的布局来调用函数设置柱形图与坐标轴之间的距离
    replot();

}




















