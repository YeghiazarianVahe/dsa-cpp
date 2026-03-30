#include "DynamicArray.h"
#include <algorithm>
#include <stdexcept>

namespace dsa {

// ── Default constructor ───────────────────────────────
template<typename T>
DynamicArray<T>::DynamicArray()
    : data_{nullptr}, size_{0}, capacity_{0}
{}

// ── YOU implement the rest below ─────────────────────

// explicit capacity constructor
template<typename T>
DynamicArray<T>::DynamicArray(std::size_t capacity) 
    : size_{0}, capacity_{capacity} {
        data_ = (capacity_ > 0) ? new T[capacity_] : nullptr;
    }

// copy constructor
template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other) 
    : size_{other.size_}, capacity_{other.capacity_} 
    {
        data_ = (capacity_ > 0) ? new T[capacity_] : nullptr;

        for (size_t i = 0; i < size_; ++i){
            data_[i] = other.data_[i];
        }
        
    }

// move constructor
template<typename T>
DynamicArray<T>::DynamicArray(DynamicArray&& other) noexcept 
    : data_{other.data_}, size_{other.size_}, capacity_{other.capacity_} 
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }


// destructor
template<typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

// copy assignment
template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& other) {
    if (this == &other) {
        return *this; 
    }

    T* new_data = (other.capacity_ > 0) ? new T[other.capacity_] : nullptr;
    for(std::size_t i = 0; i < other.size_; ++i) {
        new_data[i] = other.data_[i];
    }

    delete[] data_;

    data_ = new_data;
    size_ = other.size_;
    capacity_ = other.capacity_;

    return *this;
}


// move assignment
template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray&& other) noexcept {
    if (this != &other) {
        delete[] data_;

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}


// operator[]
template<typename T>
T& DynamicArray<T>::operator[](std::size_t index) {
    return data_[index];
}

template<typename T>
const T& DynamicArray<T>::operator[](std::size_t index) const {
    return data_[index];
}

// at()
template<typename T>
T& DynamicArray<T>::at(std::size_t index) {
    if (index >= size_)
        throw std::out_of_range("Index out of range!");
    return data_[index];
}

template<typename T>
const T& DynamicArray<T>::at(std::size_t index) const {
    if (index >= size_)
        throw std::out_of_range("Index out of range!");
    return data_[index];
}

// front() / back()
template<typename T>
T& DynamicArray<T>::front() {
    return data_[0];
};

template<typename T>
T& DynamicArray<T>::back() {
    return data_[size_ - 1];
};

// push_back(const T&)
template<typename T>
void DynamicArray<T>::push_back(const T& value) {
    if (size_ == capacity_) {
        grow();
    }
    data_[size_++] = value;
}

// push_back(T&&)
template<typename T>
void DynamicArray<T>::push_back(T&& value) {
    if (size_ == capacity_) {
        grow();
    }
    data_[size_++] = std::move(value);
}

// pop_back()
template<typename T>
void DynamicArray<T>::pop_back() {
    size_--;
}

// insert()
template<typename T>
void DynamicArray<T>::insert(std::size_t index, const T& value) {
    if (index > size_) 
        throw std::out_of_range("Index out of bounds for insert!");

    if (size_ == capacity_) {
        grow();
    }

    for (std::size_t i = size_; i > index; --i) {
        data_[i] = std::move(data_[i - 1]);
    }

    data_[index] = value;
    ++size_;
}

// erase()
template<typename T>
void DynamicArray<T>::erase(std::size_t index) {
    if (index >= size_) 
        throw std::out_of_range("Index out of bounds for erase!");

    for (std::size_t i = index; i < size_; ++i) {
        data_[i] = std::move(data_[i + 1]);
    }

    --size_;
}

// clear()
template<typename T>
void DynamicArray<T>::clear() {
    size_ = 0;
}

// reserve()
template <typename T>
void DynamicArray<T>::reserve(std::size_t new_capacity) {
    if (new_capacity <= capacity_) {
        return; 
    }

    T* new_data = new T[new_capacity];
    for (std::size_t i = 0; i < size_; ++i) {
        new_data[i] = std::move(data_[i]);
    }
    
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
}

// resize()
template <typename T>
void DynamicArray<T>::resize(std::size_t new_size, const T& fill) {
    if (new_size < size_) {
        size_ = new_size;
    } 
    else if (new_size > size_) {
        if (new_size > capacity_) {
            reserve(new_size);
        }
        
        for (std::size_t i = size_; i < new_size; ++i) {
            data_[i] = fill;
        }
        size_ = new_size;
    }
}

// size() / capacity() / empty()
template<typename T>
std::size_t DynamicArray<T>::size() const noexcept {
    return size_;
}

template<typename T>
std::size_t DynamicArray<T>::capacity() const noexcept {
    return capacity_;
}

template<typename T>
bool DynamicArray<T>::empty() const noexcept {
    return (size_ == 0);
}

// begin() / end()
template<typename T>
T* DynamicArray<T>::begin() noexcept {
    return data_;
}

template<typename T>
const T* DynamicArray<T>::begin() const noexcept {
    return data_;
}

template<typename T>
T* DynamicArray<T>::end() noexcept {
    return data_ + size_;
}

template<typename T>
const T* DynamicArray<T>::end() const noexcept {
    return data_ + size_;
}


// grow()  <- the most important private method
template<typename T>
void DynamicArray<T>::grow() {
    std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
    
    T* new_data = new T[new_capacity];
    for (std::size_t i = 0; i < size_; ++i) {
        new_data[i] = std::move(data_[i]);
    }
    delete[] data_;
    
    data_ = new_data;
    capacity_ = new_capacity;
}

// ── Explicit instantiation ────────────────────────────
template class DynamicArray<int>;
template class DynamicArray<double>;

} // namespace dsa