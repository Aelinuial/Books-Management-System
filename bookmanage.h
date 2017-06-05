//ÿ���û�����ܽ�13����
//ÿ�����ܽ�60��
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
    //����
    int type;//�û�����
    QString bookMessage[9];//����ͼ�����Ϣ
    QString mul_Message[100][9];//�洢�ļ��ж�ȡ��ͼ����Ϣ
    int mul_count;


    //��ŵ������ݵ�����
    QVector<QString>result;
    QVector<QString>card_result;

    //��¼ʱ��
    QTimer *timer;

    enum{MANAGER,USER,VISITOR};//�û����ͣ�����Ա/�û�/�οͣ�

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
    QLabel *username_label = new QLabel;//�����û���
    QLabel *password_label = new QLabel;//��������
    QLabel *reg_label;

    QLineEdit *username1;//�ڱ༭��������
    QLineEdit *password1;
    QLineEdit *username2;
    QLineEdit *password2;
    QLineEdit *reg_name;//ע����Ϣ
    QLineEdit *reg_pass1;//ע����Ϣ
    QLineEdit *reg_pass2;//ע����Ϣ


    QGridLayout *glayout;//���񲼾�
    QGridLayout *gridregister;

    QWidget *window;
    QWidget *window1;
    QWidget *window2;
    QWidget *window3;
    QWidget *window4;
    QWidget *widget;
    QWidget *titleImage;
    QWidget *user_loginWidget;//�û���¼�ؼ�
    QWidget *manager_loginWidget;//����Ա��¼�ؼ�
    QWidget *registerWindow;
    QWidget *fileWindow;

    QTabWidget *MainTabWidget;

    QTabWidget *loginTabWidget;//��¼ѡ�

    QString current_user;

    //����������
    //���һ�������ĳ�����
    QHBoxLayout *hboxLayout_search_choose;
    QHBoxLayout *hboxLayout_search;
    QVBoxLayout *vboxLayout_search_choose;
    QVBoxLayout *vboxLayout_search;
    QGridLayout *gridLayout_search;
    //�����
    QPushButton *book_clearbutton;
    //��������
    QPushButton *book_searchbutton;
    //���������
    QLineEdit *bookIdLine;
    QLineEdit *nameLine;
    QLineEdit *publishLine;
    QLineEdit *priceLine0;
    QLineEdit *priceLine1;
    QLineEdit *authorLine;
    //��ǩ
    QLabel *search_label;
    //����ѡ�񵥣�����ʱ������
    QComboBox *dateBox;
    QComboBox *dateBox1;
    QComboBox *kindBox;
    //table
    QTableWidget *book_table;
    QGroupBox *groupbox_search;
    //�ײ������Ų�
    QHBoxLayout*hboxlayout_search_buttons;
    //��ҳ����
    QPushButton*borrow_last_button;
    QPushButton*borrow_next_button;
    //ȷ�Ͻ���
    QPushButton*borrow_book_button;
    //��ʾҳ��
    int page_now=1;
    int page_total=1;
    //������item
    QTableWidgetItem*choose_item[13];
    QTableWidgetItem*information_item[13];
    //���鲿��*********************
    QPushButton *return_book_button;
    //ȫѡ��ǰҳ���ϵ�item**********
    QRadioButton*choose_all_button;
    QHBoxLayout*choose_all_layout;
    //����ĳ�ַ�ʽ����
    QComboBox *orderBox;
    QString orderby;
    int hanziorder=3;
    //ͼ����ⲿ��
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

    //�û�������
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

    //��¼��ѯ����
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

    QWidget* createLoginWindow(int type);//������¼����

private slots:
    void setWindowLogin();
    void setWindowRegister();
    void userLogin();//�û���¼
    void managerLogin();//����Ա��¼
    void deleteLogin();//ɾ����¼����
    void updateTime();
    void quitLogin();
    void Register();//�û�ע��
    void oneAddBook();//�������ͼ��
    void openFile();
    void mutiAddBook();
    void clear_search();//���������������
    void goto_search();//��ʼ��ѯ��Ŀ
    void set_book_table();//����ͼ����ʾ�б���ҳʱ���õ�
    void book_table_last_page();//��һҳ��һҳ������Ӧ�Ĳ�
    void book_table_next_page();
    void borrow_book();
    void return_book();
    void choose_all_slot();
    void search_borrow();
    void addCard();
    void delCard();


};

#endif // BOOKMANAGE_H
