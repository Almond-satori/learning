/*7.1词条模板*/
template <typename K, typename V> struct Entry {
	K key; V value;
	Entry(K k = K(), V v = V()) : key(k), value(v) {};//默认构造函数
	Entry(Entry<K, V> const& e) : key(e.key), value(e.value) {};//基于克隆的构造函数 
	bool operator< (Entry<K, V> const& e) { return key < e.key; }
	bool operator> (Entry<K, V> const& e) { return key > e.key; }
	bool operator== (Entry<K, V> const& e) { return key == e.key; }
	bool operator!= (Entry<K, V> const& e) { return key != e.key; }
};//因为有重载运算符号,可以不必区分Entry和Entry.key0