#include "bookmanage.h"


/*************************
 *
 * ��Ĺ��켰��������
 *
 * **********************/
BookManage::BookManage(QWidget *parent) :
    QMainWindow(parent)
{
    //��ʼ������
    type=VISITOR;
    current_user = "�ο�";

    //�½���ʱ��
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);

    //���ڲ���
    setLayout();

    //�������ݿ�
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("bookmanage");
    db.setUserName("root");
    db.setPassword("xiaoxiang");
    if(!db.open()){
        qDebug()<<"Fail to open the database!";
    }
    qDebug()<<"Open the database successfully!";
}

BookManage::~BookManage()
{
    db.removeDatabase("bookmanage");
}



/*************************
 *
 * ���岼��
 *
 * **********************/
void BookManage::setLayout(){
    //�����ڴ�
    QPalette palette;
    loginbutton = new QPushButton(tr("��¼"));
    registerbutton = new QPushButton(tr("ע��"));
    label = new QLabel();
    label2 = new QLabel("\n\n");
    qhBoxLayout0 = new QHBoxLayout();
    qhBoxLayout1 = new QHBoxLayout();
    qvBoxLayout1 = new QVBoxLayout();
    titleImage = new QWidget();
    window = new QWidget();
    MainTabWidget = new QTabWidget();

    //��¼ע�ᰴť״̬����
    registerbutton->setFlat(true);//���ð�ť����͸��
    registerbutton->setFixedSize(QSize(90,60));//���ð�ť��С
    palette.setColor(QPalette::ButtonText, QColor(255,251,240));//���ð�ť������ɫ
    registerbutton->setFont(QFont("Timer", 15,75));//���ð�ť����
    registerbutton->setPalette(palette);
    loginbutton->setFlat(true);
    loginbutton->setFixedSize(QSize(90,60));
    loginbutton->setFont(QFont("Timer", 15,75));
    //�����ź����
    connect(registerbutton,SIGNAL(clicked()),this,SLOT(setWindowRegister()));
    //���ע�ᰴť����ʾע�ᴰ��
    connect(loginbutton,SIGNAL(clicked()),this,SLOT(setWindowLogin()));
    //�����¼��ť����ʾ��¼����

    //���ñ���ͼƬ
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap("D:\\title.jpg")));
    titleImage->setPalette(palette);
    titleImage->setAutoFillBackground(true);
    titleImage->setLayout(qhBoxLayout1);

    //����������
    qvBoxLayout1->addWidget(label);
    qvBoxLayout1->addWidget(label2);
    qvBoxLayout1->addWidget(loginbutton);
    qvBoxLayout1->addWidget(registerbutton);
    qvBoxLayout1->addWidget(label2);

    qhBoxLayout1->addStretch();
    qhBoxLayout1->addLayout(qvBoxLayout1);
    qhBoxLayout1->addStretch();

    //�л���ǩ
    change_tab();

    //������ͼƬ�ͱ�ǩ���ڼ����ܲ���
    qhBoxLayout0->addWidget(titleImage);
    qhBoxLayout0->addWidget(MainTabWidget);
    qhBoxLayout0->setStretchFactor(titleImage,1);
    qhBoxLayout0->setStretchFactor(MainTabWidget,4);

    //�ܴ�����ʾ
    window->setAutoFillBackground(true);
    window->setLayout(qhBoxLayout0);
    window->setWindowTitle(tr("ͼ�����ϵͳ"));
    window->setFixedSize(1000,800);
    window->show();

}



/*************************
 *
 * ��¼���ܲ���
 *
 * **********************/
void BookManage::setWindowLogin(){
    //��¼ʱ���õ�¼��ע�ᰴť
    loginbutton->setDisabled(true);
    registerbutton->setDisabled(true);

    //�û��͹���Ա��������ͬ�Ĵ���
    user_loginWidget=createLoginWindow(USER);
    manager_loginWidget=createLoginWindow(MANAGER);

    //������¼ѡ����û�/����Ա��
    loginTabWidget=new QTabWidget();
    loginTabWidget->addTab(user_loginWidget,"�û���¼");
    loginTabWidget->addTab(manager_loginWidget,"����Ա��¼");

    //���ùرհ�ť
    loginTabWidget->setWindowFlags(loginTabWidget->windowFlags()&~Qt::WindowCloseButtonHint);

    loginTabWidget->show();
}

//������¼����
QWidget* BookManage::createLoginWindow(int type){
    //�����ڴ�
    QWidget *widget=new QWidget();
    QLabel *username_label = new QLabel;
    QLabel *password_label = new QLabel;
    QGridLayout *glayout = new QGridLayout;
    QPushButton *yesbutton = new QPushButton(tr("ȷ��"));
    QPushButton *quitbutton = new QPushButton(tr("�˳�"));
    QHBoxLayout *loginqh = new QHBoxLayout();
    QVBoxLayout *loginqv = new QVBoxLayout();

    //�����񲼾��м����ǩ
    username_label->setText(tr("�������û�����"));
    password_label->setText(tr("���������룺"));
    glayout->setContentsMargins(20,20,20,20);
    glayout->setSpacing(20);
    glayout->addWidget(username_label,1,1);
    glayout->addWidget(password_label,2,1);

    //���밴ť��������
    loginqh->addStretch();
    loginqh->addWidget(yesbutton);
    loginqh->addWidget(quitbutton);
    loginqh->addStretch();
    loginqv->addLayout(glayout);
    loginqv->addLayout(loginqh);
    widget->setLayout(loginqv);

    //����˳���ť������ɾ����¼���ڲ���
    connect(quitbutton,SIGNAL(clicked()),this,SLOT(deleteLogin()));


    //Ϊ�û��͹���Ա������ͬ��������
    if(type==USER){
        username1 = new QLineEdit();
        password1 = new QLineEdit();
        password1->setEchoMode(QLineEdit::Password);//��������ʱ��Ϊ����
        glayout->addWidget(username1,1,2);
        glayout->addWidget(password1,2,2);
    }
    else if(type==MANAGER){
        username2 = new QLineEdit();
        password2 = new QLineEdit();
        password2->setEchoMode(QLineEdit::Password);
        glayout->addWidget(username2,1,2);
        glayout->addWidget(password2,2,2);
    }

    //���ȷ�ϰ�ť���������Ͳ�ͬ���źŷ��͵��û���¼�͹���Ա��¼��
    if(type==USER){
        connect(yesbutton,SIGNAL(clicked()),this,SLOT(userLogin()));
    }
    else if(type==MANAGER){
        connect(yesbutton,SIGNAL(clicked()),this,SLOT(managerLogin()));
    }

    return widget;
}

//ɾ����¼����
void BookManage::deleteLogin()
{
    //����¼ע�ᰴť��Ϊ��ʹ��
    delete loginTabWidget;
    loginbutton->setEnabled(true);
    registerbutton->setEnabled(true);
}

//�û���¼
void BookManage::userLogin(){
    QSqlQuery query2;

    //��user���ѯ����
    query2.exec("select password from user where user_name='"+username1->text()+"'");
    if(!query2.isActive()){
        return;
    }

    //�ж������Ƿ���ȷ
    if(query2.next()){
        QString sel_password = query2.value(0).toString();
        if(QString::compare(sel_password,password1->text())!=0){
            QMessageBox::critical(NULL, "Error", "�����������",QMessageBox::Yes);
            return;
        }
    }

    //�ж��û��Ƿ����
    else{
        QMessageBox::critical(NULL, "Error", "���û�������",QMessageBox::Yes);
        return;
    }

    //���¼�������
    type=USER;
    int count = MainTabWidget->count();
    for(int i = 0; i < count; i++)
    {
        MainTabWidget->removeTab(0);
    }
    change_tab();
    setTitle(type);
    delete loginTabWidget;
}

//����Ա��¼
void BookManage::managerLogin(){
    QSqlQuery query2;

    //��ѯ����
    query2.exec("select password from manager where manager_id = '"+username2->text()+"'");
    if(!query2.isActive()){
        return;
    }

    //�ж������Ƿ���ȷ
    if(query2.next()){
        QString sel_password = query2.value(0).toString();
        if(QString::compare(sel_password,password2->text())!=0){
            QMessageBox::critical(NULL, "Error", "�����������",QMessageBox::Yes);
            return;
        }
    }

    //�жϹ���Ա�Ƿ����
    else{
        QMessageBox::critical(NULL, "Error", "���û�������",QMessageBox::Yes);
        return;
    }

    //���±�����
    type=MANAGER;
    int count = MainTabWidget->count();
    for(int i = 0; i < count; i++)
    {
        MainTabWidget->removeTab(0);
    }
    change_tab();
    setTitle(type);
    delete loginTabWidget;
}




/*************************
 *
 * ע�Ṧ�ܲ���
 *
 * **********************/
void BookManage::setWindowRegister(){
    registerWindow = new QWidget();
    gridregister=new QGridLayout();

    reg_label=new QLabel[3];
    reg_label[0].setText("�������û�����");
    reg_label[1].setText("���������룺");
    reg_label[2].setText("���ظ����룺");
    reg_name=new QLineEdit();
    reg_pass1=new QLineEdit();
    reg_pass2=new QLineEdit();
    reg_pass1->setEchoMode(QLineEdit::Password);
    reg_pass2->setEchoMode(QLineEdit::Password);
    reg_okbutton=new QPushButton("ȷ��");
    regqh=new QHBoxLayout();
    regqv=new QVBoxLayout();

    gridregister->addWidget(&reg_label[0],1,1);
    gridregister->addWidget(&reg_label[1],2,1);
    gridregister->addWidget(&reg_label[2],3,1);
    gridregister->addWidget(reg_name,1,2);
    gridregister->addWidget(reg_pass1,2,2);
    gridregister->addWidget(reg_pass2,3,2);
    regqv->addLayout(gridregister);
    regqv->addWidget(reg_okbutton);
    regqh->addStretch();
    regqh->addLayout(regqv);
    regqh->addStretch();

    registerWindow->setLayout(regqh);
    registerWindow->resize(300,200);
    registerWindow->show();

    //�����ź����
    connect(reg_okbutton,SIGNAL(clicked()),this,SLOT(Register()));

}

void BookManage::Register(){
    //������Ϣ�Ƿ�����
    if(reg_name->text().isEmpty()||reg_pass1->text().isEmpty()||reg_pass2->text().isEmpty()){
        QMessageBox::critical(NULL, "Error", "ע����Ϣ��д��������",QMessageBox::Yes);
        return;
    }
    //��������Ƿ�һ��
    if(reg_pass1->text().compare(reg_pass2->text())!=0){
        QMessageBox::critical(NULL, "Error", "�������벻һ�£�",QMessageBox::Yes);
        return;
    }
    //����û��Ƿ��Ѵ���
    QSqlQuery reg_query;
    reg_query.exec("select user_name from user where user_name = '" + reg_name->text() + "'");
    if(reg_query.next()){
        QMessageBox::critical(NULL, "Error", "�û��Ѵ��ڣ�",QMessageBox::Yes);
        return;
    }
    //�����ݿ��в����û�
    static int id=1;
    QString str = QString::number(id);
    reg_query.exec("insert into user values('" + str + "','" + reg_name->text() + "','" + reg_pass1->text() + "')" );
    id++;
//    reg_query.exec("insert into user values('1005','yr','123');");
    if(reg_query.isActive()){
        QMessageBox::about(0,"bookmanage","ע��ɹ���");
    }
    else{
        QMessageBox::critical(NULL, "Error", "ע��ʧ��",QMessageBox::Yes);
        return;
    }
}



/*************************
 *
 * ���������沿��
 *
 * **********************/
void BookManage::updateTime()
{
    //��ȡʱ��
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    QString text = "��ǰ�û���\n" + current_user + "\n��ǰʱ�䣺\n"+current_date;
    QFont font("Timer", 10, 75);//���������С
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);//����������ɫ
    label->setFont(font);
    label->setPalette(pa);
    label->setText(text);
}

void BookManage::setTitle(int type){
    QFont font("Timer", 10, 75);//���������С
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);//����������ɫ
    //��ȡʱ��
    QDateTime current_time=QDateTime::currentDateTime();
    QString current_date=current_time.toString("yyyy-MM-dd hh:mm:ss");
    QString text;
    label->setFont(font);
    label->setPalette(pa);
    if(type==VISITOR){
        text="��ǰ�û���\n�ο�\n��ǰʱ�䣺\n"+current_date;
        label->setText(text);
        return;
    }

    //�����¼��ɾ����ť
    delete loginbutton;
    delete registerbutton;

    //�½��˳���¼��ť
    QFont font2("Timer", 10, 75);//���������С
    quitloginbutton = new QPushButton("�˳���¼");
    quitloginbutton->setFont(font2);
    connect(quitloginbutton,SIGNAL(clicked()),this,SLOT(quitLogin()));
    qvBoxLayout1->addWidget(quitloginbutton);
    quitloginbutton->setFlat(true);

    if(type==USER){
        text="��ǰ�û���\n"+username1->text()+"\n��ǰʱ�䣺\n"+current_date;
        current_user=username1->text();
        label->setText(text);
        return;
    }
    else if(type==MANAGER){
        text = "��ǰ�û���\n"+username2->text()+"\n��ǰʱ�䣺\n"+current_date;
        current_user = username2->text();
        label->setText(text);
        return;
    }
}




/*************************
 *
 * �˳���¼����
 *
 * **********************/
void BookManage::quitLogin(){
    delete quitloginbutton;
    type=VISITOR;
    current_user="�ο�";
    int count = MainTabWidget->count();
    for(int i = 0; i < count; i++)
    {
        MainTabWidget->removeTab(0);
    }
    change_tab();

    //��ȡʱ��
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    QString text = "��ǰ�û���\n�ο�\n��ǰʱ�䣺\n"+current_date;
    QFont font("Timer", 10, 75);//���������С
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);//����������ɫ
    label->setFont(font);
    label->setPalette(pa);
    label->setText(text);

    QPalette palette;
    loginbutton = new QPushButton(tr("��¼"));
    registerbutton = new QPushButton(tr("ע��"));
    //��¼ע�ᰴť״̬����
    registerbutton->setFlat(true);//���ð�ť����͸��
    registerbutton->setFixedSize(QSize(90,60));//���ð�ť��С
    palette.setColor(QPalette::ButtonText, QColor(255,251,240));//���ð�ť������ɫ
    registerbutton->setFont(QFont("Timer", 15,75));//���ð�ť����
    registerbutton->setPalette(palette);
    loginbutton->setFlat(true);
    loginbutton->setFixedSize(QSize(90,60));
    loginbutton->setFont(QFont("Timer", 15,75));

    //����������
    qvBoxLayout1->addWidget(label);
    qvBoxLayout1->addWidget(label2);
    qvBoxLayout1->addWidget(loginbutton);
    qvBoxLayout1->addWidget(registerbutton);
    qvBoxLayout1->addWidget(label2);

    qhBoxLayout1->addStretch();
    qhBoxLayout1->addLayout(qvBoxLayout1);
    qhBoxLayout1->addStretch();

    //�����ź����
    connect(registerbutton,SIGNAL(clicked()),this,SLOT(setWindowRegister()));
    //���ע�ᰴť����ʾע�ᴰ��
    connect(loginbutton,SIGNAL(clicked()),this,SLOT(setWindowLogin()));
    //�����¼��ť����ʾ��¼����
}

//���ݲ�ͬ��ݸı��ǩ��
void BookManage::change_tab(){
    if(type==MANAGER){
        set_search_book();
        set_card_search();
        set_insert_book();
        set_user_manage();
    }
    if(type==USER)
    {
        set_search_book();
        set_card_search();
    }
}




/*************************
 *
 * ��ѯ�����¼����
 *
 * **********************/
void BookManage::set_card_search(){
    cs_input=new QLineEdit();
    cs_button=new QPushButton("��ѯ");
    window2=new QWidget();
    cs_table=new QTableWidget();
    cs_qh=new QHBoxLayout();
    cs_qv=new QVBoxLayout();

    if(type==USER){
        cs_label=new QLabel("��ֱ�Ӳ�ѯ��¼��");
        cs_qh->addWidget(cs_label);
        cs_qh->addWidget(cs_button);
    }
    if(type==MANAGER){
        cs_label=new QLabel("������Ҫ��ѯ�Ŀ��ţ�");
        cs_qh->addWidget(cs_label);
        cs_qh->addWidget(cs_input);
        cs_qh->addWidget(cs_button);
    }
    connect(cs_button,SIGNAL(clicked()),this,SLOT(search_borrow()));

    cs_qv->addLayout(cs_qh);
    cs_qv->addWidget(cs_table);

    window2->setLayout(cs_qv);
    window2->setFixedSize(790,750);
    MainTabWidget->addTab(window2,"��¼��ѯ");
    set_cs_table();
}

void BookManage::set_cs_table(){
    cs_table->clear();
    QStringList cs_head;
    QString cs_title[]={"���","����֤��","��ʼ����","��������","������"};
    cs_table->setColumnCount(5);
    cs_table->setRowCount(13);
    cs_table->sortByColumn(3, Qt::AscendingOrder);
    for(int i=0;i<5;i++){
        cs_head<<cs_title[i];
    }
    //�����ĿС����
    cs_table->setHorizontalHeaderLabels(cs_head);
    //���ñ���ͷ�Ӵ�
    QFont font = cs_table->horizontalHeader()->font();
    font.setBold(true);
    cs_table->horizontalHeader()->setFont(font);
    //�����и�
    cs_table->verticalHeader()->setDefaultSectionSize(50); //�����и�
    //����ѡ�б���ɫ
    cs_table->setStyleSheet("selection-background-color:lightblue;");
    //���ò��ɱ༭
    cs_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //���ñ�ͷ����ɫ
    cs_table->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    //���ñ�ͷ�߶�
    cs_table->horizontalHeader()->setFixedHeight(25);
    //����ÿһ�п��
    for(int i=0;i<5;i++)
    {
        cs_table->setColumnWidth(i,185);
    }
    if(card_result.length()==0) return;
    int line=card_result.length()/5;
    for(int i=0;i<line;i++){
        for(int j=0;j<5;j++){
            QTableWidgetItem *cs_item = new QTableWidgetItem(card_result.at(5*i+j));
                cs_table->setItem(i,j,cs_item);
        }
    }
    card_result.clear();
}

void BookManage::search_borrow(){
    set_cs_table();
    int flag=false;
    QSqlQuery card_query,card_query2;
    if(type==USER){
        card_query2.exec("select card_id from card where user_name='"+current_user+"'");
        if(card_query2.next()){
            QString now_cardid=card_query2.value(0).toString();
            card_query.exec("select * from borrow where card_id='"+now_cardid+"'");
            while(card_query.next()){
                flag=true;
                for(int i=0;i<5;i++)
                {
                    card_result.push_back(card_query.value(i).toString());
                }
            }
            if(!flag){
                QMessageBox::critical(0,QObject::tr("error"),"δ��ѯ����¼��");
                set_cs_table();
                }
        }
        else{
            QMessageBox::critical(0,QObject::tr("error"),"��û�н���֤��");
        }
    }
    else
    {if(cs_input->text().isEmpty()){
        QMessageBox::critical(0,QObject::tr("error"),"���������֤�ţ�");
    }
    card_query.exec("select * from borrow where card_id='"+cs_input->text()+"'");
    while(card_query.next()){
        flag=true;
        for(int i=0;i<5;i++)
        {
            card_result.push_back(card_query.value(i).toString());
        }
    }
    if(!flag){
        QMessageBox::critical(0,QObject::tr("error"),"δ��ѯ����¼��");
        set_cs_table();
        }
    }
    set_cs_table();
}




/*************************
 *
 * ͼ���������ܲ���
 *
 * **********************/
//����������Ŀ����
void BookManage::set_search_book()
{
    //���ø߼�������
    //��������
    //�����֮ǰ�ļ�¼
    result.clear();
    book_searchbutton=new QPushButton("����");
    book_clearbutton=new QPushButton("���");
    int num = 10;
    int column=4;
    int i;
    //tabѡ����ĵ�һ�����ڣ�������ѯ
    window1 = new QWidget();
    //���
    bookIdLine=new QLineEdit();
    //����
    nameLine = new QLineEdit();
    //������
    publishLine = new QLineEdit();
    //����
    authorLine = new QLineEdit();
    //��������˵�
    dateBox = new QComboBox();
    dateBox1 = new QComboBox();
    //�۸�����
    priceLine0 = new QLineEdit();
    priceLine1 = new QLineEdit();
    //ͼ�����������˵�
    kindBox=new QComboBox();
    //����*******************
    orderBox=new QComboBox(this);
    QString a[]={"  ","���","���","����","������","���","����","�۸�","�ܲ�����","���"};
    for(int i=0;i<10;i++)
    {orderBox->addItem(a[i]);}
    orderBox->show();
    //��������ж��� ����item������ʾ������
    gridLayout_search=new QGridLayout();
    groupbox_search=new QGroupBox();
    vboxLayout_search=new QVBoxLayout();
    //���
    book_table=new QTableWidget();
    search_label = new QLabel[num];
    hboxlayout_search_buttons=new QHBoxLayout();


    //����
    //�����鼮��������
    connect(book_searchbutton,SIGNAL(clicked()),this,SLOT(goto_search()));
    //���������������
    connect(book_clearbutton,SIGNAL(clicked()),this,SLOT(clear_search()));

    //��ǩ˵��
    QString str1[]={"����","������","����","�鼮���","���","��","��λ","��","���","����ʽ"};
    for(int i=0;i<num-2;i++){
       search_label[i].setText(str1[i]);
       gridLayout_search->addWidget(search_label+i,i/column+1,(i%column)*2+1);}//�кš��к�

    search_label[num-2].setText(str1[num-2]);
               gridLayout_search->addWidget(search_label+num-2,3,1);//�кš��к�
               //������������ı���Ҳ�Ĺ���
                   search_label[num-1].setText(str1[num-1]);
                   gridLayout_search->addWidget (search_label+num-1,3,3);
    //�����������ڵ���ֹʱ��
    //�ʼ��ʾ���ǿհ�
    dateBox->addItem("  ");
    dateBox1->addItem("  ");
    for(i=0;i<69;i++)//��1949����Ϊ��ʼ��
    {
        dateBox->addItem(QString::number(2017-i));
        dateBox1->addItem(QString::number(2017-i));
    }
    //���������鼮���
    QString str2[18]={"����ѧ","��ѧ","����ѧ","����ѧ","��ѧ","����ѧ","��ѧ","����","����","����ѧ","��Ȼ��ѧ","��ѧ","����ѧ","�����","ҽѧ","����","����","��ͯ"};
    kindBox->addItem("  ");
    for( i=0;i<18;i++)
    {
        kindBox->addItem(str2[i]);
    }
    //��񲼾֣���������ѡ��
    gridLayout_search->addWidget(nameLine,1,2);       //����
    gridLayout_search->addWidget(publishLine,1,4);    //����
    gridLayout_search->addWidget(authorLine,1,6);     //��������
    gridLayout_search->addWidget(kindBox,1,8);       //�����鼮���
    gridLayout_search->addWidget(dateBox,2,2);        //���
    gridLayout_search->addWidget(dateBox1,2,4);       //���ֹ
    gridLayout_search->addWidget(priceLine0,2,6);     //�۸�
    gridLayout_search->addWidget(priceLine1,2,8);     //�۸�ֹ
    gridLayout_search->addWidget(book_searchbutton,3,6);//������һ�з���������ť
    gridLayout_search->addWidget(book_clearbutton,3,8);//�������ѡ��
    gridLayout_search->addWidget(bookIdLine,3,2);//�۸�ֹ
    gridLayout_search->addWidget(orderBox,3,4);
    //����ѡ�����Ⱥ���У�������������������һ����
    groupbox_search->setLayout(gridLayout_search);
    groupbox_search->setFixedSize(770,200);
    //��ֱ���֣����μ��������򰴼� �Լ��·��ı�
    vboxLayout_search->addWidget(groupbox_search);
    vboxLayout_search->addWidget(book_table);
    set_search_belowbuttons();
    vboxLayout_search->addLayout(hboxlayout_search_buttons);


    //����1��ѯ���������ֱ����
    window1->setLayout(vboxLayout_search);
    window1->setFixedSize(790,750);
    MainTabWidget->addTab(window1,"ͼ�����");//main tab widget���ܵ�tab
    //�����������
    clear_search();
}
//�����������,ֻ��������
void BookManage::clear_search()
{
    bookIdLine->clear();
    orderBox->setCurrentIndex(0);
    nameLine->clear();
    publishLine->clear();
    authorLine->clear();
    priceLine0->clear();
    priceLine1->clear();
    dateBox->setCurrentIndex(0);
    dateBox1->setCurrentIndex(0);
    kindBox->setCurrentIndex(0);
}
//ͼ��������������
void BookManage::goto_search()
{
    //��ѯ����ȫƥ��
        result.clear();
        QSqlQuery query;
        int flag=0;
        int date1=1949,date2=2017;//ȷ��ͼ������������½�
        int price1=0,price2=20000;//ȷ���۸����½�
        //���뵱ǰѡ�������
        if(dateBox->currentText().compare("  "))
        {date1 = dateBox->currentText().toInt();}
        if(dateBox1->currentText().compare("  "))
        {date2 = dateBox1->currentText().toInt();}
        //���뵱ǰѡ��ļ۸�
        if(!priceLine0->text().isEmpty())
        {price1=priceLine0->text().toInt();}
        if(!priceLine1->text().isEmpty())
        {price2=priceLine1->text().toInt();}
        //����ʽ//����
                QString choose = orderBox->currentText();
                if(choose== "  "||choose== "����"){orderby="name";hanziorder=3;}
                else if(choose== "���"){orderby="book_id";hanziorder=0;}
                else if(choose== "���"){orderby="type";hanziorder=2;}
                else if(choose== "������"){orderby="publisher";hanziorder=4;}
                else if(choose== "���"){orderby="publish_time";hanziorder=0;}
                else if(choose== "����"){orderby="author";hanziorder=6;}
                else if(choose== "�۸�"){orderby="price";hanziorder=0;}
                else if(choose== "�ܲ�����"){orderby="total_store";hanziorder=0;}
                else if(choose== "���"){orderby="current_store";hanziorder=0;}

        //��û��������Ϣ��ʱ��
        if(bookIdLine->text().isEmpty()&&nameLine->text().isEmpty()&&publishLine->text().isEmpty()&&authorLine->text().isEmpty()&&(!kindBox->currentText().compare("  "))
                 &&(!dateBox->currentText().compare("  "))&&(!dateBox1->currentText().compare("  "))
                 &&(priceLine0->text().isEmpty())&&(priceLine1->text().isEmpty()))
        {
                QString temp="select * from book order by convert("+orderby+" using gbk) asc";
                 query.exec(temp);
                 flag=1;
        }
        //���۸�ֻ������һ����ʱ�򱨴�
         if((priceLine0->text().isEmpty()&&!priceLine1->text().isEmpty())||(!priceLine0->text().isEmpty()&&priceLine1->text().isEmpty()))
        {
            QMessageBox::critical(0,QObject::tr("�۸�ֻ������һ�� error"),"�Բ���������ļ۸��ʽ����");
            return;


        }
        //���۸�����ǰ��Ⱥ�����ʱ�򱨴�
         if(price1>price2)
        {
            QMessageBox::critical(0,QObject::tr("�۸����䲻�� error"),"�Բ���������ļ۸��ʽ����");
            return;

        }
        //�����ֻ������һ����ʱ�򱨴�
         if((dateBox->currentText().compare("  ")==0&&dateBox1->currentText().compare("  ")!=0)||(dateBox->currentText().compare("  ")!=0&&dateBox1->currentText().compare("  ")==0))
        {
            QMessageBox::critical(0,QObject::tr("���ֻ������һ�� error"),"�Բ�������������ڸ�ʽ����");
            return;

        }
        //�����ǰ������С��ʱ�򱨴�
         if(date1>date2)
        {
            QMessageBox::critical(0,QObject::tr("������䲻��error"),"�Բ�������������ڸ�ʽ����");
            return;
        }


        //�洢�û��������Ϣ
        QVector<QString>search_input;
        if(!nameLine->text().isEmpty()){
               search_input.push_back("name");
               search_input.push_back(nameLine->text());
           }
           if(!publishLine->text().isEmpty()){
               search_input.push_back("publisher");
               search_input.push_back(publishLine->text());
           }
           if(!authorLine->text().isEmpty()){
               search_input.push_back("author");
               search_input.push_back(authorLine->text());
           }
           if(!bookIdLine->text().isEmpty())
           {
               search_input.push_back("book_id");
               search_input.push_back(bookIdLine->text());
           }
           if(dateBox->currentText().compare("  ")!=0){
               search_input.push_back("publish_time");
               search_input.push_back(dateBox->currentText());
               search_input.push_back(dateBox1->currentText());
           }
           if(!priceLine0->text().isEmpty()){
               search_input.push_back("price");
               search_input.push_back(priceLine0->text());
               search_input.push_back(priceLine1->text());
           }
           if(kindBox->currentText().compare("  ")!=0)
           {
               search_input.push_back("type");
               search_input.push_back(kindBox->currentText());
           }

           //��ʼsql����ѯ
           int length=search_input.length();
           int position=0;
           QString sql="select*from book where ";

           while(position<length)
           {
               position==0?(sql=sql):(sql+=" and ");
               QString attribute=search_input.front();
               search_input.pop_front();
               if(attribute.compare("book_id")==0)
               {
                   QString item1=search_input.front();
                   search_input.pop_front();
                   sql=sql+attribute+"= '"+item1+"'";
                   position+=2;
               }
               else if(attribute.compare("type")==0)
               {
                   QString item1=search_input.front();
                   search_input.pop_front();
                   sql=sql+attribute+"='"+item1+"'";
                   position+=2;
               }
               else if(attribute.compare("name")==0)
               {
                   QString item1=search_input.front();
                   search_input.pop_front();
                   sql=sql+attribute+"='"+item1+"'";
                   position+=2;
               }
               else if(attribute.compare("publisher")==0)
               {
                   QString item1=search_input.front();
                   search_input.pop_front();
                   sql=sql+attribute+"='"+item1+"'";
                   position+=2;
               }
               else if(attribute.compare("author")==0)
               {
                   QString item1=search_input.front();
                   search_input.pop_front();
                   sql=sql+attribute+"='"+item1+"'";
                   position+=2;
               }
               else if(attribute.compare("publish_time")==0)
               {

                   QString item1=search_input.front();
                   search_input.pop_front();
                   QString item2=search_input.front();
                   search_input.pop_front();
                   sql=sql+attribute+">='"+item1+"' and "+attribute+" <='"+item2+"'";
                   position+=3;
               }
               else if(attribute.compare("price")==0)
               {

                   QString item1=search_input.front();
                   search_input.pop_front();
                   QString item2=search_input.front();
                   search_input.pop_front();
                   sql=sql+attribute+">= '"+item1+"' and "+attribute+" <='"+item2+"'";
                   position+=3;
               }
           }
           search_input.clear();
           sql=sql+" order by convert("+orderby+" using gbk) asc";
           if(!flag)query.exec(sql);
           if(!query.isActive()){QMessageBox::critical(0,QObject::tr("����ʧ�� error"),query.lastError().text());}
           else
           {
               while(query.next())
               {
                   for(int i=0;i<9;i++)
                   {
                       result.push_back(query.value(i).toString());
                   }
               }

           }


           if(result.length()==0){QMessageBox::critical(0,QObject::tr("error"),"û�в�ѯ���κν��");
               set_book_table();
               return;
           }
           page_now=1;
           page_total = result.length()/(9*12) + ((result.length()%(12*9)==0)?0:1);
           set_book_table();
}
//���õײ��ķ�ҳ��ť�ͽ��鰴ť
void BookManage::set_search_belowbuttons()
{
    page_now=1;
        page_total=1;
        borrow_last_button=new QPushButton("��һҳ");
        borrow_next_button=new QPushButton("��һҳ");
        //ȫѡ��ǰ��Ŀ*******
        if(type==USER){
        choose_all_button=new QRadioButton("ȫѡ");
        choose_all_button->setChecked(false);
        connect(choose_all_button,SIGNAL(clicked()),this,SLOT(choose_all_slot()));
        choose_all_layout=new QHBoxLayout();
        choose_all_layout->setAlignment(Qt::AlignLeft);
        choose_all_layout->addWidget(choose_all_button);
        vboxLayout_search->addLayout (choose_all_layout);
        }
        //**********
        //��һҳ
        connect(borrow_last_button,SIGNAL(clicked()),this,SLOT(book_table_last_page()));
        //��һҳ
        connect(borrow_next_button,SIGNAL(clicked()),this,SLOT(book_table_next_page()));
        if(type==1)
        {
            borrow_book_button=new QPushButton ("ȷ�Ͻ���");
            connect(borrow_book_button,SIGNAL(clicked()),this,SLOT(borrow_book()));
            hboxlayout_search_buttons->addWidget(borrow_book_button);
            return_book_button=new QPushButton("ȷ�ϻ���");
            connect(return_book_button,SIGNAL(clicked()),this,SLOT(return_book()));
            hboxlayout_search_buttons->addWidget(return_book_button);
        }
        hboxlayout_search_buttons->addWidget(borrow_last_button);
        hboxlayout_search_buttons->addWidget(borrow_next_button);
        //vboxLayout_search->addLayout(hboxlayout_search_buttons);
        set_book_table();//����һ������б���ʾ
}
//ȫѡ����
void BookManage::choose_all_slot ()
{
    if(choose_all_button->isChecked())
    {
        //���㵱ǰҳ����
        int max_line=0;
        if(page_now<page_total)
        {

            max_line=12;
        }
        else{
            if(result.length()%(9*12)==0) max_line=12;
            else{
                max_line=(result.length()%(9*12))/9;
                if(max_line==0) max_line=12;
            }
        }
        if(result.length()==0) max_line=0;
        for(int i=0;i<max_line;i++)
        {
            choose_item[i]->setCheckState (Qt::Checked);
        }
    }
    else
    {
        //���㵱ǰҳ����
        int max_line=0;
        if(page_now<page_total)
        {

            max_line=12;
        }
        else{
            if(result.length()%(9*12)==0) max_line=12;
            else{
                max_line=(result.length()%(9*12))/9;
                if(max_line==0) max_line=12;
            }
        }
        if(result.length()==0) max_line=0;
        for(int i=0;i<max_line;i++)
        {
            choose_item[i]->setCheckState (Qt::Unchecked);
        }
    }
}

//���� ȷ�ϻ���
void BookManage::return_book()
{
    int return_amount=0;
    int outdate_amount=0;
    QString cardId;//��ǰ�û��Ľ��鿨
    QSqlQuery query1,query2,query3,query4,query6,query7;
    QString date_now=(QDate::currentDate()).toString("yyyy-MM-dd");

    QString quanxian="select card_id from card where user_name= '";
    quanxian+=current_user+"'";
    query1.exec(quanxian);
    if(!query1.isActive()){QMessageBox::critical(0,QObject::tr("1 error"),query1.lastError().text());}
    if(!query1.next())
    {
        QMessageBox::critical(0,QObject::tr("error"),"�Բ��𣬵�ǰ�û�û�н���Ȩ�޺ͻ���Ȩ�ޣ��뾡���ù���Ա����������鿨��");
        set_book_table();
        return;
    }

    //����н���Ȩ��
    else{
        cardId=query1.value(0).toString();

        //���㵱ǰҳ����
        int max_line=0;
        if(page_now<page_total)
        {
            max_line=12;
        }
        else{
            if(result.length()%(9*12)==0) max_line=12;
            else{
                max_line=(result.length()%(9*12))/9;
                if(max_line==0) max_line=12;
            }
        }



        //��¼��ѡ�������Ŀ
        for(int i=0;i<max_line;i++)
        {
            if(choose_item[i]->checkState()==Qt::Checked)
            {
                //�жϴ����Ƿ񱻽�����
                QString bookId=information_item[i]->text();
                query3.exec("select card_id,end_day from borrow where book_id = '"+bookId+"'");
                if(!query3.isActive()){QMessageBox::critical(0,QObject::tr("3 error"),query3.lastError().text());}
                if(query3.next())//����Ȿ���Ѿ�������
                {
                    QString card_id=query3.value(0).toString();
                    QString end_day=query3.value(1).toString();
                    query4.exec("select user_name from card where card_id='"+card_id+"'");
                    //����Ǳ��Լ����ߵ�
                    if(!query4.isActive()){QMessageBox::critical(0,QObject::tr("4 error"),query4.lastError().text());}
                    if(query4.next())
                    {
                        if(query4.value(0).toString()==current_user)
                        {
                            return_amount++;
                            if(end_day<date_now)
                            {
                                QMessageBox::critical(0,QObject::tr("����error"),"���Ϊ"+bookId+"�����Ѿ�������ֹ����"+end_day+",�뾡����ɳ��ڷ���");
                                outdate_amount++;
                            }

                        }

                    else{
                        QMessageBox::critical(0,QObject::tr("δ�ڽ����˻���error"),"��û�н������Ϊ"+bookId+"����!��ʱĿǰ�������˽��ģ���ֹ��������Ϊ"+end_day+",������ѡ��֮����Ĵ���");
                        choose_item[i]->setCheckState(Qt::Unchecked);
                        }

                    }

                }
                //����Ȿ��û�б�������
                else{
                    choose_item[i]->setCheckState(Qt::Unchecked);
                    QMessageBox::critical(0,QObject::tr("δ�ڽ����˻���error"),"��û�н������Ϊ"+bookId+"����!��ʱĿǰ���˽��ģ������Խ��Ĵ���");

                }


            }
        }

        //û��ѡ�н���
        if(return_amount==0)
        {
            QMessageBox::critical(0,QObject::tr("û��ѡ�� error"),"��û��ѡ���κ���Ŀ");
        }

        //�黹ѡȡ�ɹ����鼮
        for(int i=0;i<max_line;i++)
        {
                if(choose_item[i]->checkState()==Qt::Checked)
                {
                    QString bookId=information_item[i]->text();
                    query6.exec("delete from borrow where book_id='"+bookId+"'");
                    if(!query6.isActive()){QMessageBox::critical(0,QObject::tr("6 error"),query6.lastError().text());}
                    query7.exec("update book set current_store=current_store+1 where book_id= '"+bookId+"'");
                    if(!query7.isActive()){QMessageBox::critical(0,QObject::tr("7 error"),query2.lastError().text());}


                }
            }
            //QString return(return_amount);
            //QString outdate(outdate_amount);
        //std::stringstream ss;
        //std::string return1,outdate;
        //ss<<return_amount;
        //ss>>return1;
        //ss<<outdate_amount;
        //ss>>outdate;
            if(return_amount!=0)QMessageBox::critical(0,QObject::tr("success")," ����ɹ�");
            goto_search();
            set_book_table();



    }
}


//���� ȷ�Ͻ���
void BookManage::borrow_book()
{

    int success=1;
    int most_books=13;
    int amount_now=0,amount_add=0;
    //qint64 days=10;
    QString cardId;//��ǰ�û��Ľ��鿨
    QSqlQuery query1,query2,query3,query4,query5,query6,query7,query8;
    QString date_now=(QDate::currentDate()).toString("yyyy-MM-dd");
    QDate date_end0=QDate::currentDate();
    date_end0=date_end0.addDays(60);
    QString date_end=date_end0.toString("yyyy-MM-dd");

    QString quanxian="select card_id from card where user_name= '";
    quanxian+=current_user+"'";
    query1.exec(quanxian);
    if(!query1.isActive()){QMessageBox::critical(0,QObject::tr("1 error"),query1.lastError().text());}
    if(!query1.next())
    {
        QMessageBox::critical(0,QObject::tr("error"),"�Բ��𣬵�ǰ�û�û�н���Ȩ��");
        set_book_table();
        return;
    }

    //����н���Ȩ��
    else{
        cardId=query1.value(0).toString();

        //���㵱ǰҳ����
        int max_line=0;
        if(page_now<page_total)
        {
            max_line=12;
        }
        else{
            if(result.length()%(9*12)==0) max_line=12;
            else{
                max_line=(result.length()%(9*12))/9;
                if(max_line==0) max_line=12;
            }
        }

        //��¼���û��Ѿ����˼�������
        QString amount="select count(book_id) from borrow where card_id= '";
        amount=amount+cardId+"'";
        query2.exec(amount);
        if(!query2.isActive()){QMessageBox::critical(0,QObject::tr("2 error"),query2.lastError().text());}
        if(query2.next())
        {
            amount_now=query2.value(0).toInt();
        }
        //��¼��ѡ�������Ŀ
        for(int i=0;i<max_line;i++)
        {
            if(choose_item[i]->checkState()==Qt::Checked)
            {
                //�жϴ����Ƿ񱻽�����
                QString bookId=information_item[i]->text();
                query3.exec("select card_id,end_day from borrow where book_id = '"+bookId+"'");
                if(!query3.isActive()){QMessageBox::critical(0,QObject::tr("3 error"),query3.lastError().text());}
                if(query3.next())//����Ȿ���Ѿ�������
                {
                    QString card_id=query3.value(0).toString();
                    QString end_day=query3.value(1).toString();
                    query4.exec("select user_name from card where card_id='"+card_id+"'");
                    //����Ǳ��Լ����ߵ�
                    if(!query4.isActive()){QMessageBox::critical(0,QObject::tr("4 error"),query4.lastError().text());}
                    if(query4.next())
                    {
                        if(query4.value(0).toString()==current_user)
                        {
                            success=0;

                            QMessageBox::critical(0,QObject::tr("�ظ�����error"),"���Ϊ"+bookId+"�����������Ľ����˻��У�");

                        }

                    else{
                            success=0;

                        QMessageBox::critical(0,QObject::tr("�Ѿ�����error"),"���Ϊ"+bookId+"�����Ѿ������ߣ���ֹ��������Ϊ"+end_day);
                        }

                    }

                }

                //���û�б����ߵ��ǿ��������
                query5.exec("select current_store from book where book_id='"+bookId+"'");
                if(!query5.isActive()){QMessageBox::critical(0,QObject::tr("5 error"),query5.lastError().text());}


                if(query5.next())
                {
                    int store=query5.value(0).toInt();
                    if(store==0)
                    {
                        success=0;

                        QMessageBox::critical(0,QObject::tr("��� error"),"���Ϊ"+bookId+"������ʱû�п�棬�޷�����");

                    }
                }

                //���û�����ߣ���ôѡ�е��鱾�ֶ���
                if(success) amount_add++;
            }
        }

        //û��ѡ�н���
        if(amount_add==0)
        {
            success=0;

            QMessageBox::critical(0,QObject::tr("û��ѡ�� error"),"��û��ѡ���κ���Ŀ");
        }
        //������������
        if((amount_add+amount_now)>most_books)
        {

            QMessageBox::critical(0,QObject::tr("�����޶� error"),"�Բ���������������������û����");
            success=0;

        }
        //����ɹ��������������
        if(success==0)
        {
            set_book_table();
            return;
        }
        else{
            for(int i=0;i<max_line;i++)
            {
                if(choose_item[i]->checkState()==Qt::Checked)
                {
                    QString bookId=information_item[i]->text();
                    QString managerId;
                    query6.exec("select manager_id from book where book_id='"+bookId+"'");
                    if(!query6.isActive()){QMessageBox::critical(0,QObject::tr("6 error"),query6.lastError().text());}
                    if(query6.next())
                    {
                        managerId=query6.value(0).toString();
                    }
                    query7.exec("update book set current_store=current_store-1 where book_id= '"+bookId+"'");
                    if(!query7.isActive()){QMessageBox::critical(0,QObject::tr("7 error"),query2.lastError().text());}
                    query8.exec("insert into borrow values('"+bookId+"','"+cardId+"','"+date_now+"','"+date_end+"','"+managerId+"')");
                    if(!query8.isActive()){QMessageBox::critical(0,QObject::tr("8 error"),query2.lastError().text());}

                }
            }
            QMessageBox::about(0,QObject::tr("success"),"����ɹ���");
            goto_search();
            set_book_table();

        }

    }
}


//�����Ӵ���ͼ����ʾ�б������
void BookManage::set_book_table()
{
    book_table->clear();
    QStringList head;
    QString attributes[]={"���", "���", "����", "������", "���", "����", "�۸�", "�ܲ�����", "���"};
    if(type==1)
    {
        book_table->setColumnCount(10);
        book_table->setRowCount(12);
        head<<("ѡ��");
        //���������
    }
    else{
        book_table->setColumnCount(9);
        book_table->setRowCount(12);
        //���������

    }
    for(int i=0;i<9;i++)
    {
        head<<attributes[i];
    }
    //�����ĿС����
    book_table->setHorizontalHeaderLabels(head);
    //���ñ���ͷ�Ӵ�
    QFont font = book_table->horizontalHeader()->font();
    font.setBold(true);
    book_table->horizontalHeader()->setFont(font);
    //�����и�
    book_table->verticalHeader()->setDefaultSectionSize(37); //�����и�
    //����ѡ�б���ɫ
    book_table->setStyleSheet("selection-background-color:lightblue;");
    //���ò��ɱ༭
    book_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //���ñ�ͷ����ɫ
    book_table->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    //���ñ�ͷ�߶�
    book_table->horizontalHeader()->setFixedHeight(25);
    //����ÿһ�п��
    for(int i=0;i<9;i++)
    {
        book_table->setColumnWidth(i,180);
    }
    if(type==1) book_table->setColumnWidth(9,180);

    //�����ǰҳ�������ʾ��Ŀ
    int max_line;
    //result.length���ļ������item
    //����Ϊ��
    if(result.length()==0)return;
    //�����ǰҳ����ж���line
    if(page_now==page_total)
    {
       max_line = result.length()%(12*9)/9;
       if(max_line==0)max_line=12;
    }
    else max_line = 12;

    //���ݵ�ǰʹ���ߵĲ�ͬ�ڱ������ʾ��ͬ��Ϣ
    if(type==1)
    {
        for(int i=0;i<max_line;i++){
                choose_item[i] = new QTableWidgetItem();
                choose_item[i]->setCheckState(Qt::Unchecked);//һ��ʼ����δ��ѡ��
                information_item[i] = new QTableWidgetItem(result.at(9*i+12*9*(page_now-1)));
                book_table->setItem(i,0,choose_item[i]);
                book_table->setItem(i,1,information_item[i]);
                for(int j=1;j<9;j++){
                    QTableWidgetItem *item = new QTableWidgetItem(result.at(9*i+j+12*9*(page_now-1)));
                    book_table->setItem(i,j + 1,item);
                }
            }
       if(hanziorder!=0)
       book_table->sortByColumn(hanziorder,Qt::AscendingOrder);
        }
    else{
        for(int i=0;i<max_line;i++)
        {
            for(int j=0;j<9;j++)
            {
               QTableWidgetItem *item = new QTableWidgetItem(result.at(9*i+j+12*9*(page_now-1)));
               book_table->setItem(i,j,item);
            }
        }
       if(hanziorder!=0)
       book_table->sortByColumn(hanziorder-1,Qt::AscendingOrder);

        }

}
void BookManage::book_table_last_page()
{
    if(page_now==1)return;
    else{
        page_now--;
        set_book_table();
    }
}
void BookManage::book_table_next_page()
{
    if(page_now==page_total) return;
    else{
        page_now++;
        set_book_table();
    }
}




/*************************
 *
 * �û�������
 *
 * **********************/
void BookManage::set_user_manage(){
    window4=new QWidget();
    um_grid=new QGridLayout();
    um_grid2=new QGridLayout();
    qv_usermanage=new QVBoxLayout();
    um_cardid_add=new QLabel("���������֤�ţ�");
    um_cardid_del=new QLabel("���������֤�ţ�");
    um_name=new QLabel("�������û�����");
    um_unit=new QLabel("�����뵥λ��");
    um_type=new QLabel("���������");
    um_button_add=new QPushButton("���ӽ���֤");
    um_button_del=new QPushButton("ɾ������֤");
    um_cardadd_input=new QLineEdit();
    um_carddel_input=new QLineEdit();
    um_name_input=new QLineEdit();
    um_unit_input=new QLineEdit();
    um_type_input=new QLineEdit();
    um_qh=new QHBoxLayout();

    um_grid->addWidget(um_cardid_add,1,1);
    um_grid->addWidget(um_name,2,1);
    um_grid->addWidget(um_unit,3,1);
    um_grid->addWidget(um_type,4,1);
    um_grid->addWidget(um_cardadd_input,1,2);
    um_grid->addWidget(um_name_input,2,2);
    um_grid->addWidget(um_unit_input,3,2);
    um_grid->addWidget(um_type_input,4,2);

    um_grid2->addWidget(um_cardid_del,1,1);
    um_grid2->addWidget(um_carddel_input,1,2);

    qv_usermanage->addStretch();
    qv_usermanage->addLayout(um_grid);
    qv_usermanage->addWidget(um_button_add);
    qv_usermanage->addLayout(um_grid2);
    qv_usermanage->addWidget(um_button_del);
    qv_usermanage->addStretch();

    um_qh->addStretch();
    um_qh->addLayout(qv_usermanage);
    um_qh->addStretch();

    connect(um_button_add,SIGNAL(clicked()),this,SLOT(addCard()));//���ӽ���֤
    connect(um_button_del,SIGNAL(clicked()),this,SLOT(delCard()));//ɾ������֤

    window4->setLayout(um_qh);
    window4->setFixedSize(790,750);

    MainTabWidget->addTab(window4,"����֤����");
}

void BookManage::addCard(){//�������֤
    if(um_cardadd_input->text().isEmpty()||
       um_name_input->text().isEmpty()||
       um_unit_input->text().isEmpty()||
       um_type_input->text().isEmpty()){
        QMessageBox::critical(NULL, "Error", "��Ϣ��д������",QMessageBox::Yes);
    }
    else{
        QSqlQuery card_query;
        card_query.exec("select card_id from card where card_id='"+um_cardadd_input->text()+"'");
        if(card_query.next())
            QMessageBox::critical(0,QObject::tr("error"),"�ý���֤�Ѵ��ڣ�");
        else{
            card_query.exec("insert into card values('"+um_cardadd_input->text()+"','"
                        +um_name_input->text()+"','"+um_unit_input->text()+"','"
                        +um_type_input->text()+"')");
            if(card_query.isActive())
               QMessageBox::about(0,"bookmanage","�������֤�ɹ���");
            else
                QMessageBox::critical(0,QObject::tr("error"),"�������֤��ʧ�ܣ�");
        }
    }
}

void BookManage::delCard(){//ɾ������֤
    QSqlQuery card_query;
    if(um_carddel_input->text().isEmpty())
        QMessageBox::critical(NULL, "Error", "���������֤��",QMessageBox::Yes);
    else{
        card_query.exec("select card_id from card where card_id='"+um_carddel_input->text()+"'");
        if(card_query.next()){
        card_query.exec("select card_id from borrow where card_id='"+um_carddel_input->text()+"'");
        if(card_query.next()){
            QMessageBox::critical(0,QObject::tr("error"),"ʧ�ܣ��ý��鿨δ���飡");
        }
        else{
            card_query.exec("delete from card where card_id='"+um_carddel_input->text()+"'");
            if(card_query.isActive())
               QMessageBox::about(0,"bookmanage","ɾ������֤�ɹ���");
            else
                QMessageBox::critical(0,QObject::tr("error"),"ɾ������֤ʧ�ܣ�");
        }
        }
        else QMessageBox::critical(0,QObject::tr("error"),"�����ڸý���֤��");
    }
}



/*************************
 *
 * ͼ����벿��
 *
 * **********************/
void BookManage::set_insert_book(){
    window3=new QWidget();
    label_insert=new QLabel[8];
    grid_insert=new QGridLayout();
    ins_id=new QLineEdit();
    ins_type=new QComboBox();
    ins_name=new QLineEdit();
    ins_publisher=new QLineEdit();
    ins_publisher_time=new QComboBox();
    ins_author=new QLineEdit();
    ins_price=new QLineEdit();
    ins_num=new QLineEdit();
    ins_qv=new QVBoxLayout();
    ins_qh=new QHBoxLayout();
    ins_okbutton=new QPushButton("���뵥��ͼ��");
    ins_filebutton=new QPushButton("���ض��ͼ������");
    ins_mulbutton=new QPushButton("������ͼ��");

    ins_publisher_time->addItem("    ");
    for(int i=0;i<68;i++)//��1949����Ϊ��ʼ��
    {
        ins_publisher_time->addItem(QString::number(2017-i));
    }

    QString ins_str[18]={"����ѧ","��ѧ","����ѧ","����ѧ","��ѧ","����ѧ","��ѧ","����","����","����ѧ","��Ȼ��ѧ","��ѧ","����ѧ","�����","ҽѧ","����","����","��ͯ"};
    ins_type->addItem("    ");
    for(int i=0;i<18;i++)
    {
        ins_type->addItem(ins_str[i]);
    }

    QString str_insert[]={"���", "���", "����", "������", "���", "����", "�۸�", "����"};
    for(int i=0;i<8;i++){
        label_insert[i].setText(str_insert[i]);
        grid_insert->addWidget(label_insert+i,i+1,1);
    }
    grid_insert->addWidget(ins_id,1,2);
    grid_insert->addWidget(ins_type,2,2);
    grid_insert->addWidget(ins_name,3,2);
    grid_insert->addWidget(ins_publisher,4,2);
    grid_insert->addWidget(ins_publisher_time,5,2);
    grid_insert->addWidget(ins_author,6,2);
    grid_insert->addWidget(ins_price,7,2);
    grid_insert->addWidget(ins_num,8,2);
    ins_qv->addStretch();
    ins_qv->addLayout(grid_insert);
    ins_qv->addWidget(ins_okbutton);
    ins_qv->addWidget(ins_filebutton);
    ins_qv->addWidget(ins_mulbutton);
    ins_qv->addStretch();
    ins_qh->addStretch();
    ins_qh->addLayout(ins_qv);
    ins_qh->addStretch();

    connect(ins_okbutton,SIGNAL(clicked()),this,SLOT(oneAddBook()));//�������ͼ��
    connect(ins_filebutton,SIGNAL(clicked()),this,SLOT(openFile()));//�����ļ�
    connect(ins_mulbutton,SIGNAL(clicked()),this,SLOT(mutiAddBook()));//������ͼ��

    window3->setLayout(ins_qh);
    window3->setFixedSize(790,750);
    MainTabWidget->addTab(window3,"ͼ�����");
}

bool BookManage::addBook(){//����ͼ��
    QSqlQuery add_query,add_query2;

    //�ж�ͼ���Ƿ����ڿ���
/*    add_query.exec("select book_id from book where book_id = '"
            + bookMessage[0] + "' and type = '"
            + bookMessage[1] + "' and name = '"
            + bookMessage[2] + "' and publisher = '"
            + bookMessage[3] + "' and publish_time = '"
            + bookMessage[4] + "' and author = '"
            + bookMessage[5] + "' and price = '"
            + bookMessage[6] + "'");*/
    add_query.exec("select book_id from book where book_id = '"
            + bookMessage[0] + "'");
    if(add_query.next()){
        add_query2.exec("update book set total_store = total_store + " + bookMessage[7]
                + " where book_id = '" + bookMessage[0] +"'");
        add_query2.exec("update book set current_store = current_store + " + bookMessage[7]
                + " where book_id = '" + bookMessage[0] +"'");
        if(add_query2.isActive()){
            return true;
        }
        else return false;
    }
    else{
    add_query2.exec("insert into book values('"+bookMessage[0]+"','"+bookMessage[1]
            +"','"+bookMessage[2]+"','"+bookMessage[3]+"','"+bookMessage[4]
            +"','"+bookMessage[5]+"','"+bookMessage[6]+"','"+bookMessage[7]
            +"','"+bookMessage[7]+"','"+bookMessage[8]+"')");
    if(add_query2.isActive()){
        return true;
    }
    else return false;
    }
}

void BookManage::oneAddBook(){
    if(ins_id->text().isEmpty()||ins_name->text().isEmpty()||ins_publisher->text().isEmpty()||ins_author->text().isEmpty()||ins_price->text().isEmpty()||ins_num->text().isEmpty()){
       QMessageBox::critical(NULL, "Error", "��Ϣ��д������",QMessageBox::Yes);
       return;
    }
    bookMessage[0]=ins_id->text();
    bookMessage[1]=ins_type->currentText();
    bookMessage[2]=ins_name->text();
    bookMessage[3]=ins_publisher->text();
    bookMessage[4]=ins_publisher_time->currentText();
    bookMessage[5]=ins_author->text();
    bookMessage[6]=ins_price->text();
    bookMessage[7]=ins_num->text();
    bookMessage[8]=current_user;

    if(addBook()){
        QMessageBox::about(0,"bookmanage","����ɹ���");

        //��������
        ins_id->clear();
        ins_name->clear();
        ins_publisher->clear();
        ins_author->clear();
        ins_price->clear();
        ins_num->clear();
    }
    else{
        QMessageBox::critical(0,QObject::tr("error"),"����ʧ�ܣ�");
    }
/*
    //��������
    ins_id->clear();
    ins_name->clear();
    ins_publisher->clear();
    ins_author->clear();
    ins_price->clear();
    ins_num->clear();
    */
}

void BookManage::openFile(){
    fileWindow=new QWidget;
    QFileDialog dlg(fileWindow,"��ͼ�������ļ�");
    dlg.resize(300,200);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);

    //�����ļ���ʽ
    dlg.setNameFilter("*txt");

    //���ļ��Ի���
    if(dlg.exec()==QDialog::Accepted ){
        QStringList files = dlg.selectedFiles();
        if(!files.isEmpty()){
            //��ֻ����ʽ��ѡ���ļ�
            QFile file;
            file.setFileName(files.at(0));
            if(file.open(QIODevice::ReadOnly)){
            if(!file.atEnd()){
                int count = 0;
                int rowNum = 0;
                //��ȡ�ı�������Ϣ��������ת�봦��
                QByteArray buff;
                buff = file.readAll();
                QString fileContent = QString::fromLocal8Bit(buff);
                QString str = "";
                //���ı����и�ʽ������
                for(int k=0;k<=fileContent.length();k++){
                    //�������з����߶����ı������鼮��Ϣ�洢
                    if(fileContent.at(k)=='\n'||k==fileContent.length()){
                        rowNum++;
                        count=0;
                        if(fileContent.at(k)=='\n'&&k==fileContent.length()-1){
                           break;
                        }
                    }
                //�������ţ��������ţ������ַ����洢��bookmessage��(����ÿ�����ҵ�����ʱҲһ��)
                else if(fileContent.at(k)==','||fileContent.at(k)==')'){
                    mul_Message[rowNum][count++] = str;
                    str = "";
                    }
                //֧�ֵ��ı����ݼ����ַ�����
                else if(fileContent.at(k).isLetter()||fileContent.at(k).isNumber()
                        ||fileContent.at(k)=='.'||fileContent.at(k)==':'
                        ||fileContent.at(k)=='-'||fileContent.at(k)=='"'
                        ||fileContent.at(k)=='��'){
                            str = str + fileContent.at(k);
                        }
                }
                mul_count=rowNum;
            }
         }
      }
   }

}

void BookManage::mutiAddBook(){
    int suc_count=0;
    for(int i=0;i<mul_count;i++){
    QSqlQuery add_query,add_query2;
    mul_Message[i][8]=current_user;
    //�ж�ͼ���Ƿ����ڿ���
/*    add_query.exec("select book_id from book where book_id = '"
            + mul_Message[i][0] + "' and type = '"
            + mul_Message[i][1] + "' and name = '"
            + mul_Message[i][2] + "' and publisher = '"
            + mul_Message[i][3] + "' and publish_time = '"
            + mul_Message[i][4] + "' and author = '"
            + mul_Message[i][5] + "' and price = '"
            + mul_Message[i][6] + "'");*/
    add_query.exec("select book_id from book where book_id = '"
                + mul_Message[i][0] + "'");
    if(add_query.next()){
        add_query2.exec("update book set total_store = total_store + " + mul_Message[i][7]
                + " where book_id = '" + mul_Message[i][0] +"'");
        add_query2.exec("update book set current_store = current_store + " + mul_Message[i][7]
                + " where book_id = '" + mul_Message[i][0] +"'");
        if(add_query2.isActive()){
            suc_count++;
        }
    }
    else{
    add_query2.exec("insert into book values('"+mul_Message[i][0]+"','"+mul_Message[i][1]
            +"','"+mul_Message[i][2]+"','"+mul_Message[i][3]+"','"+mul_Message[i][4]
            +"','"+mul_Message[i][5]+"','"+mul_Message[i][6]+"','"+mul_Message[i][7]
            +"','"+mul_Message[i][7]+"','"+mul_Message[i][8]+"')");
    if(add_query2.isActive()){
        suc_count++;
      }
    }
  }
    if(suc_count==mul_count) QMessageBox::about(0,"bookmanage","ȫ������ɹ���");
    else QMessageBox::critical(0,QObject::tr("error"),"��ͼ�����ʧ�ܣ�");
}
