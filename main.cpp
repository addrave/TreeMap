#include <iostream>
#include <string>
#include <cstring>
#include <mutex>
#include <thread>
#include <QDebug>
#include <windows.h>
#include <QMutex>
#include <QMutexLocker>

using namespace std;

template <class T, class T2>
class Node //Звено дерева
{public:
    T x;        //key
    T2 x2;      //val
    Node<T, T2> *l, *r;
    int deth = 0;
    QMutex mtx;
    bool exist = true;
    Node() : mtx(QMutex::Recursive) {}
};

template <class T, class T2>
class MyMap
{public:
    Node<T, T2> *Tree;
    int depth;
    mutex mainMtx; // не забыть использовать
    MyMap()
    {
        Tree = nullptr;
        depth = 0;
    }
    void show()
    {
            this->depth = 0;
            count_deth(Tree, 0);
            qDebug() << '\n' << "depth " << depth << '\n';
            show_(Tree);
    }
    void show_(Node<T, T2> *&MyTree) //Функция обхода
    {
            if (MyTree != nullptr) //Пока не встретится пустое звено
            {
                not_nullptr(MyTree);
                MyTree->mtx.lock();
                Node <T, T2> *MTl = MyTree->l;
                Node <T, T2> *MTr = MyTree->r;
                T MTx = MyTree->x;
                MyTree->mtx.unlock();
                show_(MTl);

                for(int i = this->depth; i > MyTree->deth; i--)
                    cout << "\t";

                cout << MTx;
                show_(MTr);
            }
            else
            {
                cout <<  '\n';
            }

    }
    void approve()
    {try{
            vector <T> bufer;
            approve(Tree, bufer);
            for (unsigned int i=0; i < bufer.size() - 1; i++)
                if (bufer.at(i) > bufer.at(i+1))
                {
                    qDebug() << "ATTENTION " << bufer.at(i);
                    qDebug() << bufer;
                    this->show();
                    exit(0);
                }
        }catch (...) {
            cout << "cool!";
        }
    }
    void approve(Node<T, T2> *&MyTree, vector <T> &bufer)
    {try{
            if (MyTree != nullptr) //Пока не встретится пустое звено
            {
                not_nullptr(MyTree);
                MyTree->mtx.lock();
                Node <T, T2> *MTl = MyTree->l;
                Node <T, T2> *MTr = MyTree->r;
                T MTx = MyTree->x;
                MyTree->mtx.unlock();

                approve(MTl, bufer);
//                this->mainMtx.lock();
                bufer.push_back(MTx);
//                this->mainMtx.unlock();
                approve(MTr, bufer);
            }
        }catch (...) {
            cout << "AXYEHHO";
        }
    }
    void count_deth(Node<T, T2> *&MyTree, int arg)
    {//MyTree->mtx.lock(); //нельзя блокирывать мьютексы потенциально НУЛЬПТРные

            if (MyTree != nullptr)
            {
//                qDebug() << MyTree->x;
                arg = arg + 1;

                not_nullptr(MyTree);
                MyTree->mtx.lock();
                MyTree->deth = arg;
                Node <T, T2> *MTl = MyTree->l;
                Node <T, T2> *MTr = MyTree->r;
                MyTree->mtx.unlock();

//                this->mainMtx.lock();
                if (arg > this->depth)
                    this->depth = arg;
//                this->mainMtx.unlock();

                count_deth(MTl, arg);
                count_deth(MTr, arg);
            }

    }
    void del(T key)
    {
        if (del_(key) == false)
            del(key);
    }

    bool del_(T key)
    {try{
        QVector <Node<T,T2>*> vL;
        Node<T, T2> **node = nullptr;
        node = get_value_(key, Tree, Tree); // получать в залоченном состоянии
        QMutexLocker node0(&node[0]->mtx);
        QMutexLocker node1(&node[1]->mtx);
        if (node == nullptr)
        {
            qDebug() << "ITEM NOT FOUND (DEl) whith key " << key << endl;
            return true;
        }
        if (node[0] == Tree)
        {
            not_nullptr(Tree);
            Tree->mtx.lock();
            qDebug() << "ITEM HEAD OF THIS TREE" << endl;

            if (Tree->l == nullptr && Tree->r == nullptr) // дебажить!
            {
                Tree->mtx.unlock();
                qDebug() << "DEPEBO BCE.";
                exit(1);
            }
            if (Tree->l == nullptr && Tree->r != nullptr) // дебажить!
                ;
            if (Tree->l != nullptr && Tree->r == nullptr) // дебажить!
                ;
            if (Tree->l != nullptr && Tree->r != nullptr) // дебажить!
                ;
            not_nullptr(Tree);
            Tree->mtx.lock();
            not_nullptr(Tree->l);
            not_nullptr(Tree->r);
            Tree->l->mtx.lock();
            Tree->r->mtx.lock();
            T TLx = Tree->l->x;
            T TRx = Tree->r->x;
            Tree->mtx.unlock();
            Tree->l->mtx.unlock();
            Tree->r->mtx.unlock();

            if (get_for_replace(node[0], TLx, TRx) == nullptr)
            {
                qDebug() << "OH NOOOOOOO";
//                Tree->r->l = Tree->l;     // добавить очистку памяти
//                qDebug() << &Tree;
//                this->Tree = this->Tree->r;
                not_nullptr(Tree);
                Tree->mtx.lock();
                not_nullptr(Tree->r);
                Tree->r->mtx.lock();
                Node<T, T2> *xx = this->Tree->r;
                this->Tree->x = xx->x;
                this->Tree->x2 = xx->x2;
//                this->Tree->l = this->Tree->r->l;
                this->Tree->r = xx->r;
                Tree->mtx.unlock();
                xx->mtx.unlock();

            }
            else
            {
                qDebug() << "OH NO11111111111";
                Node <T, T2> *a1 = get_for_replace(node[0], TLx, TRx);
                T xx = a1->x;
                T2 xx2 = a1->x2;
//            node[1]->mtx.unlock();
                del(xx);//НЕ ПУТАТЬ С delete!        // не вызывать саму себя, если не убедился что мьютексы сняты
                node[0]->x = xx;
                node[0]->x2 = xx2;
            }

//            qDebug() << "test " << get_for_replace(node[0], node[0])->x;
            return false;
        }
        not_nullptr(node[0]);
        not_nullptr(node[1]);
        if (node[0]->l == nullptr && node[0]->r == nullptr)
        {
            if (!node[1]->exist)
            {qDebug() << "1badly"; return false;}

//            qDebug() << "object without children" << endl;
            if (node[1]->l == node[0])
                node[1]->l = nullptr;
            if (node[1]->r == node[0])
                node[1]->r = nullptr;
            node[0]->exist = false;
            qDebug() << "del1 return" << key;
            return true;
        }
        if (node[0]->l == nullptr && node[0]->r != nullptr)
        {
            if (!node[1]->exist)
            {qDebug() << "2badly"; return false;}

            if (node[1]->l != node[0] && node[1]->r != node[0]) // ВАЖНЫЕ ПРОВЕРКИ
            {qDebug() << "OCHEN badly OBRATY VNIMANIE 4fix"; return false;}
            if (node[0]->x != key)                              // ВАЖНЫЕ ПРОВЕРКИ
            {qDebug() << "OCHEN badly OBRATY VNIMANIE 5fix"; return false;}

//            qDebug() << "object with only one RIGHT children" << endl;
            qDebug() << node[0]->x << node[1]->x;
            if (node[1]->l == node[0])
                node[1]->l = node[0]->r;
            if (node[1]->r == node[0])
                node[1]->r = node[0]->r;
            node[0]->exist = false;
            qDebug() << "del2 return" << key;
            return true;
        }
        if (node[0]->l != nullptr && node[0]->r == nullptr)
        {
            if (!node[1]->exist)
            {qDebug() << "3badly"; return false;}

            if (node[1]->l != node[0] && node[1]->r != node[0]) // ВАЖНЫЕ ПРОВЕРКИ
            {qDebug() << "OCHEN badly OBRATY VNIMANIE 4fix"; return false;}
            if (node[0]->x != key)                              // ВАЖНЫЕ ПРОВЕРКИ
            {qDebug() << "OCHEN badly OBRATY VNIMANIE 5fix"; return false;}

//            qDebug() << "object with only one LEFT children" << endl;
            if (node[1]->l == node[0])
                node[1]->l = node[0]->l;
            if (node[1]->r == node[0])
                node[1]->r = node[0]->l;
            node[0]->exist = false;
            qDebug() << "del3 return" << key;
            return true;
        }
        if (node[0]->l != nullptr && node[0]->r != nullptr && node[0] != node[1])
        {
            if (!node[1]->exist)
            {qDebug() << "4badly"; return false;}

            QMutexLocker node0l(&node[0]->l->mtx);
            QMutexLocker node0r(&node[0]->r->mtx);
            T MTLx = node[0]->l->x;
            T MTRx = node[0]->r->x;
            T MTx = node[0]->x;
            node1.unlock();
            node0.unlock();
            node0l.unlock();
            node0r.unlock();

            /*------ ALL UNLOCK! -------*/

            Node <T, T2> *a1 = get_for_replace(node[0], MTLx, MTRx);
            if (a1 == nullptr)
            {
                node1.relock();
                node0.relock();
                node0l.relock();
                node0r.relock();

                if (!node[1]->exist)
                {qDebug() << "5badly"; return false;}

                if (node[0]->l == nullptr || node[0]->r == nullptr || node[0] == node[1])
                    {qDebug() << "OCHEN badly OBRATY VNIMANIE 1lolfix"; return false;}
                if (node[1]->l != node[0] && node[1]->r != node[0]) // ВАЖНЫЕ ПРОВЕРКИ
                    {qDebug() << "OCHEN badly OBRATY VNIMANIE 1fix"; return false;}
                if (node[0]->x != key)                              // ВАЖНЫЕ ПРОВЕРКИ
                    {qDebug() << "OCHEN badly OBRATY VNIMANIE 2fix"; return false;}
                if (MTLx != node[0]->l->x || MTRx != node[0]->r->x || MTx != node[0]->x)// ВАЖНЫЕ ПРОВЕРКИ
                    {qDebug() << "OCHEN badly OBRATY VNIMANIE 3fix"; return false;}

                node[0]->r->l = node[0]->l;
                if (node[1]->l == node[0])
                    node[1]->l = node[0]->r;
                if (node[1]->r == node[0])
                    node[1]->r = node[0]->r;
                node[0]->exist = false;

                qDebug() << "del4 return" << key;
                return true;
            }
            else
            {
                QMutexLocker lockA1(&a1->mtx);
                T a1x = a1->x;
                lockA1.unlock();
/* a1 perent */ Node<T, T2> **nodeTemp = get_value_(a1x, Tree, Tree); // ЭТО ВСЕГО ЛИШЬ ПРЕДПОЛОЖЕНИЕ. ПРОВЕРАЙ ПОЛУЧЕННЫЕ ДАННЫЕ
                //блокируем клиента, его детей, заменителя, и его предка
                //а после проверяем все что можно
                //еще убедиться что не NULLPTR
                QMutexLocker nodeTemp0(&nodeTemp[0]->mtx);
                QMutexLocker nodeTemp1(&nodeTemp[1]->mtx);

                node1.relock();
                node0.relock();
                node0l.relock();
                node0r.relock();
                lockA1.relock();

                if (!node[1]->exist)
                {qDebug() << "6badly"; return false;}

                if (node[0]->l == nullptr || node[0]->r == nullptr || node[0] == node[1])
                {qDebug() << "OCHEN badly OBRATY VNIMANIE 2lolfix"; return false;}
                if (node[1]->l != node[0] && node[1]->r != node[0]) // ВАЖНЫЕ ПРОВЕРКИ
                {qDebug() << "OCHEN badly OBRATY VNIMANIE 4fix"; return false;}
                if (node[0]->x != key)                              // ВАЖНЫЕ ПРОВЕРКИ
                {qDebug() << "OCHEN badly OBRATY VNIMANIE 5fix"; return false;}
                if (MTLx != node[0]->l->x || MTRx != node[0]->r->x || MTx != node[0]->x)// ВАЖНЫЕ ПРОВЕРКИ
                {qDebug() << "OCHEN badly OBRATY VNIMANIE 6fix"; return false;}
                if (a1 != nodeTemp[0] || nodeTemp[0]->x != a1x)
                {qDebug() << "OCHEN badly OBRATY VNIMANIE 7fix"; return false;}
                if (nodeTemp[1]->l != nodeTemp[0] && nodeTemp[1]->r != nodeTemp[0])// ВАЖНЫЕ ПРОВЕРКИ
                {qDebug() << "OCHEN badly OBRATY VNIMANIE 8fix"; return false;}

                node[0]->x = nodeTemp[0]->x;
                node[0]->x2 = nodeTemp[0]->x2;

                if (nodeTemp[1]->l == nodeTemp[0])
                    nodeTemp[1]->l = nullptr;
                if (nodeTemp[1]->r == nodeTemp[0])
                    nodeTemp[1]->r = nullptr;
                nodeTemp[0]->exist = false;

                qDebug() << "del5 return" << key;
                return true;
            }
        }

        qDebug() << "is bed!!!!!!!!!!";
        }catch (...) {
            cout << "cool!";
        }
    }

    void add_node(T x, T2 x2)
    {try{
            add_node_(x, x2, Tree);
        }catch (...) {
            cout << "cool!";
        }
    }

    void add_node_(T x, T2 x2, Node<T, T2> *&MyTree) //Фукция добавления звена в дерево
    //между проверкой количества детей и добавленеим ребенка не должно оставаться разблакированным элемент.
    {try{

        if (nullptr == MyTree)  //То, о чем я в самом начале писал. Если дерева нет, то ложим семечко
        {
            MyTree = new Node<T, T2>; //Выделяем память под звено дерева
            MyTree->x = x; //Записываем данные в звено
            MyTree->x2 = x2;
            MyTree->deth = 0;
            MyTree->l = MyTree->r = nullptr; //Подзвенья инициализируем пустотой во избежание ошибок
        }
        else
        {
            if (x == MyTree->x)
            {
                qDebug() << "\n item alreade exist \n" << x;
            }
        }
        //предпологаемое место для лока мьютекса
        //ОБЯЗАТЕЛЬНО!!!!!!!!!!!!!!!
        //ОБЯЗАТЕЛЬНО!!!!!!!!!!!!!!!
        //между проверкой количества детей и добавленеим ребенка не должно оставаться разблакированным элемент.
        //между проверкой количества детей и добавленеим ребенка не должно оставаться разблакированным элемент.
        if (x < MyTree->x)   //Если нововведенный элемент x меньше чем элемент x из семечка дерева, уходим влево
        {
            if (MyTree->l != nullptr)
                add_node_(x, x2, MyTree->l); //При помощи рекурсии заталкиваем элемент на свободный участок
            else //Если элемент получил свой участок, то
            {
                Node<T, T2> *tempNode = new Node<T, T2>;  //Выделяем память левому подзвену. Именно подзвену, а не просто звену
                tempNode->l = tempNode->r = nullptr; //У левого подзвена будут свои левое и правое подзвенья, инициализируем их пустотой
                tempNode->x = x; //Записываем в левое подзвено записываемый элемент
                tempNode->x2 = x2;
                tempNode->deth = 0;
//                MyTree->mtx.lock();
                MyTree->l = tempNode;
//                MyTree->mtx.unlock();
            }
        }

        if (x > MyTree->x)   //Если нововведенный элемент x больше чем элемент x из семечка дерева, уходим вправо
        {
            if (MyTree->r != nullptr)
                add_node_(x, x2, MyTree->r); //При помощи рекурсии заталкиваем элемент на свободный участок
            else //Если элемент получил свой участок, то
            {
                Node<T, T2> *tempNode = new Node<T, T2>;  //Выделяем память правому подзвену. Именно подзвену, а не просто звену
                tempNode->l = tempNode->r = nullptr; //У правого подзвена будут свои левое и правое подзвенья, инициализируем их пустотой
                tempNode->x = x; //Записываем в правое подзвено записываемый элемент
                tempNode->x2 = x2;
                tempNode->deth = 0;
//                lock_guard<mutex> lock (MyTree->mtx);
//                MyTree->mtx.lock();
                MyTree->r = tempNode;
//                MyTree->mtx.unlock();
            }
        }
        }catch (...) {
            cout << "cool!";
        }
    }

    T2 get_value(T key)
    {try{
        Node<T, T2> **temp = nullptr;
        temp = get_value_(key, Tree, Tree);
        T2 xx2;
        if (temp != nullptr && temp[0] != nullptr)
        {
            //MUTEX!
            not_nullptr(temp[0]);
            temp[0]->mtx.lock();
            xx2 = temp[0]->x2;
            qDebug() << "\n Find! key = " << temp[0]->x << " val = " << temp[0]->x2 << endl;//MUTEX!
            temp[0]->mtx.unlock();
        }
        else
        {
            qDebug() << "\n Not find item with key = " << key << endl;
            exit(0);
        }
        return xx2;
        }catch (...) {
            cout << "cool!";
        }
    }

    Node<T, T2>** get_value_(T key, Node<T, T2> *&MyTree, Node<T, T2> *&ParentTree)
    {try{

            if (MyTree != nullptr)
            {;
                MyTree->mtx.lock();
            }
            else {
                qDebug() << "ATTENTION!!!!!!!!!!!!!!!!get_value_";
                exit(1);
            }
            if (MyTree->x == key)
            {
                //            cout << "AAAAAAAAAAAAA";
                Node<T, T2> **temp = new Node<T, T2>* [2] {nullptr, nullptr}; // первый эдемент с нужным ключем второй элемент родитель данного
                temp[0] = MyTree;
                temp[1] = ParentTree;
                MyTree->mtx.unlock();
                return temp;
            }
            else
            {
                Node<T, T2> *tempL = MyTree->l;
                Node<T, T2> *tempR = MyTree->r;
//                locker.unlock();
                MyTree->mtx.unlock();
                if (tempL != nullptr && get_value_(key, tempL, MyTree) != nullptr)
                {
                    return get_value_(key, tempL, MyTree);
                }
                if (tempR != nullptr && get_value_(key, tempR, MyTree) != nullptr)
                {
                    return get_value_(key, tempR, MyTree);
                }
            }
            return nullptr;
        }catch (...) {
            cout << "AXYEHHO";
        }
    }

            // ЗАБЛАКИРУЙ, СОЗДАЙ КОПИЮ. РАЗБЛАКИРУЙ, РАБОТАЙ С КОПИЕЙ. ЧТО ЗА ХУЕТУ ГОРОДИТЬ ТО
            // в функции del проверять что за время get_for_replace переданный node объект не был изменен
    Node<T, T2>* get_for_replace(Node<T, T2> *&MyTree, T Lx, T Rx)  // copyMyTree ссылка.
    {                                                                                   // если объект заменится
//        MyTree->mtx.lock();                                                             // изменится ссылка
        QMutexLocker locker(&MyTree->mtx);
        if (MyTree != nullptr)
        {;
//            MyTree->mtx.lock();
        }
        else {
            qDebug() << "ATTENTION!!!!!!!!!!!!!!!!get_for_replace";
            exit(1);
        }
        T MTx = MyTree->x;                                                               // в родительском элементе
        if (MyTree->l == nullptr && MyTree->r == nullptr)                               // может передавать ссылку
        {                                                                              // на родителя???????????
            if (MTx > Lx && MTx < Rx) // зачем вот эта *** с постоянными блокировками разблакировками если можно в параметрах к функции передать конкретно значения боль/меньше которых мы жолжны найти
            {
                return MyTree; // наложить на него мьютекс чтобы передать объект защищенный от редактирования
            }
            else
            {
                return nullptr;
            }
        }
        else
        {
            Node<T, T2> *tR = MyTree->r;
            Node<T, T2> *tL = MyTree->l;
            locker.unlock();
//            MyTree->mtx.unlock();
            Node<T, T2> *resR = nullptr;
            Node<T, T2> *resL = nullptr;
            if (tR != nullptr)
                resR = get_for_replace(tR, Lx, Rx);
            if (tL != nullptr)
                resL = get_for_replace(tL, Lx, Rx);
            if (tR != nullptr && resR != nullptr)
            {
                return resR;
            }
            if (tL != nullptr && resL != nullptr)
            {
                return resL;
            }
        }
//        MyTree->mtx.unlock(); // ????КАКОГО***????
        return nullptr;
    }
    void not_nullptr(Node<T, T2> *&MyTree, QString str = "")
    {
        if (MyTree == nullptr)
        {
//            Sleep(100);
            qDebug() << "NULLPTRNULLPTRNULLPTRNULLPTRNULLPTRNULLPTR" << str;
            exit(1);
        }
    }
    bool tl(Node<T, T2> *&MyTree, QVector <Node<T,T2>*> &vecLock) // try lock
    {
        if (MyTree == nullptr)
        {
            qDebug() << "g!1";
            return false;
        }
        else
        {
            if(vecLock.contains(MyTree))
            {
                qDebug() << "g!2";
                return true;
            }
            else
            {
                MyTree->mtx.lock();
                vecLock.push_back(MyTree);
            }
        }
        return true;
    }
    void tun(Node<T, T2> *&MyTree, QVector <Node<T,T2>*> &vecLock) // try unlock
    {
        if (MyTree == nullptr)
        {
            qDebug() << "g!3";
            return void();
        }
        else
        {
            if(!vecLock.contains(MyTree))
            {
                qDebug() << "g!4";
                return void();
            }
            else
            {
                MyTree->mtx.unlock();
                vecLock.removeOne(MyTree);
            }
        }
    }
    void unlock_all(QVector <Node<T,T2>*> &vecLock)
    {
        if (vecLock.size() != 0)
        {
            for (int i = 0; i < vecLock.size(); i++)
            {
//                qDebug() << "yes = " <<  vecLock.at(i)->x;
                vecLock.at(i)->mtx.unlock();
            }
        }
    }
    void test()
    {
        T a = 49; T b = 94;
//        Tree->mtx.lock();
//        Tree = nullptr;
        QMutexLocker locker(&Tree->mtx);
        Node <T, T2> * temp = Tree;
//        Tree->mtx.unlock();
        QMutexLocker locker1(&Tree->mtx);
        qDebug() << "strange";
        locker.unlock();
        locker1.unlock();
        Node <T, T2> * temp1 = get_for_replace(temp, a, b);
        Node <T, T2> * temp2 = get_value_(49, temp, temp)[0];
        qDebug() << "norm";
    }
};

int main()
{
    MyMap <int, double> *obj = new MyMap <int, double>;
    obj->add_node(81,1);
    obj->add_node(49,1);
    obj->add_node(34,1);
    obj->add_node(51,111);
    obj->add_node(52,1);
    obj->add_node(54,1);
    obj->add_node(53,1);
//    obj->add_node(51,1);
//    obj->add_node(50,0);
//    obj->add_node(50,0);
    obj->add_node(94,1);
    obj->add_node(93,1);
    obj->add_node(100,1);
    obj->add_node(199,1);
    obj->add_node(50,1);
    obj->add_node(51,1);

    obj->show();

    Sleep(50);

    thread t1 (&MyMap<int,double>::del, obj, 34);
    thread t2 (&MyMap<int,double>::del, obj, 49);
    thread t3 (&MyMap<int,double>::del, obj, 52);
    thread t4 (&MyMap<int,double>::del, obj, 51);
    thread t5 (&MyMap<int,double>::del, obj, 54);
    thread t6 (&MyMap<int,double>::del, obj, 50);

    t1.detach();
    t2.detach();
    t3.detach();
    t4.detach();
    t5.detach();
    t6.detach();

    Sleep(100);
    cout << "---------------------------------------------";

    obj->show();

}
// dead 81 51 50
