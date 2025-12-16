#include <iostream>
#include <map>
#include <cmath>
using namespace std;

class Polynomial {
private:
    std::map<int, int, std::greater<int>> terms;

public:
    Polynomial() = default;

    Polynomial(const Polynomial &other) : terms(other.terms) {}

    Polynomial &operator=(const Polynomial &other) {
        if (this != &other) {
            terms = other.terms;
        }
        return *this;
    }

    ~Polynomial() = default;

    friend std::istream &operator>>(std::istream &is, Polynomial &poly) {
        int n;
        is >> n;
        for (int i = 0; i < n; ++i) {
            int coef, exp;
            is >> coef >> exp;
            poly.terms[exp] += coef;
        }
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const Polynomial &poly) {
        bool first = true;
        for (auto it = poly.terms.begin(); it != poly.terms.end(); ++it) {
            int exp = it->first;
            int coef = it->second;
            if (coef == 0) continue;
            if (!first && coef > 0) os << " + ";
            if (coef < 0) os << " - " << (coef * -1);
            else if (first) os << coef;
            else os << coef;

            if (exp > 0) os << "x";
            if (exp > 1) os << "^" << exp;
            first = false;
        }
        if (first) os << "0";
        return os;
    }

    Polynomial operator+(const Polynomial &other) const {
        Polynomial result = *this;
        for (auto it = other.terms.begin(); it != other.terms.end(); ++it) {
            int exp = it->first;
            int coef = it->second;
            result.terms[exp] += coef;
            if (result.terms[exp] == 0) result.terms.erase(exp);
        }
        return result;
    }

    Polynomial operator-(const Polynomial &other) const {
        Polynomial result = *this;
        for (auto it = other.terms.begin(); it != other.terms.end(); ++it) {
            int exp = it->first;
            int coef = it->second;
            result.terms[exp] -= coef;
            if (result.terms[exp] == 0) result.terms.erase(exp);
        }
        return result;
    }

    Polynomial operator*(const Polynomial &other) const {
        Polynomial result;
        for (auto it1 = terms.begin(); it1 != terms.end(); ++it1) {
            for (auto it2 = other.terms.begin(); it2 != other.terms.end(); ++it2) {
                int newExp = it1->first + it2->first;
                int newCoef = it1->second * it2->second;
                result.terms[newExp] += newCoef;
                if (result.terms[newExp] == 0) result.terms.erase(newExp);
            }
        }
        return result;
    }

    double evaluate(double x) const {
        double result = 0;
        for (auto it = terms.begin(); it != terms.end(); ++it) {
            int exp = it->first;
            int coef = it->second;
            result += coef * std::pow(x, exp);
        }
        return result;
    }
};

int main() {
    Polynomial p1, p2, result;
    double x;

    cout << "insert p: ";
    cin >> p1;
    cout << "insert q: ";
    cin >> p2;

    cout << "p(x) = " << p1 << endl;
    cout << "q(x) = " << p2 << endl;

    result = p1 + p2;
    cout << "p + q = " << result << endl;

    result = p1 - p2;
    cout << "p - q =" << result << endl;

    result = p1 * p2;
    cout << "p * q =" << result << endl;

    cout << "insert x for p";
    cin >> x;
    cout << "if x=" << x << "result of p = " << p1.evaluate(x) << endl;

    cout << "insert x for q";
    cin >> x;
    cout << "if x=" << x << "result of q = " << p2.evaluate(x) << endl;

    return 0;
}

