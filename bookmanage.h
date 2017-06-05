//每个用户最多能借13本书
//每本书能借60天
//class QRadioButton;


#ifndef BOOKMANAGE_H
#define BOOKMANAGE_H
#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QLayout>
#include <QComboBox>
#include <QLabel>
#include <QFrame>
#include <QGroupBox>
#include <QPushButton>
#include <QTabWidget>
#include <QTreeWidget>
#include <QPalette>
#include <QBrush>
#include <QPixmap>
#include <QDateTime>
#include <QtSql>
#include <QStringList>
#include <QPluginLoader>
#include <QMessageBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QTimer>
#include <QString>
#include <QDir>
#include <QFileDialog>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QApplication>
#include <stdlib.h>
#include <string>
#include <iostream>
#include<QHeaderView>
#include<QRadioButton>
#include <sstream>
#pragma execution_character_set("utf-8")
using namespace std;

class QWidget;
class QLineEdit;
class QComboBox;
class QPushButton;
class QLabel;
class QGridLayout;
class QTabWidget;
class QVBoxLayout;
class QHBoxLayout;
class QTreeWidget;
class QTreeWidgetItem;
class QGroupBox;
class QToolButton;
class QTableWidget;
class QCheckBox;
class QTableWidgetItem;
class QTimer;
class QHeaderView;
class QRadioButton;

class BookManage : public QMainWindow
{
    Q_OBJECT

public:
    explicit BookManage(QWidget *parent = 0);
    ~BookManage();

private:
    //变量
    int type;//用户类型
    QString bookMessage[9];//插入图书的信息
    QString mul_Message[100][9];//存储文件中读取的图书信息
    int mul_count;


    //存放导入数据的容器
    QVector<QString>result;
    QVector<QString>card_result;

    //记录时间
    QTimer *timer;

    enum{MANAGER,USER,VISITOR};//用户类型（管理员/用户/游客）

    QSqlDatabase db;

    QPushButton *registerbutton;
    QPushButton *loginbutton;
    QPushButton *yesbutton;
    QPushButton *quitbutton;
    QPushButton *quitloginbutton;
    QPushButton *reg_okbutton;

    QHBoxLayout *qhBoxLayout0;
    QHBoxLayout *qhBoxLayout1;
    QHBoxLayout *loginqh;
    QHBoxLayout *regqh;

    QVBoxLayout *qvBoxLayout0;
    QVBoxLayout *qvBoxLayout1;
    QVBoxLayout *loginqv;
    QVBoxLayout *regqv;

    QLabel *label;
    QLabel *label2;
    QLabel *username_label = new QLabel;//输入用户名
    QLabel *password_label = new QLabel;//输入密码
    QLabel *reg_label;

    QLineEdit *username1;//在编辑框里输入
    QLineEdit *password1;
    QLineEdit *username2;
    QLineEdit *password2;
    QLineEdit *reg_name;//注册信息
    QLineEdit *reg_pass1;//注册信息
    QLineEdit *reg_pass2;//注册信息


    QGridLayout *glayout;//网格布局
    QGridLayout *gridregister;

    QWidget *window;
    QWidget *window1;
    QWidget *window2;
    QWidget *window3;
    QWidget *window4;
    QWidget *widget;
    QWidget *titleImage;
    QWidget *user_loginWidget;//用户登录控件
    QWidget *manager_loginWidget;//管理员登录控件
    QWidget *registerWindow;
    QWidget *fileWindow;

    QTabWidget *MainTabWidget;

    QTabWidget *loginTabWidget;//登录选项卡

    QString current_user;

    //搜索栏部分
    //左边一整个大框的长宽定义
    QHBoxLayout *hboxLayout_search_choose;
    QHBoxLayout *hboxLayout_search;
    QVBoxLayout *vboxLayout_search_choose;
    QVBoxLayout *vboxLayout_search;
    QGridLayout *gridLayout_search;
    //清除表单
    QPushButton *book_clearbutton;
    //搜索按键
    QPushButton *book_searchbutton;
    //文字输入框
    QLineEdit *bookIdLine;
    QLineEdit *nameLine;
    QLineEdit *publishLine;
    QLineEdit *priceLine0;
    QLineEdit *priceLine1;
    QLineEdit *authorLine;
    //标签
    QLabel *search_label;
    //下拉选择单，搜索时可以用
    QComboBox *dateBox;
    QComboBox *dateBox1;
    QComboBox *kindBox;
    //table
    QTableWidget *book_table;
    QGroupBox *groupbox_search;
    //底部按键排布
    QHBoxLayout*hboxlayout_search_buttons;
    //翻页按键
    QPushButton*borrow_last_button;
    QPushButton*borrow_next_button;
    //确认借书
    QPushButton*borrow_book_button;
    //显示页数
    int page_now=1;
    int page_total=1;
    //表格里的item
    QTableWidgetItem*choose_item[13];
    QTableWidgetItem*information_item[13];
    //还书部分*********************
    QPushButton *return_book_button;
    //全选当前页面上的item**********
    QRadioButton*choose_all_button;
    QHBoxLayout*choose_all_layout;
    //按照某种方式排序
    QComboBox *orderBox;
    QString orderby;
    int hanziorder=3;
    //图书入库部分
    QGridLayout *grid_insert;
    QLabel *label_insert;
    QLineEdit *ins_id;
    QComboBox *ins_type;
    QLineEdit *ins_name;
    QLineEdit *ins_publisher;
    QComboBox *ins_publisher_time;
    QLineEdit *ins_author;
    QLineEdit *ins_price;
    QLineEdit *ins_num;
    QVBoxLayout *ins_qv;
    QHBoxLayout *ins_qh;
    QPushButton *ins_okbutton;
    QPushButton *ins_filebutton;
    QPushButton *ins_mulbutton;

    //用户管理部分
    QGridLayout *um_grid;
    QGridLayout *um_grid2;
    QVBoxLayout *qv_usermanage;
    QLabel *um_cardid_add;
    QLabel *um_cardid_del;
    QLabel *um_name;
    QLabel *um_unit;
    QLabel *um_type;
    QPushButton *um_button_add;
    QPushButton *um_button_del;
    QLineEdit *um_cardadd_input;
    QLineEdit *um_carddel_input;
    QLineEdit *um_name_input;
    QLineEdit *um_unit_input;
    QLineEdit *um_type_input;
    QHBoxLayout *um_qh;

    //记录查询部分
    QLabel *cs_label;
    QLineEdit *cs_input;
    QPushButton *cs_button;
    QTableWidget *cs_table;
    QHBoxLayout *cs_qh;
    QVBoxLayout *cs_qv;


    void setLayout();
    void set_search_book();
    void set_search_belowbuttons();
    void set_insert_book();
    void set_user_manage();
    void set_card_search();
    void setTitle(int type);
    void change_tab();
    bool addBook();
    void set_cs_table();

    QWidget* createLoginWindow(int type);//创建登录窗口

private slots:
    void setWindowLogin();
    void setWindowRegister();
    void userLogin();//用户登录
    void managerLogin();//管理员登录
    void deleteLogin();//删除登录界面
    void updateTime();
    void quitLogin();
    void Register();//用户注册
    void oneAddBook();//单册添加图书
    void openFile();
    void mutiAddBook();
    void clear_search();//清除所有搜索内容
    void goto_search();//开始查询书目
    void set_book_table();//设置图书显示列表，翻页时会用到
    void book_table_last_page();//下一页上一页按键对应的曹
    void book_table_next_page();
    void borrow_book();
    void return_book();
    void choose_all_slot();
    void search_borrow();
    void addCard();
    void delCard();


};

#endif // BOOKMANAGE_H
