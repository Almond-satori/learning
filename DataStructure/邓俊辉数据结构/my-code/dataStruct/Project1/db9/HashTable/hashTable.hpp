#include "../dictionary.hpp"
#include "../Entry.hpp"
#include "BitMap.hpp"
#include <cstring>

template <typename K,typename V>
class Hashtable : public Dictionary<K, V> {
private:
	Entry<K, V>** ht; //Ͱ����(��һ��*),��Ŵ���ָ��(ǰһ��*)
	int M; //Ͱ��������
	int N; //��������
	Bitmap* lazyRemoval; //����ɾ�����
#define lazilyRemoved(x) (lazyRemoval->test(x))
#define markAsRemoved(x) (lazyRemoval->set(x))
protected:
	int probe4Hit(const K& k); //���Źؼ���k��Ӧ�Ĳ�����,�ҵ������ƥ���Ͱ
	int probe4Free(const K& k); //���Źؼ���k��Ӧ�Ĳ�����,�ҵ��׸����õ�Ͱ
	void rehash(); //��ɢ���㷨,��������Ͱ,��֤װ�������ھ���������
public:
	Hashtable(int c = 5);//����һ��������С��c��ɢ�б�
	~Hashtable();//�ͷ�Ͱ�����Լ����и�Ԫ��ָ��Ĵ���
	int size() const { return N; }//��ǰ�Ĵ�����Ŀ
	bool put(K k, V v);//���ܷ����ظ�����
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
		//hѭ��������λ
		h = (h << 5) | (h >> 27);
		h += (int) s[i];
	}
	return (size_t) h;
}


template<typename K, typename V>
int Hashtable<K, V>::probe4Hit(const K& k)
{
	int r = hashCode(k) % M;
	while ((ht[r] && (ht[r]->key != k))	//Ͱ��������,���ؼ��벻��ͬ,��������
		|| (!ht[r] && (lazilyRemoved(r))))	//Ͱ��������,������ɾ�����
		r = (r + 1) % M;
	return r;//���ܲ��ҳɹ�,�����ǲ�������ɾ����ǵĿ�Ͱ
}

template<typename K, typename V>
int Hashtable<K, V>::probe4Free(const K& k)
{
	int r = hashCode(k) % M;
	//�����Ƿ�������ɾ��,�ҵ�һ����Ͱ�ͷ���
	while (!ht[r]) r = (r + 1) % M;
	return r;
}

template<typename K, typename V>
void Hashtable<K, V>::rehash()
{
	int old_capacity = M; Entry<K, V>** old_ht = ht;
	M = primeNLT(2 * M, 1048576, "prime.txt");//����,��������Ϊ��ǰ��2��
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
	M = primeNLT(c, 1048576, "primer.txt"); //unfinished:�����1048576���������������ļ�primer.txt��
	N = 0; ht = new Entry<K, V>*[M];
	memset(ht, 0, sizeof(Entry<K, V>*) * M);//��ʼ����Ͱ
	lazyRemoval = new Bitmap(M);
}

template<typename K, typename V>
Hashtable<K, V>::~Hashtable()
{
	for (int i = 0; i < M; i++)
		if (ht[i]) release(ht[i]);
	release(ht);
	release(lazyRemoval);//�ͷ�����ɾ�����
}

template<typename K, typename V>
bool Hashtable<K, V>::put(K k, V v)
{
	if (ht[probe4Hit(k)]) return false;//Ԫ����ͬ,����Ҫ����
	int r = probe4Free(k);
	ht[r] = new Entry<K, V>(k, v);
	++N;
	//װ�����Ӵ���50%������ɢ��
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
	Bitmap B(file, n);//file����λͼ��ʽ��¼��ָ����Χ�����е�����
	while (c<n)
	{
		if (B.test(c)) c++;
		else return c;//�����׸����ֵ�����
	}
	return c;//���û������������,�򵥵ط���n(��ʵ����²�����˴���)
}