#include "widget.h"
#include "ui_widget.h"

QCryptographicHash *encrypt;
QGroupBox *loginForm;
QString newUsername,newPassword;
QLineEdit *usernameInput,*passwordInput;
QLineEdit *newPasswordBox,*newPasswordConfirmationBox,*newUsernameBox;
QLineEdit *bookNameBox,*authorNameBox,*bookPriceBox,*stockNoBox,*studentNameBox,*studentRollNoBox,*branchBox,*stu_bname,*book_sname,*rollNo,*ret_bname,*ret_sroll;
QGroupBox *grp4;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    showMaximized();

    encrypt= new QCryptographicHash(QCryptographicHash::Sha256);

    mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("password.db");

    qDebug()<<QDir::currentPath();  //I have turned of the shalow build, so make sure it shows you the file directory where the project has stored.

    if(!mydb.open())
    {
        qDebug()<<"Failed to open Database in constructor";
    }

    QSqlQuery createTabels(mydb);

    createTabels.prepare("CREATE TABLE IF NOT EXISTS users (EMAIL_ID TEXT PRIMARY KEY, Password TEXT)");

    if(!createTabels.exec())
    {
        qDebug()<<"Failed to created users table";
    }

    createTabels.prepare("CREATE TABLE IF NOT EXISTS books(book_id	INTEGER PRIMARY KEY AUTOINCREMENT,book_name	TEXT,author_id	INTEGER,stock INTEGER,availability	INTEGER)");

    if(!createTabels.exec())
    {
        qDebug()<<"Failed to created books table";
    }

    createTabels.prepare("CREATE TABLE IF NOT EXISTS authors(author_id INTEGER PRIMARY KEY AUTOINCREMENT,author_name TEXT UNIQUE)");

    if(!createTabels.exec())
    {
        qDebug()<<"Failed to created authors table";
    }

    createTabels.prepare("CREATE TABLE IF NOT EXISTS students(stu_id INTEGER PRIMARY KEY AUTOINCREMENT,stu_name TEXT, stu_rollNo INTEGER UNIQUE, stu_branch TEXT, books_taken INT)");

    if(!createTabels.exec())
    {
        qDebug()<<"Failed to created students table";
    }

    createTabels.prepare("CREATE TABLE IF NOT EXISTS issueBook(issue_id	INTEGER PRIMARY KEY AUTOINCREMENT,stu_id	INTEGER,book_id	INTEGER)");

    if(!createTabels.exec())
    {
        qDebug()<<"Failed to created issueBook table";
    }
    loginPage();


}

void Widget::loginPage()
{
//    QRegularExpression rx("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",
//                          QRegularExpression::CaseInsensitiveOption);


    QFont font,font1,font2;
    font.setPointSize(30);
    font.setBold(true);

    font1.setPointSize(15);
    font1.setBold(true);

    font2.setPointSize(15);
    font2.setBold(false);

    loginForm=new QGroupBox(this);
    loginForm->setTitle("LOGIN PAGE");
    loginForm->setFont(font);
    loginForm->setGeometry(550,20,600,350);

    QLabel *username=new QLabel(loginForm);
    username->setText("USERNAME/EMAIL ID");
    username->setStyleSheet("font:bold; background-color:white");
    username->setFont(font1);
    username->move(20,100);
    username->show();

    QRegExpValidator* usernameValidator = new QRegExpValidator(QRegExp("^[a-z | A-Z].*\\d.*$"), this);

    usernameInput=new QLineEdit(loginForm);
    usernameInput->setValidator(usernameValidator);
    //usernameInput->setValidator(new QRegularExpressionValidator(rx,loginForm));
    usernameInput->setGeometry(250,100,150,25);
    usernameInput->setFont(font2);
    usernameInput->show();

    QLabel *password=new QLabel(loginForm);
    password->setText("Password");
    password->setStyleSheet("font:bold; background-color:white");
    password->setFont(font1);
    password->move(20,150);
    password->show();

    passwordInput=new QLineEdit(loginForm);
    passwordInput->setGeometry(250,150,150,25);
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setFont(font2);
    passwordInput->show();

    QPushButton *submit=new QPushButton(loginForm);
    submit->setText("Submit");
    submit->setFont(font1);
    submit->setGeometry(150,200,100,60);
    submit->show();

    QPushButton *registration=new QPushButton(loginForm);
    registration->setText("New User?");
    registration->setFont(font1);
    registration->setGeometry(150,280,100,25);

    connect(registration,SIGNAL(clicked()),this,SLOT(createNewAccount()));
    connect(submit,SIGNAL(clicked()),this,SLOT(checkLogin()));
    loginForm->show();

}

void Widget::checkLogin()
{
       QString uname,pwd;
       uname=usernameInput->text();
       pwd=passwordInput->text();

       if(!mydb.open())
       {
           qDebug()<<"Failed to open databse in checkLogin function";
       }
       else
       {
           QByteArray hashpwd,encryptpwd;
           hashpwd.append(pwd);

           encrypt->reset();
           encrypt->addData(hashpwd);

           encryptpwd=encrypt->result();

           qDebug()<<mydb.tables();
            QSqlQuery qry(mydb);

            if(qry.exec("SELECT * FROM users where EMAIL_ID='"+uname+"' and Password='"+encryptpwd.toHex()+"'"))
            {

              int count=0;

              while(qry.next())
              {
                  qDebug()<<qry.value("EMAIL_ID");
                  count++;
                  qDebug()<<count;
              }
              if(count!=0)
              {
                  usernameInput->setText("");
                  passwordInput->setText("");

                  openLibrary(uname);
              }
              else
              {
                  QMessageBox::information(this,"Wrong credentials"," Email Id or Password is incorrect !");
              }
            }
            else
            {
                qDebug()<<qry.executedQuery();
                qDebug()<<qry.lastError().text();
                QMessageBox::warning(this,"Not Found","User Not Found");
            }
       }

}

void Widget::openLibrary(QString uname)
{
    library=new QWidget();
    //library->setStyleSheet("background-image: url(:/new/prefix1/library.jpg);");
    library->setStyleSheet("background-color: rgb(0, 255, 178);");
    library->showMaximized();
    library->show();

    QFont headingFont,font1;

    headingFont.setPointSize(30);
    headingFont.setBold(true);

    font1.setPointSize(15);
    font1.setBold(true);

    QLabel *heading=new QLabel(library);
    heading->setText("WELCOME "+uname);
    heading->setFont(headingFont);
    heading->move(library->width()/2,10);
    heading->show();


    QPushButton *insertDetails=new QPushButton(library);
    insertDetails->setGeometry(100,100,300,200);
    insertDetails->setFont(font1);
    insertDetails->setText("ADD BOOK\n");
    insertDetails->show();

    QPushButton *addStudentDetails=new QPushButton(library);
    addStudentDetails->setGeometry(600,100,300,200);
    addStudentDetails->setFont(font1);
    addStudentDetails->setText("ADD STUDENT");
    addStudentDetails->show();

    QPushButton *iss=new QPushButton(library);
    iss->setGeometry(1100,100,300,200);
    iss->setFont(font1);
    iss->setText("ISSUE BOOK");
    iss->show();

    QPushButton *bookGallery=new QPushButton(library);
    bookGallery->setGeometry(100,400,300,200);
    bookGallery->setFont(font1);
    bookGallery->setText("BOOK GALLERY");
    bookGallery->show();

    QPushButton *update=new QPushButton(library);
    update->setGeometry(600,400,300,200);
    update->setFont(font1);
    update->setText("UPDATE BOOK\nRETURN DETAILS");
    update->show();

    connect(insertDetails,SIGNAL(clicked()),this,SLOT(insertBookDetails()));
    connect(addStudentDetails,SIGNAL(clicked()),this,SLOT(addStudent()));
    connect(iss,SIGNAL(clicked()),this,SLOT(issueBook()));
    connect(bookGallery,SIGNAL(clicked()),this,SLOT(showBookGallery()));
    connect(update,SIGNAL(clicked()),this,SLOT(updateDetails()));
}

void Widget::updateDetails()
{
    bookReturn=new QDialog();
    bookReturn->setWindowTitle("Return Book");
    bookReturn->show();

    bookReturn->resize(600,600);

    QFont font1,font2;
    font1.setBold(true);
    font1.setPointSize(12);
    font2.setPointSize(12);

    QGroupBox *grp5=new QGroupBox(bookReturn);
    grp5->setGeometry(50,50,500,400);
    grp5->show();

    QLabel *return_book_name=new QLabel(grp5);
    return_book_name->setText("Returned\nBook Name");
    return_book_name->setFont(font1);
    return_book_name->move(50,50);
    return_book_name->show();

    ret_bname=new QLineEdit(grp5);
    ret_bname->setFont(font2);
    ret_bname->setGeometry(220,50,200,30);
    ret_bname->show();

    QRegExpValidator* rxv = new QRegExpValidator(QRegExp("\\d*"), this); // only pos

    QLabel *reutrn_stu_roll=new QLabel(grp5);
    reutrn_stu_roll->setText("Returned by\n(Roll no of student)");
    reutrn_stu_roll->setFont(font1);
    reutrn_stu_roll->move(50,100);
    reutrn_stu_roll->show();

    ret_sroll=new QLineEdit(grp5);
    ret_sroll->setValidator(rxv);
    ret_sroll->setFont(font2);
    ret_sroll->setGeometry(220,100,200,30);
    ret_sroll->show();

    QPushButton *returnB= new QPushButton(grp5);
    returnB->setFont(font1);
    returnB->setGeometry(150,150,120,45);
    returnB->setText("Return Book");
    returnB->show();

    connect(returnB, SIGNAL(clicked()), this,SLOT(returnBook()));
}

void Widget::returnBook()
{
    QString book_name,stu_rollNumber;
    book_name=ret_bname->text();
    stu_rollNumber=ret_sroll->text();

    if(book_name=="" or stu_rollNumber=="")
    {
        QMessageBox::warning(bookReturn,"Details Missing"," Make Sure You Have Entered All The Details !");
    }
    else
    {
        int ret_stu_rollNo=stu_rollNumber.toInt();
        int ret_book_id,ret_stock,ret_avai,ret_stu_id,b_taken;

        if(!mydb.open())
        {
            qDebug()<<mydb.lastError().text();
            qDebug()<<"Failed to open database in returnBook()";
        }
        else
        {
            int success=true;
            QSqlQuery returnB(mydb);

            if(!returnB.exec("SELECT stu_id,books_taken FROM students WHERE stu_rollNo='"+QString::number(ret_stu_rollNo)+"'"))
            {
                success=false;
                QMessageBox::warning(bookReturn,"Invalid Request","Roll No not exist or some error has been occured, Please verify");
                qDebug()<<"Roll No not exist or some error has been occured, Please verify";
                qDebug()<<"Failed to fetch student id";
            }
            else
            {
                returnB.first();
                ret_stu_id=returnB.value(0).toInt();
                b_taken=returnB.value(1).toInt();

                if(ret_stu_id==0)
                {
                    success=false;
                    QMessageBox::warning(bookReturn,"Invalid Request","Roll No not exist or some error has been occured, Please verify");
                }

                qDebug()<<"ret_stu_id: "<<ret_stu_id<<",b_taken: "<<b_taken;
            }

            if(!returnB.exec("SELECT book_id,stock,availability FROM books WHERE 	book_name='"+book_name+"'"))
            {
                success=false;
                QMessageBox::warning(bookReturn,"Invalid Request","Book Name not exist or some error has been occured, Please verify");
                qDebug()<<"Book Name not exist or some error has been occured, Please verify";
                qDebug()<<"Failed to fetch Book name";
            }
            else
            {
                returnB.first();
                ret_book_id=returnB.value(0).toInt();
                ret_stock=returnB.value(1).toInt();
                ret_avai=returnB.value(2).toInt();

                if(ret_book_id==0)
                {
                    success=false;
                    QMessageBox::warning(bookReturn,"Invalid Request","Book Name not exist or some error has been occured, Please verify");
                }
                qDebug()<<"ret_book_id: "<<ret_book_id<<",ret_stock: "<<ret_stock<<"ret_avai: "<<ret_avai;
            }

            int issue_id;

            if(!returnB.exec("SELECT issue_id FROM issueBook WHERE stu_id='"+QString::number(ret_stu_id)+"' AND book_id='"+QString::number(ret_book_id)+"'"))
            {
                success=false;
                qDebug()<<"Invalid Request"<<",Wrong Student ID or Book ID";
                qDebug()<<"Failed to fetch issue id";
            }
            else
            {
                returnB.first();
                issue_id=returnB.value(0).toInt();

                if(issue_id==0)
                {
                    success=false;
                    QMessageBox::warning(bookReturn,"Invalid Request","Wrong Student ID or Book ID");
                }
            }

            if(success)
            {

                if(returnB.exec("DELETE FROM issueBook WHERE stu_id='"+QString::number(issue_id)+"' AND book_id='"+QString::number(ret_book_id)+"'" ))
                {

                    ret_stock+=1;
                    b_taken-=1;

                    if(returnB.exec("UPDATE books SET stock='"+QString::number(ret_stock)+"' WHERE book_id='"+QString::number(ret_book_id)+"'"))
                    {
                        qDebug()<<"Successfully updated stock";
                    }
                    else
                    {
                        success=false;
                        qDebug()<<"Invalid Request Failed to set stock value";
                        qDebug()<<"Failed to update stock id";
                    }

                    if(returnB.exec("UPDATE students SET books_taken='"+QString::number(b_taken)+"' where stu_id='"+QString::number(ret_stu_id)+"'"))
                    {
                        qDebug()<<"Successfully updated books_taken attribute";
                    }
                    else
                    {
                        success=false;
                        qDebug()<<"Invalid Request Failed to set stock value";
                        qDebug()<<"Failed to update stock id";
                    }
                }
                else
                {
                    success=false;
                    QMessageBox::warning(bookReturn,"Error Occured","Issue Data Not Removed not Removed");
                    qDebug()<<"Invalid Request"<<"Issue Data Not Removed not Removed";
                    qDebug()<<"Failed to fetch issue id";
                }

            }

            if(success)
            {
                   QMessageBox::information(bookReturn,"Successfully Returned","Book Returned Successfully");
            }
        }

    }
}

void Widget::showBookGallery()
{
    gallery=new QDialog();
    gallery->setWindowTitle("GALLERY");
    gallery->show();

    QFont font1;
    font1.setBold(true);
    font1.setPointSize(15);

    grp4=new QGroupBox(gallery);
    grp4->setGeometry(50,50,700,600);
    grp4->show();

    checkDetails=new QComboBox(grp4);
    QStringList functions={"Students","Books","Authors","Issued","Book Issued To Students"};
    checkDetails->addItems(functions);
    checkDetails->show();

    QLabel *detailsOf=new QLabel(grp4);
    detailsOf->setText("CHECK DETAILS OF ");
    detailsOf->setFont(font1);
    detailsOf->move(50,50);
    detailsOf->show();

    detailsTable=new QTableView(grp4);
    detailsTable->setGeometry(50,150,600,400);
    detailsTable->show();


    checkDetails->move(250,50);

    connect(checkDetails,SIGNAL(activated(QString)),this,SLOT(showDetails(const QString)));

    gallery->resize(800,700);

}

void Widget::showDetails(QString functionality)
{
    if(!mydb.open())
    {
        qDebug()<<"Failed to open the Database in showDetails() function";
    }

    QSqlQuery dq(mydb);

    if(functionality=="Students")
    {
        if(dq.exec("SELECT * FROM students"))
        {
            QSqlQueryModel *modal=new QSqlQueryModel();
            modal->setQuery(dq);
            detailsTable->setModel(modal);
        }
        else
        {
            qDebug()<<dq.lastError().text();
            qDebug()<<dq.executedQuery();
            qDebug()<<"Failed to execute query for students in checkDetails()";
        }
    }


    if(functionality=="Books")
    {
        if(dq.exec("SELECT * FROM books"))
        {
            QSqlQueryModel *modal=new QSqlQueryModel();
            modal->setQuery(dq);
            detailsTable->setModel(modal);
        }
        else
        {
            qDebug()<<dq.lastError().text();
            qDebug()<<dq.executedQuery();
            qDebug()<<"Failed to execute query for books in checkDetails()";
        }
    }

    if(functionality=="Authors")
    {
        if(dq.exec("SELECT * FROM authors"))
        {
            QSqlQueryModel *modal=new QSqlQueryModel();
            modal->setQuery(dq);
            detailsTable->setModel(modal);
        }
        else
        {
            qDebug()<<dq.lastError().text();
            qDebug()<<dq.executedQuery();
            qDebug()<<"Failed to execute query for authors in checkDetails()";
        }
    }

    if(functionality=="Issued")
    {
        if(dq.exec("SELECT * FROM issueBook"))
        {
            QSqlQueryModel *modal=new QSqlQueryModel();
            modal->setQuery(dq);
            detailsTable->setModel(modal);
        }
        else
        {
            qDebug()<<dq.lastError().text();
            qDebug()<<dq.executedQuery();
            qDebug()<<"Failed to execute query for issueBooks in checkDetails()";
        }
    }

    if(functionality=="Book Issued To Students")
    {
        if(dq.exec("SELECT b.book_id,book_name,stu_name FROM books as b LEFT JOIN issueBook as ib on b.book_id=ib.book_id LEFT JOIN students as s on ib.stu_id=s.stu_id"))
        {
            QSqlQueryModel *modal=new QSqlQueryModel();
            modal->setQuery(dq);
            detailsTable->setModel(modal);
        }
    }
    else
    {
        qDebug()<<dq.lastError().text();
        qDebug()<<dq.executedQuery();
        qDebug()<<"Failed to execute query for Books issued to Students in checkDetails()";
    }
}

void Widget::issueBook()
{
    issueDetails=new QDialog();
    issueDetails->setWindowTitle("Publish Book To Students");
    issueDetails->show();

    QFont font1,font2;

    QGroupBox *grp3=new QGroupBox(issueDetails);
    grp3->setGeometry(50,50,500,400);
    grp3->show();

    font1.setBold(true);
    font1.setPointSize(12);

    font2.setPointSize(12);

    QLabel *b_sname=new QLabel(grp3);
    b_sname->setText("Book Name");
    b_sname->setFont(font1);
    b_sname->move(50,50);
    b_sname->show();

    book_sname=new QLineEdit(grp3);
    book_sname->setFont(font2);
    book_sname->setGeometry(220,50,200,30);
    book_sname->show();

    QRegExpValidator* rxv = new QRegExpValidator(QRegExp("\\d*"), this); // only pos

    QLabel *s_bname=new QLabel(grp3);
    s_bname->setText("Issued to\n(Roll no of student)");
    s_bname->setFont(font1);
    s_bname->move(50,100);
    s_bname->show();

    stu_bname=new QLineEdit(grp3);
    stu_bname->setValidator(rxv);
    stu_bname->setFont(font2);
    stu_bname->setGeometry(220,100,200,30);
    stu_bname->show();

    issueDetails->resize(600,600);

    //Actually Due to time shortage I am not gonna implement IssueDate to ReturnDate
    QPushButton *submitDetils= new QPushButton(grp3);
    submitDetils->setFont(font1);
    submitDetils->setGeometry(150,150,120,45);
    submitDetils->setText("Submit Details");
    submitDetils->show();

    connect(submitDetils,SIGNAL(clicked()),this,SLOT(IssueBtoS()));

}

void Widget::IssueBtoS()
{
    QString bname,srollno;
    bname=book_sname->text();
    srollno=stu_bname->text();

    if(bname=="" or srollno=="")
    {
        QMessageBox::warning(issueDetails,"Details Missing"," Make Sure You Have Entered All The Details !");
    }
    else
    {
        bool success=true;

        int stu_rollNo=srollno.toInt();

        int stu_id,book_id,stock;

        if(!mydb.open())
        {
            qDebug()<<"Failed to open mydb database inside IssueBtoS function";
            success=false;
        }

        QSqlQuery issue(mydb);

        issue.prepare("CREATE TABLE IF NOT EXISTS issueBook(issue_id INTEGER PRIMARY KEY AUTOINCREMENT,stu_id	INTEGER,book_id	INTEGER)");

        if(!issue.exec())
        {
            qDebug()<<"Failed to create issueBook databse";
        }


        if(!issue.exec("SELECT book_id,stock FROM books where book_name='"+bname+"'"))
        {
            success=false;
            qDebug()<<"Failed to search for book id";
            qDebug()<<issue.executedQuery();
        }
        else
        {
            issue.first();
            book_id=issue.value(0).toInt();
            stock=issue.value(1).toInt();
            qDebug()<<"book_id: "<<book_id;
        }

        int bookTaken;
        if(!issue.exec("SELECT stu_id,books_taken FROM students where stu_rollNo='"+QString::number(stu_rollNo)+"'"))
        {
            success=false;
            qDebug()<<"Failed to search for student id";
            qDebug()<<issue.executedQuery();
        }
        else
        {

            issue.first();

            stu_id=issue.value(0).toInt();
            bookTaken=issue.value(1).toInt();
            qDebug()<<"stu_id: "<<stu_id;
            qDebug()<<"Books taken: "<<bookTaken;

        }

    int count=0;
        if(issue.exec("SELECT stu_id,book_id FROM issueBook WHERE book_id='"+QString::number(book_id)+"' AND stu_id='"+QString::number(stu_id)+"'"))
        {
            qDebug()<<"Yes it comes here";
            while(issue.next())
            {
                qDebug()<<"count";
                count+=1;
            }
        }

        if(count>0)
        {
            success=false;
            QMessageBox::warning(issueDetails,"Dublicate books","Can't issue same book twice to same student");
        }
        else
        {
        if(stock>0 and bookTaken<=4 and stu_id!=0)
        {
        if(!issue.exec("INSERT INTO issueBook(stu_id,book_id) VALUES('"+QString::number(stu_id)+"','"+QString::number(book_id)+"')"))
        {
            success=false;
            qDebug()<<issue.executedQuery();
            qDebug()<<"error: "<<issue.lastError().text();
            QMessageBox::warning(issueDetails,"Error Occured","Failed to Insert Issue details");
        }
        else
        {
            stock-=1;
            bookTaken+=1;

            if(!issue.exec("UPDATE books SET stock='"+QString::number(stock)+"' WHERE book_id='"+QString::number(book_id)+"'"))
            {
                stock+=1; //Restoring value in case of failed command;
                qDebug()<<issue.lastError().text();
                qDebug()<<issue.executedQuery();
                qDebug()<<"Failed to update stock of books table";
            }
            else
            {
                qDebug()<<"Successfullly updated stock of books";

                if(stock==0)
                {
                    if(!issue.exec("UPDATE books SET availability=0 where book_id='"+QString::number(book_id)+"'"))
                    {
                        qDebug()<<issue.lastError().text();
                        qDebug()<<issue.executedQuery();
                        qDebug()<<"Failed to update availability of books table";
                    }
                }
            }

            if(!issue.exec("UPDATE students SET books_taken='"+QString::number(bookTaken)+"' WHERE stu_rollNo='"+QString::number(stu_rollNo)+"'"))
            {
                bookTaken-=1; //Restoring value in case of failed command;
                qDebug()<<issue.lastError().text();
                qDebug()<<issue.executedQuery();
                qDebug()<<"Failed to update stock of books table";
            }
            else
            {
                qDebug()<<"Sucessfully taken books_taken attribute";
            }
        }

        }
        else
        {
            success=false;
            if(stu_id!=0)
            {
                qDebug()<<"Book not available or Maximum limit reached";
                QMessageBox::information(issueDetails,"Out of Stock or Maximum Limit Reached","Sorry, Book Not Avaiblle or Student has taken Maximum number of Books !");
            }
            else
            {
                qDebug()<<"Invalid roll no";
                QMessageBox::information(issueDetails,"Invalid User","Roll no doesn't exist");
            }
        }
        }

        if(success)
        {
            book_sname->setText("");
            stu_bname->setText("");
            QMessageBox::information(issueDetails,"Successfully Issued","Book Successfully Issued");
        }

    }
}

void Widget::addStudent()
{
    studentDetails=new QDialog();

    studentDetails->setWindowTitle("Add Student Details");

    QFont font1,font2;

    font1.setBold(true);
    font1.setPointSize(10);

    font2.setPointSize(10);

    QGroupBox *grp2=new QGroupBox(studentDetails);
    grp2->setGeometry(50,50,500,700);
    grp2->show();

    QRegExpValidator* noDigit = new QRegExpValidator(QRegExp("\[a-z | A-Z]*"), this);

    //stu_name, rollno, branch
    QLabel *stu_name=new QLabel(grp2);
    stu_name->setText("Student Name");
    stu_name->setFont(font1);
    stu_name->move(50,50);
    stu_name->show();

    studentNameBox=new QLineEdit(grp2);
    studentNameBox->setValidator(noDigit);
    studentNameBox->setFont(font2);
    studentNameBox->setGeometry(200,50,200,30);
    studentNameBox->show();

    QRegExpValidator* rxv = new QRegExpValidator(QRegExp("\\d*"), this); // only pos


    QLabel *studentRollNo=new QLabel(grp2);
    studentRollNo->setText("Roll No");
    studentRollNo->setFont(font1);
    studentRollNo->move(50,100);
    studentRollNo->show();

    studentRollNoBox=new QLineEdit(grp2);
    studentRollNoBox->setValidator(rxv);
    studentRollNoBox->setFont(font2);
    studentRollNoBox->setGeometry(200,100,200,30);
    studentRollNoBox->show();

    QLabel *branch=new QLabel(grp2);
    branch->setText("Branch");
    branch->setFont(font1);
    branch->move(50,150);
    branch->show();


    branchBox=new QLineEdit(grp2);
    branchBox->setValidator(noDigit);
    branchBox->setFont(font2);
    branchBox->setGeometry(200,150,200,30);
    branchBox->show();

    QPushButton *submitDetils= new QPushButton(grp2);
    submitDetils->setFont(font1);
    submitDetils->setGeometry(120,200,100,45);
    submitDetils->setText("Submit Details");
    submitDetils->show();

    studentDetails->resize(600,900);
    studentDetails->show();

    connect(submitDetils,SIGNAL(clicked()),this,SLOT(addStudentEntry()));

}

void Widget::addStudentEntry()
{
    QString stu_name,stu_rolln,stu_branch;

    stu_name= studentNameBox->text();
    stu_rolln= studentRollNoBox->text();
    stu_branch= branchBox->text();

    if(stu_name=="" or stu_rolln=="" or stu_branch=="")
    {
        QMessageBox::warning(studentDetails,"Details Missing"," Make Sure You Have Entered All The Details !");
    }
    else
    {
        int stu_rollNo=stu_rolln.toInt();
        if(!mydb.open())
        {
            qDebug()<<"Failed to open database in addStudentEntry()";
        }

        QSqlQuery students(mydb);

        students.prepare("CREATE TABLE IF NOT EXISTS students(stu_id INTEGER PRIMARY KEY AUTOINCREMENT,stu_name TEXT, stu_rollNo INTEGER UNIQUE, stu_branch TEXT, books_taken INT)");

        if(!students.exec())
        {
            qDebug()<<"Error occured during creation of the students table";
        }

        if(!students.exec("INSERT INTO students(stu_name,stu_rollNo,stu_branch,books_taken) VALUES('"+stu_name+"','"+QString::number(stu_rollNo)+"','"+stu_branch+"','"+QString::number(0)+"')"))
        {
            QMessageBox::warning(studentDetails,"Already Exist","Student Already Exist");
        }
        else
        {
            studentNameBox->setText("");
            studentRollNoBox->setText("");
            branchBox->setText("");
            QMessageBox::information(studentDetails,"Student Added","Student has been Added Successfully");
        }
        mydb.close();
    }
}

void Widget::insertBookDetails()
{
    bookDetails=new QDialog();
    bookDetails->setWindowTitle("Add Book Details");

    QFont font1,font2;

    font1.setBold(true);
    font1.setPointSize(10);

    font2.setPointSize(10);

    QGroupBox *grp1=new QGroupBox(bookDetails);
    grp1->setGeometry(50,50,500,700);
    grp1->show();

    //your name,books name,price,author, branch,address, cover photo url
    QLabel *bookName=new QLabel(grp1);
    bookName->setText("Book Name");
    bookName->setFont(font1);
    bookName->move(50,50);
    bookName->show();

    bookNameBox=new QLineEdit(grp1);
    bookNameBox->setFont(font2);
    bookNameBox->setGeometry(200,50,200,30);
    bookNameBox->show();

    QRegExpValidator* noDigit = new QRegExpValidator(QRegExp("\[a-z | A-Z]*"), this);

    QLabel *authorName=new QLabel(grp1);
    authorName->setText("Author Name");
    authorName->setFont(font1);
    authorName->move(50,100);
    authorName->show();

    authorNameBox=new QLineEdit(grp1);
    authorNameBox->setValidator(noDigit);
    authorNameBox->setFont(font2);
    authorNameBox->setGeometry(200,100,200,30);
    authorNameBox->show();

    QLabel *bookPrice=new QLabel(grp1);
    bookPrice->setText("Book Price");
    bookPrice->setFont(font1);
    bookPrice->move(50,150);
    bookPrice->show();

    QRegExpValidator* rxv = new QRegExpValidator(QRegExp("\\d*"), this); // only pos

    bookPriceBox=new QLineEdit(grp1);
    bookPriceBox->setFont(font2);
    bookPriceBox->setValidator(rxv);
    bookPriceBox->setGeometry(200,150,200,30);
    bookPriceBox->show();

    QLabel *stockNo=new QLabel(grp1);
    stockNo->setText("Stock Number");
    stockNo->setFont(font1);
    stockNo->move(50,200);
    stockNo->show();

    stockNoBox=new QLineEdit(grp1);
    stockNoBox->setValidator(rxv);
    stockNoBox->setFont(font2);
    stockNoBox->setGeometry(200,200,200,30);
    stockNoBox->show();

    QPushButton *submitDetils= new QPushButton(grp1);
    submitDetils->setFont(font1);
    submitDetils->setGeometry(120,250,100,45);
    submitDetils->setText("Submit Details");
    submitDetils->show();

    bookDetails->resize(600,900);
    bookDetails->show();

    connect(submitDetils,SIGNAL(clicked()),this,SLOT(addBookEntry()));

}

void Widget::addBookEntry()
{
    QString bookName,authorName,bookP,stockN;

    bookName=bookNameBox->text();
    authorName=authorNameBox->text();
    bookP=bookPriceBox->text();
    stockN=stockNoBox->text();

    if(bookName=="" or authorName=="" or bookP=="" or stockN=="")
    {
        QMessageBox::warning(bookDetails,"Details Missing"," Make Sure You Have Entered All The Details !");
    }
    else
    {
        bool success=true;
        int bookPrice,stockNo;
        bookPrice=bookP.toInt();
        stockNo= stockN.toInt();

        if(!mydb.open())
        {
            qDebug()<<"Failed to open database in addBookEntry()";
        }

        QSqlQuery authors(mydb);

        authors.prepare("CREATE TABLE IF NOT EXISTS authors(author_id INTEGER PRIMARY KEY AUTOINCREMENT,author_name TEXT UNIQUE)");

        if(!authors.exec())
        {
            qDebug()<<"Error occured during creation of the authors table";
        }

        int authorId;

        if(!authors.exec("INSERT INTO authors(author_name) VALUES('"+authorName+"')"))
        {

                qDebug()<<"Failed to insert Data into author's table (maybe author already exist)";
                qDebug()<<authors.executedQuery();

        }

        if(!authors.exec("SELECT author_id FROM authors where author_name='"+authorName+"'"))
        {
            qDebug()<<"Failed to search for authors id";
            qDebug()<<authors.executedQuery();
            success=false;
        }
        else
        {
            authors.first();
            authorId=authors.value(0).toInt();
            qDebug()<<"author Id: "<<authorId;

            int availibility;

            if(stockNo>0)
            {
                availibility=1;
            }
            else
            {
                availibility=0;
            }

            QSqlQuery books(mydb);

            books.prepare("CREATE TABLE IF NOT EXISTS books(book_id	INTEGER PRIMARY KEY AUTOINCREMENT,book_name	TEXT,author_id	INTEGER,stock INTEGER,availability	INTEGER)");

            if(!books.exec())
            {
                qDebug()<<"Error occured during creation of the book table";
            }

            if(!books.exec("INSERT INTO books(book_name,author_id,stock,availability) VALUES('"+bookName+"','"+QString::number(authorId)+"','"+QString::number(stockNo)+"','"+QString::number(availibility)+"')"))
            {
                qDebug()<<books.executedQuery();
                qDebug()<<"error: "<<books.lastError().text();
                QMessageBox::warning(bookDetails,"Error Occured","Failed to Insert Book details");
                success=false;
            }

            if(success)
            {
                bookNameBox->setText("");
                authorNameBox->setText("");
                bookPriceBox->setText("");
                stockNoBox->setText("");

                //qDebug()<<"qurey: "<<bookDetails.executedQuery();
                QMessageBox::information(bookDetails,"SUCCESSFUL","Book Details Successfully Added");
            }
            else
            {
                QMessageBox::information(bookDetails,"Error Detected","Some error has been detected while inserting into Database");
            }
        }
        mydb.close();

    }

}

void Widget::paintEvent(QPaintEvent *event)
{
    QPixmap bg(":/new/prefix1/GLobussoft-logo.png");
    QPainter bgPainter(this);
    bgPainter.drawPixmap(0,0,width(),height(),bg.scaled(width(),height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    //drawBgPic.drawPixmap(w,h,pic.scaled(WOI,HOI,Qt::IgnoreAspectRatio,Qt::FastTransformation));

}

void Widget::resizeEvent(QResizeEvent *event)
{

}

void Widget::createNewAccount()
{
//    QRegularExpression rx("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",
//                          QRegularExpression::CaseInsensitiveOption);

    newAcc=new QDialog();
    newAcc->show();

    QFont font,font1,font2;
    font.setPointSize(30);
    font.setBold(true);

    font1.setPointSize(15);
    font1.setBold(true);

    font2.setPointSize(15);
    font2.setBold(false);

    QGroupBox *accForm=new QGroupBox(newAcc);
    accForm->setTitle("Registration Page");
    accForm->setFont(font);
    accForm->setGeometry(50,20,500,350);

    QLabel *username=new QLabel(accForm);
    username->setText("USERNAME/EMAIL ID");
    username->setStyleSheet("font:bold; background-color:white");
    username->setFont(font1);
    username->move(20,100);
    username->show();

    QRegExpValidator* usernameValidator = new QRegExpValidator(QRegExp("^[a-z | A-Z].*\\d.*$"), this);

    newUsernameBox=new QLineEdit(accForm);
    newUsernameBox->setValidator(usernameValidator);
    newUsernameBox->setGeometry(250,100,150,25);
    newUsernameBox->setFont(font2);
    newUsernameBox->show();

    QLabel *password=new QLabel(accForm);
    password->setText("Password");
    password->setStyleSheet("font:bold; background-color:white");
    password->setFont(font1);
    password->move(20,150);
    password->show();

    newPasswordBox=new QLineEdit(accForm);
    newPasswordBox->setEchoMode(QLineEdit::Password);
    newPasswordBox->setGeometry(250,150,150,25);
    newPasswordBox->setFont(font2);
    newPasswordBox->show();

    QLabel *passwordConfirmation=new QLabel(accForm);
    passwordConfirmation->setText("Confirm Password");
    passwordConfirmation->setStyleSheet("font:bold; background-color:white");
    passwordConfirmation->setFont(font1);
    passwordConfirmation->move(20,200);
    passwordConfirmation->show();

    newPasswordConfirmationBox=new QLineEdit(accForm);
    newPasswordConfirmationBox->setEchoMode(QLineEdit::Password);
    newPasswordConfirmationBox->setGeometry(250,200,150,25);
    newPasswordConfirmationBox->setFont(font2);
    newPasswordConfirmationBox->show();

    QPushButton *createAcc=new QPushButton(accForm);
    createAcc->setText("Create Account");
    createAcc->setFont(font1);
    createAcc->setGeometry(150,250,150,60);
    createAcc->show();


    connect(createAcc,SIGNAL(clicked()),this,SLOT(addUser()));

    accForm->show();

}

void Widget::addUser()
{
    QString pwd,cpwd;
    pwd=newPasswordBox->text();
    cpwd=newPasswordConfirmationBox->text();

    if(pwd==cpwd)
    {

        newUsername=newUsernameBox->text();
        newPassword=newPasswordBox->text();
        qDebug()<<"username: "<<newUsername<<", password: "<<newPassword;

        insertIntoDbms();
    }
    else
    {
        QMessageBox::warning(newAcc,"Not match","Password Not match");
    }

}

void Widget::insertIntoDbms()
{
    if(!mydb.open())
    {
        QMessageBox::warning(newAcc,"Error","Failed To Connect Database !");
    }
    else
    {
        QSqlQuery qry(mydb);

         qry.prepare("CREATE TABLE IF NOT EXISTS users (EMAIL_ID TEXT PRIMARY KEY, Password TEXT)");

         if(!qry.exec())
         {
             qDebug()<<"Error occured during creation of the Database";
         }

         QByteArray newEncryptPwd,newpwd;
         newEncryptPwd.append(newPassword);
         encrypt->reset();
         encrypt->addData(newEncryptPwd);
         newpwd=encrypt->result();

        if(!qry.exec("INSERT INTO users VALUES('"+newUsername+"','"+newpwd.toHex()+"')"))
        {
            QMessageBox::warning(newAcc,"Already Exist","User already exist");
        }
        else
        {
            newPasswordBox->setText("");
            newPasswordConfirmationBox->setText("");
            newUsernameBox->setText("");

            QMessageBox::information(newAcc,"Successfull","User successfully created");
        }

        if(!qry.exec("SELECT  * FROM users"))
        {
            qDebug()<<"Failed to retrieve data from database";
        }
        else
        {
            while(!qry.next())
            {
                qDebug()<<"Username: "<<qry.value(0).toString()<<", password: "<<qry.value(1).toString();
            }
        }
mydb.close();
    }

}

Widget::~Widget()
{
    delete ui;
}

