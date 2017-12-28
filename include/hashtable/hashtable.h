#ifndef  __HASHTABLE_H_
#define  __HASHTABLE_H_

#include <string.h>
#include <iostream>
#include "alloc.h"
#include "sample_alloc.h"
#include "ht_utils.h"
#include "hashutils.h"
#include "debug.h"

namespace lib
{

template <typename _Tp, class _Alloc>
struct hashtable_node_t;

template <typename _Tp, class _Alloc>
struct hashtable_node_t
{
	_Tp val;
	typedef typename _Alloc::template rebind<hashtable_node_t>::other::pointer pointer;
	pointer next;
};

template <class _Key, class _Value, class _HashFun, class _Equl, class _GetKey, class _InnerAlloc> 
class hashtable;

template <class _Key, class _Value, class _HashFun, class _Equl, class _GetKey, class _InnerAlloc> 
struct hashtable_iterator;

template <class _Key, class _Value, class _HashFun, class _Equl, class _GetKey, class _InnerAlloc> 
struct hashtable_const_iterator;

//hash表迭代器实现
template <class _Key, class _Value, class _HashFun, class _Equl, class _GetKey, class _InnerAlloc> 
struct hashtable_iterator
{
	typedef hashtable<_Key, _Value, _HashFun, _Equl, _GetKey, _InnerAlloc> ht;
	typedef hashtable_iterator<_Key, _Value, _HashFun, _Equl, _GetKey, _InnerAlloc> iterator;
	typedef hashtable_const_iterator<_Key, _Value, _HashFun, 
                                        _Equl, _GetKey, _InnerAlloc> const_iterator;
	typedef hashtable_node_t<_Value, _InnerAlloc> node_t;
	typedef typename ht::node_pointer node_pointer;

	typedef std::forward_iterator_tag iterator_category;
	typedef _Value value_type;
	typedef ptrdiff_t	difference_type;
	typedef value_type * pointer;
	typedef value_type & reference;

	ht *_ht;
	size_t _bucketpos;
	node_pointer _node;

public:
	hashtable_iterator() { 
        _ht = 0; 
        _bucketpos = 0; 
        _node = 0; 
    }
	hashtable_iterator(const iterator &iter) {
		_ht = iter._ht;
		_bucketpos = iter._bucketpos;
		_node = iter._node;
	}
	hashtable_iterator (const const_iterator & iter) {
		_ht = iter._ht;
		_bucketpos = iter._bucketpos;
		_node = iter._node;
	}
	hashtable_iterator(ht *ht, size_t bp, node_pointer node) {
        _ht = ht;
		_bucketpos = bp;
		_node = node;
        if(_ht != 0){
    		while (_node == 0 && _bucketpos < _ht->_bitems) {
	    		_node = _ht->_bucket[_bucketpos];
		    	if (_node == 0) {
			    	++_bucketpos;
			    }
		    }
        }
	}  
	reference operator * () {
        if(0 == _ht){
            //throw bsl::NullPointerException()<<BSL_EARG
            std::cout << "can not dereference a null interator." << std::endl;
        }
		return _ht->_sp_alloc.getp(_node)->val;
	}
	pointer operator -> () {
        if(0 == _ht){
            //throw bsl::NullPointerException()<<BSL_EARG
            std::cout << "can not evaluate using a null interator." << std::endl;
        }
		return &(_ht->_sp_alloc.getp(_node)->val);
	}

	//这里可以优化
	bool operator == (const iterator & iter) const {
		return (_node == iter._node);
	}
	bool operator != (const iterator & iter) const {
		return (_node != iter._node);
	}

	iterator & operator ++ () {
        if(0 == _ht){
            //throw bsl::NullPointerException()<<BSL_EARG
            std::cout << "can not increase a null interator." << std::endl;
        }
		if (_node) {
			_node = _ht->_sp_alloc.getp(_node)->next;
			if (_node) {
				return *this;
			}
		}
		for (size_t i = _bucketpos + 1; i < _ht->_bitems; ++i) {
			_node = _ht->_bucket[i];
			if (_node) {
				_bucketpos = i;
				return *this;
			}
		}
		_bucketpos = _ht->_bitems;
		return *this;
	}
	iterator operator ++ (int) {
		iterator iter = *this;
		++ *this;
		return iter;
	}
};

/**
 * @brief hashtable的只读迭代器
 */
template <class _Key, class _Value, class _HashFun, class _Equl, class _GetKey, class _InnerAlloc> 
struct hashtable_const_iterator
{
	typedef hashtable<_Key, _Value, _HashFun, _Equl, _GetKey, _InnerAlloc> ht;
	typedef hashtable_iterator<_Key, _Value, _HashFun, _Equl, _GetKey, _InnerAlloc> iterator;
	typedef hashtable_const_iterator<_Key, _Value, _HashFun, 
                                        _Equl, _GetKey, _InnerAlloc> const_iterator;

	typedef hashtable_node_t<_Value, _InnerAlloc> node_t;
	typedef typename ht::node_pointer node_pointer;
	typedef std::forward_iterator_tag iterator_category;
	typedef _Value value_type;
	typedef ptrdiff_t	difference_type;

	typedef const value_type * pointer;
	typedef const value_type & reference;

	const ht *_ht;
	size_t _bucketpos;
	node_pointer _node;

public:
	hashtable_const_iterator() { 
        _ht = 0; 
        _bucketpos = 0; 
        _node = 0; 
    }
	hashtable_const_iterator(const const_iterator &iter) {
		_ht = iter._ht;
		_bucketpos = iter._bucketpos;
		_node = iter._node;
	}
	hashtable_const_iterator(const iterator &iter) {
		_ht = iter._ht;
		_bucketpos = iter._bucketpos;
		_node = iter._node;
	}
	hashtable_const_iterator(const ht *ht, size_t bp, node_pointer node) {
        _ht = ht;
		_bucketpos = bp;
		_node = node;
        if(_ht != 0){
		    while (_node == 0 && _bucketpos < _ht->_bitems) {
			    _node = _ht->_bucket[_bucketpos];
			    if (_node == 0) {
				    ++_bucketpos;
			    }
		    }
        }
	}
	reference operator * () const {
        if(0 == _ht){
            //throw bsl::NullPointerException()<<BSL_EARG
            std::cout << "can not dereference a null interator." << std::endl;
        }
		return _ht->_sp_alloc.getp(_node)->val;
	}
	pointer operator -> () const {
        if(0 == _ht){
            //throw bsl::NullPointerException()<<BSL_EARG
            std::cout << "can not evaluate using a null interator." << std::endl;
        }
		return &(_ht->_sp_alloc.getp(_node)->val);
	}

	//这里可以优化
	bool operator == (const const_iterator & iter) const {
		return (_node == iter._node);
	}
	bool operator != (const const_iterator & iter) const {
		return (_node != iter._node);
	}

	const_iterator & operator ++ () {
        if(0 == _ht){
            //throw bsl::NullPointerException()<<BSL_EARG
            std::cout << "can not increase a null interator." << std::endl;
        }
        if (_node) {
			_node = _ht->_sp_alloc.getp(_node)->next;
			if (_node) {
				return *this;
			}
		}
		for (size_t i = _bucketpos + 1; i < _ht->_bitems; ++i) {
			_node = _ht->_bucket[i];
			if (_node) {
				_bucketpos = i;
				return *this;
			}
		}
		_bucketpos = _ht->_bitems;
		return *this;
	}
	const_iterator operator ++ (int) {
		iterator iter = *this;
		++ *this;
		return iter;
	}

};

/**
 * @brief 普通的hashtable
 */
template <class _Key	//hash key
	, class _Value	//存储的数据 能萃取除key值
	, class _HashFun	//hash 函数
	, class _Equl	//key值的比较函数
	, class _GetKey	//从 _Value萃取出key值的函数
	, class _InnerAlloc	//维护状态数据的内部allocator
	>
class hashtable
{
public:
	typedef hashtable<_Key, _Value, _HashFun, _Equl, _GetKey, _InnerAlloc> _Self;
	typedef _Value value_type;
	typedef _Key key_type;
	typedef value_type * pointer;

	typedef hashtable_node_t<value_type, _InnerAlloc> node_t;
	typedef typename _InnerAlloc::pool_type pool_type;

	typedef typename _InnerAlloc::template rebind<node_t>::other sample_alloc_type;
	typedef typename sample_alloc_type::pointer node_pointer;
	typedef typename pool_type::template rebind<node_pointer >::other node_p_alloc_type;

	friend struct hashtable_iterator<_Key, _Value, _HashFun, _Equl, _GetKey, _InnerAlloc>;
	friend struct hashtable_const_iterator<_Key, _Value, _HashFun, 
                                                _Equl, _GetKey, _InnerAlloc>;

public:
	sample_alloc_type _sp_alloc;
	node_p_alloc_type _np_alloc;

	node_pointer *_bucket;
	size_t _bitems;
	size_t _size;

	_HashFun _hashfun;
	_Equl _equl;
	_GetKey _getkey;

public:
	typedef hashtable_iterator<_Key, _Value, _HashFun, _Equl, _GetKey, _InnerAlloc> iterator;
	typedef hashtable_const_iterator<_Key, _Value, _HashFun, 
                                        _Equl, _GetKey, _InnerAlloc> const_iterator;

	iterator _end_iter;

public:
    /**
     * @brief 构造函数
     * 无异常
     * 需调create
     */
	hashtable() {
		_reset();
    }

    /**
     * @brief 带桶大小的构造函数，抛异常
     *        如果构造失败，将销毁对象
     * 不需调create
     *
     * @param  [in] bitems  : size_t hash桶的大小
     * @param  [in] hf      : const _HashFun& hash函数
     * @param  [in] eq      : const _Equl& hash结点的比较仿函数
     */
	hashtable(size_t bitems, const _HashFun& hf = _HashFun(), const _Equl& eq = _Equl()) {
        _reset();
        if (create(bitems,hf,eq) != 0) {
            destroy();
            //throw BadAllocException()<<BSL_EARG
            std::cout << "create error when create hashtable bitems " << bitems << std::endl;
        }
    }

    /**
     * @brief 拷贝构造函数, 抛异常
     *        如果构造失败，将销毁对象
     *  
     * @param  [in] other   : const _Self&
     * @return
     * @retval
     * @see
     **/
	hashtable(const _Self& other) {
        _reset();
        if (assign(other.begin(),other.end()) != 0) {
            destroy();
            //throw BadAllocException()<<BSL_EARG<<"assign hashtable error";
            std::cout << "assign hashtable error" << std::endl;
        }
    }

    /**
     * @brief 赋值运算符，抛异常
     *        先获得自身的一个拷贝，如果赋值失败，将恢复成原来的hashtable
     *          
     * @param
     * @return
     * @retval
     * @see
     **/
	_Self & operator = (const _Self& other) {
        if (this != &other) {
            _Self self_(*this); // 先获得自身的一个拷贝
            if (assign(other.begin(),other.end()) != 0) {
                destroy();
                *this = self_;
                //throw BadAllocException()<<BSL_EARG<<"assign hashtable error";
                std::cout << "assign hashtable error" << std::endl;
            }
        }
        return *this; 
    }

	~hashtable() {
		destroy();
	}

	/**
	 * @brief 创建hash表
	 *
	 * @param [in/out] bitems   : size_t	hash桶的大小
	 * @param [in] hf	: const _HashFun 	hash函数
	 * @param [in/out] eq   : const _Equl&	hash结点的比较仿函数
	 * @return  int 成功返回0，其他返回失败
	 * @retval   
	 * @see 
	**/
	int create(size_t bitems, const _HashFun &hf = _HashFun(), const _Equl &eq = _Equl()) {
		//判断溢出
		if (bitems >= size_t(-1) / sizeof(void *)) {
			__BSL_ERROR("too large bitems, overflower");
			return -1;
		}

		if (bitems == 0) {
			__BSL_ERROR("bitems == 0");
			return -1;
		}

		if (destroy() != 0) {
			__BSL_ERROR("destroy error when create hash bitems[%lu]", (unsigned long)bitems);
			return -1;
		}
		if (recreate(bitems) != 0) {
			__BSL_ERROR("recreate error when create hash bitems[%lu]", (unsigned long)bitems);
			return -1;
		}
		_hashfun = hf;
		_equl = eq;
		return 0;
	}

    /* *
     * @brief 判断hashtable是否已初始化
     * */
    bool is_created() const{
        return (_bucket != 0);
    }

	//destroy分两部分，一个有构造函数，一个没有构造函数
	int destroy() {
		//具体实现
		if (_bucket) {
			if (sample_alloc_type::recycle_space) {
				lib::bsl_destruct(begin(), end());
			} else {
				node_pointer nd = 0;
				for (size_t i=0; i<_bitems; ++i) {
					while (_bucket[i]) {
						nd = _bucket[i];
						_bucket[i] = _sp_alloc.getp(_bucket[i])->next;
						//nd->val.~value_type();
						lib::bsl_destruct(&(_sp_alloc.getp(nd)->val));
						_sp_alloc.deallocate(nd, 1);
					}
				}
			}
		}
		_sp_alloc.destroy();

		if (_bucket) {
			_np_alloc.deallocate(_bucket, _bitems);
		}
		_np_alloc.destroy();
		_bitems = 0;
		_bucket = 0;

		_size = 0;
		_end_iter._bucketpos = 0;
		return 0;
	}

	iterator begin() {
		return iterator(this, 0, 0);
	}

	const_iterator begin() const {
		return const_iterator(this, 0, 0);
	}

	iterator end()  {
		return _end_iter;
	}

	const_iterator end() const {
		return const_iterator(this, _bitems, 0);
	}
	
	size_t size() const {
		return _size;
	}

public:
	/**
	 * @brief 用其他容器对hash表进行附值操作
	 *
	 * @param [in/out] __begin   : _Iterator	迭代器的起始地址
	 * @param [in/out] __end   : _Iterator	迭代器的结束地址
	 * @return 	int 0	表示成功附值，其他失败
	 * 			如果返回出错，请调用destroy，销毁该对象
	 * @retval   
	 * @see 
	**/
	template <class _Iterator>
	int assign(_Iterator __begin, _Iterator __end)
	{
		size_t bkt = 0;
		bkt = std::distance(__begin, __end) * 2;
		if (bkt < _bitems) {
			bkt = _bitems;
		}
		if (destroy() != 0) {
			return -1;
		}
		if (recreate(bkt) != 0) {
			return -1;
		}
		for (_Iterator iter = __begin; iter != __end; ++iter) {
			if (set(_hashfun(_getkey(*iter)), _getkey(*iter), *iter) == -1) {
				return -1;
			}
		}
		return 0;
	}

	/**
	 * @brief 根据hash值，和key查找元素
	 *
	 * @param [in/out] hashval   : size_t
	 * @param [in/out] k   : const key_type&
	 * @return  value_type* 存在返回值指针,否这返回NULL
	 * @retval   
	 * @see 
	 * @note 
	**/
	value_type * find (size_t hashval, const key_type &k) const {
        if(0 == _bitems || 0 == _bucket){
            return NULL;
        }
		node_pointer lst = _bucket[hashval % _bitems];
		while (lst) {
			if (_equl(_getkey(_sp_alloc.getp(lst)->val), k)) {
				return &(_sp_alloc.getp(lst)->val);
			}
			lst = _sp_alloc.getp(lst)->next;
		}
		return NULL;
	}
	value_type * find (size_t hashval, const key_type &k) {
        if(0 == _bitems || 0 == _bucket){
            return NULL;
        }
		node_pointer lst = _bucket[hashval % _bitems];
		while (lst) {
			if (_equl(_getkey(_sp_alloc.getp(lst)->val), k)) {
				return &(_sp_alloc.getp(lst)->val);
			}
			lst = _sp_alloc.getp(lst)->next;
		}
		return NULL;
	}

	 //根据行为产生一个copy on write的还有直接附值的hash表
	/**
	 * @brief 将val插入hash表中
	 *
	 * @param [in/out] hashval   : size_t	hash key值
	 * @param [in/out] k   : const key_type&	key 值
	 * @param [in/out] val   : const value_type&	数据值
	 * @param [in/out] flag   : int	0表示如果值存在，直接返回，非0表示替换旧值
	 * @return  int 
	 * 				返回	-1	表示set调用出错, (无法分配新结点)
	 * 			其他均表示插入成功：插入成功分下面三个状态
	 * 				返回	HASH_OVERWRITE	表示覆盖旧结点成功(在flag非0的时候返回）
	 * 				返回	HASH_INSERT_SEC	表示插入新结点成功
	 * 				返回	HASH_EXIST	表示hash表结点存在（在flag为0的时候返回)
	 * @retval   
	 * @see 
	**/
	int set (size_t hashval, const key_type &k, const value_type &val, int flag = 0) {
        if(0 == _bitems || 0 == _bucket){
            return -1;
        }
		size_t key = hashval % _bitems;
		node_pointer lst = _bucket[key];
		while (lst) {
			if (_equl(_getkey(_sp_alloc.getp(lst)->val), k)) {
				if (flag) {
					_sp_alloc.getp(lst)->val = val;
					return HASH_OVERWRITE;
				}
				return HASH_EXIST;
			}
			lst = _sp_alloc.getp(lst)->next;
		}
		node_pointer node = _sp_alloc.allocate(1);
		if (0 == node) {
			return -1;
		}
		lib::bsl_construct(&(_sp_alloc.getp(node)->val), val);
		//::new (&node->val) value_type(val);
		_sp_alloc.getp(node)->next = _bucket[key];
		_bucket[key] = node;
		++ _size;
		return HASH_INSERT_SEC;
	}

	template <typename pair_sec_type>
	int set_map(size_t hashval, const key_type &k, const pair_sec_type &val, int flag = 0) {
        if(0 == _bitems || 0 == _bucket){
            return -1;
        }
		size_t key = hashval % _bitems;
		node_pointer lst = _bucket[key];
		while (lst) {
			if (_equl(_sp_alloc.getp(lst)->val.first, k)) {
				if (flag) {
					_sp_alloc.getp(lst)->val.second = val;
					return HASH_OVERWRITE;
				}
				return HASH_EXIST;
			}
			lst = _sp_alloc.getp(lst)->next;
		}
		node_pointer node = _sp_alloc.allocate(1);
		if (node == 0) {
			return -1;
		}
		::new (&(_sp_alloc.getp(node)->val)) value_type(k, val);
		_sp_alloc.getp(node)->next = _bucket[key];
		_bucket[key] = node;
		++ _size;
		return HASH_INSERT_SEC;
	}
public:

	/**
	 * @brief 根据指定key删除结点
	 *
	 * @param [in/out] hashval   : size_t
	 * @param [in/out] k   : const key_type&
	 * @return  int 
	 * 		返回	HASH_EXIST表示结点存在并删除成功
	 * 		返回	HASH_NOEXIST表示结点不存在不用删除
	 * @retval   
	 * @see 
	**/
	int erase (size_t hashval, const key_type &k) {
        if(0 == _bitems || 0 == _bucket){
            return HASH_NOEXIST;
        }
		size_t key = hashval % _bitems;
		node_pointer lst = _bucket[key];
		node_pointer front = 0;
		while (lst != 0) {
			if (_equl(_getkey(_sp_alloc.getp(lst)->val), k)) {
				if (front == 0) {
					_bucket[key] = _sp_alloc.getp(lst)->next;
				} else {
					_sp_alloc.getp(front)->next = _sp_alloc.getp(lst)->next;
				}
				lib::bsl_destruct(&(_sp_alloc.getp(lst)->val));
				//lst->val.~value_type();
				_sp_alloc.deallocate(lst, 1 );
				-- _size;
				return HASH_EXIST;
			}
			front = lst;
			lst = _sp_alloc.getp(lst)->next;
		}
		return HASH_NOEXIST;
	}

public:
	template <class _Archive>
	int serialization(_Archive & ar) {
		if (lib::serialization(ar, _bitems)) {
			return -1;
		}
		__BSL_DEBUG("write _bitems=%ld", (long)_bitems);
		if (lib::serialization(ar, _size)) {
			return -1;
		}
		__BSL_DEBUG("write _size=%ld", (long)_size);
		if (_size > 0) {
			for (iterator iter = begin(); iter != end(); ++iter) {
				if (lib::serialization(ar, *iter)) {
					return -1;
				}
			}
		}
		return 0;
	}

	template <class _Archive>
	int deserialization(_Archive & ar) {
		size_t bitems = 0;
		if (lib::deserialization(ar, bitems)) {
			__BSL_ERROR("deserialization bitems error");
			return -1;
		}
		__BSL_DEBUG("bitems = %ld", (long)bitems);

		//已经被初始化过
		if (_bucket) {
			if (clear()) {
				__BSL_ERROR("clear phashtable error");
				return -1;
			}
		} else {	//没有初始化过
			if (create(bitems, _HashFun(), _Equl())) {
				__BSL_ERROR("create hashtable error");
				return -1;
			}
		}

		size_t __size = 0;
		if (lib::deserialization(ar, __size)) {
			__BSL_ERROR("deserialization size error");
			return -1;
		}
		value_type val;
		for (size_t i=0; i<__size; ++i) {
			if (lib::deserialization(ar, val)) {
				__BSL_ERROR("deserialization val at[%ld] error", (long)i);
				return -1;
			}
			if (set(_hashfun(_getkey(val)), _getkey(val), val) == -1) {
				__BSL_ERROR("deserialization set at[%ld] error", (long)i);
				return -1;
			}
		}

		return 0;
	}

	int clear() {
		for (size_t i=0; i<_bitems; ++i) {
			node_pointer nd = 0;
			while (_bucket[i]) {
				nd = _bucket[i];
				_bucket[i] = _sp_alloc.getp(_bucket[i])->next;
				lib::bsl_destruct(&_sp_alloc.getp(nd)->val);
				_sp_alloc.deallocate(nd, 1);
			}
		}
		_size = 0;
		return 0;
	}
      
protected:
	int recreate(size_t bitems) {
		_bitems = bitems;

		_sp_alloc.create();
		_np_alloc.create();
		_bucket = _np_alloc.allocate(_bitems);
		if (_bucket == 0) {
			goto err;
		}
		memset(_bucket, 0, sizeof(node_pointer ) * _bitems);

		//初始化end 迭代器
		_end_iter._ht = this;
		_end_iter._bucketpos = _bitems;
		_end_iter._node = 0;

		_size = 0;
		return 0;
    err:
		_sp_alloc.destroy();
		_np_alloc.destroy();
		_bitems = 0;
		_end_iter._bucketpos = 0;
		return -1;
	}
private:
    void _reset(){
        _bitems = 0;
		_bucket = 0;
		_end_iter._ht = this;
		_end_iter._bucketpos = 0;
		_end_iter._node = 0;
		_size = 0;
    }
};

}

#endif  //__BSL_HASHTABLE_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
