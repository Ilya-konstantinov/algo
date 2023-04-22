#include <iostream>
#include <vector>

using std::vector;

template<class Base>
class ST{
public:
  vector<Base> tree;
  vector<long long> base_array;
  long long base_size;
  ST(){}
  
  long long log2l(long long x){
    long long ans = 0, i = 1;
    while((i <<= 1) <= x)
      ans++;
    return ans;
  }

  template<class Function, class SimpleFunction>
  void build(int v, int tree_l, int tree_r, Function comporator, SimpleFunction base_comporator){
    if (tree_l + 1 == tree_r){
      tree[v] = base_comporator(base_array[tree_l],tree_l);
      return;
    }
    int tree_middle = (tree_l + tree_r)/2;
    build(v*2, tree_l, tree_middle, comporator, base_comporator);
    build(v*2+1, tree_middle, tree_r, comporator, base_comporator);	
    tree[v] = comporator(tree[v*2],tree[v*2+1]);
  }
  
  template<class Function, class SimpleFunction>
  ST(vector<long long> _base_array, Function comporator, SimpleFunction base_comporator, int base_val = 10e9): base_array(_base_array) {
    if (1 << log2l(base_array.size()) != base_array.size()){ // checks if a _base_size() is a power of two 
      base_array.resize(1 << (log2l((int) base_array.size()) + 1));
      for(int i = _base_array.size(); i < base_array.size(); i++)
        base_array[i] = base_val;
    }
    
    base_size = base_array.size();
    tree.resize(base_size * 4);
    build(1, 0, base_size, comporator, base_comporator);
  }

  template<class Function>
  Base get(int l, int r, Function comporator, Base base_val, int v = 1, int tree_l = 0, int tree_r = 0){
    if (tree_r == 0) tree_r = base_size;
    if (tree_l >= r || tree_r <= l) // Вышли за пределы запроса
      return base_val;
    if (tree_l >= l && tree_r <= r) // Запрос зашел в границы полнсоть
      return tree[v];
    
    int tree_middle = (tree_r+tree_l)/2;
    Base left_val = get(l, r, comporator, base_val, 2*v, tree_l, tree_middle);
    Base right_val = get(l, r, comporator, base_val, 2*v+1, tree_middle, tree_r);
    return comporator(left_val, right_val);
  }

  template<class Function>
  void change_element(Base x, int v, Function comporator){
    base_array[v] = x;
    tree[v+base_array.size()] = x;
    do_branch((v+base_array.size())/2,comporator);
  }

  template<class Function>
  void do_branch(int v, Function comporator){
    if (v == 0)
      return;
    if (tree[v] == comporator(tree[v*2+1],tree[v*2]))
      return;
    tree[v] = comporator(tree[v*2+1],tree[v*2]);
    do_branch(v/2,comporator);
  }

};

template<class T>
std::ostream& operator<<(std::ostream& out, ST<T>& a){
  for(int i = 1; i <= a.base_array.size(); i <<= 1){
    for(int j = i; j < i*2; j++)
      out << a.tree[j] << ' ';
    out << '\n';
  }
  return out;
}