#include <iostream>
#include <string>
#include <cstring>
#include <mutex>
#include <thread>
#include <QDebug>

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
            cout << Tree->x;;
            show(Tree->r);
        }
        else
        {
            cout <<  '\n';
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

    void del(Node<T, T2> *&Tree) {
        if (Tree != NULL) //Пока не встретится пустое звено
        {
            del(Tree->l); //Рекурсивная функция прохода по левому поддереву
            del(Tree->r); //Рекурсивная функци для прохода по правому поддереву
            delete Tree; //Убиваем конечный элемент дерева
            Tree = NULL; //Может и не обязательно, но плохого не будет
        }

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
        pair<T2, bool> temp;
        temp = get_value_(key, Tree);
        if (temp.second == true)
            qDebug() << "\n Find! key = " << key << " val = " << temp.first << endl;
        else
            cout << "\n Not find item with key = " << key << endl;
        return temp.first;
    }

    pair<T2, bool> get_value_(T key, Node<T, T2> *&MyTree)
    {
        MyTree->mtx.lock();
        if (MyTree->x == key)
        {
            pair<T2, bool> temp; temp.first = MyTree->x2; temp.second = true;
            MyTree->mtx.unlock();
            return temp;
        }
        else
        {
            Node<T, T2> *tempL = MyTree->l;
            Node<T, T2> *tempR = MyTree->r;
            MyTree->mtx.unlock();
            if (tempL != NULL && get_value_(key, tempL).second == true)
            {
                return get_value_(key, tempL);
            }
            if (tempR != NULL && get_value_(key, tempR).second == true)
            {
                return get_value_(key, tempR);
            }
        }
        pair<T2, bool> temp; temp.first = NULL; temp.second = false;
        return temp;
    }

};

int main()
{
    MyMap <int, int> obj;
    obj.add_node(81,1);
    obj.add_node(49,1);
    obj.add_node(34,1);
    obj.add_node(51,111);
    obj.add_node(52,1);
    obj.add_node(53,1);
//    obj.add_node(50,0);
    obj.add_node(50,0);
    obj.add_node(94,1);
    obj.add_node(93,1);
    obj.add_node(100,1);
//    obj.show();
//    qDebug() << obj.get_value(52);
//    thread t1 (&MyMap<int,int>::add_node, obj, 1, 1);
//    thread t2 (&MyMap<int,int>::add_node, obj, 1, 1);
//    t1.join();
//    t2.join();
    thread t1 (&MyMap<int,int>::get_value, obj, 50);
    thread t2 (&MyMap<int,int>::get_value, obj, 51);
    thread t3 (&MyMap<int,int>::get_value, obj, 510);
    t1.join();
    t2.join();
    t3.join();

//    obj.show();

//    cout << ("11kkj0" < "1kkj0") ? true : false;
//    cout << strcmp("11kkj0", "1kkj0") ? true:false;
//    cout << '\n';
}
