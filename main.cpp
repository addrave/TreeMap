#include <iostream>
#include <string>
#include <cstring>
#include <mutex>
#include <thread>
#include <QDebug>
#include <windows.h>
//стереть все делиты нахуй!!!!!!!!!!!!!!!!!!!! чтобы они не удаляли объекты в которые уже открыты на чтение в других тредах
//нельзя блокирывать мьютексы потенциально НУЛЬПТРные
// не так страшно обращаться к полям удаленного объекта, как страшно юзать мьютекс удаленного объекта
// если в функцию попадает NULLPRT и у него функция попытается заблокировать мьютекс, ТОБИ ПИЗДА
using namespace std;

template <class T, class T2>
class Node //Звено дерева
{public:
    T x;        //key
    T2 x2;      //val
    Node<T, T2> *l, *r;
    int deth = 0;
    mutex mtx;
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
//            mainMtx.lock(); // только один поток может юзать эту функцию
            this->depth = 0;
            count_deth(Tree, 0);
//            mainMtx.unlock();
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
//                this->mainMtx.lock();
                for(int i = this->depth; i > MyTree->deth; i--)
                    cout << "\t";
                //            cout << MyTree->x << "-" << MyTree->x2;
//                this->mainMtx.unlock();
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
            cout << "AXYEHHO";
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
    {try{
        QVector <Node<T,T2>*> vL;
        Node<T, T2> **node = nullptr;
        node = get_value_(key, Tree, Tree);
        if (node == nullptr)
        {
            qDebug() << "ITEM NOT FOUND (DEl) whith key " << key << endl;
            return void();
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
                qDebug() << "FUCKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK";
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
//                delete xx;
//                this->Tree->deth = this->Tree->r->deth;
//                qDebug() << &Tree;
//                Tree->x = 1111;
//                qDebug() << "ATTANTION i can not replace item because can not find a replacement for it " << node[0]->x;
//                this->show();
//                exit(0);
            }
            else
            {
                qDebug() << "FUCK11111111111111111111111111111111111111111111111111111";
                Node <T, T2> *a1 = get_for_replace(node[0], TLx, TRx);
                T xx = a1->x;
                T2 xx2 = a1->x2;
//            node[1]->mtx.unlock();
                del(xx);//НЕ ПУТАТЬ С delete!        // не вызывать саму себя, если не убедился что мьютексы сняты
                node[0]->x = xx;
                node[0]->x2 = xx2;
            }

//            qDebug() << "test " << get_for_replace(node[0], node[0])->x;
            return void();
        }
        not_nullptr(node[0]);
        not_nullptr(node[1]);
//        node[0]->mtx.lock();
//        node[1]->mtx.lock();
        if (!tl(node[0], vL)) {unlock_all(vL); del(key); return void();}
        if (!tl(node[1], vL)) {unlock_all(vL); del(key); return void();}
        if (node[1]->l != node[0] && node[1]->r != node[0]) // ВАЖНЫЕ ПРОВЕРКИ
            {unlock_all(vL); del(key); return void();
            qDebug() << "OCHEN HUEVO OBRATY VNIMANIE fix";}
        if (node[0]->x != key)                              // ВАЖНЫЕ ПРОВЕРКИ
            {unlock_all(vL); del(key); return void();
            qDebug() << "OCHEN HUEVO OBRATY VNIMANIE fix";}
        if (node[0]->x == key)
            qDebug() << "<debug> example of output. </debug>";
//        qDebug() << '\n' << node[0]->x << '\t' << node[1]->x << '\t';
        if (node[0]->l == nullptr && node[0]->r == nullptr)
        {
//            qDebug() << "object without children" << endl;
            if (node[1]->l == node[0])
                node[1]->l = nullptr;
            if (node[1]->r == node[0])
                node[1]->r = nullptr;
            node[0]->mtx.unlock();
            node[1]->mtx.unlock();
//            delete node[0];
//            node[0] = nullptr;
            unlock_all(vL);
            return void();
        }
        if (node[0]->l == nullptr && node[0]->r != nullptr)
        {
//            qDebug() << "object with only one RIGHT children" << endl;
            if (node[1]->l == node[0])
                node[1]->l = node[0]->r;
            if (node[1]->r == node[0])
                node[1]->r = node[0]->r;
            node[0]->mtx.unlock();
            node[1]->mtx.unlock();
//            delete node[0];
//            node[0] = nullptr;
            unlock_all(vL);
            return void();
        }
        if (node[0]->l != nullptr && node[0]->r == nullptr)
        {
//            qDebug() << "object with only one LEFT children" << endl;
            if (node[1]->l == node[0])
                node[1]->l = node[0]->l;
            if (node[1]->r == node[0])
                node[1]->r = node[0]->l;
            node[0]->mtx.unlock();
            node[1]->mtx.unlock();
//            delete node[0];
//            node[0] = nullptr;
            unlock_all(vL);
            return void();
        }
        if (node[0]->l != nullptr && node[0]->r != nullptr && node[0] != node[1])
        {
//            node[1]->mtx.unlock();
            tun(node[1],vL);

//            not_nullptr(node[0]->l);
//            node[0]->l->mtx.lock();
            if (!tl(node[0]->l,vL)) {unlock_all(vL); del(key); return void();}
//            not_nullptr(node[0]->r);
//            node[0]->r->mtx.lock();
            if (!tl(node[0]->r,vL)) {unlock_all(vL); del(key); return void();}
            T MTLx = node[0]->l->x;
            T MTRx = node[0]->r->x;
            T MTx = node[0]->x;
//            node[0]->l->mtx.unlock();
            tun(node[0]->l, vL);
//            node[0]->r->mtx.unlock();
            tun(node[0]->r, vL);
//            node[0]->mtx.unlock();
            tun(node[0], vL);
//            qDebug() << "OBJECT WHITH 2 CHILD AND PARENT";
            qDebug() << vL.size() << " = MUST BE NULL";
            Node <T, T2> *a1 = get_for_replace(node[0], MTLx, MTRx);
            if (a1 == nullptr)
            {
//                qDebug() << "ATTANTION i can not replace item because can not find a replacement for it " << node[0]->x;
//                this->show();
//                exit(0);
                //а вот здесь мы будем проверять не изменились ли данные за время выполнения get_for_replace. проверять что они до сих пор валидны
                // блокировать node[1]
//                not_nullptr(node[1], "node[1]");
//                node[1]->mtx.lock();
                if (!tl(node[1], vL)) {unlock_all(vL); del(key); return void();}
//                not_nullptr(node[0], "node[0]");
//                node[0]->mtx.lock();
                if (!tl(node[0], vL)) {unlock_all(vL); del(key); return void();}
//                not_nullptr(node[0]->l, "node[0]->l");
//                node[0]->l->mtx.lock();
                if (!tl(node[0]->l, vL)) {unlock_all(vL); del(key); return void();}
//                not_nullptr(node[0]->r, "node[0]->r");
//                node[0]->r->mtx.lock();
                if (!tl(node[0]->r, vL)) {unlock_all(vL); del(key); return void();}
                //MUTEX
                if (node[1]->l != node[0] && node[1]->r != node[0]) // ВАЖНЫЕ ПРОВЕРКИ
                    {unlock_all(vL); del(key); return void();
                    qDebug() << "OCHEN HUEVO OBRATY VNIMANIE fix";}
                if (node[0]->x != key)                              // ВАЖНЫЕ ПРОВЕРКИ
                    {unlock_all(vL); del(key); return void();
                    qDebug() << "OCHEN HUEVO OBRATY VNIMANIE fix";}
                if (MTLx != node[0]->l->x || MTRx != node[0]->r->x || MTx != node[0]->x)     // ВАЖНЫЕ ПРОВЕРКИ
                    {unlock_all(vL); del(key); return void();
                    qDebug() << "OCHEN HUEVO OBRATY VNIMANIE fix";}
                node[0]->r->l = node[0]->l;
                if (node[1]->l == node[0])
                    node[1]->l = node[0]->r;
                if (node[1]->r == node[0])
                    node[1]->r = node[0]->r;

//                node[1]->mtx.unlock();
                tun(node[1], vL);
//                node[0]->mtx.lock();
                if (!tl(node[0], vL)) {unlock_all(vL); del(key); return void();}
//                node[0]->l->mtx.lock();
                if (!tl(node[0]->l, vL)) {unlock_all(vL); del(key); return void();}
//                node[0]->r->mtx.lock();
                if (!tl(node[0]->r, vL)) {unlock_all(vL); del(key); return void();}
            }
            else
            {
//                not_nullptr(a1,"a1");
//                a1->mtx.lock();
                if (!tl(a1, vL)) {unlock_all(vL); del(key); return void();}
//                if (!vecLockMtx.contains(a1)) {a1->mtx.lock(); vecLockMtx.push_back(a1);} else qDebug() << "FUCKA1";
                T a1x = a1->x;
//                a1->mtx.unlock();// ТАК ЧТО ПРОВЕРИТЬ ЕГО ПОДЛИННОСТЬ!!!!!
                tun(a1, vL);
                qDebug() << vL.size() << " = MUST BE NULL";
                Node<T, T2> **nodeTemp = get_value_(a1x, Tree, Tree); // ЭТО ВСЕГО ЛИШЬ ПРЕДПОЛОЖЕНИЕ. ПРОВЕРАЙ ПОЛУЧЕННЫЕ ДАННЫЕ
/*                not_nullptr(node[0], "node[0]->");
//                if (!vecLockMtx.contains(node[0])) node[0]->mtx.lock(); else qDebug() << "FUCKA1";
                node[0]->mtx.lock();*/                            // node[0]
                if (!tl(node[0], vL)) {unlock_all(vL); del(key); return void();}
/*                not_nullptr(node[0]->r, "node[0]->r->");
                node[0]->r->mtx.lock();  */                       // node[0]->r
                if (!tl(node[0]->r, vL)) {unlock_all(vL); del(key); return void();}
/*                not_nullptr(node[0]->l,"node[0]->l->");
                node[0]->l->mtx.lock();  */                       // node[0]->l
                if (!tl(node[0]->l, vL)) {unlock_all(vL); del(key); return void();}
//                if (try_mtx_lock(node[0]->l) == false) {a1->mtx.unlock(); node[0]->mtx.unlock(); node[0]->r->mtx.unlock(); del(key); return void();} // разблокировать все мьютексы
                if (a1 != node[0] && a1 != node[0]->r && a1 != node[0]->l)
                {
//                    not_nullptr(a1,"a1->");
//                    a1->mtx.lock();                             // a1
                    if (!tl(a1, vL)) {unlock_all(vL); del(key); return void();}
                }
                else
                {unlock_all(vL); del(key); return void();
                    qDebug() << "ERROR!!!";
                    exit(0);
                }


                if (node[1]->l != node[0] && node[1]->r != node[0]) // ВАЖНЫЕ ПРОВЕРКИ
                    {unlock_all(vL); del(key); return void();
                    qDebug() << "OCHEN HUEVO OBRATY VNIMANIE fix";}
                if (node[0]->x != key)                              // ВАЖНЫЕ ПРОВЕРКИ
                    {unlock_all(vL); del(key); return void();
                    qDebug() << "OCHEN HUEVO OBRATY VNIMANIE fix";}
                if (MTLx != node[0]->l->x || MTRx != node[0]->r->x || MTx != node[0]->x)     // ВАЖНЫЕ ПРОВЕРКИ
                    {unlock_all(vL); del(key); return void();
                    qDebug() << "OCHEN HUEVO OBRATY VNIMANIE fix";}
                if (a1 != nodeTemp[0] || nodeTemp[0]->x != a1x)
                    {unlock_all(vL); del(key); return void();
                    qDebug() << "OCHEN HUEVO OBRATY VNIMANIE fix";}

                if (MTLx == node[0]->l->x && MTRx == node[0]->r->x && MTx == node[0]->x)
                {
                    bool flag = false;
                    if (node[0]->l == nodeTemp[1] || node[0]->r == nodeTemp[1])
                    {
                        flag = true;
                        qDebug() << "NORMPLAN";
                    }
                    else
                    {
//                        not_nullptr(nodeTemp[1], "nodeTemp[1]");
//                        nodeTemp[1]->mtx.lock();                // nodeTemp[1]
                        if (!tl(nodeTemp[1], vL)) {unlock_all(vL); del(key); return void();}
                    }
//                    nodeTemp[0]->mtx.lock();                    // nodeTemp[0] незлья ведь a1 == nodeTemp[0]!!!
                    if (a1 == nodeTemp[0] && nodeTemp[0]->x == a1x)
                    {
                        node[0]->x = nodeTemp[0]->x;
                        node[0]->x2 = nodeTemp[0]->x2;

                        if (nodeTemp[1]->l == nodeTemp[0])
                            nodeTemp[1]->l = nullptr;
                        if (nodeTemp[1]->r == nodeTemp[0])
                            nodeTemp[1]->r = nullptr;

                        if (flag == false)
                        {
//                            nodeTemp[1]->mtx.unlock();
                            tun(nodeTemp[1], vL);
                        }
//                        a1->mtx.unlock();
                        tun(a1, vL);
//                        node[0]->mtx.unlock();
                        tun(node[0], vL);
//                        node[0]->r->mtx.unlock();
                        tun(node[0]->r, vL);
//                        node[0]->l->mtx.unlock();
                        tun(node[0]->l, vL);
                    }
                    else
                    {unlock_all(vL); del(key); return void();
                        qDebug() << "ERRORNODETEMP0"; //изменися элемент за время операций
                        exit(0);
                    }
                }
                else
                {unlock_all(vL); del(key); return void();
                    qDebug() << "PIZDEC";//разлочить всё и пустить по новой
                    exit(1);
                }
            }
//            esle
//            {
//                not_nullptr(a1);
//                a1->mtx.lock();
//                T xx = a1->x;
//                T2 xx2 = a1->x2;
//                a1->mtx.unlock();
//                Node<T, T2> **nodeTemp = get_value_(xx, Tree, Tree);
////                a1->mtx.lock();
//                not_nullptr(node[0]);
//                node[0]->mtx.lock();
//                not_nullptr(node[0]->l);
//                node[0]->l->mtx.lock();
//                not_nullptr(node[0]->r);
//                node[0]->r->mtx.lock();
//                if (MTLx == node[0]->l->x && MTRx == node[0]->r->x)
//                {
//                    node[0]->mtx.unlock();
//                    node[0]->l->mtx.unlock();
//                    node[0]->r->mtx.unlock();
//                    not_nullptr(nodeTemp[0]);
//                    nodeTemp[0]->mtx.lock();
//                    not_nullptr(nodeTemp[1]);
//                    nodeTemp[1]->mtx.lock();
//                    if (nodeTemp[1]->l == nodeTemp[0])
//                        nodeTemp[1]->l = nullptr;
//                    if (nodeTemp[1]->r == nodeTemp[0])
//                        nodeTemp[1]->r = nullptr;
//                    nodeTemp[1]->mtx.unlock();
//                    nodeTemp[0]->mtx.unlock();

//                    not_nullptr(node[0]);
//                    node[0]->mtx.lock();
//                    node[0]->x = xx;
//                    node[0]->x2 = xx2;
//                    node[0]->mtx.unlock();
//                }
//                else
//                {
//                    qDebug() << "FUCKINDEL";
////                    exit(1);
//                }
////            node[1]->mtx.unlock();
////                del(xx);//НЕ ПУТАТЬ С delete!        // не вызывать саму себя, если не убедился что мьютексы сняты
//            }

//            T deletedX = node[0]->x;
//            Node<T, T2> node;
//            while (node.)
//            {

//            }
//            if (vL.size() != 0)
//            {
//                for (int i = 0; i < vL.size(); i++)
//                {
//                    qDebug() << "HUEVO = " <<  vL.at(i)->x;
//                    vL.at(i)->mtx.unlock();
//                }

//            }
            unlock_all(vL); del(key); return void();
            return void();
        }
        unlock_all(vL); del(key); return void();
        qDebug() << "VERY BAD";
        node[0]->mtx.unlock();
        node[1]->mtx.unlock();
        }catch (...) {
            cout << "AXYEHHO";
        }
    }

    void add_node(T x, T2 x2)
    {try{
            add_node_(x, x2, Tree);
        }catch (...) {
            cout << "AXYEHHO";
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
                MyTree->mtx.lock();
                MyTree->l = tempNode;
                MyTree->mtx.unlock();
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
                MyTree->mtx.lock();
                MyTree->r = tempNode;
                MyTree->mtx.unlock();
            }
        }
        }catch (...) {
            cout << "AXYEHHO";
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
            cout << "AXYEHHO";
        }
    }

    Node<T, T2>** get_value_(T key, Node<T, T2> *&MyTree, Node<T, T2> *&ParentTree)
    {try{
            //        cout << MyTree->x << "aa";
            if (MyTree != nullptr)
                MyTree->mtx.lock();
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
            // ЗАБЛАКИРУЙ, СОЗДАЙ КОПИЮ. РАЗБЛАКИРУЙ, РАБОТАЙ С КОПИЕЙ. ЧТО ЗА ХУЕТУ ГОРОДИТЬ ТО
            // ЗАБЛАКИРУЙ, СОЗДАЙ КОПИЮ. РАЗБЛАКИРУЙ, РАБОТАЙ С КОПИЕЙ. ЧТО ЗА ХУЕТУ ГОРОДИТЬ ТО
            // ЗАБЛАКИРУЙ, СОЗДАЙ КОПИЮ. РАЗБЛАКИРУЙ, РАБОТАЙ С КОПИЕЙ. ЧТО ЗА ХУЕТУ ГОРОДИТЬ ТО
            // ЗАБЛАКИРУЙ, СОЗДАЙ КОПИЮ. РАЗБЛАКИРУЙ, РАБОТАЙ С КОПИЕЙ. ЧТО ЗА ХУЕТУ ГОРОДИТЬ ТО
            // ЗАБЛАКИРУЙ, СОЗДАЙ КОПИЮ. РАЗБЛАКИРУЙ, РАБОТАЙ С КОПИЕЙ. ЧТО ЗА ХУЕТУ ГОРОДИТЬ ТО
            // ЗАБЛАКИРУЙ, СОЗДАЙ КОПИЮ. РАЗБЛАКИРУЙ, РАБОТАЙ С КОПИЕЙ. ЧТО ЗА ХУЕТУ ГОРОДИТЬ ТО
            // ЗАБЛАКИРУЙ, СОЗДАЙ КОПИЮ. РАЗБЛАКИРУЙ, РАБОТАЙ С КОПИЕЙ. ЧТО ЗА ХУЕТУ ГОРОДИТЬ ТО
            // в функции del проверять что за время get_for_replace переданный node объект не был изменен
    Node<T, T2>* get_for_replace(Node<T, T2> *&MyTree, T &Lx, T &Rx)  // copyMyTree ссылка.
    {                                                                                   // если объект заменится
//        MyTree->mtx.lock();                                                             // изменится ссылка
        if (MyTree != nullptr)
            MyTree->mtx.lock();
        else {
            qDebug() << "ATTENTION!!!!!!!!!!!!!!!!get_for_replace";
            exit(1);
        }
        T MTx = MyTree->x;                                                               // в родительском элементе
        if (MyTree->l == nullptr && MyTree->r == nullptr)                               // может передавать ссылку
        {                                                                              // на родителя???????????
            MyTree->mtx.unlock();


            if (MTx > Lx && MTx < Rx) // нахуя вот эта ебля с постоянными блокировками разблакировками если можно в параметрах к функции передать конкретно значения боль/меньше которых мы жолжны найти
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
            MyTree->mtx.unlock();
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
//        MyTree->mtx.unlock(); // ????КАКОГОХУЯ????
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
//    void test()
//    {
//        T a = 49; T b = 94;
//        qDebug() << get_for_replace(Tree, a, b)->x;
//        qDebug() << get_value_(49, Tree, Tree)[0]->x << get_value_(49, Tree, Tree)[1]->x;
//        qDebug() << "norm";
//    }
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
//    Sleep(100);
//    obj->show();
//    qDebug() << obj.get_value(52);
//    thread t1 (&MyMap<int,int>::add_node, obj, 1, 1);
//    thread t2 (&MyMap<int,int>::add_node, obj, 1, 1);
//    t1.join();
//    t2.join();
    Sleep(100);
    obj->show();
    thread t1 (&MyMap<int,double>::del, obj, 49);
    thread t2 (&MyMap<int,double>::del, obj, 51);
    thread t3 (&MyMap<int,double>::del, obj, 52);
    thread t4 (&MyMap<int,double>::del, obj, 34);
    thread t5 (&MyMap<int,double>::del, obj, 54);
    thread t6 (&MyMap<int,double>::del, obj, 50);
//    thread t7 (&MyMap<int,double>::del, obj, 50);
//    t1.join();
//    t2.join();
//    t3.join();
//    t4.join();
    t1.detach();
    t2.detach();
    t3.detach();
    t4.detach();
    t5.detach();
    t6.detach();
//    t7.detach();
//    obj.Tree->l->r->r->r->l->x = 1;
//    obj.del(81);
//    qDebug() << obj.Tree->x;
//    Sleep(100);
//    obj->show();
//    obj->show();
    Sleep(500);
    cout << "---------------------------------------------";
    obj->approve();
    obj->show();

//    cout << ("11kkj0" < "1kkj0") ? true : false;
//    cout << strcmp("11kkj0", "1kkj0") ? true:false;
//    cout << '\n';
}
// dead 81 51 50
