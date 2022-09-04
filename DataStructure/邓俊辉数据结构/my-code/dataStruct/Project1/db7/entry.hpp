/*7.1����ģ��*/
template <typename K, typename V> struct Entry {
	K key; V value;
	Entry(K k = K(), V v = V()) : key(k), value(v) {};//Ĭ�Ϲ��캯��
	Entry(Entry<K, V> const& e) : key(e.key), value(e.value) {};//���ڿ�¡�Ĺ��캯�� 
	bool operator< (Entry<K, V> const& e) { return key < e.key; }
	bool operator> (Entry<K, V> const& e) { return key > e.key; }
	bool operator== (Entry<K, V> const& e) { return key == e.key; }
	bool operator!= (Entry<K, V> const& e) { return key != e.key; }
};//��Ϊ�������������,���Բ�������Entry��Entry.key0