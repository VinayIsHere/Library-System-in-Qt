#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QDebug"
#include "QtSql/QSqlDatabase"
#include "QFileInfo"
#include "QPixmap"
#include "QPainter"
#include "QFont"
#include "QGroupBox"
#include "QLabel"
#include "QVBoxLayout"
#include "QLineEdit"
#include "QPushButton"
#include "QDialog"
#include "QDebug"
#include "QMessageBox"
#include "QSqlQuery"
#include "QSqlError"
#include "QDir"
#include "QRegExpValidator"
#include "QComboBox"
#include "QTableView"
#include "QSqlQueryModel"
#include "QCryptographicHash"
#include "QByteArray"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    QSqlDatabase mydb;

    QWidget *library;
    QDialog *newAcc;
    QDialog *bookDetails;
    QDialog *studentDetails;
    QDialog *issueDetails;
    QDialog *gallery;
    QDialog *bookReturn;
    QComboBox *checkDetails;
    QTableView *detailsTable;


    Widget(QWidget *parent = nullptr);
    ~Widget();

    void loginPage();
    void insertIntoDbms();
    void openLibrary(QString);

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public slots:

    //Used in login form
    void checkLogin();

    //Used by Create account dialog
    void createNewAccount();
    void addUser();

    //Slots used in library
    void insertBookDetails();
    void addBookEntry();
    void addStudentEntry();
    void addStudent();
    void issueBook();
    void IssueBtoS();
    void showBookGallery();
    void showDetails(QString);
    void updateDetails();
    void returnBook();

private:
    Ui::Widget *ui;
};


#endif // WIDGET_H
