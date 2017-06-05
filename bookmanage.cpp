#include "bookmanage.h"


/*************************
 *
 * 类的构造及析构函数
 *
 * **********************/
BookManage::BookManage(QWidget *parent) :
    QMainWindow(parent)
{
    //初始化变量
    type=VISITOR;
    current_user = "游客";

    //新建计时器
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);

    //窗口布局
    setLayout();

    //连接数据库
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
 * 总体布局
 *
 * **********************/
void BookManage::setLayout(){
    //申请内存
    QPalette palette;
    loginbutton = new QPushButton(tr("登录"));
    registerbutton = new QPushButton(tr("注册"));
    label = new QLabel();
    label2 = new QLabel("\n\n");
    qhBoxLayout0 = new QHBoxLayout();
    qhBoxLayout1 = new QHBoxLayout();
    qvBoxLayout1 = new QVBoxLayout();
    titleImage = new QWidget();
    window = new QWidget();
    MainTabWidget = new QTabWidget();

    //登录注册按钮状态设置
    registerbutton->setFlat(true);//设置按钮背景透明
    registerbutton->setFixedSize(QSize(90,60));//设置按钮大小
    palette.setColor(QPalette::ButtonText, QColor(255,251,240));//设置按钮字体颜色
    registerbutton->setFont(QFont("Timer", 15,75));//设置按钮字体
    registerbutton->setPalette(palette);
    loginbutton->setFlat(true);
    loginbutton->setFixedSize(QSize(90,60));
    loginbutton->setFont(QFont("Timer", 15,75));
    //建立信号与槽
    connect(registerbutton,SIGNAL(clicked()),this,SLOT(setWindowRegister()));
    //点击注册按钮，显示注册窗口
    connect(loginbutton,SIGNAL(clicked()),this,SLOT(setWindowLogin()));
    //点击登录按钮，显示登录窗口

    //设置标题图片
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap("D:\\title.jpg")));
    titleImage->setPalette(palette);
    titleImage->setAutoFillBackground(true);
    titleImage->setLayout(qhBoxLayout1);

    //标题栏布局
    qvBoxLayout1->addWidget(label);
    qvBoxLayout1->addWidget(label2);
    qvBoxLayout1->addWidget(loginbutton);
    qvBoxLayout1->addWidget(registerbutton);
    qvBoxLayout1->addWidget(label2);

    qhBoxLayout1->addStretch();
    qhBoxLayout1->addLayout(qvBoxLayout1);
    qhBoxLayout1->addStretch();

    //切换标签
    change_tab();

    //将标题图片和标签窗口加入总布局
    qhBoxLayout0->addWidget(titleImage);
    qhBoxLayout0->addWidget(MainTabWidget);
    qhBoxLayout0->setStretchFactor(titleImage,1);
    qhBoxLayout0->setStretchFactor(MainTabWidget,4);

    //总窗口显示
    window->setAutoFillBackground(true);
    window->setLayout(qhBoxLayout0);
    window->setWindowTitle(tr("图书管理系统"));
    window->setFixedSize(1000,800);
    window->show();

}



/*************************
 *
 * 登录功能部分
 *
 * **********************/
void BookManage::setWindowLogin(){
    //登录时禁用登录和注册按钮
    loginbutton->setDisabled(true);
    registerbutton->setDisabled(true);

    //用户和管理员用两个不同的窗口
    user_loginWidget=createLoginWindow(USER);
    manager_loginWidget=createLoginWindow(MANAGER);

    //创建登录选项卡（用户/管理员）
    loginTabWidget=new QTabWidget();
    loginTabWidget->addTab(user_loginWidget,"用户登录");
    loginTabWidget->addTab(manager_loginWidget,"管理员登录");

    //禁用关闭按钮
    loginTabWidget->setWindowFlags(loginTabWidget->windowFlags()&~Qt::WindowCloseButtonHint);

    loginTabWidget->show();
}

//创建登录窗口
QWidget* BookManage::createLoginWindow(int type){
    //申请内存
    QWidget *widget=new QWidget();
    QLabel *username_label = new QLabel;
    QLabel *password_label = new QLabel;
    QGridLayout *glayout = new QGridLayout;
    QPushButton *yesbutton = new QPushButton(tr("确定"));
    QPushButton *quitbutton = new QPushButton(tr("退出"));
    QHBoxLayout *loginqh = new QHBoxLayout();
    QVBoxLayout *loginqv = new QVBoxLayout();

    //在网格布局中加入标签
    username_label->setText(tr("请输入用户名："));
    password_label->setText(tr("请输入密码："));
    glayout->setContentsMargins(20,20,20,20);
    glayout->setSpacing(20);
    glayout->addWidget(username_label,1,1);
    glayout->addWidget(password_label,2,1);

    //加入按钮和输入栏
    loginqh->addStretch();
    loginqh->addWidget(yesbutton);
    loginqh->addWidget(quitbutton);
    loginqh->addStretch();
    loginqv->addLayout(glayout);
    loginqv->addLayout(loginqh);
    widget->setLayout(loginqv);

    //点击退出按钮，进行删除登录窗口操作
    connect(quitbutton,SIGNAL(clicked()),this,SLOT(deleteLogin()));


    //为用户和管理员建立不同的输入栏
    if(type==USER){
        username1 = new QLineEdit();
        password1 = new QLineEdit();
        password1->setEchoMode(QLineEdit::Password);//输入密码时变为暗码
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

    //点击确认按钮，根据类型不同将信号发送到用户登录和管理员登录中
    if(type==USER){
        connect(yesbutton,SIGNAL(clicked()),this,SLOT(userLogin()));
    }
    else if(type==MANAGER){
        connect(yesbutton,SIGNAL(clicked()),this,SLOT(managerLogin()));
    }

    return widget;
}

//删除登录界面
void BookManage::deleteLogin()
{
    //将登录注册按钮设为可使用
    delete loginTabWidget;
    loginbutton->setEnabled(true);
    registerbutton->setEnabled(true);
}

//用户登录
void BookManage::userLogin(){
    QSqlQuery query2;

    //在user表查询密码
    query2.exec("select password from user where user_name='"+username1->text()+"'");
    if(!query2.isActive()){
        return;
    }

    //判断密码是否正确
    if(query2.next()){
        QString sel_password = query2.value(0).toString();
        if(QString::compare(sel_password,password1->text())!=0){
            QMessageBox::critical(NULL, "Error", "密码输入错误",QMessageBox::Yes);
            return;
        }
    }

    //判断用户是否存在
    else{
        QMessageBox::critical(NULL, "Error", "该用户不存在",QMessageBox::Yes);
        return;
    }

    //更新检索窗口
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

//管理员登录
void BookManage::managerLogin(){
    QSqlQuery query2;

    //查询密码
    query2.exec("select password from manager where manager_id = '"+username2->text()+"'");
    if(!query2.isActive()){
        return;
    }

    //判断密码是否正确
    if(query2.next()){
        QString sel_password = query2.value(0).toString();
        if(QString::compare(sel_password,password2->text())!=0){
            QMessageBox::critical(NULL, "Error", "密码输入错误",QMessageBox::Yes);
            return;
        }
    }

    //判断管理员是否存在
    else{
        QMessageBox::critical(NULL, "Error", "该用户不存在",QMessageBox::Yes);
        return;
    }

    //更新标题栏
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
 * 注册功能部分
 *
 * **********************/
void BookManage::setWindowRegister(){
    registerWindow = new QWidget();
    gridregister=new QGridLayout();

    reg_label=new QLabel[3];
    reg_label[0].setText("请输入用户名：");
    reg_label[1].setText("请输入密码：");
    reg_label[2].setText("请重复密码：");
    reg_name=new QLineEdit();
    reg_pass1=new QLineEdit();
    reg_pass2=new QLineEdit();
    reg_pass1->setEchoMode(QLineEdit::Password);
    reg_pass2->setEchoMode(QLineEdit::Password);
    reg_okbutton=new QPushButton("确定");
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

    //建立信号与槽
    connect(reg_okbutton,SIGNAL(clicked()),this,SLOT(Register()));

}

void BookManage::Register(){
    //输入信息是否完整
    if(reg_name->text().isEmpty()||reg_pass1->text().isEmpty()||reg_pass2->text().isEmpty()){
        QMessageBox::critical(NULL, "Error", "注册信息填写不完整！",QMessageBox::Yes);
        return;
    }
    //检测密码是否一致
    if(reg_pass1->text().compare(reg_pass2->text())!=0){
        QMessageBox::critical(NULL, "Error", "密码输入不一致！",QMessageBox::Yes);
        return;
    }
    //检测用户是否已存在
    QSqlQuery reg_query;
    reg_query.exec("select user_name from user where user_name = '" + reg_name->text() + "'");
    if(reg_query.next()){
        QMessageBox::critical(NULL, "Error", "用户已存在！",QMessageBox::Yes);
        return;
    }
    //向数据库中插入用户
    static int id=1;
    QString str = QString::number(id);
    reg_query.exec("insert into user values('" + str + "','" + reg_name->text() + "','" + reg_pass1->text() + "')" );
    id++;
//    reg_query.exec("insert into user values('1005','yr','123');");
    if(reg_query.isActive()){
        QMessageBox::about(0,"bookmanage","注册成功！");
    }
    else{
        QMessageBox::critical(NULL, "Error", "注册失败",QMessageBox::Yes);
        return;
    }
}



/*************************
 *
 * 更新主界面部分
 *
 * **********************/
void BookManage::updateTime()
{
    //获取时间
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    QString text = "当前用户：\n" + current_user + "\n当前时间：\n"+current_date;
    QFont font("Timer", 10, 75);//设置字体大小
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);//设置字体颜色
    label->setFont(font);
    label->setPalette(pa);
    label->setText(text);
}

void BookManage::setTitle(int type){
    QFont font("Timer", 10, 75);//设置字体大小
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);//设置字体颜色
    //获取时间
    QDateTime current_time=QDateTime::currentDateTime();
    QString current_date=current_time.toString("yyyy-MM-dd hh:mm:ss");
    QString text;
    label->setFont(font);
    label->setPalette(pa);
    if(type==VISITOR){
        text="当前用户：\n游客\n当前时间：\n"+current_date;
        label->setText(text);
        return;
    }

    //如果登录就删除按钮
    delete loginbutton;
    delete registerbutton;

    //新建退出登录按钮
    QFont font2("Timer", 10, 75);//设置字体大小
    quitloginbutton = new QPushButton("退出登录");
    quitloginbutton->setFont(font2);
    connect(quitloginbutton,SIGNAL(clicked()),this,SLOT(quitLogin()));
    qvBoxLayout1->addWidget(quitloginbutton);
    quitloginbutton->setFlat(true);

    if(type==USER){
        text="当前用户：\n"+username1->text()+"\n当前时间：\n"+current_date;
        current_user=username1->text();
        label->setText(text);
        return;
    }
    else if(type==MANAGER){
        text = "当前用户：\n"+username2->text()+"\n当前时间：\n"+current_date;
        current_user = username2->text();
        label->setText(text);
        return;
    }
}




/*************************
 *
 * 退出登录功能
 *
 * **********************/
void BookManage::quitLogin(){
    delete quitloginbutton;
    type=VISITOR;
    current_user="游客";
    int count = MainTabWidget->count();
    for(int i = 0; i < count; i++)
    {
        MainTabWidget->removeTab(0);
    }
    change_tab();

    //获取时间
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    QString text = "当前用户：\n游客\n当前时间：\n"+current_date;
    QFont font("Timer", 10, 75);//设置字体大小
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);//设置字体颜色
    label->setFont(font);
    label->setPalette(pa);
    label->setText(text);

    QPalette palette;
    loginbutton = new QPushButton(tr("登录"));
    registerbutton = new QPushButton(tr("注册"));
    //登录注册按钮状态设置
    registerbutton->setFlat(true);//设置按钮背景透明
    registerbutton->setFixedSize(QSize(90,60));//设置按钮大小
    palette.setColor(QPalette::ButtonText, QColor(255,251,240));//设置按钮字体颜色
    registerbutton->setFont(QFont("Timer", 15,75));//设置按钮字体
    registerbutton->setPalette(palette);
    loginbutton->setFlat(true);
    loginbutton->setFixedSize(QSize(90,60));
    loginbutton->setFont(QFont("Timer", 15,75));

    //标题栏布局
    qvBoxLayout1->addWidget(label);
    qvBoxLayout1->addWidget(label2);
    qvBoxLayout1->addWidget(loginbutton);
    qvBoxLayout1->addWidget(registerbutton);
    qvBoxLayout1->addWidget(label2);

    qhBoxLayout1->addStretch();
    qhBoxLayout1->addLayout(qvBoxLayout1);
    qhBoxLayout1->addStretch();

    //建立信号与槽
    connect(registerbutton,SIGNAL(clicked()),this,SLOT(setWindowRegister()));
    //点击注册按钮，显示注册窗口
    connect(loginbutton,SIGNAL(clicked()),this,SLOT(setWindowLogin()));
    //点击登录按钮，显示登录窗口
}

//根据不同身份改变标签栏
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
 * 查询借书记录部分
 *
 * **********************/
void BookManage::set_card_search(){
    cs_input=new QLineEdit();
    cs_button=new QPushButton("查询");
    window2=new QWidget();
    cs_table=new QTableWidget();
    cs_qh=new QHBoxLayout();
    cs_qv=new QVBoxLayout();

    if(type==USER){
        cs_label=new QLabel("请直接查询记录：");
        cs_qh->addWidget(cs_label);
        cs_qh->addWidget(cs_button);
    }
    if(type==MANAGER){
        cs_label=new QLabel("请输入要查询的卡号：");
        cs_qh->addWidget(cs_label);
        cs_qh->addWidget(cs_input);
        cs_qh->addWidget(cs_button);
    }
    connect(cs_button,SIGNAL(clicked()),this,SLOT(search_borrow()));

    cs_qv->addLayout(cs_qh);
    cs_qv->addWidget(cs_table);

    window2->setLayout(cs_qv);
    window2->setFixedSize(790,750);
    MainTabWidget->addTab(window2,"记录查询");
    set_cs_table();
}

void BookManage::set_cs_table(){
    cs_table->clear();
    QStringList cs_head;
    QString cs_title[]={"书号","借书证号","起始日期","还书日期","经手人"};
    cs_table->setColumnCount(5);
    cs_table->setRowCount(13);
    cs_table->sortByColumn(3, Qt::AscendingOrder);
    for(int i=0;i<5;i++){
        cs_head<<cs_title[i];
    }
    //添加栏目小标题
    cs_table->setHorizontalHeaderLabels(cs_head);
    //设置表格表头加粗
    QFont font = cs_table->horizontalHeader()->font();
    font.setBold(true);
    cs_table->horizontalHeader()->setFont(font);
    //设置行高
    cs_table->verticalHeader()->setDefaultSectionSize(50); //设置行高
    //设置选中背景色
    cs_table->setStyleSheet("selection-background-color:lightblue;");
    //设置不可编辑
    cs_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置表头背景色
    cs_table->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    //设置表头高度
    cs_table->horizontalHeader()->setFixedHeight(25);
    //设置每一列宽度
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
                QMessageBox::critical(0,QObject::tr("error"),"未查询到记录！");
                set_cs_table();
                }
        }
        else{
            QMessageBox::critical(0,QObject::tr("error"),"您没有借书证！");
        }
    }
    else
    {if(cs_input->text().isEmpty()){
        QMessageBox::critical(0,QObject::tr("error"),"请输入借书证号！");
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
        QMessageBox::critical(0,QObject::tr("error"),"未查询到记录！");
        set_cs_table();
        }
    }
    set_cs_table();
}




/*************************
 *
 * 图书搜索功能部分
 *
 * **********************/
//设置搜索书目界面
void BookManage::set_search_book()
{
    //设置高级搜索栏
    //两个按键
    //先清空之前的记录
    result.clear();
    book_searchbutton=new QPushButton("查找");
    book_clearbutton=new QPushButton("清除");
    int num = 10;
    int column=4;
    int i;
    //tab选项里的第一个窗口，用来查询
    window1 = new QWidget();
    //书号
    bookIdLine=new QLineEdit();
    //书名
    nameLine = new QLineEdit();
    //出版社
    publishLine = new QLineEdit();
    //作者
    authorLine = new QLineEdit();
    //年份下拉菜单
    dateBox = new QComboBox();
    dateBox1 = new QComboBox();
    //价格区间
    priceLine0 = new QLineEdit();
    priceLine1 = new QLineEdit();
    //图书种类下拉菜单
    kindBox=new QComboBox();
    //排序*******************
    orderBox=new QComboBox(this);
    QString a[]={"  ","书号","类别","书名","出版社","年份","作者","价格","总藏书量","库存"};
    for(int i=0;i<10;i++)
    {orderBox->addItem(a[i]);}
    orderBox->show();
    //这个排序有毒啊 加了item都是显示不出来
    gridLayout_search=new QGridLayout();
    groupbox_search=new QGroupBox();
    vboxLayout_search=new QVBoxLayout();
    //表格
    book_table=new QTableWidget();
    search_label = new QLabel[num];
    hboxlayout_search_buttons=new QHBoxLayout();


    //连接
    //搜索书籍按键按下
    connect(book_searchbutton,SIGNAL(clicked()),this,SLOT(goto_search()));
    //清除条件按键按下
    connect(book_clearbutton,SIGNAL(clicked()),this,SLOT(clear_search()));

    //标签说明
    QString str1[]={"书名","出版社","作者","书籍类别","年份","到","价位","到","书号","排序方式"};
    for(int i=0;i<num-2;i++){
       search_label[i].setText(str1[i]);
       gridLayout_search->addWidget(search_label+i,i/column+1,(i%column)*2+1);}//行号、列号

    search_label[num-2].setText(str1[num-2]);
               gridLayout_search->addWidget(search_label+num-2,3,1);//行号、列号
               //设置排序，上面的变量也改过了
                   search_label[num-1].setText(str1[num-1]);
                   gridLayout_search->addWidget (search_label+num-1,3,3);
    //设置搜索日期的起止时间
    //最开始显示的是空白
    dateBox->addItem("  ");
    dateBox1->addItem("  ");
    for(i=0;i<69;i++)//从1949年作为起始年
    {
        dateBox->addItem(QString::number(2017-i));
        dateBox1->addItem(QString::number(2017-i));
    }
    //设置搜索书籍类别
    QString str2[18]={"政治学","文学","经济学","军事学","法学","艺术学","哲学","体育","语言","教育学","自然科学","理学","天文学","计算机","医学","建筑","环境","儿童"};
    kindBox->addItem("  ");
    for( i=0;i<18;i++)
    {
        kindBox->addItem(str2[i]);
    }
    //表格布局，放置搜索选项
    gridLayout_search->addWidget(nameLine,1,2);       //书名
    gridLayout_search->addWidget(publishLine,1,4);    //类型
    gridLayout_search->addWidget(authorLine,1,6);     //作者名称
    gridLayout_search->addWidget(kindBox,1,8);       //搜索书籍类别
    gridLayout_search->addWidget(dateBox,2,2);        //年份
    gridLayout_search->addWidget(dateBox1,2,4);       //年份止
    gridLayout_search->addWidget(priceLine0,2,6);     //价格
    gridLayout_search->addWidget(priceLine1,2,8);     //价格止
    gridLayout_search->addWidget(book_searchbutton,3,6);//最下面一行放上搜索按钮
    gridLayout_search->addWidget(book_clearbutton,3,8);//清空所有选项
    gridLayout_search->addWidget(bookIdLine,3,2);//价格止
    gridLayout_search->addWidget(orderBox,3,4);
    //搜索选项加入群组中，好像就是外面框起来的一个框
    groupbox_search->setLayout(gridLayout_search);
    groupbox_search->setFixedSize(770,200);
    //垂直布局，依次加入搜索框按键 以及下方的表单
    vboxLayout_search->addWidget(groupbox_search);
    vboxLayout_search->addWidget(book_table);
    set_search_belowbuttons();
    vboxLayout_search->addLayout(hboxlayout_search_buttons);


    //窗口1查询加入这个垂直布局
    window1->setLayout(vboxLayout_search);
    window1->setFixedSize(790,750);
    MainTabWidget->addTab(window1,"图书查找");//main tab widget是总的tab
    //清除所有数据
    clear_search();
}
//搜索条件清空,只针对输入框
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
//图书搜索按键按下
void BookManage::goto_search()
{
    //查询是完全匹配
        result.clear();
        QSqlQuery query;
        int flag=0;
        int date1=1949,date2=2017;//确定图书出版日期上下界
        int price1=0,price2=20000;//确定价格上下界
        //读入当前选择的日期
        if(dateBox->currentText().compare("  "))
        {date1 = dateBox->currentText().toInt();}
        if(dateBox1->currentText().compare("  "))
        {date2 = dateBox1->currentText().toInt();}
        //读入当前选择的价格
        if(!priceLine0->text().isEmpty())
        {price1=priceLine0->text().toInt();}
        if(!priceLine1->text().isEmpty())
        {price2=priceLine1->text().toInt();}
        //排序方式//改了
                QString choose = orderBox->currentText();
                if(choose== "  "||choose== "书名"){orderby="name";hanziorder=3;}
                else if(choose== "书号"){orderby="book_id";hanziorder=0;}
                else if(choose== "类别"){orderby="type";hanziorder=2;}
                else if(choose== "出版社"){orderby="publisher";hanziorder=4;}
                else if(choose== "年份"){orderby="publish_time";hanziorder=0;}
                else if(choose== "作者"){orderby="author";hanziorder=6;}
                else if(choose== "价格"){orderby="price";hanziorder=0;}
                else if(choose== "总藏书量"){orderby="total_store";hanziorder=0;}
                else if(choose== "库存"){orderby="current_store";hanziorder=0;}

        //当没有输入信息的时候
        if(bookIdLine->text().isEmpty()&&nameLine->text().isEmpty()&&publishLine->text().isEmpty()&&authorLine->text().isEmpty()&&(!kindBox->currentText().compare("  "))
                 &&(!dateBox->currentText().compare("  "))&&(!dateBox1->currentText().compare("  "))
                 &&(priceLine0->text().isEmpty())&&(priceLine1->text().isEmpty()))
        {
                QString temp="select * from book order by convert("+orderby+" using gbk) asc";
                 query.exec(temp);
                 flag=1;
        }
        //当价格只输入了一个的时候报错
         if((priceLine0->text().isEmpty()&&!priceLine1->text().isEmpty())||(!priceLine0->text().isEmpty()&&priceLine1->text().isEmpty()))
        {
            QMessageBox::critical(0,QObject::tr("价格只输入了一个 error"),"对不起，您输入的价格格式有误");
            return;


        }
        //当价格输入前面比后面大的时候报错
         if(price1>price2)
        {
            QMessageBox::critical(0,QObject::tr("价格区间不对 error"),"对不起，您输入的价格格式有误");
            return;

        }
        //当年份只输入了一个的时候报错
         if((dateBox->currentText().compare("  ")==0&&dateBox1->currentText().compare("  ")!=0)||(dateBox->currentText().compare("  ")!=0&&dateBox1->currentText().compare("  ")==0))
        {
            QMessageBox::critical(0,QObject::tr("年份只输入了一个 error"),"对不起，您输入的日期格式有误");
            return;

        }
        //当年份前面打后面小的时候报错
         if(date1>date2)
        {
            QMessageBox::critical(0,QObject::tr("年份区间不对error"),"对不起，您输入的日期格式有误");
            return;
        }


        //存储用户输入的信息
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

           //开始sql语句查询
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
           if(!query.isActive()){QMessageBox::critical(0,QObject::tr("搜索失败 error"),query.lastError().text());}
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


           if(result.length()==0){QMessageBox::critical(0,QObject::tr("error"),"没有查询到任何结果");
               set_book_table();
               return;
           }
           page_now=1;
           page_total = result.length()/(9*12) + ((result.length()%(12*9)==0)?0:1);
           set_book_table();
}
//设置底部的翻页按钮和借书按钮
void BookManage::set_search_belowbuttons()
{
    page_now=1;
        page_total=1;
        borrow_last_button=new QPushButton("上一页");
        borrow_next_button=new QPushButton("下一页");
        //全选当前条目*******
        if(type==USER){
        choose_all_button=new QRadioButton("全选");
        choose_all_button->setChecked(false);
        connect(choose_all_button,SIGNAL(clicked()),this,SLOT(choose_all_slot()));
        choose_all_layout=new QHBoxLayout();
        choose_all_layout->setAlignment(Qt::AlignLeft);
        choose_all_layout->addWidget(choose_all_button);
        vboxLayout_search->addLayout (choose_all_layout);
        }
        //**********
        //上一页
        connect(borrow_last_button,SIGNAL(clicked()),this,SLOT(book_table_last_page()));
        //下一页
        connect(borrow_next_button,SIGNAL(clicked()),this,SLOT(book_table_next_page()));
        if(type==1)
        {
            borrow_book_button=new QPushButton ("确认借书");
            connect(borrow_book_button,SIGNAL(clicked()),this,SLOT(borrow_book()));
            hboxlayout_search_buttons->addWidget(borrow_book_button);
            return_book_button=new QPushButton("确认还书");
            connect(return_book_button,SIGNAL(clicked()),this,SLOT(return_book()));
            hboxlayout_search_buttons->addWidget(return_book_button);
        }
        hboxlayout_search_buttons->addWidget(borrow_last_button);
        hboxlayout_search_buttons->addWidget(borrow_next_button);
        //vboxLayout_search->addLayout(hboxlayout_search_buttons);
        set_book_table();//设置一下书的列表显示
}
//全选按键
void BookManage::choose_all_slot ()
{
    if(choose_all_button->isChecked())
    {
        //计算当前页条数
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
        //计算当前页条数
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

//按键 确认还书
void BookManage::return_book()
{
    int return_amount=0;
    int outdate_amount=0;
    QString cardId;//当前用户的借书卡
    QSqlQuery query1,query2,query3,query4,query6,query7;
    QString date_now=(QDate::currentDate()).toString("yyyy-MM-dd");

    QString quanxian="select card_id from card where user_name= '";
    quanxian+=current_user+"'";
    query1.exec(quanxian);
    if(!query1.isActive()){QMessageBox::critical(0,QObject::tr("1 error"),query1.lastError().text());}
    if(!query1.next())
    {
        QMessageBox::critical(0,QObject::tr("error"),"对不起，当前用户没有借书权限和还书权限，请尽快让管理员给您办理借书卡！");
        set_book_table();
        return;
    }

    //如果有借书权限
    else{
        cardId=query1.value(0).toString();

        //计算当前页条数
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



        //记录被选中书的数目
        for(int i=0;i<max_line;i++)
        {
            if(choose_item[i]->checkState()==Qt::Checked)
            {
                //判断此书是否被借走了
                QString bookId=information_item[i]->text();
                query3.exec("select card_id,end_day from borrow where book_id = '"+bookId+"'");
                if(!query3.isActive()){QMessageBox::critical(0,QObject::tr("3 error"),query3.lastError().text());}
                if(query3.next())//如果这本书已经被借走
                {
                    QString card_id=query3.value(0).toString();
                    QString end_day=query3.value(1).toString();
                    query4.exec("select user_name from card where card_id='"+card_id+"'");
                    //如果是被自己借走的
                    if(!query4.isActive()){QMessageBox::critical(0,QObject::tr("4 error"),query4.lastError().text());}
                    if(query4.next())
                    {
                        if(query4.value(0).toString()==current_user)
                        {
                            return_amount++;
                            if(end_day<date_now)
                            {
                                QMessageBox::critical(0,QObject::tr("超期error"),"书号为"+bookId+"的书已经超过截止日期"+end_day+",请尽快缴纳超期费用");
                                outdate_amount++;
                            }

                        }

                    else{
                        QMessageBox::critical(0,QObject::tr("未在借阅账户中error"),"您没有借阅书号为"+bookId+"的书!此时目前正被他人借阅，截止还书日期为"+end_day+",您可以选择之后借阅此书");
                        choose_item[i]->setCheckState(Qt::Unchecked);
                        }

                    }

                }
                //如果这本书没有被借阅走
                else{
                    choose_item[i]->setCheckState(Qt::Unchecked);
                    QMessageBox::critical(0,QObject::tr("未在借阅账户中error"),"您没有借阅书号为"+bookId+"的书!此时目前无人借阅，您可以借阅此书");

                }


            }
        }

        //没有选中借书
        if(return_amount==0)
        {
            QMessageBox::critical(0,QObject::tr("没有选择 error"),"您没有选择任何项目");
        }

        //归还选取成功的书籍
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
            if(return_amount!=0)QMessageBox::critical(0,QObject::tr("success")," 还书成功");
            goto_search();
            set_book_table();



    }
}


//按键 确认借书
void BookManage::borrow_book()
{

    int success=1;
    int most_books=13;
    int amount_now=0,amount_add=0;
    //qint64 days=10;
    QString cardId;//当前用户的借书卡
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
        QMessageBox::critical(0,QObject::tr("error"),"对不起，当前用户没有借书权限");
        set_book_table();
        return;
    }

    //如果有借书权限
    else{
        cardId=query1.value(0).toString();

        //计算当前页条数
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

        //记录该用户已经借了几本书了
        QString amount="select count(book_id) from borrow where card_id= '";
        amount=amount+cardId+"'";
        query2.exec(amount);
        if(!query2.isActive()){QMessageBox::critical(0,QObject::tr("2 error"),query2.lastError().text());}
        if(query2.next())
        {
            amount_now=query2.value(0).toInt();
        }
        //记录被选中书的数目
        for(int i=0;i<max_line;i++)
        {
            if(choose_item[i]->checkState()==Qt::Checked)
            {
                //判断此书是否被借走了
                QString bookId=information_item[i]->text();
                query3.exec("select card_id,end_day from borrow where book_id = '"+bookId+"'");
                if(!query3.isActive()){QMessageBox::critical(0,QObject::tr("3 error"),query3.lastError().text());}
                if(query3.next())//如果这本书已经被借走
                {
                    QString card_id=query3.value(0).toString();
                    QString end_day=query3.value(1).toString();
                    query4.exec("select user_name from card where card_id='"+card_id+"'");
                    //如果是被自己借走的
                    if(!query4.isActive()){QMessageBox::critical(0,QObject::tr("4 error"),query4.lastError().text());}
                    if(query4.next())
                    {
                        if(query4.value(0).toString()==current_user)
                        {
                            success=0;

                            QMessageBox::critical(0,QObject::tr("重复借书error"),"书号为"+bookId+"的书已在您的借书账户中！");

                        }

                    else{
                            success=0;

                        QMessageBox::critical(0,QObject::tr("已经被借error"),"书号为"+bookId+"的书已经被借走！截止还书日期为"+end_day);
                        }

                    }

                }

                //如果没有被借走但是库存量不足
                query5.exec("select current_store from book where book_id='"+bookId+"'");
                if(!query5.isActive()){QMessageBox::critical(0,QObject::tr("5 error"),query5.lastError().text());}


                if(query5.next())
                {
                    int store=query5.value(0).toInt();
                    if(store==0)
                    {
                        success=0;

                        QMessageBox::critical(0,QObject::tr("库存 error"),"书号为"+bookId+"的书暂时没有库存，无法借阅");

                    }
                }

                //如果没被借走，那么选中的书本又多了
                if(success) amount_add++;
            }
        }

        //没有选中借书
        if(amount_add==0)
        {
            success=0;

            QMessageBox::critical(0,QObject::tr("没有选择 error"),"您没有选择任何项目");
        }
        //借书数量超过
        if((amount_add+amount_now)>most_books)
        {

            QMessageBox::critical(0,QObject::tr("超出限度 error"),"对不起，您借书的数量将超过用户额度");
            success=0;

        }
        //借书成功，更改相关数据
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
            QMessageBox::about(0,QObject::tr("success"),"借书成功！");
            goto_search();
            set_book_table();

        }

    }
}


//设置子窗口图书显示列表的排列
void BookManage::set_book_table()
{
    book_table->clear();
    QStringList head;
    QString attributes[]={"书号", "类别", "书名", "出版社", "年份", "作者", "价格", "总藏书量", "库存"};
    if(type==1)
    {
        book_table->setColumnCount(10);
        book_table->setRowCount(12);
        head<<("选择");
        //按序号排序
    }
    else{
        book_table->setColumnCount(9);
        book_table->setRowCount(12);
        //按序号排序

    }
    for(int i=0;i<9;i++)
    {
        head<<attributes[i];
    }
    //添加栏目小标题
    book_table->setHorizontalHeaderLabels(head);
    //设置表格表头加粗
    QFont font = book_table->horizontalHeader()->font();
    font.setBold(true);
    book_table->horizontalHeader()->setFont(font);
    //设置行高
    book_table->verticalHeader()->setDefaultSectionSize(37); //设置行高
    //设置选中背景色
    book_table->setStyleSheet("selection-background-color:lightblue;");
    //设置不可编辑
    book_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置表头背景色
    book_table->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    //设置表头高度
    book_table->horizontalHeader()->setFixedHeight(25);
    //设置每一列宽度
    for(int i=0;i<9;i++)
    {
        book_table->setColumnWidth(i,180);
    }
    if(type==1) book_table->setColumnWidth(9,180);

    //求出当前页的最多显示条目
    int max_line;
    //result.length是文件读入的item
    //读入为空
    if(result.length()==0)return;
    //求出当前页最多有多少line
    if(page_now==page_total)
    {
       max_line = result.length()%(12*9)/9;
       if(max_line==0)max_line=12;
    }
    else max_line = 12;

    //根据当前使用者的不同在表格上显示不同信息
    if(type==1)
    {
        for(int i=0;i<max_line;i++){
                choose_item[i] = new QTableWidgetItem();
                choose_item[i]->setCheckState(Qt::Unchecked);//一开始按键未被选择
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
 * 用户管理部分
 *
 * **********************/
void BookManage::set_user_manage(){
    window4=new QWidget();
    um_grid=new QGridLayout();
    um_grid2=new QGridLayout();
    qv_usermanage=new QVBoxLayout();
    um_cardid_add=new QLabel("请输入借书证号：");
    um_cardid_del=new QLabel("请输入借书证号：");
    um_name=new QLabel("请输入用户名：");
    um_unit=new QLabel("请输入单位：");
    um_type=new QLabel("请输入类别：");
    um_button_add=new QPushButton("增加借书证");
    um_button_del=new QPushButton("删除借书证");
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

    connect(um_button_add,SIGNAL(clicked()),this,SLOT(addCard()));//增加借书证
    connect(um_button_del,SIGNAL(clicked()),this,SLOT(delCard()));//删除借书证

    window4->setLayout(um_qh);
    window4->setFixedSize(790,750);

    MainTabWidget->addTab(window4,"借书证管理");
}

void BookManage::addCard(){//插入借书证
    if(um_cardadd_input->text().isEmpty()||
       um_name_input->text().isEmpty()||
       um_unit_input->text().isEmpty()||
       um_type_input->text().isEmpty()){
        QMessageBox::critical(NULL, "Error", "信息填写不完整",QMessageBox::Yes);
    }
    else{
        QSqlQuery card_query;
        card_query.exec("select card_id from card where card_id='"+um_cardadd_input->text()+"'");
        if(card_query.next())
            QMessageBox::critical(0,QObject::tr("error"),"该借书证已存在！");
        else{
            card_query.exec("insert into card values('"+um_cardadd_input->text()+"','"
                        +um_name_input->text()+"','"+um_unit_input->text()+"','"
                        +um_type_input->text()+"')");
            if(card_query.isActive())
               QMessageBox::about(0,"bookmanage","插入借书证成功！");
            else
                QMessageBox::critical(0,QObject::tr("error"),"插入借书证入失败！");
        }
    }
}

void BookManage::delCard(){//删除借书证
    QSqlQuery card_query;
    if(um_carddel_input->text().isEmpty())
        QMessageBox::critical(NULL, "Error", "请输入借书证号",QMessageBox::Yes);
    else{
        card_query.exec("select card_id from card where card_id='"+um_carddel_input->text()+"'");
        if(card_query.next()){
        card_query.exec("select card_id from borrow where card_id='"+um_carddel_input->text()+"'");
        if(card_query.next()){
            QMessageBox::critical(0,QObject::tr("error"),"失败！该借书卡未还书！");
        }
        else{
            card_query.exec("delete from card where card_id='"+um_carddel_input->text()+"'");
            if(card_query.isActive())
               QMessageBox::about(0,"bookmanage","删除借书证成功！");
            else
                QMessageBox::critical(0,QObject::tr("error"),"删除借书证失败！");
        }
        }
        else QMessageBox::critical(0,QObject::tr("error"),"不存在该借书证！");
    }
}



/*************************
 *
 * 图书插入部分
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
    ins_okbutton=new QPushButton("插入单册图书");
    ins_filebutton=new QPushButton("加载多册图书数据");
    ins_mulbutton=new QPushButton("插入多册图书");

    ins_publisher_time->addItem("    ");
    for(int i=0;i<68;i++)//从1949年作为起始年
    {
        ins_publisher_time->addItem(QString::number(2017-i));
    }

    QString ins_str[18]={"政治学","文学","经济学","军事学","法学","艺术学","哲学","体育","语言","教育学","自然科学","理学","天文学","计算机","医学","建筑","环境","儿童"};
    ins_type->addItem("    ");
    for(int i=0;i<18;i++)
    {
        ins_type->addItem(ins_str[i]);
    }

    QString str_insert[]={"书号", "类别", "书名", "出版社", "年份", "作者", "价格", "数量"};
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

    connect(ins_okbutton,SIGNAL(clicked()),this,SLOT(oneAddBook()));//单册添加图书
    connect(ins_filebutton,SIGNAL(clicked()),this,SLOT(openFile()));//加载文件
    connect(ins_mulbutton,SIGNAL(clicked()),this,SLOT(mutiAddBook()));//多册添加图书

    window3->setLayout(ins_qh);
    window3->setFixedSize(790,750);
    MainTabWidget->addTab(window3,"图书插入");
}

bool BookManage::addBook(){//插入图书
    QSqlQuery add_query,add_query2;

    //判断图书是否已在库中
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
       QMessageBox::critical(NULL, "Error", "信息填写不完整",QMessageBox::Yes);
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
        QMessageBox::about(0,"bookmanage","插入成功！");

        //清空输入框
        ins_id->clear();
        ins_name->clear();
        ins_publisher->clear();
        ins_author->clear();
        ins_price->clear();
        ins_num->clear();
    }
    else{
        QMessageBox::critical(0,QObject::tr("error"),"插入失败！");
    }
/*
    //清空输入框
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
    QFileDialog dlg(fileWindow,"打开图书数据文件");
    dlg.resize(300,200);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);

    //限制文件格式
    dlg.setNameFilter("*txt");

    //打开文件对话框
    if(dlg.exec()==QDialog::Accepted ){
        QStringList files = dlg.selectedFiles();
        if(!files.isEmpty()){
            //以只读形式打开选中文件
            QFile file;
            file.setFileName(files.at(0));
            if(file.open(QIODevice::ReadOnly)){
            if(!file.atEnd()){
                int count = 0;
                int rowNum = 0;
                //读取文本所有信息，并进行转码处理
                QByteArray buff;
                buff = file.readAll();
                QString fileContent = QString::fromLocal8Bit(buff);
                QString str = "";
                //对文本进行格式化处理
                for(int k=0;k<=fileContent.length();k++){
                    //读到换行符或者读完文本，把书籍信息存储
                    if(fileContent.at(k)=='\n'||k==fileContent.length()){
                        rowNum++;
                        count=0;
                        if(fileContent.at(k)=='\n'&&k==fileContent.length()-1){
                           break;
                        }
                    }
                //读到逗号，跳过逗号，并把字符串存储到bookmessage中(读到每行最右的括号时也一样)
                else if(fileContent.at(k)==','||fileContent.at(k)==')'){
                    mul_Message[rowNum][count++] = str;
                    str = "";
                    }
                //支持的文本内容加入字符串中
                else if(fileContent.at(k).isLetter()||fileContent.at(k).isNumber()
                        ||fileContent.at(k)=='.'||fileContent.at(k)==':'
                        ||fileContent.at(k)=='-'||fileContent.at(k)=='"'
                        ||fileContent.at(k)=='·'){
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
    //判断图书是否已在库中
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
    if(suc_count==mul_count) QMessageBox::about(0,"bookmanage","全部插入成功！");
    else QMessageBox::critical(0,QObject::tr("error"),"有图书插入失败！");
}
