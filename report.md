# 41143148
作業三報告

## Polynomial+Available List
---

## 解題說明

### 1.1 題目需求
實作Polynomia類別，使用circular linked list with header node表示多項式。

每個節點包含三個成員：`coef`（int）、`exp`（int）、`link`。

必須實作：
(a) `istream& operator>>`：讀入一整行 coef exp 配對
(b) `ostream& operator<<`：美觀輸出 
(c) Copy constructor  
(d) Assignment operator  
(e) Destructor
(f) `operator+`  
(g) `operator-` 
(h) `operator*`
(i) `Evaluate const`

 加分項：Available list已實作。

### 1.2 範例測試
輸入 p (coef exp 成對，例如：2 4 3 1 5 0)：2 4 3 1 5 0
輸入 q：1 2 -5 1 8 0
p(x) = 2x^4 + 3x^1 + 5
q(x) = x^2 - 5x^1 + 8
p + q = 2x^4 + 1x^2 - 2x +13
p - q = 2x^4 - 1x^2 + 8x - 3
p * q = 2x^6 - 10x^5 + 16x^4 + 3x^3 - 10x^2 - 1x + 40
Insert x for p: 2
p(2) = 43
Insert x for q: 2
q(2) = 2

---

## 解題策略

| 步驟                  | 實作說明                                                                                           |
|-----------------------|----------------------------------------------------------------------------------------------------|
| **1. ChainNode**      | 泛型結構體，包含 data（本作為 Term）、link，提供建構函式。                                             |
| **2. Chain<T>**       | 循環鏈結串列帶頭節點，維護 head，提供 Begin()、End()、Clear()。 |
| **3. ChainIterator**  | 提供 operator*()、++、!=，讓多項式能透過 iterator 遍歷節點。                   |
| **4. Polynomial**     | 成員 Chain<Term> poly，提供 InsertTerm()、四則運算、多項式求值與 I/O 運算子。    |
| **5. Available List** | static ChainNode<T>* avail；GetNode 優先重用，ReturnNode 回收節點，Destructor 自動回收。     |

---

## 程式實作（關鍵程式碼）

> 完整程式已上傳至  
> [Polynomial ADT（循環鏈結串列實作 + Available List 加分項）](https://github.com/jasonwang1211/NFU-DataStructure-Homework/blob/main/Homework3/src/Polynomial.cpp)

```cpp
// Term 結構
struct Term { int coef; int exp; Term(int c=0,int e=0):coef(c),exp(e){} };

// 插入單項
void Polynomial::InsertTerm(int coef, int exp) {
    if(coef==0) return;
    ChainNode<Term>* head = poly.GetHead();
    ChainNode<Term>* prev = head;
    ChainNode<Term>* cur = head->link;
    while(cur!=head && cur->data.exp>exp){ prev=cur; cur=cur->link; }
    if(cur!=head && cur->data.exp==exp){
        cur->data.coef+=coef;
        if(cur->data.coef==0){ prev->link=cur->link; AvailableList<Term>::ReturnNode(cur); }
    } else {
        ChainNode<Term>* node = AvailableList<Term>::GetNode(Term(coef,exp));
        node->link=cur; prev->link=node;
    }
}

// 多項式加法（operator+）
Polynomial Polynomial::operator+(const Polynomial& b) const {
    Polynomial c = *this;
    for(auto it = b.poly.Begin(); it!=b.poly.End(); ++it)
        c.InsertTerm(it->coef,it->exp);
    return c;
}
```
## 效能分析

| 演算法 / 操作              | 時間複雜度                  | 空間複雜度                  | 說明                                                                 |
|----------------------------|-----------------------------|-----------------------------|----------------------------------------------------------------------|
| **單項插入（InsertBack）** | **`O(n)（查找位置）`**                  | **`O(1)`** | 已按指數遞減插入，若輸入為遞減可為 O(1)           |
| **Add（多項式相加）**      | **`O(t₁ + t₂)`**            | **`O(t₁ + t₂)`** | 遍歷兩個多項式，逐項合併相同指數 |
| **Subtract（多項式相減）** | **`O(t₁ + t₂)`**            | **`O(t₁ + t₂)`**            | 先將 b 取負，呼叫加法       |
| **Mult（多項式相乘）**     | **`O(t₁ ⋅ t₂)`**            | **`O(t₁ ⋅ t₂)`** | 雙層迴圈，每對項相乘再 InsertTerm |
| **Evaluate（求值）**       | **`O(t)`**                  | **`O(1)`**                  | 遍歷多項式每項計算 pow(x,exp)*coef               |
| **I/O (>> / <<)**               | **`O(t)`**                  | **`O(t)`**                  | 逐項讀入與輸出，不需額外排序            |
| **Copy / Assignment**      | **`O(t)`**                  | **`O(t)`**                  | 深拷貝每個節點                      |
| **Destructor**             | **`O(t)`**                  | **`O(1)`**    | 回收至 Available List，可重用節點  |

## 測試與驗證

輸入第一個多項式: 2 4 3 1 5 0
輸入第二個多項式: 1 2 -5 1 8 0

p(x) = 2x^4 + 3x^1 + 5
p + q = 2x^4 + 1x^2 - 2x +13
p - q = 2x^4 - 1x^2 + 8x - 3
p * q = 2x^6 - 10x^5 + 16x^4 + 3x^3 - 10x^2 - 1x + 40
Insert x for p: 2
p(2) = 43
Insert x for q: 2
q(2) = 2

## 心得討論
1. **Available List 的實用**：減少 `new/delete` 的系統開銷，提升整體效能。

2. **指數遞減設計的優勢**：輸入保證指數遞減，使單項插入可為 O(1)，加法可線性完成。

3. **迭代器設計**：自訂 iterator 仍能實現範圍式遍歷（for(auto it=poly.Begin(); it!=poly.End(); ++it)），程式碼結構乾淨且易維護。
