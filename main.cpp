#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <iostream>

class MyWidget : public QWidget
{
public:
	void paintEvent(QPaintEvent* event)
	{
		QPainter painter(this);
		QTransform transform;
		transform.scale(1, -1);
		painter.setTransform(transform);

		painter.setPen(QPen(Qt::black, 1));
		painter.setFont(QFont("consolas", 24));
		painter.drawText(200, 200, "QTransform");
	}
};

int main(int argc, char** argv)
{
	QRect r(100, 100, 100, 100);
	QTransform transform;
	transform.translate(150, 150);
	transform.scale(2, 2);
	transform.translate(-150, -150);
	QRect r2 = transform.mapRect(r);
	std::cout << r.x() << " " << r.y() << std::endl;
	std::cout << r2.x() << " " << r2.y() << std::endl;

	QApplication app(argc, argv);
	MyWidget widget;
	widget.show();

	return app.exec();
}