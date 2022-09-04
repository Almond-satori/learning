template <typename K, typename V> struct Dictionary {
	virtual int size() const = 0;
	virtual bool put(K , V) const = 0;//插入新词条
	virtual V* get(K k) = 0;//根据key读取词条的value
	virtual bool remove(K k) = 0;
};