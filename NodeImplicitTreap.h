#include<random>

std::mt19937_64 rd;

#define int long long

struct Node{
  int key, y;
  int min, size;
  bool rv;
  Node *l, *r;
  
  Node(int _key) : key(_key), l(nullptr), r(nullptr), y(rd()), size(1), min(key), rv(0) {}
};

int get_size(Node *root){
  return root ? root->size : 0;
}

int get_min(Node *root){
  return root ? root->min : 1e10;
}

int min(int a, int b){
  return a < b ? a : b; 
}

void push(Node *root){
  if (root == nullptr ) return;
  if (!root->rv) return;
  root->rv = 0;
  std::swap(root->l, root->r);
  if (root->l) root->l->rv ^= 1;
  if (root->r) root->r->rv ^= 1;
}

void recalc(Node *root){
  if (!root) return;
  root->size = get_size(root->l) + get_size(root->r) + 1;
  root->min = min(min(get_min(root->l),get_min(root->r)), root->key);
}

void split(Node *root, int x, Node *&tr1, Node *&tr2){
  push(root);

  if (root == nullptr){
    tr1 = tr2 = nullptr;
    return;
  } 

  int curPos = get_size(root->l);
  if (curPos < x){
    split(root->r, x - curPos - 1, root->r, tr2);
    tr1 = root;
  }
  else{
    split(root->l, x, tr1, root->l);
    tr2 = root;
  }
  recalc(tr1);
  recalc(tr2);
  recalc(root);
}

Node* merge(Node *A, Node *B){
  push(A);
  push(B);
  if (!A) return B;
  if (!B) return A;
  if (A->y > B->y){
    A->r = merge(A->r, B);
    recalc(A);
    return A;
  }
  else{
    B->l = merge(A, B->l);
    recalc(B);
    return B;
  }
}

int minInInterval(Node *root, int L, int R){
  Node *l, *m, *r;
  split(root, L, l, r);
  split(r, R - L, m, r);
  int ans = get_min(m);
  root = merge(merge(l, m), r);
  return ans;
}


void reverse(Node *root, int L, int R){
  Node *l, *m, *r;
  split(root, L, l, r);
  split(r, R - L, m, r);
  m->rv ^= 1;
  root = merge(l, merge(m,r));
}

#undef int