#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;

template <class T>
class ChainNode {
public:
    T data;
    ChainNode<T>* link;

    ChainNode(T d = T(), ChainNode<T>* l = nullptr)
        : data(d), link(l) {}
};

template <class T>
class AvailableList {
public:
    static ChainNode<T>* avail;

    static ChainNode<T>* GetNode(const T& d) {
        if (avail) {
            ChainNode<T>* node = avail;
            avail = avail->link;
            node->data = d;
            node->link = nullptr;
            return node;
        }
        return new ChainNode<T>(d);
    }

    static void ReturnNode(ChainNode<T>* node) {
        node->link = avail;
        avail = node;
    }
};

template <class T>
ChainNode<T>* AvailableList<T>::avail = nullptr;

template <class T>
class ChainIterator {
private:
    ChainNode<T>* current;

public:
    ChainIterator(ChainNode<T>* node = nullptr)
        : current(node) {}

    T& operator*() const {
        return current->data;
    }

    ChainIterator<T>& operator++() {
        current = current->link;
        return *this;
    }

    bool operator!=(const ChainIterator<T>& other) const {
        return current != other.current;
    }
};

template <class T>
class Chain {
private:
    ChainNode<T>* head;

public:
    using iterator = ChainIterator<T>;

    Chain() {
        head = new ChainNode<T>();
        head->link = head;   // circular
    }

    ~Chain() {
        Clear();
        delete head;
    }

    void Clear() {
        ChainNode<T>* cur = head->link;
        while (cur != head) {
            ChainNode<T>* tmp = cur;
            cur = cur->link;
            AvailableList<T>::ReturnNode(tmp);
        }
        head->link = head;
    }

    ChainNode<T>* GetHead() const {
        return head;
    }

    iterator Begin() const {
        return iterator(head->link);
    }

    iterator End() const {
        return iterator(head);
    }
};

struct Term {
    int coef;
    int exp;

    Term(int c = 0, int e = 0) : coef(c), exp(e) {}
};


class Polynomial {
    friend istream& operator>>(istream&, Polynomial&);
    friend ostream& operator<<(ostream&, const Polynomial&);

private:
    Chain<Term> poly;

    void InsertTerm(int coef, int exp) {
        if (coef == 0) return;

        ChainNode<Term>* head = poly.GetHead();
        ChainNode<Term>* prev = head;
        ChainNode<Term>* cur = head->link;

 
        while (cur != head && cur->data.exp > exp) {
            prev = cur;
            cur = cur->link;
        }

  
        if (cur != head && cur->data.exp == exp) {
            cur->data.coef += coef;
            if (cur->data.coef == 0) {
                prev->link = cur->link;
                AvailableList<Term>::ReturnNode(cur);
            }
        } else {
            ChainNode<Term>* node =
                AvailableList<Term>::GetNode(Term(coef, exp));
            node->link = cur;
            prev->link = node;
        }
    }

public:
    Polynomial() = default;

    Polynomial(const Polynomial& p) {
        auto it = p.poly.Begin();
        auto end = p.poly.End();
        for (; it != end; ++it)
            InsertTerm((*it).coef, (*it).exp);
    }

    ~Polynomial() = default;

    Polynomial& operator=(const Polynomial& p) {
        if (this == &p) return *this;
        poly.Clear();
        auto it = p.poly.Begin();
        auto end = p.poly.End();
        for (; it != end; ++it)
            InsertTerm((*it).coef, (*it).exp);
        return *this;
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial c = *this;
        auto it = b.poly.Begin();
        auto end = b.poly.End();
        for (; it != end; ++it)
            c.InsertTerm((*it).coef, (*it).exp);
        return c;
    }

    Polynomial operator-(const Polynomial& b) const {
        Polynomial c = *this;
        auto it = b.poly.Begin();
        auto end = b.poly.End();
        for (; it != end; ++it)
            c.InsertTerm(-(*it).coef, (*it).exp);
        return c;
    }

    Polynomial operator*(const Polynomial& b) const {
        Polynomial c;
        auto it1 = poly.Begin();
        auto end1 = poly.End();
        auto end2 = b.poly.End();

        for (; it1 != end1; ++it1) {
            auto it2 = b.poly.Begin();
            for (; it2 != end2; ++it2) {
                c.InsertTerm(
                    (*it1).coef * (*it2).coef,
                    (*it1).exp + (*it2).exp
                );
            }
        }
        return c;
    }

    double Evaluate(double x) const {
        double sum = 0;
        auto it = poly.Begin();
        auto end = poly.End();
        for (; it != end; ++it)
            sum += (*it).coef * pow(x, (*it).exp);
        return sum;
    }
};

istream& operator>>(istream& in, Polynomial& p) {
    p.poly.Clear();

    string line;
    getline(in >> ws, line);
    stringstream ss(line);

    int coef, exp;
    while (ss >> coef >> exp) {
        p.InsertTerm(coef, exp);
    }
    return in;
}

ostream& operator<<(ostream& out, const Polynomial& p) {
    auto it = p.poly.Begin();
    auto end = p.poly.End();
    bool first = true;

    for (; it != end; ++it) {
        int c = (*it).coef;
        int e = (*it).exp;

        if (!first) {
            if (c > 0) out << " + ";
            else out << " - ";
        } else if (c < 0) {
            out << "-";
        }

        int abs_c = abs(c);

        if (e == 0)
            out << abs_c;
        else if (e == 1)
            out << abs_c << "x";
        else
            out << abs_c << "x^" << e;

        first = false;
    }
    if (first) out << "0";
    return out;
}


int main() {
    Polynomial p1, p2, result;
    double x;

    cout << "Insert p (coef exp ...): ";
    cin >> p1;
    cout << "Insert q (coef exp ...): ";
    cin >> p2;

    cout << "p(x) = " << p1 << endl;
    cout << "q(x) = " << p2 << endl;

    result = p1 + p2;
    cout << "p + q = " << result << endl;

    result = p1 - p2;
    cout << "p - q = " << result << endl;

    result = p1 * p2;
    cout << "p * q = " << result << endl;

    cout << "Insert x for p: ";
    cin >> x;
    cout << "p(" << x << ") = " << p1.Evaluate(x) << endl;

    cout << "Insert x for q: ";
    cin >> x;
    cout << "q(" << x << ") = " << p2.Evaluate(x) << endl;

    return 0;
}

