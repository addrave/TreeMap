#include <iostream>
#include <string>
#include <cstring>
#include <mutex>
#include <thread>
#include <QDebug>
#include <windows.h>

using namespace std;

template <class T, class T2>
class Node //Звено дерева
{public:
    T x;        //key
    T2 x2;      //val
    Node<T, T2> *l, *r;
    int deth;
    mutex mtx;
};

template <class T, class T2>
class MyMap
{public:
    Node<T, T2> *Tree;
    int depth;
    MyMap()
    {
        Tree = NULL;
        depth = 0;
    }
    void show()
    {
        this->depth = 0;
        count_deth(Tree, 0);
        cout << '\n' << "depth " << depth << '\n';
        show(Tree);
    }
    void show(Node<T, T2> *&Tree) //Функция обхода
    {
        if (Tree != NULL) //Пока не встретится пустое звено
        {
            show(Tree->l);
            for(int i = this->depth; i > Tree->deth; i--)
                cout << "\t";
//            cout << Tree->x << "-" << Tree->x2;
            cout << Tree->x;
            show(Tree->r);
        }
        else
        {
            cout <<  '\n';
        }
    }
    void approve()
    {
        vector <T> bufer;
        approve(Tree, bufer);
        for (int i=0; i < bufer.size() - 1; i++)
            if (bufer.at(i) > bufer.at(i+1))
            {
                qDebug() << "ATTENTION " << bufer.at(i);
                qDebug() << bufer;
                this->show();
                exit(0);
            }
    }
    void approve(Node<T, T2> *&MyTree, vector <T> &bufer)
    {
        if (MyTree != NULL) //Пока не встретится пустое звено
        {
            approve(MyTree->l, bufer);
            bufer.push_back(MyTree->x);
            approve(MyTree->r, bufer);
        }
    }
    void count_deth(Node<T, T2> *&Tree, int arg)
    {
        if (Tree != NULL)
        {
            arg = ++arg;
            Tree->deth = arg;
            if (arg > this->depth)
                this->depth = arg;
            count_deth(Tree->l, arg);
            count_deth(Tree->r, arg);
        }
    }

    void del(T key)
    {
        Node<T, T2> **temp = NULL;
        temp = get_value_(key, Tree, Tree);
        if (temp == NULL)
        {
            qDebug() << "ITEM NOT FOUND (DEl) whith key " << key << endl;
            return void();
        }
        if (temp[0] == Tree)
        {
            qDebug() << "ITEM HEAD OF THIS TREE" << endl;
            if (get_for_replace(temp[0], temp[0]) == NULL)
            {
//                Tree->r->l = Tree->l;     // добавить очистку памяти
//                qDebug() << &Tree;
//                this->Tree = this->Tree->r;
                this->Tree->x = this->Tree->r->x;
                this->Tree->x2 = this->Tree->r->x2;
//                this->Tree->l = this->Tree->r->l;
                this->Tree->r = this->Tree->r->r;
//                this->Tree->deth = this->Tree->r->deth;
//                delete Tree->r;
//                qDebug() << &Tree;
//                Tree->x = 1111;
//                qDebug() << "ATTANTION i can not replace item because can not find a replacement for it " << temp[0]->x;
//                this->show();
//                exit(0);
            }

//            qDebug() << "test " << get_for_replace(temp[0], temp[0])->x;
            return void();
        }
        temp[0]->mtx.lock();
        temp[1]->mtx.lock();
        qDebug() << '\n' << temp[0]->x << '\t' << temp[1]->x << '\t';
        if (temp[0]->l == NULL && temp[0]->r == NULL)
        {
            qDebug() << "object without children" << endl;
            if (temp[1]->l == temp[0])
                temp[1]->l = NULL;
            if (temp[1]->r == temp[0])
                temp[1]->r = NULL;
            temp[0]->mtx.unlock();
            temp[1]->mtx.unlock();
            delete temp[0];
            temp[0] = NULL;
            return void();
        }
        if (temp[0]->l == NULL && temp[0]->r != NULL)
        {
            qDebug() << "object with only one RIGHT children" << endl;
            if (temp[1]->l == temp[0])
                temp[1]->l = temp[0]->r;
            if (temp[1]->r == temp[0])
                temp[1]->r = temp[0]->r;
            temp[0]->mtx.unlock();
            temp[1]->mtx.unlock();
            delete temp[0];
            temp[0] = NULL;
            return void();
        }
        if (temp[0]->l != NULL && temp[0]->r == NULL)
        {
            qDebug() << "object with only one LEFT children" << endl;
            if (temp[1]->l == temp[0])
                temp[1]->l = temp[0]->l;
            if (temp[1]->r == temp[0])
                temp[1]->r = temp[0]->l;
            temp[0]->mtx.unlock();
            temp[1]->mtx.unlock();
            delete temp[0];
            temp[0] = NULL;
            return void();
        }
        if (temp[0]->l != NULL && temp[0]->r != NULL && temp[0] != temp[1])
        {
            temp[0]->mtx.unlock();
            temp[1]->mtx.unlock();
            qDebug() << "OBJECT WHITH 2 CHILD AND PARENT";
            if (get_for_replace(temp[0], temp[0]) == NULL)
            {
//                qDebug() << "ATTANTION i can not replace item because can not find a replacement for it " << temp[0]->x;
//                this->show();
//                exit(0);
                temp[0]->r->l = temp[0]->l;
                if (temp[1]->l == temp[0])
                    temp[1]->l = temp[0]->r;
                if (temp[1]->r == temp[0])
                    temp[1]->r = temp[0]->r;
            }
            else
            {
                Node <T, T2> *a1 = get_for_replace(temp[0], temp[0]);
                T xx = a1->x;
                T2 xx2 = a1->x2;
//            temp[1]->mtx.unlock();
                del(xx);        // не вызывать саму себя, если не убедился что мьютексы сняты
                temp[0]->x = xx;
                temp[0]->x2 = xx2;
            }

//            T deletedX = temp[0]->x;
//            Node<T, T2> node;
//            while (node.)
//            {

//            }
        }
//        if (Tree != NULL) //Пока не встретится пустое звено
//        {
//            del(Tree->l); //Рекурсивная функция прохода по левому поддереву
//            del(Tree->r); //Рекурсивная функци для прохода по правому поддереву
//            delete Tree; //Убиваем конечный элемент дерева
//            Tree = NULL; //Может и не обязательно, но плохого не будет
//        }
        temp[0]->mtx.unlock();
        temp[1]->mtx.unlock();
    }

    void add_node(T x, T2 x2)
    {
        add_node_(x, x2, Tree);
    }

    void add_node_(T x, T2 x2, Node<T, T2> *&MyTree) //Фукция добавления звена в дерево
    {

        if (NULL == MyTree)  //То, о чем я в самом начале писал. Если дерева нет, то ложим семечко
        {
            MyTree = new Node<T, T2>; //Выделяем память под звено дерева
            MyTree->x = x; //Записываем данные в звено
             MyTree->x2 = x2;
            MyTree->l = MyTree->r = NULL; //Подзвенья инициализируем пустотой во избежание ошибок
        }
        else
        {
            if (x == MyTree->x)
            {
                cout << "\n item alreade exist \n" << x;
            }
        }
        //предпологаемое место для лока мьютекса
        if (x < MyTree->x)   //Если нововведенный элемент x меньше чем элемент x из семечка дерева, уходим влево
        {
            if (MyTree->l != NULL)
                add_node_(x, x2, MyTree->l); //При помощи рекурсии заталкиваем элемент на свободный участок
            else //Если элемент получил свой участок, то
            {
                Node<T, T2> *tempNode = new Node<T, T2>;  //Выделяем память левому подзвену. Именно подзвену, а не просто звену
                tempNode->l = tempNode->r = NULL; //У левого подзвена будут свои левое и правое подзвенья, инициализируем их пустотой
                tempNode->x = x; //Записываем в левое подзвено записываемый элемент
                tempNode->x2 = x2;
                MyTree->mtx.lock();
                MyTree->l = tempNode;
                MyTree->mtx.unlock();
            }
        }

        if (x > MyTree->x)   //Если нововведенный элемент x больше чем элемент x из семечка дерева, уходим вправо
        {
            if (MyTree->r != NULL)
                add_node_(x, x2, MyTree->r); //При помощи рекурсии заталкиваем элемент на свободный участок
            else //Если элемент получил свой участок, то
            {
                Node<T, T2> *tempNode = new Node<T, T2>;  //Выделяем память правому подзвену. Именно подзвену, а не просто звену
                tempNode->l = tempNode->r = NULL; //У правого подзвена будут свои левое и правое подзвенья, инициализируем их пустотой
                tempNode->x = x; //Записываем в правое подзвено записываемый элемент
                tempNode->x2 = x2;
//                lock_guard<mutex> lock (MyTree->mtx);
                MyTree->mtx.lock();
                MyTree->r = tempNode;
                MyTree->mtx.unlock();
            }
        }

    }

    T2 get_value(T key)
    {
        Node<T, T2> **temp = NULL;
        temp = get_value_(key, Tree, Tree);
//        cout << temp[0]->x << "AAAAAAAAAAA";
        if (temp != NULL)
        {
            //MUTEX!
            qDebug() << "\n Find! key = " << temp[0]->x << " val = " << temp[0]->x2 << endl;//MUTEX!
        }
        else
            cout << "\n Not find item with key = " << key << endl;
        //MUTEX!
        return temp[0]->x;//MUTEX!
    }

    Node<T, T2>** get_value_(T key, Node<T, T2> *&MyTree, Node<T, T2> *&ParentTree)
    {
//        cout << MyTree->x << "aa";
        MyTree->mtx.lock();
        if (MyTree->x == key)
        {
//            cout << "AAAAAAAAAAAAA";
            Node<T, T2> **temp = new Node<T, T2>* [2] {NULL, NULL}; // первый эдемент с нужным ключем второй элемент родитель данного
            temp[0] = MyTree;
            temp[1] = ParentTree;
            MyTree->mtx.unlock();
            return temp;
        }
        else
        {
            Node<T, T2> *tempL = MyTree->l;
            Node<T, T2> *tempR = MyTree->r;
            MyTree->mtx.unlock();
            if (tempL != NULL && get_value_(key, tempL, MyTree) != NULL)
            {
                return get_value_(key, tempL, MyTree);
            }
            if (tempR != NULL && get_value_(key, tempR, MyTree) != NULL)
            {
                return get_value_(key, tempR, MyTree);
            }
        }
        return NULL;
    }
    Node<T, T2>* get_for_replace(Node<T, T2> *&MyTree, Node<T, T2> *&copyMyTree)  // copyMyTree ссылка.
    {                                                                                   // если объект заменится
//        MyTree->mtx.lock();                                                             // изменится ссылка
//        qDebug() << MyTree->x;
        if (MyTree->l == NULL && MyTree->r == NULL)                                     // в родительском элементе
        {                                                                               // может передавать ссылку
//            copyMyTree->mtx.lock();                                                     // на родителя???????????
//            copyMyTree->l->mtx.lock();
//            copyMyTree->r->mtx.lock();
            if (MyTree->x > copyMyTree->l->x && MyTree->x < copyMyTree->r->x)
            {
//                qDebug() << "     qqqqqqqqq" << copyMyTree->l->x << " " << copyMyTree->r->x;
//                MyTree->mtx.unlock();
//                copyMyTree->mtx.unlock();
//                copyMyTree->l->mtx.unlock();
//                copyMyTree->r->mtx.unlock();
                return MyTree;
            }
            else
            {
//                MyTree->mtx.unlock();
//                copyMyTree->mtx.unlock();
//                copyMyTree->l->mtx.unlock();
//                copyMyTree->r->mtx.unlock();
                return NULL;
            }
        }
        else
        {
            Node<T, T2> *tempL = MyTree->l;
            Node<T, T2> *tempR = MyTree->r;
//            MyTree->mtx.unlock();
            if (tempR != NULL && get_for_replace(tempR, copyMyTree) != NULL)
            {
                return get_for_replace(tempR, copyMyTree);
            }
            if (tempL != NULL && get_for_replace(tempL, copyMyTree) != NULL)
            {
                return get_for_replace(tempL, copyMyTree);
            }
        }
//        MyTree->mtx.unlock();
//        return NULL;
    }
};

int main()
{
    MyMap <double, double> obj;
    obj.add_node(81,1);
    obj.add_node(49,1);
//    obj.add_node(34,1);
//    obj.add_node(51,111);
//    obj.add_node(52,1);
//    obj.add_node(54,1);
//    obj.add_node(53,1);
//    obj.add_node(51,1);
////    obj.add_node(50,0);
//    obj.add_node(50,0);
//    obj.add_node(94,1);
//    obj.add_node(93,1);
    obj.add_node(100,1);
    obj.add_node(101,1);
    obj.add_node(4,1);
    obj.show();
//    qDebug() << obj.get_value(52);
//    thread t1 (&MyMap<int,int>::add_node, obj, 1, 1);
//    thread t2 (&MyMap<int,int>::add_node, obj, 1, 1);
//    t1.join();
//    t2.join();
    thread t1 (&MyMap<double,double>::del, obj, 81);
    Sleep(1000);
    obj.show();
    thread t2 (&MyMap<double,double>::del, obj, 49);
//    thread t3 (&MyMap<double,double>::del, obj, 81);
    t1.join();
    t2.join();
//    t3.join();
//    obj.Tree->l->r->r->r->l->x = 1;
//    obj.del(81);
    qDebug() << obj.Tree->x;
    obj.show();
    obj.show();
    obj.approve();

//    cout << ("11kkj0" < "1kkj0") ? true : false;
//    cout << strcmp("11kkj0", "1kkj0") ? true:false;
//    cout << '\n';
}
