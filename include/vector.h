/**
 * @file vector.h
 * @author Konstantin A. Pankov(explorus@mail.ru)
 *
 * @license Licensed under the MIT License, Version 2.0 (see LICENSE.txt).
 *
 * @brief vector implementation with STL-compatibility
 */

#pragma once

#include <cstring> // std::memcpy
#include <memory> // std::allocator

// do it yourself namespace
namespace diy
{
	template<typename T, typename Alloc>
	struct vector_base : Alloc
	{
		T* start{ nullptr };
		T* finish{ nullptr };
		T* end_of_storage{ nullptr };

		vector_base(size_t n = 0) : 
			start(n ? this->allocate(n) : nullptr),
			finish(start),
			end_of_storage(n ? start + n : nullptr)
		{
            while(n--)
                construct(finish++);
        }

        vector_base(size_t n, const T& value) : 
            start(n ? this->allocate(n) : nullptr),
			finish(start),
			end_of_storage(n ? start + n : nullptr)
        {
            while(n--)
                construct(finish++, value);
        }

		vector_base(const vector_base&) = delete;

		vector_base& operator=(const vector_base&) = delete;

		vector_base(vector_base&& rhs) noexcept
			: vector_base(0)
		{
			swap(rhs);
		}

		~vector_base()
		{
			clear();
		}

		vector_base& operator=(vector_base&& rhs) noexcept
		{
			vector_base tmp(std::move(rhs));
			swap(tmp);
			return *this;
		}

		void swap(vector_base& v) noexcept
		{
			std::swap(start, v.start);
			std::swap(finish, v.finish);
			std::swap(end_of_storage, v.end_of_storage);
		}

		T* realloc(size_t n)
		{
			size_t current_capacity = end_of_storage - start;
			size_t current_size = finish - start;
			size_t new_size = std::min(n, current_size);
			T* new_start = nullptr;
			if (n)
			{
				new_start = this->allocate(n);
				std::memcpy(new_start, start, new_size * sizeof(T));
				this->deallocate(start, current_capacity);
			}
			start = new_start;
			finish = start + new_size;
			end_of_storage = start + n;
			return start;
		}

		template<typename... _Args>
		void construct(T* p, _Args&&... args)
		{
			new (p) T(std::forward<_Args>(args)...);
		}			

		void destroy(T* p) noexcept
		{
			p->~T();
		}

        void clear() noexcept
        {
            while(finish > start)
                    destroy(--finish);
            this->deallocate(start, end_of_storage - start);
            start = finish = end_of_storage = nullptr;
        }
	};

	template<typename T, typename Alloc = std::allocator<T> >
	class vector final : protected vector_base<T, Alloc>
	{
	public:
		using base = vector_base<T, Alloc>;
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		// Iterators
		template<typename Iter>
		using iterator_category_t = typename std::iterator_traits<Iter>::iterator_category;
		template<typename Iter>
		using IsInputIterator = 
			std::is_convertible<iterator_category_t<Iter>, std::input_iterator_tag>;
		template<typename InIter>
		using RequireInputIter =
			std::enable_if_t<IsInputIterator<InIter>::value>;

		class vector_const_iterator
		{
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using pointer = const T*;
			using reference = const T&;
			explicit vector_const_iterator(pointer ptr) : ptr_(ptr)
			{}

			reference operator*() const noexcept
			{
				return *ptr_;
			}
			pointer operator->() const noexcept
			{
				return *ptr_;
			}
			vector_const_iterator& operator++() noexcept
			{
				++ptr_;
				return *this;
			}
			vector_const_iterator operator++(int) noexcept
			{
				auto retval = *this;
				++(*this);
				return retval;
			}
			vector_const_iterator& operator--() noexcept
			{
				--ptr_;
				return *this;
			}
			vector_const_iterator operator--(int) noexcept
			{
				auto retval = *this;
				--(*this);
				return retval;
			}

			vector_const_iterator& operator+=(const difference_type n) noexcept
			{
				ptr_ += n;
				return *this;
			}

			vector_const_iterator& operator-=(const difference_type n) noexcept
			{
				ptr_ -= n;
				return *this;
			}

			vector_const_iterator operator+(const difference_type n) const noexcept
			{
				vector_const_iterator tmp = *this;
				tmp += n;
				return tmp;
			}

			vector_const_iterator operator-(const difference_type n) const noexcept
			{
				vector_const_iterator tmp = *this;
				tmp -= n;
				return tmp;
			}

			bool operator==(const vector_const_iterator& other) const noexcept
			{
				return ptr_ == other.ptr_;
			}

         	bool operator!=(const vector_const_iterator& other) const noexcept
			{
				return !(*this == other);
			}

			difference_type operator-(const vector_const_iterator& other) const noexcept
			{
				return ptr_ - other.ptr_;
			}

		protected:
			pointer ptr_;
		};

		class vector_iterator : public vector_const_iterator
		{
		public:
			using base = vector_const_iterator;
			using iterator_category = std::random_access_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using pointer = T*;
			using reference = T&;
			explicit vector_iterator(pointer ptr) : vector_const_iterator(ptr)
			{}

			reference operator*() const noexcept
			{
				return const_cast<reference>(base::operator*());
			}
			pointer operator->() const noexcept
			{
				return *base::ptr_;
			}
			vector_iterator& operator++() noexcept
			{
				base::operator++();
				return *this;
			}
			vector_iterator operator++(int) noexcept
			{
				auto retval = *this;
				base::operator++();
				return retval;
			}
			vector_iterator& operator--() noexcept
			{
				base::operator--();
				return *this;
			}
			vector_iterator operator--(int) noexcept
			{
				auto retval = *this;
				base::operator--();
				return retval;
			}

			vector_iterator& operator+=(const difference_type n) noexcept
			{
				base::operator+=(n);
				return *this;
			}

			vector_iterator& operator-=(const difference_type n) noexcept
			{
				base::operator-=(n);
				return *this;
			}

			vector_iterator operator+(const difference_type n) const noexcept
			{
				vector_iterator tmp = *this;
				tmp += n;
				return tmp;
			}

			vector_iterator operator-(const difference_type n) const noexcept
			{
				vector_iterator tmp = *this;
				tmp -= n;
				return tmp;
			}

			difference_type operator-(const vector_iterator& other) const noexcept
			{
				return base::operator-(other);
			}
		};

		using iterator = vector_iterator;
		using const_iterator = vector_const_iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	public:
		explicit vector(size_type count = 0) : base(count)
		{}

		explicit vector(size_type count, const T& value) : base(count, value)
		{}

		template<typename InputIterator, typename = RequireInputIter<InputIterator>>
		vector(InputIterator first, InputIterator last)
		{
			try
			{
            	for(; first != last; ++first)
#if __cplusplus >= 201103L
            		emplace_back(*first);
#else
            		push_back(*first);
#endif
			}
			catch(...)
			{
            	clear();
            	throw;
			}
		}

		vector(const vector& rhs) : base(rhs.size())
		{
            this->finish = this->start;
			for (const auto& rhs_it: rhs)
				this->construct(this->finish++, rhs_it);
		}

		vector& operator=(const vector& rhs)
		{
			vector tmp(rhs);
			std::swap(*this, tmp);
			return *this;
		}

		vector(vector&&) = default;

		vector& operator=(vector&&) = default;

		void assign(size_type n, const value_type& val)
		{
			if (n > capacity())
			{
				vector tmp(n, val);
				swap(tmp);
			}
			else
			{
				while(this->finish > this->start)
					this->destroy(--this->finish);
				while(n--)
					this->construct(this->finish++, val);
			}
		}

		// element access
		reference at(size_type n)
		{
			range_check(n);
			return (*this)[n];
		}

		const_reference at(size_type n) const
		{
			range_check(n);
			return (*this)[n];
		}

		reference operator[](size_type n) noexcept
		{
			return *(this->start + n);
		}

		const_reference operator[](size_type n) const noexcept
		{
			return *(this->start + n);
		}

		reference front() noexcept
		{
			return *begin();
		}
		
		const_reference front() const noexcept
		{
			return *begin();
		}
		
		reference back() noexcept
		{
			return *(end() - 1);
		}
		
		const_reference back() const noexcept
		{
			return *(end() - 1);
		}

		pointer data() noexcept
		{
			return this->start;
		}

		const_pointer data() const noexcept
		{
			return this->start;
		}

		// iterator functions
		iterator begin() noexcept
		{
			return iterator(this->start);
		}

		const_iterator begin() const noexcept
		{
			return const_iterator(this->start);
		}

		const_iterator cbegin() const noexcept
		{
			return const_iterator(this->start);
		}

		iterator end() noexcept
		{
			return iterator(this->finish);
		}

		const_iterator end() const noexcept
		{
			return const_iterator(this->finish);
		}

		const_iterator cend() const noexcept
		{
			return const_iterator(this->finish);
		}

		reverse_iterator rbegin() noexcept
		{
			return reverse_iterator(end());
		}
		
		const_reverse_iterator rbegin() const noexcept
		{
			return const_reverse_iterator(end());
		}

		const_reverse_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(end());
		}
		
		reverse_iterator rend() noexcept
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const noexcept
		{
			return const_reverse_iterator(begin());
		}

		const_reverse_iterator crend() const noexcept
		{
			return const_reverse_iterator(begin());
		}

		// capacity
		bool empty() const noexcept
		{
			return begin() == end();
		}

		size_type size() const noexcept
		{
			return this->finish - this->start;
		}

		size_type max_size() const noexcept
		{
			return base::max_size();
		}

		size_type capacity() const noexcept
		{
			return this->end_of_storage - this->start;
		}

		void reserve(size_type n)
		{
			if (n > this->max_size())
				throw std::length_error("vector::reserve");
			if (capacity() < n)
				this->realloc(n);
		}

		void shrink_to_fit()
		{
			if (capacity() > size())
				this->realloc(size());
		}

		void resize(size_type new_size)
		{
			if (new_size > size())
			{
				this->realloc(new_size);
				auto difference = new_size - size();
				while (difference--)
					this->construct(this->finish++);
			}
			else if (new_size < size())
			{
				auto difference = size() - new_size;
				while (difference--)
					this->destroy(--this->finish);
				this->realloc(new_size);
			}
		}

		void resize(size_type new_size, const value_type& x)
		{
			if (new_size > size())
				insert(end(), new_size - size(), x);
			else if (new_size < size())
				erase_at_end(this->start + new_size);
		}

        void clear() noexcept
        {
            base::clear();
        }

		void swap(vector& x) noexcept
		{
			base::swap(x);
		}

        iterator insert(const_iterator position, const value_type& x)
        {
            auto index = position - begin();
            if (index == size())
                push_back(x);
            else
                insert(begin() + index, 1, x);
            return begin() + index;
        }

        iterator insert(const_iterator position, size_type n, const value_type& x)
        {
			difference_type offset = position - cbegin();
        	fill_insert(begin() + offset, n, x);
            return begin() + offset;
        }

        iterator insert(const_iterator position, value_type&& x)
        {
			return insert_rval(position, std::move(x));
        }

        iterator insert(const_iterator position, std::initializer_list<value_type> l)
        {
            auto offset = position - cbegin();
        	range_insert(begin() + offset, l.begin(), l.end());
        	return begin() + offset;
        }

		template<typename InputIterator, typename = RequireInputIter<InputIterator>>
		iterator insert(const_iterator position, InputIterator first, InputIterator last)
        {
			auto offset = position - begin();
			range_insert(begin() + offset, first, last);
			return begin() + offset;
        }

		void push_back(const_reference x)
		{
			if (this->finish == this->end_of_storage)
			{
				auto current_capacity = capacity();
				this->realloc(current_capacity ? current_capacity * 2 : 1);
			}
			this->construct(this->finish++, x);
		}

		void push_back(value_type&& x)
		{
			emplace_back(std::move(x));
		}

		template<typename... _Args>
		reference emplace_back(_Args&&... __args)
		{
			if (this->finish == this->end_of_storage)
			{
				auto current_capacity = capacity();
				this->realloc(current_capacity ? current_capacity * 2 : 1);
			}
			this->construct(this->finish++, std::forward<_Args>(__args)...);
			return back();
		}

		iterator erase(const_iterator position)
		{
			auto pos = begin() + (position - cbegin());
			if (pos + 1 != end())
				std::move(pos + 1, end(), pos);
			--this->finish;
			this->destroy(this->finish);
			return pos;
		}

		void pop_back()
		{
			--this->finish;
			this->destroy(this->finish);
		}

	protected:
		void range_check(size_type n) const
		{
			if (n >= this->size())
				throw std::out_of_range("vector::range_check");
		}

		template<typename InputIterator, typename = RequireInputIter<InputIterator>>
		void range_insert(iterator position, InputIterator first, InputIterator last)
		{
			auto n = last - first;
			size_t new_capacity = this->capacity();
			while(new_capacity < this->size() + n)
				new_capacity *= 2;
			if (new_capacity != this->capacity())
			{
				auto current_start = this->start;
				auto current_finish = this->finish;
				auto current_end_of_storage = this->end_of_storage;

				this->start = this->allocate(new_capacity);
				this->finish = this->start;
				this->end_of_storage = this->start + new_capacity;

				if (&*position > current_start)
				{
					std::copy(iterator(current_start), position, begin());
					this->finish += &*position - current_start;
				}
				
				for (; first != last; ++first)
					this->construct(this->finish++, *first);

				if (&*position < current_finish)
				{
					std::copy(position, iterator(current_finish), end());
					this->finish += (current_finish - &*position);
				}

				this->deallocate(current_start, current_end_of_storage - current_start);
			}
			else
			{
				if (&*position < this->finish)
				{
					std::copy(position, iterator(this->finish), position + n);
					this->finish += n;
				}
				for (; first != last; ++first)
					this->construct(this->finish++, *first);
			}
		}

		void fill_insert(iterator position, size_type n, const value_type& x)
		{
			if (n == 0)
				return;
			size_t new_capacity = this->capacity();
			while(new_capacity < this->size() + n)
				new_capacity *= 2;
			if (new_capacity != this->capacity())
			{
				auto current_start = this->start;
				auto current_finish = this->finish;
				auto current_end_of_storage = this->end_of_storage;

				this->start = this->allocate(new_capacity);
				this->finish = this->start;
				this->end_of_storage = this->start + new_capacity;

				if (&*position > current_start)
				{
					std::copy(iterator(current_start), position, begin());
					this->finish += &*position - current_start;
				}
				
				while (n--)
				{
					this->construct(this->finish++, x);
				}

				if (&*position < current_finish)
				{
					std::copy(position, iterator(current_finish), end());
					this->finish += (current_finish - &*position);
				}

				this->deallocate(current_start, current_end_of_storage - current_start);
			}
			else
			{
				if (&*position < this->finish)
				{
					std::copy(position, iterator(this->finish), position + n);
					this->finish += n;
				}
				while (n--)
				{
					this->construct(&*(position++), x);
				}
			}
		}

		void insert_aux(iterator position, value_type&& v)
		{
			this->construct(this->finish, std::move(*(this->finish - 1)));
			++this->finish;
			std::move_backward(position, end() - 1, end());
			*position = std::move(v);
		}

		void realloc_insert(const_iterator position, value_type&& v)
		{
                // Need to reallocate
                size_t new_capacity = capacity() ? capacity() * 2 : 1;

                T* new_start = this->allocate(new_capacity);
                T* new_finish = new_start;
                T* new_end_of_storage = new_start + new_capacity;

                // Copy elements before position
                std::copy(cbegin(), position, iterator{ new_start });
                new_finish += position - cbegin();
                
                // Construct new element at position
                this->construct(new_finish, std::move(v));
                ++new_finish;

                // Copy elements after position
                std::copy(position, cend(), iterator{ new_finish });
                new_finish += cend() - position;

                // Destroy old elements
                while (this->finish > this->start)
                    this->destroy(--this->finish);
                this->deallocate(this->start, this->end_of_storage - this->start);
                
                // Update pointers
                this->start = new_start;
                this->finish = new_finish;
                this->end_of_storage = new_end_of_storage;
		}

		iterator insert_rval(const_iterator position, value_type&& v)
		{
			const auto n = position - cbegin();
			if (this->finish != this->end_of_storage)
				if (position == cend())
				{
					this->construct(this->finish, std::move(v));
					++this->finish;
				}
				else
					insert_aux(begin() + n, std::move(v));
			else
				realloc_insert(begin() + n, std::move(v));
		
			return iterator(this->start + n);
		}

		void erase_at_end(pointer pos) noexcept
		{
			if (size_type n = this->finish - pos)
			{
				for (auto first = pos; first != this->finish; ++first)

				this->destroy(first);
				this->finish = pos;
			}
		}
 	};

	// Free functions of comparison operators
	template<typename T, typename Alloc>
	bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template<typename T, typename Alloc>
	bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T, typename Alloc>
	bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<typename T, typename Alloc>
	bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}

	template<typename T, typename Alloc>
	bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return rhs < lhs;
	}

	template<typename T, typename Alloc>
	bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}

} // namespace diy
