#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>

using namespace std;

class BoolV
{
    unsigned int *vec;
    int nbit;// nbit – количество бит вектора
    int size_v;//размер вектора или количество элементов массива vec (bytes)
public:
    BoolV(int nn = 1)
    {
        nbit = nn;
        size_v = (nn - 1) / 32 + 1;
        vec = new unsigned int[size_v];
        for(int i = 0; i < size_v; i++)
            vec[i] = 0;
    }

    BoolV(const char *str) : BoolV:: BoolV(strlen(str))
    {
        unsigned int mask = 1;

        for(int i = 0; i < nbit; i++)
        {
            if(mask == 0)
                mask = 1;

            if(str[i] != '0')
                vec[i/32] = vec[i/32] | mask;

            mask = mask << 1;


        }
    }

    BoolV(const BoolV &other)
    {
        nbit = other.nbit;
        size_v = other.size_v;
        vec = new unsigned int[size_v];
        for(int i = 0; i < size_v; i++)
        {
            vec[i] = other.vec[i];
        }
    }

    ~BoolV()
    {
        if(vec != NULL)
            delete[] vec;

        nbit = 0;
        size_v = 0;
    }

    int weight()
    {
        int count = 0;
        unsigned mask = 1;
        for(int i = 0; i < nbit; i++)
        {
            if(vec[i/32] & mask)
                count++;

            mask = mask << 1;

            if(mask == 0)
                mask = 1;
        }
        return count;
    }

    void set1(int pos)//5
    {
        if(pos > nbit || pos < 0)
        {
            cout << "Index out of range" << endl;
            exit(-1);
        }

        unsigned int mask = 1;
        int byte_v = pos / 32;
        mask = mask << (pos % 32);
        vec[byte_v] = vec[byte_v] | mask;
    }

    void set0(int pos)
    {
        if(pos > nbit || pos < 0)
        {
            cout << "INdex out of range" << endl;
            exit(-2);
        }

        unsigned int mask = 1;
        int byte_v = pos / 32;
        mask = mask << (pos % 32);
        vec[byte_v] = vec[byte_v] & ~(mask);
    }

    int operator[](int pos)
    {
        if(pos > nbit || pos < 0)
        {
            cout << "Index out of range" << endl;
            exit(-3);
        }

        unsigned int mask = 1;
        int byte_v = pos / 32;
        mask = mask << (pos % 32);
        bool res = vec[byte_v] & mask;

        return res;
    }

    BoolV operator= (const BoolV &other)
    {
        if(this == &other)
            return *this;

        delete[] vec;

        vec = new unsigned int[other.size_v];
        nbit = other.nbit;
        size_v = other.size_v;
        for(int i = 0; i < size_v; i++)
        {
            vec[i] = other.vec[i];
        }

        return *this;
    }

    bool operator==(const BoolV &other)
    {
        if(nbit != other.nbit)
            return false;
        for(int i = 0; i < size_v; i++)
        {
            if(vec[i] != other.vec[i])
                return false;
        }

        return true;
    }

    BoolV operator|(const BoolV &other)//логическое или
    {
        int  size = nbit;

        BoolV res(size);

        for(int i = 0; i < size_v; i++)
        {
            res.vec[i] = vec[i] | other.vec[i];
        }
        return res;
    }

    BoolV operator& (const BoolV &other)
    {
        int size = nbit;

        BoolV res(size);

        for(int i = 0; i < size_v; i++)
        {
            res.vec[i] = vec[i] & other.vec[i];
        }
        return res;
    }

    BoolV operator~()//????????
    {
        BoolV res(*this);//конструктор копирования

        unsigned int mask = 65535;
        for(int i = 0; i < size_v; i++)
        {
            res.vec[i] = ~vec[i];
        }
        mask = (1 << (nbit % 32)) - 1;
        res.vec[size_v - 1] = res.vec[size_v - 1] & mask;

        return res;
    }

    friend ostream& operator<< (ostream &os, const BoolV &boolV)
    {
        char *str = new char[boolV.nbit + 1];
        str[boolV.nbit] = '\0';
        for(int i = 0; i < boolV.nbit; i++)
        {
            unsigned int mask = 1;
            if(boolV.vec[i/32] & (mask << (i % 32)))
                str[i] = '1';
            else
                str[i] = '0';
        }
        os << str << endl;

        return os;
    }

    friend istream& operator>> (istream &in, BoolV &boolV)
    {
        string str;
        in >> str;

        BoolV res(str.c_str());//Чтобы сделать указатель на строку в C++, можно использовать функцию-член c_str() для std::string.

        boolV = res;

        return in;
    }

};

class BoolM
{
    BoolV *bm;
    int m, n;// m – количество строк, n – количество столбцов
public:
    BoolM(int k = 1, int l = 1) : m(k), n(l)
    {
        if(k <= 0 || l <= 0)
        {
            cout << "Error index" << endl;
            exit(-4);
        }

        bm = new BoolV[m];
        for(int i = 0; i < m; i++)
        {
            bm[i] = BoolV(n);
        }
    }

    BoolM(const BoolM &other) : m(other.m), n(other.n)
    {
        bm = new BoolV[m];
        for(int i = 0; i < m; i++)
        {
            bm[i] = other.bm[i];
        }
    }

    ~BoolM()
    {
        delete[] bm;
    }

    int getRows()//строчки
    {
        return m;
    }

    int getCols()//cтолбцы
    {
        return n;
    }

    BoolV & operator[] (int pos)
    {
        return bm[pos];
    }

    BoolM operator= (const BoolM &other)
    {
        if(this == &other)
            return *this;

        delete[] bm;
        m = other.m;
        n = other.n;
        bm = new BoolV[m];
        for(int i = 0; i < m; i++)
        {
            bm[i] = other.bm[i];
        }
        return *this;
    }

    void TopSort(BoolM &mx, int m, vector<int> &a)
    {



        BoolV vec0(m), vec1(m), vec2(m), mask(m);

        while(vec0.weight() != m)
        {
            for(int i = 0; i < m; i++)
                    cout << mx;

            vec1 = mask;

            for(int i = 0; i < m; i++)
                vec1 = vec1 | mx[i];

            vec1 = ~vec1;//вершины в которые нельзя попасть;

            vec2 = vec1 & (~vec0);//вершины в которых ещё не побывали

            if(vec2 == mask)
            {
                cout << "dsafjkasjf" << endl;
                exit(-5);
            }

            for(int i = 0; i < m; i++)
            {
                if(vec2[i] == 1)
                {
                    a.push_back(i + 1);
                    mx[i] = mask;
                }
            }

            vec0 = vec0 | vec2;

            cout << "Vec0 - " << vec0 << endl;
            cout << "Weight - " << vec0.weight() << endl;

        }

    }

    BoolM CreateMxByGraf()
    {
        string path = "graf.txt";
        //ifstream file("graf.txt");
        ifstream file;
        file.open(path);

        if(!file.is_open())
        {
            cout << "file not open" << endl;
            exit(-4);
        }

        int a, b;//a - начало b - конец 
        int m;//количество вершин

        file >> m;
        BoolM Mx(m, m);

        while(file >> a >> b)
        {
            Mx[a - 1].set1(b - 1);
        }

        file.close();

        return Mx;
    }

    friend ostream& operator<< (ostream& os, BoolM &boolM)
    {
        for(int i = 0; i < boolM.m; i++)
        {
            os << boolM[i] << endl;
        }
        return os;
    }
};

int main()
{
    // setlocale(LC_ALL, "ru");

    BoolM mx;   

    mx.CreateMxByGraf();

    cout << mx << endl;

    vector<int> a;

    mx.TopSort(mx, mx.getRows(), a);

    for(int p : a)
    {
        cout << p << " ";
    }
    cout << endl;

    return 0;
}
