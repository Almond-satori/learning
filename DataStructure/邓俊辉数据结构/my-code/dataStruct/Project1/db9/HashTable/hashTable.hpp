#include "../dictionary.hpp"
#include "../Entry.hpp"
#include "BitMap.hpp"
#include <cstring>

template <typename K,typename V>
class Hashtable : public Dictionary<K, V> {
private:
	Entry<K, V>** ht; //桶数组(后一个*),存放词条指针(前一个*)
	int M; //桶数组容量
	int N; //词条容量
	Bitmap* lazyRemoval; //懒惰删除标记
#define lazilyRemoved(x) (lazyRemoval->test(x))
#define markAsRemoved(x) (lazyRemoval->set(x))
protected:
	int probe4Hit(const K& k); //沿着关键码k对应的查找链,找到与词条匹配的桶
	int probe4Free(const K& k); //沿着关键码k对应的查找链,找到首个可用的桶
	void rehash(); //重散列算法,扩充数组桶,保证装填因子在警戒线以下
public:
	Hashtable(int c = 5);//创建一个容量不小于c的散列表
	~Hashtable();//释放桶数组以及其中各元素指向的词条
	int size() const { return N; }//当前的词条数目
	bool put(K k, V v);//不能放入重复词条
	V* get(K k);
	bool remove(K k);
};


static size_t hashCode(char c) { return (size_t)c; }
static size_t hashCode(int k) { return (size_t)k; }
static size_t hashCode(long long i) { return (size_t)( (i>>32) + (int)i ); }
static size_t hashCode(char s[]) { 
	int h = 0;
	for (size_t n = strlen(s),int i=0; i < n; i++)
	{
		//h循环左移五位
		h = (h << 5) | (h >> 27);
		h += (int) s[i];
	}
	return (size_t) h;
}


template<typename K, typename V>
int Hashtable<K, V>::probe4Hit(const K& k)
{
	int r = hashCode(k) % M;
	while ((ht[r] && (ht[r]->key != k))	//桶中有数据,但关键码不相同,继续查找
		|| (!ht[r] && (lazilyRemoved(r))))	//桶中无数据,有懒惰删除标记
		r = (r + 1) % M;
	return r;//可能查找成功,可能是不带懒惰删除标记的空桶
}

template<typename K, typename V>
int Hashtable<K, V>::probe4Free(const K& k)
{
	int r = hashCode(k) % M;
	//不论是否有懒惰删除,找到一个空桶就返回
	while (!ht[r]) r = (r + 1) % M;
	return r;
}

template<typename K, typename V>
void Hashtable<K, V>::rehash()
{
	int old_capacity = M; Entry<K, V>** old_ht = ht;
	M = primeNLT(2 * M, 1048576, "prime.txt");//扩充,容量至少为当前的2倍
	N = 0; ht = new Entry<K, V>*[M];
	memset(ht, 0, sizeof(Entry<K, V>) * M);
	release(lazyRemoval); lazyRemoval = new Bitmap(M);
	for (int i = 0; i < old_capacity; i++)
	{
		if (old_ht[i])
			put(old_ht[i]->key, old_ht[i]->value);
	}
	release(old_ht);
}

template<typename K, typename V>
Hashtable<K, V>::Hashtable(int c)
{
	M = primeNLT(c, 1048576, "primer.txt"); //unfinished:计算出1048576的所有素数放在文件primer.txt中
	N = 0; ht = new Entry<K, V>*[M];
	memset(ht, 0, sizeof(Entry<K, V>*) * M);//初始化各桶
	lazyRemoval = new Bitmap(M);
}

template<typename K, typename V>
Hashtable<K, V>::~Hashtable()
{
	for (int i = 0; i < M; i++)
		if (ht[i]) release(ht[i]);
	release(ht);
	release(lazyRemoval);//释放懒惰删除标记
}

template<typename K, typename V>
bool Hashtable<K, V>::put(K k, V v)
{
	if (ht[probe4Hit(k)]) return false;//元素雷同,不需要插入
	int r = probe4Free(k);
	ht[r] = new Entry<K, V>(k, v);
	++N;
	//装填因子大于50%后重新散列
	if (N * 2 > M) rehash();
	return true;
}

template<typename K, typename V>
V* Hashtable<K, V>::get(K k)
{
	int r = probe4Hit(k);
	return ht[r] ? &(ht[r]->value) : NULL;
}

template<typename K, typename V>
bool Hashtable<K, V>::remove(K k)
{
	int r = probe4Hit(k);
	if (ht[r] == NULL) return false;
	release(ht[r]);
	ht[r] = NULL;
	N--;
	markAsRemoved(r);
	return true;
}

int primeNLT(int c, int n, char* file) {
	Bitmap B(file, n);//file按照位图格式记录了指定范围内所有的素数
	while (c<n)
	{
		if (B.test(c)) c++;
		else return c;//返回首个发现的素数
	}
	return c;//如果没有这样的素数,简单地返回n(真实情况下不能如此处理)
}