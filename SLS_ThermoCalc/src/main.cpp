#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("SLS火箭发动机热力计算系统");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("BUAA");
    
    // 创建主窗口
    MainWindow window;
    window.show();
    
    return app.exec();
}
