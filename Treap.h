#include<random>
std::mt19937_64 rd;

struct Node
{
  int x, y;
  Node *l, *r;
  Node (int _key) : x(_key), y(rd()), l(nullptr), r(nullptr){} 
};

Node* merge(Node *A, Node *B){
  if (!A) return B;
  if (!B) return A;

  if (A->y > B->y){
    A->r = merge(A->r, B);
    return A;
  }
  else{
    B->l = merge(A, B->l);
    return B;
  }
}

void split(Node *t, int x, Node *&tl, Node *&tr){
  if (!t){
    tl = tr = nullptr;
    return;
  }
  if (t->x <= x){
    split(t->r, x, t->r, tr);
    tl = t;
  }
  else{
    split(t->l, x, tl, t->l);
    tr = t;
  }
}

void insert(Node *&r, Node *nn){ 
  if (!r){
    r = nn;
    return;
  } 

  if (nn->y > r->y){
    split(r, nn->x, nn->l, nn->r);
    r = nn;
  }
  else{
    if (nn->x <= r->x)
      insert(r->l, nn);
    else
      insert(r->r, nn);
  }
}

int get_min(Node *t){ 
  if (t->l)
    return get_min(t->l);
  else
    return t->x;
}