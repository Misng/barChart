#ifndef BARCHART_H
#define BARCHART_H

#include <QStringList>
#include <QDebug>

#include <Qwt/qwt_plot.h>
#include <Qwt/qwt_plot_barchart.h>
#include <Qwt/qwt_column_symbol.h>
#include <Qwt/qwt_plot_canvas.h>
#include <Qwt/qwt_legend.h>
#include <Qwt/qwt_scale_draw.h>
#include <Qwt/qwt_plot_layout.h>

class DistroChartItem;

class BarChart : public QwtPlot
{
    Q_OBJECT
public:
    explicit BarChart(QWidget * = NULL);

public slots:
    void setOrienttation(int o);

signals:

private:
    void populate();

    QStringList d_distros;
    DistroChartItem * d_barChartItem;
};

#endif // BARCHART_H
