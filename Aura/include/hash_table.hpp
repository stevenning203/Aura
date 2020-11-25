namespace nsl
{
	template <typename HashKey, typename HashValue>
	class HashNode
	{
	private:
		HashKey key;
		HashValue value;
		HashNode *next;
	public:
		HashNode(const HashKey& key_reference, const HashValue& value_reference)
		{
			this->key = key_reference;
			this->value = value_reference;
		}
		HashKey GetKey() const
		{
			return key;
		}
		HashValue GetValue() const
		{
			return value;
		}
		void SetValue(HashValue value)
		{
			this->value = value;
		}
		HashNode *GetNext() const
		{
			return next;
		}
		void SetNext(HashNode *next)
		{
			this->next = next;
		}
	};
}