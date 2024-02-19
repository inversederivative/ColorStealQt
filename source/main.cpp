#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QFileDialog>
#include <QImage>
#include <QMouseEvent>
#include <QMenuBar>
#include <QObject>

class ImageWidget : public QLabel {
Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = nullptr) : QLabel(parent) {}

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (!pixmap().isNull()) {
            QPoint pos = event->pos();
            QRgb pixel = pixmap().toImage().pixel(pos);
            emit pixelClicked(qRed(pixel), qGreen(pixel), qBlue(pixel), qAlpha(pixel));
        }
    }

signals:
    void pixelClicked(int r, int g, int b, int a);
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setupUI();
    }

private slots:
    void openImage() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
        if (!fileName.isEmpty()) {
            QImage image(fileName);
            if (image.isNull()) {
                // Error handling
                return;
            }
            imageWidget->setPixmap(QPixmap::fromImage(image));
        }
    }

    void updatePixelValues(int r, int g, int b, int a) {
        QString text = QString("RGBA: %1, %2, %3, %4").arg(r).arg(g).arg(b).arg(a);
        rLabel->setText(QString("Red: %1").arg(r));
        gLabel->setText(QString("Green: %1").arg(g));
        bLabel->setText(QString("Blue: %1").arg(b));
        aLabel->setText(QString("Alpha: %1").arg(a));
    }

private:
    void setupUI() {
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        QVBoxLayout *verticalLayout = new QVBoxLayout(centralWidget);
        QHBoxLayout *horizontalLayout = new QHBoxLayout();

        imageWidget = new ImageWidget(this);
        connect(imageWidget, &ImageWidget::pixelClicked, this, &MainWindow::updatePixelValues);

        rLabel = new QLabel("Red: ");
        gLabel = new QLabel("Green: ");
        bLabel = new QLabel("Blue: ");
        aLabel = new QLabel("Alpha: ");

        verticalLayout->addLayout(horizontalLayout);
        horizontalLayout->addWidget(imageWidget);

        QVBoxLayout *valueLayout = new QVBoxLayout();
        valueLayout->addWidget(rLabel);
        valueLayout->addWidget(gLabel);
        valueLayout->addWidget(bLabel);
        valueLayout->addWidget(aLabel);
        horizontalLayout->addLayout(valueLayout);

        QMenuBar *menuBar = new QMenuBar(this);
        QMenu *fileMenu = menuBar->addMenu(tr("&File"));
        QAction *openAction = fileMenu->addAction(tr("&Open Image"));
        connect(openAction, &QAction::triggered, this, &MainWindow::openImage);

        setMenuBar(menuBar);
    }

    ImageWidget *imageWidget;
    QLabel *rLabel;
    QLabel *gLabel;
    QLabel *bLabel;
    QLabel *aLabel;
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

#include "main.moc"