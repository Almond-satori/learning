template <typename K, typename V> struct Dictionary {
	virtual int size() const = 0;
	virtual bool put(K , V) const = 0;//�����´���
	virtual V* get(K k) = 0;//����key��ȡ������value
	virtual bool remove(K k) = 0;
};